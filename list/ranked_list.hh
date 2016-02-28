// Copyright 2016, Georg Sauthoff <mail@georg.so>

/* {{{ LGPLv3

    This file is part of tree-model.

    tree-model is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    tree-model is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with tree-model.  If not, see <http://www.gnu.org/licenses/>.

}}} */
#ifndef LIST_RANKED_LIST_HH
#define LIST_RANKED_LIST_HH

/*

# Ranked List

2015, Georg Sauthoff <mail@georg.so>

A Ranked List is a probabilistic data structure for efficiently
determining the rank (i.e. position) of a list element,
inserting, removing and accessing the k-th element of a list.

It is basically a variant of a [Skip List][1] (see also Section
3.4 Linear List Operations of the [Skip List Cookbook][2]). In
comparison, it employs the probabilistic skip list machinery only
for rank operations, not for search key queries.


[1]: William Pugh: Skip Lists: A Probabilistic Alternative to Balanced Trees,
     Communications of the ACM 33 (6): 668, 1990.
     http://epaperpress.com/sortsearch/download/skiplist.pdf
     ftp://ftp.cs.umd.edu/pub/skipLists/skiplists.pdf
     https://en.wikipedia.org/wiki/Skip_list
[2]: William Pugh: A Skip List Cookbook. Techical Report, 1990.
     http://drum.lib.umd.edu/bitstream/1903/544/2/CS-TR-2286.1.pdf


## Complexity

    Operation                           Complexity     Case
    --------------------------------------------------------
    lookup k-th element                 O(log n)       AC
    insert element at front             O(1)           WC
    insert element at position k        O(log n)       AC
    insert m elements at position k     O(log n + m)   AC
    remove element at position k        O(log n)       AC
    remove m elements at position k     O(log n + m)   AC
    size of list                        O(1)           WC
    iterate over list                   O(n)           WC
    create list from n-element range    O(n)           WC
    destroy list                        O(n)           WC

where:

    n = |l|
    l denotes a list
    AC: average case
    WC: worst case

 */

#include <algorithm>
#include <limits>
#include <type_traits>
#include <stdlib.h>
#include <assert.h>

namespace list {

  namespace ranked {

    template <typename deref_t, bool Returns_Node>
      struct Deref_Operator {
      };
    template <typename deref_t> struct Deref_Operator<deref_t, true> {
      template <typename pointer>
      deref_t operator()(pointer node) const { return node; }
    };
    template <typename deref_t> struct Deref_Operator<deref_t, false> {
      template <typename pointer>
      deref_t operator()(pointer node) const { return node->value(); }
    };

    template <typename Node_T, typename Size_T, typename Level_Size_T>
      class Node_Base {
        private:
          Node_T **forward_;
          Size_T *distance_;
        public:
          Node_Base(Level_Size_T level)
            :
              // leave one as sentinel
              forward_(new Node_T*[level+1]()),
              distance_(new Size_T[level+1]())
          {
          }
          Node_Base(Node_Base &&o)
            :
              forward_(o.forward_),
              distance_(o.distance_)
          {
            o.forward_ = nullptr;
            o.distance_ = nullptr;
          }
          Node_Base &operator=(Node_Base &&o)
          {
            // done inside list, for now
            //delete forward_[0];
            delete[] forward_;
            forward_ = o.forward_;
            o.forward_ = nullptr;

            delete[] distance_;
            distance_ = o.distance_;
            o.distance_ = nullptr;
            return *this;
          }
          Node_Base(const Node_Base &o) =delete;
          Node_Base &operator=(const Node_Base &o) =delete;

          ~Node_Base()
          {
            //auto next = forward_[0];
            delete[] forward_;
            delete[] distance_;
            // done inside list, for now
            // tail-call optimized?
            //delete next;
          }
          Node_T *forward(Level_Size_T level) { return forward_[level]; }
          const Node_T *forward(Level_Size_T level) const
          {
            return forward_[level];
          }
          void set_forward(Level_Size_T level, Node_T *node)
          {
            forward_[level] = node;
          }
          Size_T distance(Level_Size_T level) const
          {
            return distance_[level];
          }
          void set_distance(Level_Size_T level, Size_T n)
          {
            distance_[level] = n;
          }
          Level_Size_T level() const
          {
            Level_Size_T l = 0;
            if (distance_)
              while (distance(l))
                ++l;
            return l;
          }
          Size_T successors() const
          {
            Size_T n = 0;
            const Node_Base *node = this;
            Level_Size_T level = this->level();
            while (node) {
              if (node->distance(level)) {
                while (node->distance(level+1))
                  ++level;
              } else {
                for ( ; level > 0; --level)
                  if (node->distance(level))
                    break;
              }
              n += node->distance(level);
              node = node->forward(level);
            }
            return n;
          }
          void resize(Level_Size_T level)
          {
            auto old_level = this->level();
            auto forward =  new Node_T*[level+1];
            std::copy(forward_, forward_ + old_level, forward);
            std::fill(forward + old_level, forward + level + 1, nullptr);
            delete[] forward_;
            forward_ = forward;
            auto distance = new Size_T[level+1];
            std::copy(distance_, distance_ + old_level, distance);
            std::fill(distance + old_level, distance + level + 1, Size_T());
            // same as:
            //std::fill(distance + old_level, distance + level + 1, 0);
            delete[] distance_;
            distance_ = distance;
          }
      };


