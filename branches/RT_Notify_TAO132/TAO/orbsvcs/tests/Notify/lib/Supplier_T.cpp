// $Id$
#ifndef TAO_NS_Supplier_T_CPP
#define TAO_NS_Supplier_T_CPP

#include "Supplier_T.h"

#if ! defined (__ACE_INLINE__)
#include "Supplier_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Supplier_T, "$id$")

template <class Supplier_Traits>
TAO_NS_Supplier_T<Supplier_Traits>::TAO_NS_Supplier_T (void)
{
}

template <class Supplier_Traits>
TAO_NS_Supplier_T<Supplier_Traits>::~TAO_NS_Supplier_T ()
{
}

template <class Supplier_Traits> TAO_NS_Supplier_T<Supplier_Traits>::Proxy_Traits_PTR
TAO_NS_Supplier_T<Supplier_Traits>::get_proxy_consumer (void)
{
  return this->get_proxy ();
}

template <class Supplier_Traits> TAO_NS_Supplier_T<Supplier_Traits>::Proxy_Traits_PTR
TAO_NS_Supplier_T<Supplier_Traits>::obtain_proxy (TAO_NS_Supplier_T<Supplier_Traits>::Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL)
{
  Supplier_Traits traits;

  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    admin_ptr->obtain_notification_push_consumer (traits.type_
                                                  , this->proxy_id_
                                                  ACE_ENV_ARG_PARAMETER
                                                  );

  ACE_CHECK_RETURN (Proxy_Traits_INTERFACE::_nil ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  return Proxy_Traits_INTERFACE::_narrow (proxy_consumer.in ()
  ACE_ENV_ARG_PARAMETER);
}

template <class Supplier_Traits> TAO_NS_Supplier_T<Supplier_Traits>::Proxy_Traits_PTR
TAO_NS_Supplier_T<Supplier_Traits>::obtain_proxy (TAO_NS_Supplier_T<Supplier_Traits>::Admin_Ext_Traits_PTR admin_ptr
                                             , CosNotification::QoSProperties& qos
                                             ACE_ENV_ARG_DECL)
{
  Supplier_Traits traits;

  // create the proxy consumer
  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    admin_ptr->obtain_notification_push_consumer_with_qos (traits.type_
                                                           , this->proxy_id_
                                                           , qos
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Proxy_Traits_INTERFACE::_nil ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  return Proxy_Traits_INTERFACE::_narrow (proxy_consumer.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

template <class Supplier_Traits> void
TAO_NS_Supplier_T<Supplier_Traits>::offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "(%P, %t) Calling Supplier offer change: \n"));

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
  ACE_TYPENAME Proxy_Traits::PTR proxy_consumer = this->get_proxy ();

  proxy_consumer->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class Supplier_Traits> void
TAO_NS_Supplier_T<Supplier_Traits>::subscription_change (
                                                         const CosNotification::EventTypeSeq & /*added*/,
                                                         const CosNotification::EventTypeSeq & /*removed*/
        ACE_ENV_ARG_DECL_NOT_USED
        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  // No op.
}
#endif /* TAO_NS_Supplier_T_CPP */
