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
  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

  //Constructor
  Messenger_i () = default;

  //Destructor
  ~Messenger_i () override = default;


  void send_message (CORBA::OctetSeq const& user_name) override;
};

#endif /* MESSENGERI_H_  */
