/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_StructuredPushSupplier
//
// = DESCRIPTION
//   This class is to be used by clients of the Notification Service
//   to implement Structured Push Suppliers.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H
#define TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H
#include "ace/pre.h"

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_test_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_NOTIFY_TEST_Export TAO_Notify_StructuredPushSupplier:public POA_CosNotifyComm::StructuredPushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Implements rudimentary StructuredPushSupplier functionality.
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_StructuredPushSupplier (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/);
  // Init

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV);
  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier and connects to it.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event,
                           CORBA::Environment &ACE_TRY_ENV);
  // Send one event.

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr get_proxy_consumer (void);
  // Accessor for <proxy_consumer_>.

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate the object.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

protected:
  // = Data Members
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This <proxy_consumer_> id.

  PortableServer::POA_var default_POA_;
  // The default POA.

  virtual ~TAO_Notify_StructuredPushSupplier ();
  // Destructor

  // = NotifySubscribe
    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H */
