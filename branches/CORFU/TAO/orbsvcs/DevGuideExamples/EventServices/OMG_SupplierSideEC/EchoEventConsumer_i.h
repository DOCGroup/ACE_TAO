// $Id$

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
    EchoEventConsumer_i(CORBA::ORB_ptr orb,
                        CosEventChannelAdmin::ProxyPushSupplier_ptr supplier,
                        int event_limit);

    // Override operations from PushConsumer interface.
    virtual void push(const CORBA::Any & data);

    virtual void disconnect_push_consumer();

  private:
    CORBA::ORB_var orb_;
    CosEventChannelAdmin::ProxyPushSupplier_var supplier_;
    int event_limit_;
};

#endif // _EchoEventConsumer_i_h_
