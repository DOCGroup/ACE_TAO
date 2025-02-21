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
  typedef QApplication inherited;
public:
  QTestApplication( int argc, char *argv[] );
  virtual void exec( int msec = 0 );
public slots:
  virtual void finishTest(); //!< slot to finish the test, connected to finishTimer_
private:
  QTimer finishTimer_; //!< timer to finish the test
};

#endif /*QTREACTOR_TEST_H*/
