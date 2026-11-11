/* -*- C++ -*-  */

#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "orbsvcs/SSLIOPC.h"

#include "MessengerS.h"

class Messenger_i : public virtual POA_Messenger
{
public:
  Messenger_i (CORBA::ORB_ptr orb, SSLIOP::Current_ptr ssliop_current);
  ~Messenger_i () override = default;

  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

  CORBA::Boolean send_message (char const *user_name,
                               char const *subject,
                               char *&message) override;

  void shutdown (char const * user_name) override;

protected:
  CORBA::ORB_var orb_;
  SSLIOP::Current_var ssliop_current_;
};

#endif /* MESSENGERI_H_ */
