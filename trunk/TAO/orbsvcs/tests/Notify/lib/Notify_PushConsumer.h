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
#include "ace/pre.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_test_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_NOTIFY_TEST_Export TAO_Notify_PushConsumer : public POA_CosNotifyComm::PushConsumer, public PortableServer::RefCountServantBase
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

  void init (PortableServer::POA_ptr poa TAO_ENV_ARG_DECL);
  // Init

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin TAO_ENV_ARG_DECL);
  // Activates this object with the <default_POA_>
  // Creates a new proxy supplier and connects to it.

  void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  void deactivate (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Deactivate the object from the POA.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:
  virtual ~TAO_Notify_PushConsumer ();
  // Destructor

  // = NotifyPublish method
   void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        TAO_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    void push (
        const CORBA::Any & data
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ));
  // The default operation is no op.

    void disconnect_push_consumer (
        TAO_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // = Data members
    PortableServer::POA_var default_POA_;
    // The default POA.

    CosNotifyChannelAdmin::ProxyID proxy_id_;
    // The proxy's id.

    CosNotifyChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
    // The proxy that we are connected to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PUSHCONSUMER_H */
