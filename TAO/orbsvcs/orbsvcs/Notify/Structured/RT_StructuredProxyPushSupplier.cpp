// $Id$

#include "RT_StructuredProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "RT_StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_RT_StructuredProxyPushSupplier, "$id$")

TAO_Notify_RT_StructuredProxyPushSupplier::TAO_Notify_RT_StructuredProxyPushSupplier (void)
{
}

TAO_Notify_RT_StructuredProxyPushSupplier::~TAO_Notify_RT_StructuredProxyPushSupplier ()
{
}

CORBA::Object_ptr
TAO_Notify_RT_StructuredProxyPushSupplier::activate (PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  CORBA::Object_var object = TAO_Notify_Proxy::activate (servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Obtain our ref.
  CORBA::Object_var obj = this->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->event_forwarder_ = Event_Forwarder::StructuredProxyPushSupplier::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);

  return object._retn ();
}

void
TAO_Notify_RT_StructuredProxyPushSupplier::push (const TAO_Notify_Event* event ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      event->push (this->event_forwarder_.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 2)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "(%P|%t) TAO_Notify_RT_StructuredProxyPushSupplier::push failed\n");
        }

    }
  ACE_ENDTRY;
}

void
TAO_Notify_RT_StructuredProxyPushSupplier::push_no_filtering (const TAO_Notify_Event* event ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      event->push_no_filtering (this->event_forwarder_.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 2)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "(%P|%t) TAO_Notify_RT_StructuredProxyPushSupplier::push failed\n");
        }

    }
  ACE_ENDTRY;
}

void
TAO_Notify_RT_StructuredProxyPushSupplier::push (const TAO_Notify_Event_var &event ACE_ENV_ARG_DECL)
{
  this->push (event.get () ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_RT_StructuredProxyPushSupplier::push_no_filtering (const TAO_Notify_Event_var &event ACE_ENV_ARG_DECL)
{
  this->push_no_filtering (event.get () ACE_ENV_ARG_PARAMETER);
}
