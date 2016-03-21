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
#include <catch.hpp>

#include <random> //optional
#include <stdlib.h>
#include <vector>
//#include <iostream>

#include <list/ranked_list.hh>

using namespace std;

TEST_CASE("ranked basic", "[list][ranked]" )
{
  list::ranked::List<int> l;
  CHECK(l.size() == 0);
  CHECK(l.empty() == true);
  CHECK_THROWS_AS(l[0], std::out_of_range);
}

// optional
TEST_CASE("bernoulli plusplus", "[list][ranked]" )
{
  // for the list we just use random() because
  // - including the engine/distribution in each list object would
  //   cost too much memory
  // - putting it in an extra function would yield too much overhead
  size_t n = 1000000;
  size_t ok = 0;
  size_t nok = 0;

  std::random_device random_device;
  //cerr << "sizeof(random_device): " << sizeof(random_device) << '\n';
  std::mt19937 random_engine{random_device()};
  //cerr << "sizeof(random_engine): " << sizeof(random_engine) << '\n';
  std::bernoulli_distribution d{0.5};
  //cerr << "sizeof(d): " << sizeof(d) << '\n';
  for (size_t i = 0; i < n; ++i)
    if (d(random_engine))
      ++ok;
    else
      ++nok;
  CHECK(fabs(double(ok)/n  - 0.5) < 0.001*2);
  CHECK(fabs(double(nok)/n - 0.5) < 0.001*2);
}

TEST_CASE("bernoulli", "[list][ranked]" )
{
  size_t n = 1000000;
  size_t ok = 0;
  size_t nok = 0;

  double p = 0.5;
  for (size_t i = 0; i < n; ++i)
#if (defined(__MINGW32__) || defined(__MINGW64__))
    if (double(rand())/RAND_MAX < p)
#else
    if (double(random())/RAND_MAX < p)
#endif
      ++ok;
    else
      ++nok;
  CHECK(fabs(double(ok)/n  - 0.5) < 0.001*2);
  CHECK(fabs(double(nok)/n - 0.5) < 0.001*2);
}

TEST_CASE("random level", "[list][ranked]" )
{
  list::ranked::List<int> l;
  for (size_t i = 0; i < 1000; ++i)
    CHECK(l.random_level() > 0);
  for (size_t i = 0; i < 1000; ++i)
    CHECK(l.random_level(5) < 6);
  size_t cnt5 = 0;
  for (size_t i = 0; i < 1000; ++i)
    if (l.random_level(5) == 5)
      ++cnt5;
  CHECK(cnt5 > 0);
}

TEST_CASE("ranked push", "[list][ranked]" )
{
  list::ranked::List<int> l;
  l.push_front(1);
  CHECK(l.size() == 1);
  CHECK(l.empty() == false);
  CHECK(l[0] == 1);
  CHECK_THROWS_AS(l[1], std::out_of_range);
  CHECK(l.rank(l.node(0)) == 0);
}

TEST_CASE("ranked push more", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 10000;
  for (int i = n - 1; i > 0; --i) {
    l.push_front(i);
  }
  REQUIRE(l.size() == n-1);
  for (int i = 1; i < n; ++i) {
    CHECK(l[i-1] == i);
  }
}

TEST_CASE("ranked push max_level", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 10000;
  CHECK(l.level() == 4);
  for (int i = n - 1; i > 0; --i) {
    l.push_front(i);
  }
  // on mingw there is more variation, usually it is 14
  CHECK(l.level() >= 12);
  CHECK(l.level() <= 15);
}

TEST_CASE("ranked push max_level limit", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 10000;
  l.set_max_level(8);
  CHECK(l.level() == 4);
  for (int i = n - 1; i > 0; --i) {
    l.push_front(i);
  }
  CHECK(l.level() == 8);
}

TEST_CASE("ranked for each", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i > 0; --i) {
    l.push_front(i);
  }
  int count = 0;
  for (auto i = l.node_begin(); i != l.node_end(); ++i) {
    auto node = *i;
    CHECK(node->value() == l[count]);
    CHECK(l.rank(*node) == count);
    ++count;
  }
  CHECK(count+1 == n);
}

