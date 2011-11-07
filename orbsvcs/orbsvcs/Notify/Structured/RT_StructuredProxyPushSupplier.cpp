// $Id$

#include "orbsvcs/Notify/Structured/RT_StructuredProxyPushSupplier.h"
#include "tao/debug.h"

#include "orbsvcs/Notify/Method_Request_Dispatch.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_RT_StructuredProxyPushSupplier::TAO_Notify_RT_StructuredProxyPushSupplier (void)
{
}

TAO_Notify_RT_StructuredProxyPushSupplier::~TAO_Notify_RT_StructuredProxyPushSupplier ()
{
}

CORBA::Object_ptr
TAO_Notify_RT_StructuredProxyPushSupplier::activate (PortableServer::Servant servant)
{
  CORBA::Object_var object = TAO_Notify_Proxy::activate (servant);

  // Obtain our ref.
  CORBA::Object_var obj = this->ref ();

  this->event_forwarder_ = Event_Forwarder::StructuredProxyPushSupplier::_narrow (obj.in ());

  return object._retn ();
}

CORBA::Object_ptr
TAO_Notify_RT_StructuredProxyPushSupplier::activate (PortableServer::Servant ,
                                                     CORBA::Long)
{
   return CORBA::Object::_nil ();
}

void
TAO_Notify_RT_StructuredProxyPushSupplier::deliver (TAO_Notify_Method_Request_Dispatch_No_Copy & request)
{
  try
    {
      request.event()->push (this->event_forwarder_.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 2)
        {
          ex._tao_print_exception (
            "(%P|%t) TAO_Notify_RT_StructuredProxyPushSupplier::push failed\n");
        }

    }
}

void
TAO_Notify_RT_StructuredProxyPushSupplier::push_no_filtering (const TAO_Notify_Event* event)
{
  try
    {
      event->push_no_filtering (this->event_forwarder_.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 2)
        {
          ex._tao_print_exception (
            "(%P|%t) TAO_Notify_RT_StructuredProxyPushSupplier::push failed\n");
        }

    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
