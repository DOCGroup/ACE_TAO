/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   CosECConsumer.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   Defines a simple Push Consumer.
//
// ============================================================================

#ifndef COSECCONSUMER_H_
#define COSECCONSUMER_H_

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"

class CosECConsumer : public POA_CosEventComm::PushConsumer
{
  // = TITLE
  //   CosECConsumer
  //
  // = DESCRIPTION
  //   The CosECConsumer is a simple PushConsumer that connects to the
  //   CosEC and receives events from it.

public:
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel,
             CORBA::Environment& TAO_TRY_ENV);
  // This method connects the consumer to the EC.

  void close (CORBA::Environment &TAO_TRY_ENV);
  // Disconnect from the EC.

  void connect (CORBA::Environment &TAO_TRY_ENV);
  // Connect the CosECConsumer to the EventChannel.

  void disconnect (CORBA::Environment &TAO_TRY_ENV);
  // Disconnect from the supplier, but do not forget about it or close
  // it.

  virtual void push (const CORBA::Any &data,
                     CORBA::Environment &TAO_TRY_ENV);
  // push the event to the consumer.

  virtual void disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV);
  // disconnect the consumer from the EC.

private:
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // We talk to the EC using this proxy.
};

#endif /* COSECCONSUMER_H_ */
