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
#include "notify_test_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_NOTIFY_TEST_Export TAO_Notify_StructuredPushConsumer : public POA_CosNotifyComm::StructuredPushConsumer, public PortableServer::RefCountServantBase
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

  void init (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL);
  // Saves the POA ref.

  // Accessor to set/get our name.
  void name (ACE_CString& name);
  const ACE_CString& name (void);

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL);
  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier via the <consumer_admin> supplied and connects
  // to it.

  void connect (CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);
  // Activates this servant with the POA supplied in init.
  /// Connect to given proxy

  virtual void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  void deactivate (ACE_ENV_SINGLE_ARG_DECL);
  // Deactivate the object from the default POA.

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier (void);
  // Accessor for <proxy_supplier_>.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
protected:
  // = Data Members

  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// My name.
  ACE_CString name_;

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;
  // The <proxy_supplier_> id.

  PortableServer::POA_var default_POA_;
  // The default POA.

  virtual ~TAO_Notify_StructuredPushConsumer (void);
  // Destructor

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
  // Default does nothing.

  virtual void disconnect_structured_push_consumer (
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
#endif /* NOTIFY_STRUCTUREDPUSHCONSUMER_H */