    template <typename T, typename Size_T, typename Level_Size_T>
      class Node : public Node_Base<Node<T, Size_T, Level_Size_T>,
                                    Size_T, Level_Size_T> {
        private:
          T value_;
        public:
          Node(const T &value, Level_Size_T level)
            :
              Node_Base<Node<T, Size_T, Level_Size_T>,
                        Size_T, Level_Size_T>(level),
              value_(value)
          {
          }
          Node(T &&value, Level_Size_T level)
            :
              Node_Base<Node<T, Size_T, Level_Size_T>,
                        Size_T, Level_Size_T>(level),
              value_(std::move(value))
          {
          }
          T &value() { return value_; }
          const T &value() const { return value_; }
      };

    // p = 1/Q
    template <typename T,
              unsigned Q            = 2,
              typename Size_T       = uint32_t,
              typename Level_Size_T = uint8_t,
              typename Basic_Node_T = Node<T, Size_T, Level_Size_T>
              >
      class List {
        public:
          using Basic_Node      = Basic_Node_T;
          using Basic_Node_Base = Node_Base<Basic_Node_T, Size_T, Level_Size_T>;
        private:
          Basic_Node_Base node_      {4};
          Level_Size_T    level_     {4};
          Level_Size_T    max_level_ {sizeof(Size_T)*8/2};
          Size_T          size_      {0};


          Level_Size_T choose_level()
          {
            Level_Size_T level = random_level(
                std::min(Level_Size_T(level_ + 1u), max_level_));
            if (level > level_) {
              node_.resize(level);
              level_ = level;
            }
            return level;
          }

        public:
          List()
          {
          }
          template <typename Iterator>
            List(Iterator begin, Iterator end)
            {
              insert(0, begin, end);
            }
          List(const List &l)
          {
            insert(0, l.begin(), l.end());
          }
          List &operator=(const List &l)
          {
            List m;
            *this = std::move(m);
            insert(0, l.begin(), l.end());
            return *this;
          }
          List(List &&l)
            :
              node_(std::move(l.node_)),
              level_(l.level_),
              max_level_(l.max_level_),
              size_(l.size_)
          {
            l.level_ = 4;
            // leave l.max_level as-is
            l.node_.resize(l.level_);
            l.size_ = 0;
          }
          void clear()
          {
            List m;
            *this = std::move(m);
          }
          List &operator=(List &&l)
          {
            for (auto i = node_begin(); i != node_end(); ) {
              auto p = *i;
              ++i;
              delete p;
            }

            node_ = std::move(l.node_);
            level_ = l.level_;
            max_level_ = l.max_level_;
            size_ = l.size_;

            l.level_ = 4;
            // leave l.max_level as-is
            l.node_.resize(l.level_);
            l.size_ = 0;
            return *this;
          }
          ~List()
          {
            // destructor of node_ could also take care of it
            for (auto i = node_begin(); i != node_end(); ) {
              auto p = *i;
              ++i;
              delete p;
            }
          }
          Size_T size() const { return size_; }
          bool empty() const { return !size_; }
          Level_Size_T max_level() const { return max_level_; }
          void set_max_level(Level_Size_T l) { max_level_ = l; }
          Level_Size_T level() const { return level_; }


          template <bool is_const, bool returns_node>
          class basic_iterator {
            public:
              using pointer = typename std::conditional<
                is_const, const Basic_Node_T*, Basic_Node_T*>::type;
              using ref_t = typename
                std::conditional<is_const, const T&, T&>::type;
              using deref_t =
                typename std::conditional< returns_node, pointer, ref_t>::type;
            private:
              pointer node_ {nullptr};
            public:
              basic_iterator() =default;
              basic_iterator(pointer node) : node_(node) {}
              basic_iterator &operator++()
              {
                node_ = node_->forward(0);
                return *this;
              }

