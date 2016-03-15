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
#ifndef TREE_MODEL_QMI_PROXY_HH
#define TREE_MODEL_QMI_PROXY_HH

#include <tree_model/df_qmi_traverser.hh>

#include <xfsx/xfsx.hh>
#include <xfsx/integer.hh>

#include <QModelIndex>

#include <utility>
#include <string>

namespace tree_model {

  class QMI_Proxy {
    private:
      const xfsx::Name_Translator &translator_;
    public:
      QMI_Proxy(const xfsx::Name_Translator &translator)
        : translator_(translator) {}

      xfsx::Tag_Int tag(const DF_QMI_Traverser &t) const
      {
        const QModelIndex &i = *t;
        auto s = i.data().toString().toStdString();
        auto shape_klasse_tag = translator_.translate(
            std::make_pair(s.data(), s.data() + s.size()));
        return std::get<2>(shape_klasse_tag);
      }
      uint32_t height(const DF_QMI_Traverser &t) const
      {
        return t.height();
      }
      void string(const DF_QMI_Traverser &t, std::string &s) const
      {
        const QModelIndex &i = *t;
        s = i.sibling(i.row(), 1).data().toString().toStdString();
      }
      uint64_t uint64(const DF_QMI_Traverser &t) const
      {
        const QModelIndex &i = *t;
        auto s = i.sibling(i.row(), 1).data().toString().toStdString();
        return xfsx::integer::range_to_uint64(std::make_pair(s.data(),
              s.data() + s.size()));
      }
      uint32_t uint32(const DF_QMI_Traverser &t) const
      {
        const QModelIndex &i = *t;
        auto s = i.sibling(i.row(), 1).data().toString().toStdString();
        return xfsx::integer::range_to_uint32(std::make_pair(s.data(),
              s.data() + s.size()));
      }

      void advance(DF_QMI_Traverser &t) { t.advance(); }
      void skip_children(DF_QMI_Traverser &t) { t.skip_children(); }
      bool eot(const DF_QMI_Traverser &t) const { return t.eot(); }
  };

} // tree_model

#endif

