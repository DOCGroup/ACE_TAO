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


#if defined(ACE_HAS_QT)

#include "tao/qt_resource.h"
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

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void send_val (CORBA::Long val
                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

 signals:
  void set_value (int val);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_QT */

#endif /* SERVER_TEST_I_H */
