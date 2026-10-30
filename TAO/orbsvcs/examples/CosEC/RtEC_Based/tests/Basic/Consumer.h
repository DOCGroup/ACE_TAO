/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Consumer.h
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * Defines a simple Push Consumer.
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
  Consumer () = default;
  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  /// This method connects the consumer to the EC.
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel,
             CORBA::ORB_ptr orb);

  /// Disconnect from the EC.
  void close ();

  /// Connect the Consumer to the EventChannel.
  void connect ();

  /// Disconnect from the supplier, but do not forget about it or close
  /// it.
  void disconnect ();

  /// push the event to the consumer.
  void push (CORBA::Any const &data) override;

  /// disconnect the consumer from the EC.
  void disconnect_push_consumer () override;

private:
  /// The orb that we as using.
  CORBA::ORB_var orb_;

  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
};

#endif /* COSECCONSUMER_H_ */
