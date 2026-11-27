#ifndef MESSENGER_I_H_
#define MESSENGER_I_H_

#include "MessengerS.h"

class  Messenger_i : public virtual POA_Messenger
{
public:
  Messenger_i () = default;

  CORBA::Boolean send_message (Message *&msg) override;

  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;
};

#endif
