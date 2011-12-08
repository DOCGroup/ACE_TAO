// $Id$

#ifndef MESSENGER_I_H_
#define MESSENGER_I_H_

#include "MessengerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//Class Messenger_i
class  Messenger_i : public virtual POA_Messenger
{
public:
  Messenger_i (CORBA::ORB_ptr orb);

  virtual ~Messenger_i (void);

  virtual void send_message (const CORBA::OctetSeq & user_name);

  virtual void shutdown (void);
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

};


#endif /* MESSENGERI_H_  */
