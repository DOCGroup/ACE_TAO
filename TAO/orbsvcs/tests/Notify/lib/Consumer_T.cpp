// $Id$
#ifndef TAO_NS_Consumer_T_CPP
#define TAO_NS_Consumer_T_CPP

#include "Consumer_T.h"

#if ! defined (__ACE_INLINE__)
#include "Consumer_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Consumer_T, "$id$")

template <class Consumer_Traits>
TAO_NS_Consumer_T<Consumer_Traits>::TAO_NS_Consumer_T (void)
{
}

template <class Consumer_Traits>
TAO_NS_Consumer_T<Consumer_Traits>::~TAO_NS_Consumer_T ()
{
}

template <class Consumer_Traits> TAO_NS_Consumer_T<Consumer_Traits>::Proxy_Traits_PTR
TAO_NS_Consumer_T<Consumer_Traits>::get_proxy_supplier (void)
{
  return this->get_proxy ();
}

template <class Consumer_Traits> TAO_NS_Consumer_T<Consumer_Traits>::Proxy_Traits_PTR
TAO_NS_Consumer_T<Consumer_Traits>::obtain_proxy (TAO_NS_Consumer_T<Consumer_Traits>::Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL)
{
  Consumer_Traits traits;

  CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
    admin_ptr->obtain_notification_push_supplier (traits.type_
                                                  , this->proxy_id_
                                                  ACE_ENV_ARG_PARAMETER
                                                  );

  ACE_CHECK_RETURN (Proxy_Traits_INTERFACE::_nil ());

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier.in ()));

  return Proxy_Traits_INTERFACE::_narrow (proxy_supplier.in ()
  ACE_ENV_ARG_PARAMETER);
}

template <class Consumer_Traits> TAO_NS_Consumer_T<Consumer_Traits>::Proxy_Traits_PTR
TAO_NS_Consumer_T<Consumer_Traits>::obtain_proxy (TAO_NS_Consumer_T<Consumer_Traits>::Admin_Ext_Traits_PTR admin_ptr
                                             , CosNotification::QoSProperties& qos
                                             ACE_ENV_ARG_DECL)
{
  Consumer_Traits traits;

  // create the proxy consumer
  CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
    admin_ptr->obtain_notification_push_supplier_with_qos (traits.type_
                                                           , this->proxy_id_
                                                           , qos
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Proxy_Traits_INTERFACE::_nil ());

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier.in ()));

  return Proxy_Traits_INTERFACE::_narrow (proxy_supplier.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

template <class Consumer_Traits> void
TAO_NS_Consumer_T<Consumer_Traits>::subscription_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "Calling Consumer subscription change: "));

  CORBA::ULong i = 0;

  for (; i < added.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "+(%s,%s), ", added[i].domain_name.in (), added[i].type_name.in ()));
    }
  for (i = 0; i < removed.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "-(%s,%s), ", removed[i].domain_name.in (), removed[i].type_name.in ()));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  /// subscribe
  ACE_TYPENAME Proxy_Traits::PTR proxy_supplier = this->get_proxy ();

  if (proxy_supplier != 0)
    {
      proxy_supplier->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "Proxy Supplier not available, subscription change not made."));
}

template <class Consumer_Traits> void
TAO_NS_Consumer_T<Consumer_Traits>::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosNotifyComm::InvalidEventType))
{
  // No-Op.
}

#endif /* TAO_NS_Consumer_T_CPP */
