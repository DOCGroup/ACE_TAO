
//=============================================================================
/**
 *  @file   client.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef _QT_CLIENT_H
#define _QT_CLIENT_H

#include "testC.h"

#include <QtGui/qapplication.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qslider.h>
#include <QtGui/qpushbutton.h>

class Client : public QObject
{
  Q_OBJECT
public:
  /// ctor
  Client (CORBA::ORB_ptr orb,
          QApplication &app);

  ///Dtor..
  ~Client ();

  /// Adds the callbacks to the GUI underneath.....
  void create_widgets ();

  void show ();

  void parse_args (int argc, ACE_TCHAR *argv[]);

  /// A box widget..
  QHBoxLayout box_;
  QWidget mainwindow_;

 public slots:
   /// Two slot handlers for the two widgets that we have
   void remote_call (int val);
   void shutdown_call ();

private:
  /// A push button
  QPushButton *push_button_;

  /// A slider widget
  QSlider *slider_;

  /// The ORB
  CORBA::ORB_var orb_;

  LCD_Display_var server_;

  QApplication *qapp_;
};

#endif /* _QT_CLIENT_H */
