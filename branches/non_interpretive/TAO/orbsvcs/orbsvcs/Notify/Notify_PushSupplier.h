// $Id$
#ifndef TAO_NOTIFY_PUSHSUPPLIER_H
#define TAO_NOTIFY_PUSHSUPPLIER_H

#include "orbsvcs/CosNotifyChannelAdminS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_PushSupplier : public POA_CosNotifyComm::PushSupplier, public PortableServer::RefCountServantBase
{
 public:
    // = Initialization and Termination code
  TAO_Notify_PushSupplier (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV);
  // Init

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV);
  // Activates this object with the <default_POA_>
  // Creates a new proxy consumer and connects to it.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the consumer proxy.

  void send_event (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV);
  // Send the event to the channel.

  void deactivate (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Deactivate the object.

  // = ServantBase operations
  PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  CosNotifyChannelAdmin::ProxyID my_id_;
  // This supplier's id.

 protected:
  virtual ~TAO_Notify_PushSupplier ();
  // Destructor

  // = NotifySubscribe methods
    void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    void disconnect_push_supplier (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // = Data members
    PortableServer::POA_ptr default_POA_;
    // The default POA.

    CosNotifyChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
    // The proxy that we are connected to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_PUSHSUPPLIER_H */
