// -*- C++ -*-

#ifndef ACE_EXAMPLES_QOS_SESSION_PTR_H
#define ACE_EXAMPLES_QOS_SESSION_PTR_H

#include "ace/QoS/QoS_Session_Factory.h"

#include <memory>

struct QoS_Session_Deleter
{
  explicit QoS_Session_Deleter (ACE_QoS_Session_Factory &factory)
    : factory_ (factory)
  {
  }

  void operator() (ACE_QoS_Session *session) const
  {
    if (session != nullptr)
      {
        this->factory_.destroy_session (session);
        delete session;
      }
  }

private:
  ACE_QoS_Session_Factory &factory_;
};

using QoS_Session_Ptr =
  std::unique_ptr<ACE_QoS_Session, QoS_Session_Deleter>;

#endif /* ACE_EXAMPLES_QOS_SESSION_PTR_H */
