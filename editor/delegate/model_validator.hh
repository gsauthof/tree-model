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

#ifndef EDITOR_DELEGATE_MODEL_VALIDATOR_HH
#define EDITOR_DELEGATE_MODEL_VALIDATOR_HH

#include <QValidator>

class QAbstractItemModel;

namespace editor {
  namespace delegate {

    class Model_Validator : public QValidator {
      public:
        Model_Validator(const QAbstractItemModel &model,
            QObject *parent = nullptr);

        State validate(QString &input, int &pos) const override;

        void fixup(QString &input) const override;

      private:
        const QAbstractItemModel &model_;
    };

  } // delegate
} // editor

#endif // EDITOR_DELEGATE_MODEL_VALIDATOR_HH
