/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * Defines a simple Push Consumer.
 *
 *
 */
//=============================================================================


#ifndef COSECCONSUMER_H_
#define COSECCONSUMER_H_

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"

/**
 * @class Consumer
 *
 * @brief Consumer
 *
 * The Consumer is a simple PushConsumer that connects to the
 * CosEC and receives events from it.
 */
class Consumer : public POA_CosEventComm::PushConsumer
{

public:
  /// This method connects the consumer to the EC.
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel,
             CORBA::ORB_ptr orb);

  /// Disconnect from the EC.
  void close (void);

  /// Connect the Consumer to the EventChannel.
  void connect (void);

  /// Disconnect from the supplier, but do not forget about it or close
  /// it.
  void disconnect (void);

  /// push the event to the consumer.
  virtual void push (const CORBA::Any &data);

  /// disconnect the consumer from the EC.
  virtual void disconnect_push_consumer (void);

private:
  /// The orb that we as using.
  CORBA::ORB_var orb_;

  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
};

#endif /* COSECCONSUMER_H_ */
