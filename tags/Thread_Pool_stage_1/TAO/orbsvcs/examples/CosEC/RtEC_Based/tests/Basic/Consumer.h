/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   Consumer.h
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

class Consumer : public POA_CosEventComm::PushConsumer
{
  // = TITLE
  //   Consumer
  //
  // = DESCRIPTION
  //   The Consumer is a simple PushConsumer that connects to the
  //   CosEC and receives events from it.

public:
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel,
             CORBA::ORB_ptr orb,
             CORBA::Environment& ACE_TRY_ENV);
  // This method connects the consumer to the EC.

  void close (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the EC.

  void connect (CORBA::Environment &ACE_TRY_ENV);
  // Connect the Consumer to the EventChannel.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier, but do not forget about it or close
  // it.

  virtual void push (const CORBA::Any &data,
                     CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ));
  // push the event to the consumer.

  virtual void disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  // disconnect the consumer from the EC.

private:
  CORBA::ORB_var orb_;
  // The orb that we as using.

  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // We talk to the EC using this proxy.
};

#endif /* COSECCONSUMER_H_ */
