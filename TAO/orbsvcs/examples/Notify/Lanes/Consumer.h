/* -*- C++ -*- */
/**
 *  @file Consumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_CONSUMER_H
#define TAO_Notify_CONSUMER_H

#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include <string>

/**
 * @class TAO_Notify_Lanes_Consumer
 *
 * @brief Consumer
 */

class TAO_Notify_Lanes_Consumer
  : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
  /// Constructor
  explicit TAO_Notify_Lanes_Consumer (TAO_Notify_ORB_Objects& orb_objects);
  TAO_Notify_Lanes_Consumer (TAO_Notify_Lanes_Consumer const&) = delete;
  TAO_Notify_Lanes_Consumer& operator= (TAO_Notify_Lanes_Consumer const&) = delete;
  TAO_Notify_Lanes_Consumer (TAO_Notify_Lanes_Consumer&&) = delete;
  TAO_Notify_Lanes_Consumer& operator= (TAO_Notify_Lanes_Consumer&&) = delete;

  /// Init
  void init (PortableServer::POA_ptr poa,
             CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
             std::string const& event_type);

protected:
  // = Data members

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_ {};

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};

  // POA.
  PortableServer::POA_var default_POA_ {};

  // The Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var admin_ {};

  /// The Type the Consumer should subscribe to.
  std::string event_type_;

  // = Methods
  /// Destructor
  ~TAO_Notify_Lanes_Consumer () override = default;

  // = ServantBase operations
  PortableServer::POA_ptr _default_POA () override;

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect ();

  /// Disconnect the supplier.
  void disconnect ();

  /// Deactivate.
  void deactivate ();

  // = NotifyPublish method
  void offer_change (CosNotification::EventTypeSeq const& added,
                     CosNotification::EventTypeSeq const& removed) override;

  // = StructuredPushSupplier methods
  void push_structured_event (CosNotification::StructuredEvent const& notification) override;

  void disconnect_structured_push_consumer () override;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_H */
