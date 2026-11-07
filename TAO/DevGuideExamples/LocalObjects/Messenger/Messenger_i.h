#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//Class Messenger_i
class  Messenger_i : public virtual Messenger,
         public virtual ::CORBA::LocalObject
{
public:
  //Constructor
  Messenger_i ();

  //Destructor
  ~Messenger_i () override = default;

  CORBA::Boolean send_message (
    char const *user_name,
    char const *subject,
    char *& message) override;

  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;
};

#endif /* MESSENGERI_H_  */
