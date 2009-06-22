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
  //Constructor
  Messenger_i (void);

  //Destructor
  virtual ~Messenger_i (void);


  virtual void send_message (const CORBA::OctetSeq & user_name);

};


#endif /* MESSENGERI_H_  */