              template<typename = std::enable_if<
                !std::is_const<deref_t>::value, deref_t> >
                  deref_t operator*()
                  {
                    return Deref_Operator<deref_t, returns_node>()(node_);
                  }
              template<typename = std::enable_if<
                std::is_const<deref_t>::value, deref_t> >
                  deref_t operator*() const
                  {
                    return Deref_Operator<deref_t, returns_node>()(node_);
                  }
              bool operator==(const basic_iterator &o) const
              {
                return node_==o.node_;
              }
              bool operator!=(const basic_iterator &o) const
              {
                return !(*this == o);
              }
          };

          using iterator = basic_iterator<false, false>;
          iterator begin() { return iterator(node_.forward(0)); }
          iterator end()   { return iterator(); }

          using const_iterator = basic_iterator<true, false>;
          const_iterator begin() const
          {
            return const_iterator(node_.forward(0));
          }
          const_iterator end()   const { return const_iterator(); }

          using node_iterator = basic_iterator<false, true>;
          node_iterator node_begin() { return node_iterator(node_.forward(0)); }
          node_iterator node_end()   { return node_iterator(); }

          using const_node_iterator = basic_iterator<true, true>;
          const_node_iterator node_begin() const
          {
            return const_node_iterator(node_.forward(0));
          }
          const_node_iterator node_end() const { return const_node_iterator(); }


          void update_vector_k(Size_T k,
              std::vector<std::pair<Size_T, Basic_Node_Base*> > &update_vector)
          {
            // we don't check if list is empty because
            // the vector can be used for insert into an empty list
            update_vector.resize(level());

            ++k;
            Level_Size_T l = level() - 1;
            update_vector[l].first = 0;
            update_vector[l].second = &node_;

            for (;;) {
              // unlike node_prime() we compare >= k instead of > k
              // since we cannot finish early, i.e. we need
              // the right-most forward pointer parents
              // on the lower levels for insert/removal ...
              if (!update_vector[l].second->forward(l)
                  || update_vector[l].second->distance(l) >= k) {
                if (l) {
                  update_vector[l-1] = update_vector[l];
                  --l;
                } else
                  break;
              } else {
                k -= update_vector[l].second->distance(l);
                update_vector[l].first += update_vector[l].second->distance(l);
                update_vector[l].second = update_vector[l].second->forward(l);
              }
            }
            assert(l == 0);
          }


          void update_vector(Size_T k,
              std::vector<Basic_Node_Base*> &update_vector)
          {
            if (empty())
              throw std::out_of_range("ranked list is empty");
            update_vector.resize(level());

            ++k;
            Level_Size_T l = level() - 1;
            update_vector[l] = &node_;

            for (;;) {
              // unlike node_prime() we compare >= k instead of > k
              // since we cannot finish early, i.e. we need
              // the right-most forward pointer parents
              // on the lower levels for insert/removal ...
              if (!update_vector[l]->forward(l)
                  || update_vector[l]->distance(l) >= k) {
                if (l) {
                  update_vector[l-1] = update_vector[l];
                  --l;
                } else
                  break;
              } else {
                k -= update_vector[l]->distance(l);
                update_vector[l] = update_vector[l]->forward(l);
              }
            }
            assert(l == 0);
          }

          void remove(Size_T k, Size_T count = 1u)
          {
            if (k + count > size())
              throw std::out_of_range("remove: no such element");
            std::vector<Basic_Node_Base*> v;
            update_vector(k, v);
            assert(!v.empty());

            for (Size_T c = 0; c < count; ++c) {
              auto p = v[0]->forward(0);
              assert(p);
              Level_Size_T i = 0;
              for (auto x : v) {
                if (x->forward(i)) {
                  if (x->forward(i) == p) {
                    if (x->forward(i)->forward(i))
                      x->set_distance(i,
                          x->distance(i) + x->forward(i)->distance(i) - 1);
                    else
                      x->set_distance(i, 0);
                    x->set_forward(i, x->forward(i)->forward(i));
                  } else {
                    if (x->distance(i))
                      x->set_distance(i, x->distance(i)-1);
                  }
                }
                ++i;
              }
              --size_;
              delete p;
            }
          }

          void insert(Size_T k, Basic_Node_T *node, Level_Size_T level)
          {
            if (k > size())
              throw std::out_of_range("insert: no such element");

            std::vector<std::pair<Size_T, Basic_Node_Base*> > v;
            update_vector_k(k, v);
            assert(!v.empty());

            Level_Size_T l = 0;
            for ( ; l < level; ++l) {
              assert(k >= v[l].first);
              Size_T delta = k - v[l].first;
              if (v[l].second->forward(l)) {
                node->set_forward(l, v[l].second->forward(l));
                node->set_distance(l, v[l].second->distance(l) - delta);
              }
              v[l].second->set_forward(l, node);
              v[l].second->set_distance(l, delta + 1);
            }
            for ( ; l < level_; ++l) {
              if (v[l].second->distance(l))
                v[l].second->set_distance(l, v[l].second->distance(l)+1);
              else
                break;
            }

            ++size_;
          }