TEST_CASE("ranked for each insert front", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i > 0; --i) {
    l.insert(0, i);
  }
  REQUIRE(l.size() == n-1);
  int count = 0;
  for (auto i = l.node_begin(); i != l.node_end(); ++i) {
    auto node = *i;
    CHECK(node->value() == l[count]);
    CHECK(l.rank(*node) == count);
    ++count;
  }
  CHECK(count+1 == n);
}

TEST_CASE("ranked for each insert back", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = 0; i < n; ++i) {
    l.insert(i, i);
  }
  REQUIRE(l.size() == n);
  int count = 0;
  for (auto i = l.node_begin(); i != l.node_end(); ++i) {
    auto node = *i;
    CHECK(node->value() == l[count]);
    CHECK(l.rank(*node) == count);
    ++count;
  }
  CHECK(count == n);
}

TEST_CASE("ranked for each after removed half", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 1000;
  for (int i = n-1; i >= 0; --i) {
    l.push_front(i);
  }
  int m = n/2;
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  for (int i = 0; i < m; ++i)
  {
    std::uniform_int_distribution<int> d{0, n-1};
    l.remove(d(random_engine));
    --n;
  }
  REQUIRE(l.size() == n);
  int count = 0;
  for (auto i = l.node_begin(); i != l.node_end(); ++i) {
    auto node = *i;
    CHECK(l.rank(*node) == count);
    ++count;
  }
  CHECK(count == n);
}

TEST_CASE("ranked for each insert mid", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 1000;
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  for (int i = 0; i < n; ++i) {
    std::uniform_int_distribution<int> d{0, i};
    l.insert(d(random_engine), i);
  }
  REQUIRE(l.size() == n);
  int count = 0;
  for (auto i = l.node_begin(); i != l.node_end(); ++i) {
    auto node = *i;
    CHECK(l.rank(*node) == count);
    ++count;
  }
  CHECK(count == n);
}

static void check_level_distribution(const list::ranked::List<int> &l)
{
  int n = l.size();

  int level_one = 0;
  double level_sum = 0;
  for (auto i = l.node_begin(); i != l.node_end(); ++i) {
    auto node = *i;
    if (node->level() == 1)
      ++level_one;
    level_sum += node->level();
  }
  //CHECK(fabs(double(level_one)/n - 0.50) < 0.02);
  // for mingw
  CHECK(fabs(double(level_one)/n - 0.50) < 0.03);
  //CHECK(fabs(level_sum/n - 1.99) < 0.0444);
  CHECK(fabs(level_sum/n - 1.99) < 0.05);
}

TEST_CASE("level distribution", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 10000;
  for (int i = 0; i < n; ++i)
    l.push_front(i);
  check_level_distribution(l);
}

TEST_CASE("level distribution insert mid", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 10000;

  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  for (int i = 0; i < n; ++i) {
    std::uniform_int_distribution<int> d{0, i};
    l.insert(d(random_engine), i);
  }
  check_level_distribution(l);
}

TEST_CASE("level distribution remove mid", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 10000;
  int m =  5000;

  for (int i = 0; i < n+m; ++i)
    l.push_front(i);

  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  for (int i = n+m; i > n; --i) {
    std::uniform_int_distribution<int> d{0, i-1};
    l.remove(d(random_engine));
  }
  REQUIRE(l.size() == n);

  check_level_distribution(l);
}

TEST_CASE("level distribution insertit mid", "[list][ranked]" )
{
  int n = 1000;
  int m = 100;
  int k = 20;
  list::ranked::List<int> l;
  for (int i = 0; i < n; ++i)
    l.push_front(i);
  vector<int> v;
  for (int i = 0; i < m; ++i)
    v.push_back(n+i);
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};

  int x = 0;
  for (int i = 0; i < k; ++i) {
    std::uniform_int_distribution<int> d{0, n};
    std::uniform_int_distribution<int> e{0, m};
    int ex = e(random_engine);
    x += ex;
    l.insert(d(random_engine), v.begin(), v.begin() + ex);
  }
  REQUIRE(l.size() == n + x);
  n += x;

  check_level_distribution(l);
}

