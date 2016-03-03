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
#include "select_open.hh"

#include <QDebug>
#include <QTimer>
#include <QMainWindow>
#include <QSignalSpy>
#include <QDir>
#include <QEventLoop>

#include <editor/gui_command/select_open.hh>
#include <editor/gui_command/open.hh>
#include <editor/file_type.hh>
#include <test/test.hh>

#include <string>
#include <boost/filesystem.hpp>

namespace test {
  namespace editor {
    namespace gui_command {

      Select_Open::Select_Open(QObject *parent)
        :
          QObject(parent)
      {
        ::editor::File_Type::register_meta_type();
      }
      void Select_Open::test_cancel()
      {
        boost::filesystem::current_path(test::path::in());
        QMainWindow w;
        ::editor::gui_command::Select_Open so(&w);
        QSignalSpy spy_finished(&so, SIGNAL(finished()));
        QSignalSpy spy_opened(&so, SIGNAL(file_opened(QString)));
        QTimer::singleShot(200, [&w]{
            QTest::keyClick(w.focusWidget(), Qt::Key_Escape, Qt::NoModifier, 100);
            });
        so.open();
        QCOMPARE(spy_finished.count(), 0);
        QCOMPARE(spy_opened.count(), 0);
      }

      void Select_Open::test_open()
      {
        boost::filesystem::current_path(test::path::in());
        QMainWindow w;
        ::editor::gui_command::Select_Open so(&w);
        QSignalSpy spy_finished(&so, SIGNAL(finished()));
        qRegisterMetaType<QAbstractItemModel*>();
        QSignalSpy spy_model(&so,
            SIGNAL(item_tree_model_created(QAbstractItemModel*)));
        QSignalSpy spy_opened(&so, SIGNAL(file_opened(QString)));
        QTimer::singleShot(200, [&w]{
            QTest::keyClicks(w.focusWidget(), "small.xml", Qt::NoModifier, 100);
            QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 500);
            });
        so.open();
        QCOMPARE(spy_finished.count(), 1);
        QCOMPARE(spy_model.count(), 1);
        QCOMPARE(spy_opened.count(), 1);
        QVERIFY(spy_opened.front().at(0).toString().endsWith("small.xml"));
        QVERIFY(qvariant_cast<QAbstractItemModel*>(spy_model.front().at(0))
              != nullptr);

        delete qvariant_cast<QAbstractItemModel*>(spy_model.front().at(0));
      }

      void Select_Open::test_abort()
      {
        std::string input(test::path::in() + "/small.xml");
        QMainWindow w;
        ::editor::gui_command::Open so(&w);
        so.set_delay(10000);

        QSignalSpy spy_finished(&so, SIGNAL(finished()));
        qRegisterMetaType<QAbstractItemModel*>();
        QSignalSpy spy_model(&so,
            SIGNAL(item_tree_model_created(QAbstractItemModel*)));
        QSignalSpy spy_opened(&so, SIGNAL(file_opened(QString)));
        QString name;
        QTimer::singleShot(5000, [&w, &name]{
            QWindow *x = QGuiApplication::focusWindow();
            if (x)
              name = x->objectName();
            QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 500);
            });
        so.open(input.c_str());
        QCOMPARE(name, QString("editor::Progress_DialogClassWindow"));
        QCOMPARE(spy_finished.count(), 1);
        QCOMPARE(spy_model.count(), 0);
        QCOMPARE(spy_opened.count(), 0);
      }

      void Select_Open::test_fail()
      {
        std::string input(test::path::in() + "/small_doesnotexist.xml");
        QMainWindow w;
        ::editor::gui_command::Open so(&w);

        QSignalSpy spy_finished(&so, SIGNAL(finished()));
        qRegisterMetaType<QAbstractItemModel*>();
        QSignalSpy spy_model(&so,
            SIGNAL(item_tree_model_created(QAbstractItemModel*)));
        QSignalSpy spy_opened(&so, SIGNAL(file_opened(QString)));
        QString title;
        QTimer::singleShot(1000, [&w, &title]{
            QWindow *x = QGuiApplication::focusWindow();
            if (x)
              title = x->title();
            QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 500);
            });
        so.open(input.c_str());
        QEventLoop e;
        QTimer::singleShot(3000, &e, &QEventLoop::quit);
        e.exec();
        QCOMPARE(title, QString("Could not open file"));
        QCOMPARE(spy_finished.count(), 1);
        QCOMPARE(spy_model.count(), 0);
        QCOMPARE(spy_opened.count(), 0);
      }

      void Select_Open::test_fail_no_popup()
      {
        std::string input(test::path::in() + "/small_doesnotexist.xml");
        QMainWindow w;
        ::editor::gui_command::Open so(&w);

        QSignalSpy spy_finished(&so, SIGNAL(finished()));
        qRegisterMetaType<QAbstractItemModel*>();
        QSignalSpy spy_model(&so,
            SIGNAL(item_tree_model_created(QAbstractItemModel*)));
        QSignalSpy spy_opened(&so, SIGNAL(file_opened(QString)));
        QString title;
        QTimer::singleShot(1000, [&w, &title]{
            QWindow *x = QGuiApplication::focusWindow();
            if (x)
              title = x->title();
            });
        int window_count = 0;
        QTimer::singleShot(2000, [&w, &window_count]{
            window_count = QGuiApplication::topLevelWindows().size();
            QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 500);
            });
        so.open(input.c_str());
        QEventLoop e;
        QTimer::singleShot(3000, &e, &QEventLoop::quit);
        e.exec();
        QCOMPARE(title, QString("Could not open file"));
        QCOMPARE(spy_finished.count(), 1);
        QCOMPARE(spy_model.count(), 0);
        QCOMPARE(spy_opened.count(), 0);
        QCOMPARE(window_count, 2);
      }


    }
  }
}

// QTEST_MAIN expands to
// int main(int argc, char *argv[])
// {
//   QApplication app(argc, argv);
//   app.setAttribute(Qt::AA_Use96Dpi, true);
//   test::editor::gui_command::Open tc;
//   return QTest::qExec(&tc, argc, argv);
// }

QTEST_MAIN(test::editor::gui_command::Select_Open)
