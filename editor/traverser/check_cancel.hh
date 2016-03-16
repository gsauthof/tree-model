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
#ifndef EDITOR_TRAVERSER_CHECK_CANCEL_HH
#define EDITOR_TRAVERSER_CHECK_CANCEL_HH


#include <xfsx/traverser/traverser.hh>

#include <exception>
#include <functional>

namespace editor {

  namespace traverser {

    class Cancel_Exception : public std::exception {
      private:
      public:
        using std::exception::exception;
    };

    struct Check_Cancel {

      unsigned counter_ {0};
      unsigned epoche_ {10000};
      std::function<bool()> was_canceled_fn_;

      template <typename T, typename Proxy>
        xfsx::traverser::Hint operator()(const Proxy &p, const T &t)
        {
          ++counter_;
          if (counter_ % epoche_ == 0
              && was_canceled_fn_ && was_canceled_fn_())
            throw Cancel_Exception();

          // when traversing with multiple function, the traversal
          // stops iff all of them return STOP
          return xfsx::traverser::Hint::STOP;
        }
    };

  }
}

#endif