TEST_CASE("level distribution removek mid", "[list][ranked]" )
{
  int n = 10000;
  int m = 100;
  int k = 20;
  list::ranked::List<int> l;
  for (int i = 0; i < n; ++i)
    l.push_front(i);
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};

  int x = 0;
  for (int i = 0; i < k; ++i) {
    std::uniform_int_distribution<int> d{0, int(l.size())-1};
    int dx = d(random_engine);
    std::uniform_int_distribution<int> e{0, min(m, int(l.size())-dx)};
    int ex = e(random_engine);
    x += ex;
    l.remove(dx, ex);
  }
  REQUIRE(l.size() == n - x);
  n -= x;

  check_level_distribution(l);
}

static void check_expected_cost(const list::ranked::List<int> &l)
{
  int n = l.size();
  struct Counter { size_t n = 0; void operator()() { ++n; } };
  Counter counter;
  double step_sum = 0;
  for (int i = 0; i < n; ++i)
    l.node_prime(i, counter);
  CHECK(double(counter.n)/n < 2 * log2(n));
  //CHECK(fabs(double(counter.n)/n - 30) < 3.15);
  CHECK(fabs(double(counter.n)/n - 30) < 3.31);
}

TEST_CASE("expected cost", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100000;
  for (int i = 0; i < n; ++i)
    l.push_front(i);
  check_expected_cost(l);
}

TEST_CASE("expected cost insert front", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100000;
  for (int i = 0; i < n; ++i)
    l.insert(0, i);
  check_expected_cost(l);
}

TEST_CASE("expected cost insert back", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100000;
  for (int i = 0; i < n; ++i)
    l.insert(i, i);
  check_expected_cost(l);
}

TEST_CASE("expected cost insert mid", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100000;
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  for (int i = 0; i < n; ++i) {
    std::uniform_int_distribution<int> d{0, i};
    l.insert(d(random_engine), i);
  }
  REQUIRE(l.size() == n);
  check_expected_cost(l);
}

TEST_CASE("expected cost remove mid", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100000;
  int m =   5000;

  for (int i = 0; i < n+m; ++i)
    l.push_front(i);

  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  for (int i = n+m; i > n; --i) {
    std::uniform_int_distribution<int> d{0, i-1};
    l.remove(d(random_engine));
  }
  REQUIRE(l.size() == n);

  check_expected_cost(l);
}

TEST_CASE("expected cost inserit mid", "[list][ranked]" )
{
  int n = 10000;
  int m = 1000;
  int k = 100;
  list::ranked::List<int> l;
  for (int i = 0; i < n; ++i)
    l.push_front(i);
  vector<int> v;
  for (int i = 0; i < m; ++i)
    v.push_back(n+i);
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};

  int x = 0;
  for (int i = 0; i < k; ++i) {
    std::uniform_int_distribution<int> d{0, n};
    std::uniform_int_distribution<int> e{0, m};
    int ex = e(random_engine);
    x += ex;
    l.insert(d(random_engine), v.begin(), v.begin() + ex);
  }
  REQUIRE(l.size() == n + x);
  n += x;

  check_expected_cost(l);
}

TEST_CASE("expected cost removek mid", "[list][ranked]" )
{
  int n = 100000;
  int m = 100;
  int k = 100;
  list::ranked::List<int> l;
  for (int i = 0; i < n; ++i)
    l.push_front(i);
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};

  int x = 0;
  for (int i = 0; i < k; ++i) {
    std::uniform_int_distribution<int> d{0, int(l.size())-1};
    int dx = d(random_engine);
    std::uniform_int_distribution<int> e{0, min(m, int(l.size())-dx)};
    int ex = e(random_engine);
    x += ex;
    l.remove(dx, ex);
  }
  REQUIRE(l.size() == n - x);
  n -= x;

  check_expected_cost(l);
}


TEST_CASE("ranked for each value", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i > 0; --i)
    l.push_front(i);
  int count = 0;
  for (auto &value : l) {
    CHECK(value == l[count]);
    ++count;
  }
  CHECK(count+1 == n);
}

TEST_CASE("ranked for each value const", "[list][ranked]" )
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i > 0; --i)
    l.push_front(i);
  int count = 0;
  const list::ranked::List<int> &m = l;
  for (auto &value : m) {
    CHECK(value == m[count]);
    ++count;
  }
  CHECK(count+1 == n);
}

