// $Id$

#include "Factory.h"
#include "EventChannelFactory.h"
#include "EventChannel.h"
#include "Admin.h"
#include "Proxy.h"
#include "SupplierAdmin.h"
#include "ConsumerAdmin.h"
#include "Structured/StructuredProxyPushConsumer.h"
#include "Structured/StructuredProxyPushSupplier_Tie.h"
#include "QoSAdmin.h"

#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"

#if ! defined (__ACE_INLINE__)
#include "Factory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RN_Notify, TAO_NS_Factory, "$Id$")

  
TAO_NS_Factory::TAO_NS_Factory (void)
{
}

TAO_NS_Factory::~TAO_NS_Factory ()
{
}

ACE_Lock*
TAO_NS_Factory::create_object_lock (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_Lock* lock;
  ACE_NEW_THROW_EX (lock,
                    ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (),
                    CORBA::NO_MEMORY ());
  return lock;
}

void
TAO_NS_Factory::create (TAO_NS_QoSAdmin*& qos_admin ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (qos_admin, TAO_NS_QoSAdmin (), CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_Supplier_Collection*& collection ACE_ENV_SINGLE_ARG_DECL)
{
  typedef TAO_ESF_Proxy_List<TAO_NS_ProxyConsumer>::Iterator ProxyIterator;
  typedef TAO_ESF_Copy_On_Write<TAO_NS_ProxyConsumer, TAO_ESF_Proxy_List<TAO_NS_ProxyConsumer>,ProxyIterator, ACE_SYNCH> ProxyType; 
  
  ACE_NEW_THROW_EX (collection,
		    ProxyType (),
		    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_Consumer_Collection*& collection ACE_ENV_SINGLE_ARG_DECL)
{
  typedef TAO_ESF_Proxy_List<TAO_NS_ProxySupplier>::Iterator ProxyIterator;
  typedef TAO_ESF_Copy_On_Write<TAO_NS_ProxySupplier, TAO_ESF_Proxy_List<TAO_NS_ProxySupplier>,ProxyIterator, ACE_SYNCH> ProxyType; 
  
  ACE_NEW_THROW_EX (collection,
		    ProxyType (),
		    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_EventChannel_Collection*& collection ACE_ENV_SINGLE_ARG_DECL)
{
  typedef TAO_ESF_Proxy_List<TAO_NS_EventChannel>::Iterator ProxyIterator;
  typedef TAO_ESF_Copy_On_Write<TAO_NS_EventChannel, TAO_ESF_Proxy_List<TAO_NS_EventChannel>,ProxyIterator, ACE_SYNCH> ProxyType; 
  
  ACE_NEW_THROW_EX (collection,
		    ProxyType (),
		    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_Admin_Collection*& collection ACE_ENV_SINGLE_ARG_DECL)
{
  typedef TAO_ESF_Proxy_List<TAO_NS_Admin>::Iterator ProxyIterator;
  typedef TAO_ESF_Copy_On_Write<TAO_NS_Admin, TAO_ESF_Proxy_List<TAO_NS_Admin>,ProxyIterator, ACE_SYNCH> ProxyType; 

  ACE_NEW_THROW_EX (collection,
		    ProxyType (),
		    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_Proxy_Collection*& collection ACE_ENV_SINGLE_ARG_DECL)
{
  typedef TAO_ESF_Proxy_List<TAO_NS_Proxy>::Iterator ProxyIterator;
  typedef TAO_ESF_Copy_On_Write<TAO_NS_Proxy,TAO_ESF_Proxy_List<TAO_NS_Proxy>,ProxyIterator,ACE_SYNCH> ProxyType;

  ACE_NEW_THROW_EX (collection,
		    ProxyType (),
		    CORBA::NO_MEMORY ());
}

void 
TAO_NS_Factory::create (TAO_NS_EventChannelFactory*& factory ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (factory,
                    TAO_NS_EventChannelFactory (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_EventChannel*& channel ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (channel,
                    TAO_NS_EventChannel (),
                    CORBA::NO_MEMORY ());
}


void
TAO_NS_Factory::create (TAO_NS_SupplierAdmin*& admin ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (admin,
                    TAO_NS_SupplierAdmin (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_ConsumerAdmin*& admin ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (admin,
                    TAO_NS_ConsumerAdmin (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_StructuredProxyPushConsumer*& proxy ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_NS_StructuredProxyPushConsumer (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_Factory::create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_StructuredProxyPushSupplier_Tie* object;

  ACE_NEW_THROW_EX (object,
                    TAO_NS_StructuredProxyPushSupplier_Tie (),
                    CORBA::NO_MEMORY ());
  proxy = object->_tied_object ();

  /// set correct callback.
  proxy->destroy_callback (object);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Container_T <TAO_NS_EventChannelFactory, TAO_NS_NULL_PARENT>;
template class TAO_NS_Container_T <TAO_NS_EventChannel, TAO_NS_EventChannelFactory>;
template class TAO_NS_Container_T <TAO_NS_Admin, TAO_NS_EventChannel>;

template class TAO_NS_Object_T <TAO_NS_EC_NULL_PARENT>;
template class TAO_NS_Object_T <TAO_NS_EventChannelfactory>;
template class TAO_NS_Object_T <TAO_NS_EventChannel>;
template class TAO_NS_Object_T <TAO_NS_Admin>;

template class TAO_NS_Container_T <TAO_NS_EventChannel>;
template class TAO_NS_Container_T <TAO_NS_Admin>;
template class TAO_NS_Container_T <TAO_NS_Proxy>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Container_T <TAO_NS_EventChannelFactory, TAO_NS_NULL_PARENT>
#pragma instantiate TAO_NS_Container_T <TAO_NS_EventChannel, TAO_NS_EventChannelFactory>
#pragma instantiate TAO_NS_Container_T <TAO_NS_Admin, TAO_NS_EventChannel>

#pragma instantiate TAO_NS_Object_T <TAO_NS_EC_NULL_PARENT>
#pragma instantiate TAO_NS_Object_T <TAO_NS_EventChannelfactory>
#pragma instantiate TAO_NS_Object_T <TAO_NS_EventChannel>
#pragma instantiate TAO_NS_Object_T <TAO_NS_Admin>

#pragma instantiate TAO_NS_Container_T <TAO_NS_EventChannel>
#pragma instantiate TAO_NS_Container_T <TAO_NS_Admin>
#pragma instantiate TAO_NS_Container_T <TAO_NS_Proxy>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
