/* -*- C++ -*-  */

#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Class Messenger_i
class Messenger_i : public virtual POA_Messenger
{
public:
  // Constructor
  Messenger_i (CORBA::ORB_var orb);

  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

  ~Messenger_i () override = default;

  CORBA::Boolean send_message (char const *user_name,
                               char const *subject,
                               char *&message) override;

  CORBA::Boolean call_message (char const *user_name) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* MESSENGERI_H_  */
