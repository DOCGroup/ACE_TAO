/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_SequencePushConsumer.h
//
// = DESCRIPTION
//   This is a utility class is to be used by clients of the Notification
//   Service to implement the servant for CosNotifyComm::SequencePushConsumer
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_SEQUENCEPUSHCONSUMER_H
#define NOTIFY_SEQUENCEPUSHCONSUMER_H
#include "ace/pre.h"

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_test_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_NOTIFY_TEST_Export TAO_Notify_SequencePushConsumer : public POA_CosNotifyComm::SequencePushConsumer, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   Notify_SequencePushConsumer
  //
  // = DESCRIPTION
  //   Implements a common servant for the SequencePushConsumer.
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_SequencePushConsumer (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa TAO_ENV_ARG_DECL);
  // Saves the POA ref.

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin TAO_ENV_ARG_DECL);
  // Activates this servant with the POA supplied in init.
  // Creates a new proxy supplier via the <consumer_admin> supplied and connects
  // to it.

  virtual void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  void deactivate (TAO_ENV_SINGLE_ARG_DECL);
  // Deactivate the object from the default POA.

  CosNotifyChannelAdmin::SequenceProxyPushSupplier_ptr get_proxy_supplier (void);
  // Accessor for <proxy_supplier_>.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
protected:
  // = Data Members
  CosNotifyChannelAdmin::SequenceProxyPushSupplier_var proxy_supplier_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;
  // The <proxy_supplier_> id.

  PortableServer::POA_var default_POA_;
  // The default POA.

  virtual ~TAO_Notify_SequencePushConsumer (void);
  // Destructor

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = SequencePushConsumer methods
  virtual void push_structured_events (
        const CosNotification::EventBatch & notifications
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));
  // Default does nothing.

  virtual void disconnect_sequence_push_consumer (
        TAO_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* NOTIFY_SEQUENCEPUSHCONSUMER_H */
