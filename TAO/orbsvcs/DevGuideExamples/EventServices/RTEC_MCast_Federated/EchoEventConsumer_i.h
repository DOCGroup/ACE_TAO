// EchoEventConsumer_i.h
// Implements a PushConsumer.

#ifndef _EchoEventConsumer_i_h_
#define _EchoEventConsumer_i_h_

#include "orbsvcs/RtecEventCommS.h"// for POA_CosEventComm::PushConsumer
#include "orbsvcs/RtecEventChannelAdminC.h"

class EchoEventConsumer_i : public virtual POA_RtecEventComm::PushConsumer
{
  public:
    // Constructor
    EchoEventConsumer_i(CORBA::ORB_ptr orb,
                        RtecEventChannelAdmin::ProxyPushSupplier_ptr supplier,
                        int event_limit);

    // Override operations from PushConsumer interface.
    EchoEventConsumer_i (EchoEventConsumer_i const &) = delete;
    EchoEventConsumer_i (EchoEventConsumer_i &&) = delete;
    EchoEventConsumer_i &operator= (EchoEventConsumer_i const &) = delete;
    EchoEventConsumer_i &operator= (EchoEventConsumer_i &&) = delete;

    void push (RtecEventComm::EventSet const & events) override;

    void disconnect_push_consumer () override;

  private:
    CORBA::ORB_var orb_;
    RtecEventChannelAdmin::ProxyPushSupplier_var supplier_;
    int event_limit_;
};

#endif // _EchoEventConsumer_i_h_
