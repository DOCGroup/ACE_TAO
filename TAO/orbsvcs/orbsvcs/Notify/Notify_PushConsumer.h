// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_PushConsumer.h
//
// = DESCRIPTION
//   This is a utility class is to be used by clients of the Notification
//   Service to implement the servant for CosNotifyComm::PushConsumer
//
// = HOW-TO
//   1. Derive from Notify_ProxyConsumer and implement <push>.
//   2. Create the servant on the heap.(refcounted servant).
//   3. Initialize the object, provide a POA.
//   4. To connect, call <connect>, this will activate the object in the POA.
//       You must provide a ConsumerAdmin to <connect>.
//   5. To disconnect, call <disconnect> this will also deactivate the object.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_PUSHCONSUMER_H
#define TAO_NOTIFY_PUSHCONSUMER_H

#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_PushConsumer : public POA_CosNotifyComm::PushConsumer, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_PushConsumer
  //
  // = DESCRIPTION
  //   Implements a common servant for the PushConsumer.
  //
 public:
    // = Initialization and Termination code
  TAO_Notify_PushConsumer (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV);
  // Init

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment &ACE_TRY_ENV);
  // Activates this object with the <default_POA_>
  // Creates a new proxy supplier and connects to it.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  void deactivate (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Deactivate the object from the POA.

  // = ServantBase operations
  PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

 protected:
  virtual ~TAO_Notify_PushConsumer ();
  // Destructor

  // = NotifyPublish method
   void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    void push (
        const CORBA::Any & data,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      )) = 0;

    void disconnect_push_consumer (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // = Data members
    PortableServer::POA_ptr default_POA_;
    // The default POA.

    CosNotifyChannelAdmin::ProxyID proxy_id_;
    // The proxy's id.

    CosNotifyChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
    // The proxy that we are connected to.
};

#endif /* TAO_NOTIFY_PUSHCONSUMER_H */