TEST_CASE("ranked update vector front", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == n);
  vector<list::ranked::List<int>::Basic_Node_Base*> update_vector;
  l.update_vector(0, update_vector);
  REQUIRE(update_vector.size() == l.level());
  for (auto p : update_vector)
    CHECK(p != nullptr);

  auto x = update_vector[0];
  for (auto p : update_vector)
    CHECK(p == x);

  CHECK(update_vector[0]->forward(0)->value() == 0);
  for (unsigned i = 1; i < l.level(); ++i) {
    CHECK(update_vector[i]->forward(i)->value() >= 0);
  }
}

TEST_CASE("ranked update vector", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == n);
  vector<list::ranked::List<int>::Basic_Node_Base*> update_vector;
  l.update_vector(23, update_vector);
  REQUIRE(update_vector.size() == l.level());
  for (auto p : update_vector)
    CHECK(p != nullptr);

  CHECK(update_vector[0]->forward(0)->value() == 23);
  for (unsigned i = 1; i < l.level(); ++i) {
    // we can't require that forward(i) is unequal null:
    if (update_vector[i]->forward(i))
      CHECK(update_vector[i]->forward(i)->value() >= 23);
  }
}

TEST_CASE("ranked update vector back minus one", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == n);
  vector<list::ranked::List<int>::Basic_Node_Base*> update_vector;
  l.update_vector(n-1, update_vector);
  REQUIRE(update_vector.size() == l.level());
  for (auto p : update_vector)
    CHECK(p != nullptr);

  CHECK(update_vector[0]->forward(0)->value() == n-1);
  auto x = update_vector[0]->forward(0);
  for (unsigned i = 1; i < l.level(); ++i) {
    bool b = (update_vector[i]->forward(i) == nullptr || update_vector[i]->forward(i) == x);
    CHECK(b == true);
  }
}

TEST_CASE("ranked update vector back", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == n);
  vector<list::ranked::List<int>::Basic_Node_Base*> update_vector;
  l.update_vector(n, update_vector);
  REQUIRE(update_vector.size() == l.level());
  for (auto p : update_vector)
    CHECK(p != nullptr);

  CHECK(static_cast<list::ranked::List<int>::Basic_Node*>(update_vector[0])
      ->value() == n-1);
  for (unsigned i = 0; i < l.level(); ++i) {
    CHECK(update_vector[i]->forward(i) == nullptr);
  }
}

TEST_CASE("ranked remove", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 100);
  CHECK(l[0] == 0);
  CHECK(l[1] == 1);
  CHECK(l[2] == 2);
  l.remove(1);
  REQUIRE(l.size() == 99);
  CHECK(l[0] == 0);
  CHECK(l[1] == 2);
  CHECK(l[2] == 3);
  CHECK(l[98] == 99);
  l.remove(0);
  REQUIRE(l.size() == 98);
  CHECK(l[0] == 2);
  CHECK(l[1] == 3);
  CHECK(l[2] == 4);
  l.remove(97);
  REQUIRE(l.size() == 97);
  CHECK(l[96] == 98);
  CHECK_THROWS_AS(l.remove(97), std::out_of_range);
  REQUIRE(l.size() == 97);
}

TEST_CASE("ranked remove all", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 100);
  for (int i = n - 1; i >= 0; --i)
    l.remove(0);
  REQUIRE(l.size() == 0);
}

TEST_CASE("ranked update k front", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 100);
  std::vector<
    std::pair<uint32_t, list::ranked::List<int>::Basic_Node_Base*> > v;
  l.update_vector_k(0, v);
  for (auto &x : v) {
    CHECK(x.first == 0);
    CHECK(x.second != nullptr);
  }
}

TEST_CASE("ranked update k middle", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 100);
  std::vector<
    std::pair<uint32_t, list::ranked::List<int>::Basic_Node_Base*> > v;
  l.update_vector_k(23, v);
  CHECK(v[0].first == 23);
  uint32_t i = 0;
  for (auto &x : v) {
    CHECK(x.first <= 23);
    if (x.second->distance(i))
      CHECK(x.first + x.second->distance(i) > 23);
    CHECK(x.second != nullptr);
    ++i;
  }
}

TEST_CASE("ranked update k all", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 100);
  for (unsigned k = 0; k < 100; ++k) {
    std::vector<
      std::pair<uint32_t, list::ranked::List<int>::Basic_Node_Base*> > v;
    l.update_vector_k(k, v);
    CHECK(v[0].first == k);
    uint32_t i = 0;
    for (auto &x : v) {
      CHECK(x.first <= k);
      if (x.second->distance(i))
        CHECK(x.first + x.second->distance(i) > k);

      CHECK(x.first == l.rank(*x.second) + 1);

      CHECK(x.second != nullptr);
      ++i;
    }
  }
}

