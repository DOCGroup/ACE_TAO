// -*- C++ -*-

#ifndef TESTI_H_
#define TESTI_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Terminator;

//Class Test_i
class  Test_i : public virtual POA_Messenger
{
public:
  //Constructor
  Test_i (Terminator &terminator);

  //Destructor
  virtual ~Test_i (void);

  virtual CORBA::Boolean send_message (
    const char * user_name,
    const char * subject,
    char *& message);

private:
  int message_count_;

  Terminator& terminator_;
};


#endif /* TESTI_H_  */