          Basic_Node_T &insert(Size_T k, const T &t)
          {
            if (k > size())
              throw std::out_of_range("insert: no such element");

            Level_Size_T level = choose_level();
            Basic_Node_T *node = new Basic_Node_T(t, level);
            insert(k, node, level);
            return *node;
          }

          Basic_Node_T & insert(Size_T k, T &&t)
          {
            if (k > size())
              throw std::out_of_range("insert: no such element");

            Level_Size_T level = choose_level();
            Basic_Node_T *node = new Basic_Node_T(std::move(t), level);
            insert(k, node, level);
            return *node;
          }

          template <typename Iterator>
          void insert(Size_T k, Iterator begin, Iterator end)
          {
            if (k > size())
              throw std::out_of_range("insert: no such element");

            std::vector<std::pair<Size_T, Basic_Node_Base*> > v;
            update_vector_k(k, v);
            assert(!v.empty());

            for (auto i = begin; i != end; ++i) {

              Level_Size_T level = choose_level();
              if (v.size() < level_) {
                v.resize(level_ );
                v[level_-1].first = 0;
                v[level_-1].second = &node_;
              }
              Basic_Node_T *node = new Basic_Node_T(*i, level);
              Level_Size_T l = 0;
              for ( ; l < level; ++l) {
                assert(k >= v[l].first);
                Size_T delta = k - v[l].first;
                if (v[l].second->forward(l)) {
                  node->set_forward(l, v[l].second->forward(l));
                  node->set_distance(l, v[l].second->distance(l) - delta);
                }
                v[l].second->set_forward(l, node);
                auto old_distance = v[l].second->distance(l);
                v[l].second->set_distance(l, delta + 1);

                v[l].second = node;
                v[l].first = k+1;
              }
              for ( ; l < level_; ++l) {
                if (v[l].second->distance(l))
                  v[l].second->set_distance(l, v[l].second->distance(l)+1);
                else
                  break;
              }
              ++size_;
              ++k;

            }
          }


          template <typename Counter>
          Basic_Node_T &node_prime(Size_T k, Counter &counter)
          {
            if (empty())
              throw std::out_of_range("ranked list is empty");
            
            ++k;
            Basic_Node_Base *node = &node_;
            Level_Size_T l = level_ - 1;

            for (;;) {
              counter();
              if (!node->forward(l) || node->distance(l) > k) {
                if (l)
                  --l;
                else
                  throw std::out_of_range("ranked list too small");
              } else if (node->distance(l) == k) {
                return *node->forward(l);
              } else {
                k -= node->distance(l);
                node = node->forward(l);
              }
            }
          }

          template <typename Counter>
          const Basic_Node_T &node_prime(Size_T k, Counter &counter) const
          {
            return const_cast<List*>(this)->node_prime(k, counter);
          }

          Basic_Node_T &node(Size_T k)
          {
            struct Counter { void operator()() const {} };
            Counter counter;
            return node_prime(k, counter);
          }
          const Basic_Node_T &node(Size_T k) const
          {
            return const_cast<List*>(this)->node(k);
          }
          T &operator[](Size_T k)
          {
            return node(k).value();
          }
          const T &operator[](Size_T k) const
          {
            return node(k).value();
          }
          Size_T rank(const Basic_Node_Base &node) const
          {
            Size_T n = size();
            n -= node.successors();
            return n-1;
          }

          Level_Size_T random_level(Level_Size_T max_level
              = std::numeric_limits<Level_Size_T>::max())
          {
            double p = 1.0/Q;

            Level_Size_T level = 1;
            while (
#if (defined(__MINGW32__) || defined(__MINGW64__))
                double(rand())/RAND_MAX
#else
                double(random())/RAND_MAX
#endif
                  < p && level < max_level)
              ++level;
            return level;
          }

          void push_front(Basic_Node_T *node, Level_Size_T level)
          {
            Level_Size_T l = 0;
            for ( ; l < level; ++l) {
              node->set_forward(l, node_.forward(l));
              node->set_distance(l, node_.distance(l));
              node_.set_forward(l, node);
              node_.set_distance(l, 1);
            }
            for ( ; l < level_; ++l) {
              if (node_.distance(l))
                node_.set_distance(l, node_.distance(l)+1);
              else
                break;
            }
            ++size_;
          }

          void push_front(const T &t)
          {
            Level_Size_T level = choose_level();
            Basic_Node_T *node = new Basic_Node_T(t, level);
            push_front(node, level);
          }

          void push_front(T &&t)
          {
            Level_Size_T level = choose_level();
            Basic_Node_T *node = new Basic_Node_T(std::move(t), level);
            push_front(node, level);
          }
      };
  }

}

#endif
