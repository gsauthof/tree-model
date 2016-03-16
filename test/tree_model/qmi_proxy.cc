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
#include <test/test.hh>

#include <xfsx/xml_writer_arguments.hh>
#include <xfsx/tap/traverser.hh>
#include <xfsx/traverser/traverser.hh>

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

#include <editor/traverser/qmi_proxy.hh>
#include <editor/traverser/check_cancel.hh>
#include <editor/command/open_xml.hh>

#include <memory>
#include <deque>

using namespace std;

TEST_CASE("proxy basic", "[qmi][dfs][proxy][xml][tree-model]" )
{
  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);
  auto root = m->index(0, 0);

  deque<string> asn_filenames = { test::path::in()
      + "/../../libgrammar/test/in/asn1/tap_3_12_strip.asn1"  };
  xfsx::xml::Pretty_Writer_Arguments pargs(asn_filenames);

  editor::traverser::QMI_Proxy p(pargs.name_translator);
  xfsx::tap::traverser::Audit_Control_Info aci;

  tree_model::DF_QMI_Traverser t(root);

  aci(p, t);

  CHECK(aci.count() == 4);
  CHECK(aci.sum() == 71200);
  CHECK(aci.first_timestamp().first == "20140301140342");
  CHECK(aci.first_timestamp().second == "+0200");
  CHECK(aci.last_timestamp().first == "20140302150800");
  CHECK(aci.last_timestamp().second == "+0200");
}

struct Fake_Progress {
  unsigned counter_ {0};
  unsigned limit_ {0};
  Fake_Progress(unsigned limit)
    :
      limit_(limit)
  {
  }
  bool wasCanceled()
  {
    if (counter_ == limit_)
      return true;
    ++counter_;
    return false;
  }
};

struct Collect {
  deque<xfsx::Tag_Int> tags;

  template <typename T, typename Proxy>
  xfsx::traverser::Hint operator()(const Proxy &p, const T &t)
  {
    tags.push_back(p.tag(t));
    return xfsx::traverser::Hint::DESCEND;
  }
};


TEST_CASE("proxy cancel", "[qmi][dfs][proxy][xml][tree-model]" )
{
  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);
  auto root = m->index(0, 0);

  deque<string> asn_filenames = { test::path::in()
      + "/../../libgrammar/test/in/asn1/tap_3_12_strip.asn1"  };
  xfsx::xml::Pretty_Writer_Arguments pargs(asn_filenames);

  Fake_Progress fake_progress(3);
  editor::traverser::QMI_Proxy p(pargs.name_translator);

  xfsx::traverser::Traverse aci;
  Collect collect;
  editor::traverser::Check_Cancel check_cancel;
  check_cancel.epoche_ = 2;
  check_cancel.was_canceled_fn_ = [&fake_progress](){
    return fake_progress.wasCanceled(); };

  tree_model::DF_QMI_Traverser t(root);

  bool was_canceled = false;
  try {
    aci(p, t, collect, check_cancel);
  } catch (const editor::traverser::Cancel_Exception &e) {
    was_canceled = true;
  }

  CHECK(collect.tags.size() == 8);
  CHECK(was_canceled);
}

