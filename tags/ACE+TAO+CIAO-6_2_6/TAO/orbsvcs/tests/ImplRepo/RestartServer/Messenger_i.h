/* -*- C++ -*- $Id$ */

#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Terminator;

//Class Messenger_i
class  Messenger_i : public virtual POA_Messenger
{
public:
  //Constructor
  Messenger_i (CORBA::ORB_ptr orb, Terminator &terminator);

  //Destructor
  virtual ~Messenger_i (void);

  virtual CORBA::Boolean send_message (
    const char * user_name,
    const char * subject,
    char *& message);

  virtual void abort (CORBA::Short delay_secs);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  int message_count_;

  Terminator& terminator_;
};


#endif /* MESSENGERI_H_  */
