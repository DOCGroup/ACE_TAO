/* -*- C++ -*- */
/**
 *  @file Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_CONSUMER_H
#define TAO_Notify_CONSUMER_H

#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/CosNotifyCommC.h"

/**
 * @class TAO_Notify_Lanes_Consumer
 *
 * @brief Consumer
 *
 */

class TAO_Notify_Lanes_Consumer : public POA_CosNotifyComm::StructuredPushConsumer, public PortableServer::RefCountServantBase
{
public:
  /// Constuctor
  TAO_Notify_Lanes_Consumer (TAO_Notify_ORB_Objects& orb_objects);

  /// Init
  void init (PortableServer::POA_var& poa, CosNotifyChannelAdmin::ConsumerAdmin_var& admin, ACE_CString& event_type ACE_ENV_ARG_DECL);

  /// Run
  void run (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

protected:
  // = Data members

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  // POA.
  PortableServer::POA_var default_POA_;

  // The Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var admin_;

  /// The Type the Consumer should subscribe to.
  ACE_CString event_type_;

  // = Methods
  /// Destructor
  virtual ~TAO_Notify_Lanes_Consumer (void);

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (ACE_ENV_SINGLE_ARG_DECL);

  /// Disconnect the supplier.
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Deactivate.
  void deactivate (ACE_ENV_SINGLE_ARG_DECL);

  // = NotifyPublish method
  virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

  virtual void disconnect_structured_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL
        )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_H */
