/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_StructuredPushConsumer.h
//
// = DESCRIPTION
//   This is a utility class is to be used by clients of the Notification
//   Service to implement the servant for CosNotifyComm::PushConsumer
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_STRUCTUREDPUSHCONSUMER_H
#define NOTIFY_STRUCTUREDPUSHCONSUMER_H
#include "ace/pre.h"

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_StructuredPushConsumer : public POA_CosNotifyComm::StructuredPushConsumer, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   Notify_StructuredPushConsumer
  //
  // = DESCRIPTION
  //   Implements a common servant for the StructuredPushConsumer.
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_StructuredPushConsumer (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV);
  // Init

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment &ACE_TRY_ENV);
  // Connect the CosECConsumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  virtual void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  void deactivate (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Deactivate the object from the POA.

  // = ServantBase operations
  PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;
  // The proxy that we are connected to.

  // @@ Pradeep: you may want to be consitent about your field names,
  // here is <proxy_id> for the supplier is <my_id_>, for the
  // TAO_Notify_Proxy is just <myID_>....
  CosNotifyChannelAdmin::ProxyID proxy_id_;
  // The proxy_supplier id.

protected:
  virtual ~TAO_Notify_StructuredPushConsumer (void);
  // Destructor

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       )) = 0;

  virtual void disconnect_structured_push_consumer (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  // = Data members
  PortableServer::POA_ptr default_POA_;
  // The default POA.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* NOTIFY_STRUCTUREDPUSHCONSUMER_H */
