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
#ifndef TREE_MODEL_OPERATION_BASE_HH
#define TREE_MODEL_OPERATION_BASE_HH

class QAbstractItemModel;

namespace tree_model {
  namespace operation {

    class Base {
      private:
        // 3 states: 0, 1, 2
        // begin: 0
        //
        // 0 -> 2 | begin_finalize()
        // 2 -> 1 | begin_rewind()
        // 1 -> 2 | begin_forward()
        unsigned state_ {0};
      public:
        enum State { INITIALIZED = 0, REWOUND, FORWARDED };
        virtual ~Base();
        virtual void forward(QAbstractItemModel &model) = 0;
        virtual void rewind(QAbstractItemModel &model) = 0;

        State state() const;
      protected:
        // call them when subclassing ...
        void begin_forward();
        void begin_rewind();
        void begin_finalize();
    };

  }
}

#endif
