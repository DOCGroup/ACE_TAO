/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_SequencePushSupplier
//
// = DESCRIPTION
//   This class is to be used by clients of the Notification Service
//   to implement Sequence Push Suppliers.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_SEQUENCEPUSHSUPPLIER_H
#define TAO_NOTIFY_SEQUENCEPUSHSUPPLIER_H
#include "ace/pre.h"

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_test_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_NOTIFY_TEST_Export TAO_Notify_SequencePushSupplier:public POA_CosNotifyComm::SequencePushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_SequencePushSupplier
  //
  // = DESCRIPTION
  //   Implements rudimentary SequencePushSupplier functionality.
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_SequencePushSupplier (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/);
  // Init

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV);
  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier and connects to it.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  virtual void send_events (const CosNotification::EventBatch & notifications,
                           CORBA::Environment &ACE_TRY_ENV);
  // Send one event.

  CosNotifyChannelAdmin::SequenceProxyPushConsumer_ptr get_proxy_consumer (void);
  // Accessor for <proxy_consumer_>.

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate the object.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

protected:
  // = Data Members
  CosNotifyChannelAdmin::SequenceProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This <proxy_consumer_> id.

  PortableServer::POA_var default_POA_;
  // The default POA.

  virtual ~TAO_Notify_SequencePushSupplier ();
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

    // = SequencePushSupplier method
    virtual void disconnect_sequence_push_supplier (
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
#endif /* TAO_NOTIFY_SequencePUSHSUPPLIER_H */
