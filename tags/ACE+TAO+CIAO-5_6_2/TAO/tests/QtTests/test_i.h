// $Id$
// ============================================================================
//
// = LIBRARY
//   TAO/tests/QtTests
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef _QT_TEST_I_H
#define _QT_TEST_I_H

#include "testS.h"

#include <qapplication.h>
#include <qobject.h>

class LCD_Display_imp : public QObject,
                        public POA_LCD_Display

{
  Q_OBJECT
  // = TITLE
  //   LCD_Display implementation class
  //
  // = DESCRIPTION
  //   Implements the LCD_Display interface in test.idl
  //
public:
  LCD_Display_imp (CORBA::ORB_ptr);

  // ctor

  void shutdown (void);

  void send_val (CORBA::Long val);

 signals:
  void set_value (int val);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* SERVER_TEST_I_H */
