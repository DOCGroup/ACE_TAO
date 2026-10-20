#ifndef EXPLOIT_PUBLISHER_IMPL_H
#define EXPLOIT_PUBLISHER_IMPL_H

#include "publisherS.h"
#include "subscriberC.h"

class Publisher_impl : public POA_Publisher
{
public:
  explicit Publisher_impl (CORBA::ORB_ptr orb);
  ~Publisher_impl () override;

  Publisher_impl (Publisher_impl const &) = delete;
  Publisher_impl &operator= (Publisher_impl const &) = delete;
  Publisher_impl (Publisher_impl &&) = delete;
  Publisher_impl &operator= (Publisher_impl &&) = delete;

  void subscribe (::Subscriber_ptr subscriber) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
  class Worker;
  friend class Worker;
  Worker *worker;
};

#endif
