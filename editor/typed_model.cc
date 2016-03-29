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

#include "typed_model.hh"

#include <editor/delegate/value/local_time.hh>

#include <xfsx/xfsx.hh>
#include <xfsx/tap.hh>
#include <grammar/grammar.hh>

#include <QDebug>

using namespace std;

namespace editor {


  void Typed_Model::set_asn_filenames(const std::deque<std::string> &asn_filenames)
  {
    asn_filenames_ = asn_filenames;

    grammar::Grammar g = xfsx::tap::read_asn_grammar(asn_filenames_);

    tag_dereferencer_.push(xfsx::Klasse::APPLICATION,
        grammar::tag_closure(g, "LocalTimeStamp", 1),
        xfsx::Klasse::UNIVERSAL, 129u
        );

    name_translator_ = xfsx::Name_Translator(
        grammar::map_name_to_shape_klasse_tag(g));
  }

  // We return a custom Local_Time object instead of a QDateTime
  // such that we can setup a QDateTimeEdit editor with some non-default
  // properties.
  QVariant Typed_Model::data(const tree_model::Index &index, int role) const
  {
    try {
      if (role == Qt::EditRole && index.column() == 1) {
        auto tag_str = index.attribute(0).data().toString().toStdString();
        auto shape_klasse_tag = name_translator_.translate(make_pair(
              tag_str.data(), tag_str.data() + tag_str.size()));
        auto klasse_tag = tag_dereferencer_.dereference(
            std::get<1>(shape_klasse_tag), std::get<2>(shape_klasse_tag));
        if (klasse_tag.first == xfsx::Klasse::UNIVERSAL
            && klasse_tag.second == 129u) {
          return QVariant::fromValue(
              delegate::value::Local_Time(
                tree_model::XML::data(index, role).toString()));
        }
      }
    } catch (const out_of_range &e) {
      qCritical() << tr("Could not translate tag");
    }
    return tree_model::XML::data(index, role);
  }

} // editor
