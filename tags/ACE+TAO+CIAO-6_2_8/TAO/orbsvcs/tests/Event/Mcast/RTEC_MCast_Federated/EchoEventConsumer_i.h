// $Id$

// EchoEventConsumer_i.h
// Implements a PushConsumer.

#ifndef _EchoEventConsumer_i_h_
#define _EchoEventConsumer_i_h_

#include <orbsvcs/RtecEventCommS.h>    // for POA_CosEventComm::PushConsumer

class EchoEventConsumer_i : public virtual POA_RtecEventComm::PushConsumer
{
  public:
    // Constructor
    EchoEventConsumer_i(CORBA::ORB_ptr orb, int event_limit);

    // Override operations from PushConsumer interface.
    virtual void push(const RtecEventComm::EventSet& events);

    virtual void disconnect_push_consumer();

  private:
    CORBA::ORB_var orb_;
    int event_limit_;
};

#endif // _EchoEventConsumer_i_h_
