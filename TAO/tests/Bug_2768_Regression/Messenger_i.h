#ifndef __MESSENGER_I_H__
#define __MESSENGER_I_H__

#include "MessengerS.h"

class Messenger_i : public POA_SimpleMessenger::Messenger
{
public:
  explicit Messenger_i (CORBA::ORB_ptr orb);
  Messenger_i (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

  void send () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif
