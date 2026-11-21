/* -*- C++ -*- */
#ifndef QTREACTOR_TEST_H
#define QTREACTOR_TEST_H

#ifdef ACE_HAS_QT5
#include /**/ <QtWidgets/QApplication>
#else
#include /**/ <QtGui/QApplication>
#endif

#include <QtCore/qtimer.h>

class QTestApplication: public QApplication
{
  Q_OBJECT
public:
  using inherited = QApplication;
public:
  QTestApplication( int argc, char *argv[] );
  virtual void exec( int msec = 0 );
public slots:
  /// Slot connected to finishTimer_ to finish the test.
  virtual void finishTest();
private:
  /// Timer to finish the test.
  QTimer finishTimer_;
};

#endif /*QTREACTOR_TEST_H*/
