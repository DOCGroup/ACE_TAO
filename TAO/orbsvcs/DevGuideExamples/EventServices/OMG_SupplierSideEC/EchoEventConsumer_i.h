// EchoEventConsumer_i.h
// Implements a PushConsumer.

#ifndef _EchoEventConsumer_i_h_
#define _EchoEventConsumer_i_h_

#include "orbsvcs/CosEventCommS.h"// for POA_CosEventComm::PushConsumer
#include "orbsvcs/CosEventChannelAdminC.h"

class EchoEventConsumer_i : public virtual POA_CosEventComm::PushConsumer
{
  public:
    // Constructor
    EchoEventConsumer_i (CORBA::ORB_ptr orb,
                         CosEventChannelAdmin::ProxyPushSupplier_ptr supplier,
                         int event_limit);

    EchoEventConsumer_i (EchoEventConsumer_i const &) = delete;
    EchoEventConsumer_i (EchoEventConsumer_i &&) = delete;
    EchoEventConsumer_i &operator= (EchoEventConsumer_i const &) = delete;
    EchoEventConsumer_i &operator= (EchoEventConsumer_i &&) = delete;

    // Override operations from PushConsumer interface.
    void push (CORBA::Any const & data) override;

    void disconnect_push_consumer () override;

  private:
    CORBA::ORB_var orb_;
    CosEventChannelAdmin::ProxyPushSupplier_var supplier_;
    int event_limit_;
};

#endif // _EchoEventConsumer_i_h_
