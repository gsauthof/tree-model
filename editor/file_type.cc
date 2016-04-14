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

#include "file_type.hh"

#include <QString>
#include <QObject>
#include <QCoreApplication>

#include <map>

using namespace std;

namespace editor {

  File_Type::File_Type() =default;
  File_Type::File_Type(Major major) : major_(major) {}
  File_Type::File_Type(Major major, Minor minor)
    : major_(major), minor_(minor) {}

  File_Type::Major File_Type::major() const { return major_; }
  File_Type::Minor File_Type::minor() const { return minor_; }
  const std::deque<std::string> &File_Type::asn_filenames() const
  {
    return asn_filenames_;
  }
  void File_Type::set_asn_filenames(std::deque<std::string> &&asn_filenames)
  {
    asn_filenames_ = std::move(asn_filenames);
  }
  const std::deque<std::string> &File_Type::constraint_filenames() const
  {
    return constraint_filenames_;
  }
  void File_Type::set_constraint_filenames(std::deque<std::string> &&constraint_filenames)
  {
    constraint_filenames_ = std::move(constraint_filenames);
  }

  void File_Type::register_meta_type()
  {
    qRegisterMetaType<File_Type>();
    qRegisterMetaType<File_Type>("File_Type");
  }

  // We have to make sure that the translate (or tr) methods are
  // called after the translators are initialized. Thus, we can't
  // rely on static initialization.
  static QString xml_filter()
  {
    return QCoreApplication::translate("name_filter", "XML files (*.xml)");
  }
  static QString any_filter()
  {
    return QCoreApplication::translate("name_filter", "Any files (*)");
  }

  // We have to make sure that the translate (or tr) methods are
  // called after the translators are initialized. Thus, we can't
  // rely on static initialization.
  static map<QString, File_Type> filter_to_type_map()
  {
    return  {
      { xml_filter(),
        {File_Type::XML} },
      { QCoreApplication::translate("name_filter", "TAP files (CD* TD*)"),
        {File_Type::BER, File_Type::TAP} },
      { QCoreApplication::translate("name_filter", "RAP files (RC*)"),
        {File_Type::BER, File_Type::RAP} },
      { QCoreApplication::translate("name_filter", "NRTRDE files (NR*)") ,
        {File_Type::BER, File_Type::NRT} },
      { QCoreApplication::translate("name_filter", "BER files (*.ber)"),
        {File_Type::BER} },
      { any_filter(),
        {} }
    };
  }

  QStringList all_name_filters()
  {
    QStringList r;
    auto m = filter_to_type_map();
    for (auto &i : m)
      r << i.first;
    return r;
  }
  QString default_open_name_filter()
  {
    return xml_filter();
  }

  QString File_Type::default_name_filter() const
  {
    auto m = filter_to_type_map();
    for (auto &i : m)
      if (i.second.major() == major_
            && (minor_ == ANY_MINOR || i.second.minor() == minor_))
        return i.first;
    return any_filter();
  }

  File_Type File_Type::construct_from_name_filter(const QString &name)
  {
    auto m = filter_to_type_map();
    return m.at(name);
  }

}
