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

class TAO_NOTIFY_TEST_Export TAO_NS_StructuredPushSupplier:public POA_CosNotifyComm::StructuredPushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_NS_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Implements rudimentary StructuredPushSupplier functionality.
  //
 public:
  // = Initialization and Termination code
  TAO_NS_StructuredPushSupplier (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa  ACE_ENV_ARG_DECL);
  // Init

  // Accessor to set/get our name.
  void name (ACE_CString& name);
  const ACE_CString& name (void);

  void connect (CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr proxy, CosNotifyChannelAdmin::ProxyID ACE_ENV_ARG_DECL);
  // Activates this servant with the POA supplied in init.
  // Connects to given proxy.

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin ACE_ENV_ARG_DECL);
  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier and connects to it.

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event
                           ACE_ENV_ARG_DECL);
  // Send one event.

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr get_proxy_consumer (void);
  // Accessor for <proxy_consumer_>.

  void deactivate (ACE_ENV_SINGLE_ARG_DECL);
  // Deactivate the object.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:
  // = Data Members

  /// My name.
  ACE_CString name_;

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This <proxy_consumer_> id.

  PortableServer::POA_var default_POA_;
  // The default POA.

  virtual ~TAO_NS_StructuredPushSupplier ();
  // Destructor

  // = NotifySubscribe
    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL
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
