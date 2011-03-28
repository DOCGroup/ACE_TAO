
//=============================================================================
/**
 *  @file   client.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef _QT_CLIENT_H
#define _QT_CLIENT_H

#include "testC.h"

#include <qapplication.h>
#include <qvbox.h>
#include <qslider.h>
#include <qpushbutton.h>

class Client : public QObject
{
  Q_OBJECT
public:

  /// ctor
  Client (CORBA::ORB_ptr orb,
          QApplication &app);

  ///Dtor..
  ~Client (void);

  /// Adds the callbacks to the GUI underneath.....
  void create_widgets (void);

  void show (void);

  void parse_args (int argc, ACE_TCHAR *argv[]);

  /// A box widget..
  QVBox box_;

 public slots:
   /// Two slot handlers for the two widgets that we have
   void remote_call (int val);
   void shutdown_call (void);

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
