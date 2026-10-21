#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"

class  Messenger_i : public virtual POA_Messenger
{
public:
  Messenger_i () = default;
  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

  CORBA::Boolean send_message (
    char const * user_name,
    char const * subject,
    char *& message
  ) override;
};

#endif
