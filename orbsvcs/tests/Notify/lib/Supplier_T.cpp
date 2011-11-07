// $Id$
#ifndef TAO_Notify_Tests_Supplier_T_CPP
#define TAO_Notify_Tests_Supplier_T_CPP

#include "Supplier_T.h"



template <class Supplier_Traits>
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::TAO_Notify_Tests_Supplier_T (void)
{
}

template <class Supplier_Traits>
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::~TAO_Notify_Tests_Supplier_T ()
{
}

template <class Supplier_Traits> typename TAO_Notify_Tests_Supplier_T<Supplier_Traits>::Proxy_Traits_PTR
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::get_proxy_consumer (void)
{
  return this->get_proxy ();
}

template <class Supplier_Traits> typename TAO_Notify_Tests_Supplier_T<Supplier_Traits>::Proxy_Traits_PTR
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::obtain_proxy (typename TAO_Notify_Tests_Supplier_T<Supplier_Traits>::Admin_Traits_PTR admin_ptr)
{
  Supplier_Traits traits;

  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    admin_ptr->obtain_notification_push_consumer (traits.type_
                                                  , this->proxy_id_
                                                  );


  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  return Proxy_Traits_INTERFACE::_narrow (proxy_consumer.in ());
}

template <class Supplier_Traits> typename TAO_Notify_Tests_Supplier_T<Supplier_Traits>::Proxy_Traits_PTR
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::obtain_proxy (typename TAO_Notify_Tests_Supplier_T<Supplier_Traits>::Admin_Ext_Traits_PTR admin_ptr
                                             , CosNotification::QoSProperties& qos)
{
  Supplier_Traits traits;

  // create the proxy consumer
  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    admin_ptr->obtain_notification_push_consumer_with_qos (traits.type_
                                                           , this->proxy_id_
                                                           , qos);

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  return Proxy_Traits_INTERFACE::_narrow (proxy_consumer.in ());
}

template <class Supplier_Traits> void
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "(%P, %t) Calling Supplier offer change:\n"));

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
  typename Proxy_Traits::PTR proxy_consumer = this->get_proxy ();

  proxy_consumer->offer_change (added, removed);
}

template <class Supplier_Traits> void
TAO_Notify_Tests_Supplier_T<Supplier_Traits>::subscription_change (
                                                         const CosNotification::EventTypeSeq & /*added*/,
                                                         const CosNotification::EventTypeSeq & /*removed*/
        )
{
  // No op.
}
#endif /* TAO_Notify_Tests_Supplier_T_CPP */
