// $Id$

#ifndef TAO_NOTIFY_PUSHSUPPLIER_H
#define TAO_NOTIFY_PUSHSUPPLIER_H
#include "ace/pre.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_test_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_NOTIFY_TEST_Export TAO_Notify_PushSupplier : public POA_CosNotifyComm::PushSupplier, public PortableServer::RefCountServantBase
{
public:
    // = Initialization and Termination code
  TAO_Notify_PushSupplier (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa TAO_ENV_ARG_DECL);
  // Init

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin TAO_ENV_ARG_DECL);
  // Activates this object with the <default_POA_>
  // Creates a new proxy consumer and connects to it.

  void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  // Disconnect from the consumer proxy.

  void send_event (const CORBA::Any & data TAO_ENV_ARG_DECL);
  // Send the event to the channel.

  void deactivate (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Deactivate the object.

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  CosNotifyChannelAdmin::ProxyID my_id_;
  // This supplier's id.

 protected:
  virtual ~TAO_Notify_PushSupplier ();
  // Destructor

  // = NotifySubscribe methods
    void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    void disconnect_push_supplier (
        TAO_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // = Data members
    PortableServer::POA_var default_POA_;
    // The default POA.

    CosNotifyChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
    // The proxy that we are connected to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PUSHSUPPLIER_H */