TEST_CASE("ranked update k back", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 100;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 100);
  std::vector<
    std::pair<uint32_t, list::ranked::List<int>::Basic_Node_Base*> > v;
  l.update_vector_k(100, v);
  CHECK(v[0].first == 100);
  uint32_t i = 0;
  for (auto &x : v) {
    CHECK(x.second->distance(i) == 0);
    CHECK(x.first <= 100);
    CHECK(x.second != nullptr);
    ++i;
  }
}


TEST_CASE("ranked insert", "[list][ranked]")
{
  list::ranked::List<int> l;
  int n = 10;
  for (int i = n - 1; i >= 0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 10);
  l.insert(0, 100);
  REQUIRE(l.size() == 11);
  CHECK(l[0] == 100);
  CHECK(l[1] == 0);
  CHECK(l[2] == 1);
  l.insert(11, 101);
  REQUIRE(l.size() == 12);
  CHECK(l[9] == 8);
  CHECK(l[10] == 9);
  CHECK(l[11] == 101);
  l.insert(11, 102);
  REQUIRE(l.size() == 13);
  CHECK(l[10] == 9);
  CHECK(l[11] == 102);
  CHECK(l[12] == 101);
}

TEST_CASE("ranked insert throw", "[list][ranked]")
{
  list::ranked::List<int> l;
  CHECK_THROWS_AS(l.insert(1, 100), std::out_of_range);
}

TEST_CASE("ranked push front move", "[list][ranked]")
{
  struct Value {
    int v {0};
    bool move_constructed {false};
    Value()
    {
    }
    Value(int v)
      : v(v)
    {
    }
    Value(Value &&v)
      :
        v(v.v),
        move_constructed(true)
    {
    }
  };
  list::ranked::List<Value> l;
  l.push_front(Value(23));
  CHECK(l.size() == 1);
  CHECK(l[0].v == 23);
  CHECK(l[0].move_constructed == true);
}

TEST_CASE("ranked copy construct", "[list][ranked]")
{
  list::ranked::List<int> l;
  l.push_front(42);
  list::ranked::List<int> m(l);
  CHECK(m.size() == 1);
  CHECK(m[0] == 42);
  CHECK(&l.node(0) != &m.node(0));
  CHECK(l.size() == 1);
  CHECK(l[0] == 42);
}

TEST_CASE("ranked copy assign", "[list][ranked]")
{
  list::ranked::List<int> l;
  l.push_front(43);
  l.push_front(42);
  list::ranked::List<int> m;
  m.push_front(11);
  m.push_front(12);
  m.push_front(13);
  m = l;
  CHECK(m.size() == 2);
  CHECK(m[0] == 42);
  CHECK(m[1] == 43);
  CHECK(&l.node(0) != &m.node(0));
  CHECK(&l.node(1) != &m.node(1));
  CHECK(l.size() == 2);
  CHECK(l[0] == 42);
  CHECK(l[1] == 43);
}

TEST_CASE("ranked move construct", "[list][ranked]")
{
  list::ranked::List<int> l;
  l.push_front(42);
  list::ranked::List<int> m(std::move(l));

  CHECK(m.size() == 1);
  CHECK(m[0] == 42);

  CHECK(l.size() == 0);
  l.push_front(43);
  CHECK(l.size() == 1);
  CHECK(l[0] == 43);
}

TEST_CASE("ranked move assign", "[list][ranked]")
{
  list::ranked::List<int> l;
  l.push_front(42);
  list::ranked::List<int> m;
  m.push_front(11);
  m.push_front(12);
  m.push_front(13);
  
  m = std::move(l);

  CHECK(m.size() == 1);
  CHECK(m[0] == 42);

  CHECK(l.size() == 0);
  l.push_front(43);
  CHECK(l.size() == 1);
  CHECK(l[0] == 43);
}

TEST_CASE("ranked iterator construct", "[list][ranked]")
{
  vector<int> v = { 23, 42, 2, 1, 11 };

  list::ranked::List<int> l(v.begin(), v.end());

  CHECK(l.size() == v.size());
  for (size_t i = 0; i < v.size(); ++i)
    CHECK(v[i] == l[i]);
}

