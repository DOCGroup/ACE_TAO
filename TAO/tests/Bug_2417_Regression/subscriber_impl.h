#ifndef EXPLOIT_PUBLISHER_IMPL_H
#define EXPLOIT_PUBLISHER_IMPL_H

#include "subscriberS.h"

class Subscriber_impl : public POA_Subscriber
{
public:
  explicit Subscriber_impl (CORBA::ORB_ptr orb);
  ~Subscriber_impl () override;

  Subscriber_impl (Subscriber_impl const &) = delete;
  Subscriber_impl &operator= (Subscriber_impl const &) = delete;
  Subscriber_impl (Subscriber_impl &&) = delete;
  Subscriber_impl &operator= (Subscriber_impl &&) = delete;

  void onData (::CORBA::Double value) override;

  ::CORBA::Boolean isAlive () override;

  void shutdown ();

private:
  CORBA::ORB_var orb_;
  unsigned int count;
};

#endif
