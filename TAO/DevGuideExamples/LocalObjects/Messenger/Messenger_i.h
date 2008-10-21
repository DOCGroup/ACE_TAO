#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"
#include <tao/LocalObject.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//Class Messenger_i
class  Messenger_i : public virtual Messenger,
		     public virtual TAO_Local_RefCounted_Object
{
public:
  //Constructor 
  Messenger_i (void);
  
  //Destructor 
  virtual ~Messenger_i (void);
  
virtual CORBA::Boolean send_message (
    const char * user_name,
    const char * subject,
    char *& message
  )
  throw(CORBA::SystemException);

};


#endif /* MESSENGERI_H_  */