TEST_CASE("ranked iterator insert empty", "[list][ranked]")
{
  vector<int> v = { 23, 42, 2, 1, 11 };

  list::ranked::List<int> l;
  l.insert(0, v.begin(), v.end());

  CHECK(l.size() == v.size());
  for (size_t i = 0; i < v.size(); ++i)
    CHECK(v[i] == l[i]);
}

TEST_CASE("ranked iterator insert in between", "[list][ranked]")
{
  vector<int> v = { 23, 42, 21, 8, 11 };
  vector<int> w = { 1, 2, 23, 42, 21, 8, 11, 3, 4 };

  list::ranked::List<int> l;
  l.push_front(4);
  l.push_front(3);
  l.push_front(2);
  l.push_front(1);
  l.insert(2, v.begin(), v.end());

  CHECK(l.size() == w.size());
  for (size_t i = 0; i < v.size(); ++i)
    CHECK(w[i] == l[i]);
}

TEST_CASE("ranked iterator insert frontback", "[list][ranked]")
{
  vector<int> v = { 23, 42, 21, 8, 11 };
  vector<int> w = { 23, 42, 1, 2, 3, 4, 21, 8, 11 };

  list::ranked::List<int> l;
  l.push_front(4);
  l.push_front(3);
  l.push_front(2);
  l.push_front(1);
  l.insert(0, v.begin(), v.begin()+2);
  l.insert(l.size(), v.begin()+2, v.end());

  CHECK(l.size() == w.size());
  for (size_t i = 0; i < v.size(); ++i)
    CHECK(w[i] == l[i]);
}

TEST_CASE("ranked move insert", "[list][ranked]")
{
  struct String {
    string s;
    bool move_constructed {false};
    String(const char *s) : s(s) {}
    String(String &&o) : s(std::move(o.s)), move_constructed(true) {}
    String(const String &o) : s(o.s) {}
  };
  list::ranked::List<String> l;
  String foo("foo");
  String bar("bar");
  l.push_front(foo);
  l.push_front(bar);
  l.insert(1, "42");
  REQUIRE(l.size() == 3);
  CHECK(l[0].s == "bar");
  CHECK(l[0].move_constructed == false);
  CHECK(l[1].s == "42");
  CHECK(l[1].move_constructed == true);
  CHECK(l[2].s == "foo");
  CHECK(l[2].move_constructed == false);
}

TEST_CASE("ranked remove range", "[list][ranked]")
{
  list::ranked::List<int> l;
  for (int i = 9; i>=0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 10);
  l.remove(2, 4);
  REQUIRE(l.size() == 6);
  CHECK(l[0] == 0);
  CHECK(l[1] == 1);
  CHECK(l[2] == 6);
  CHECK(l[3] == 7);
  CHECK_THROWS_AS(l.remove(6), std::out_of_range);
  CHECK_THROWS_AS(l.remove(5, 2), std::out_of_range);
  CHECK_THROWS_AS(l.remove(4, 3), std::out_of_range);
}

TEST_CASE("ranked clear", "[list][ranked]")
{
  list::ranked::List<int> l;
  for (int i = 9; i>=0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 10);
  l.clear();
  REQUIRE(l.size() == 0);
  for (int i = 9; i>=0; --i)
    l.push_front(i);
  REQUIRE(l.size() == 10);
  CHECK(l[0] == 0);
  CHECK(l[9] == 9);
}

TEST_CASE("ranked custom node", "[list][ranked]")
{
  struct Node : public list::ranked::Node_Base<Node, uint32_t, uint8_t> {
    int value_{0};
    list::ranked::List<int, 2, uint32_t, uint8_t, Node> children_;
    Node(int value, uint8_t level)
      :
        list::ranked::Node_Base<Node, uint32_t, uint8_t>(level),
        value_(value)
    {
    }
    //const Node &value() const { return *this; }
    const int &value() const { return value_; }
    int &value() { return value_; }
  };
  list::ranked::List<int, 2, uint32_t, uint8_t,
    Node> l;
  l.push_front(42);
  l.node(0).children_.push_front(23);
  CHECK(l[0] == 42);
  CHECK(l.node(0).children_[0] == 23);
}



// XXX add Allocator-Policy
