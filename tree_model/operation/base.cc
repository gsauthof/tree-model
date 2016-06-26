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
#include "base.hh"

#include <string>
#include <stdexcept>

using namespace std;

namespace tree_model {
  namespace operation {

    Base::~Base() =default;

    void Base::begin_forward()
    {
      if (state_ != 1)
        throw logic_error("Can't forward operation from state "
            + std::to_string(state_));
      state_ = 2;
    }
    void Base::begin_rewind()
    {
      if (state_ != 2)
        throw logic_error("Can't rewind operation from state "
            + std::to_string(state_));
      state_ = 1;
    }
    void Base::begin_finalize()
    {
      if (state_ != 0)
        throw logic_error("Can't finalize operation from state "
            + std::to_string(state_));
      state_ = 2;
    }
    Base::State Base::state() const
    {
      return State(state_);
    }

  }
}
