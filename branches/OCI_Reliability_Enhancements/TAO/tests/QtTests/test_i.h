//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef _QT_TEST_I_H
#define _QT_TEST_I_H

#include "testS.h"

#include <QtGui/qapplication.h>
#include <QtCore/qobject.h>

class LCD_Display_imp : public QObject,
                        public POA_LCD_Display

{
  /**
   * = TITLE
   *   LCD_Display implementation class
   *
   * = DESCRIPTION
   *   Implements the LCD_Display interface in test.idl
   *
   */
  Q_OBJECT
public:
  LCD_Display_imp (CORBA::ORB_ptr);

  // ctor

  void shutdown (void);

  void send_val (CORBA::Long val);

 signals:
  void set_value (int val);

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* SERVER_TEST_I_H */
