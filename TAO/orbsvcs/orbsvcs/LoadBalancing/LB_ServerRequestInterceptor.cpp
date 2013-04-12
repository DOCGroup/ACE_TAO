// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/LoadBalancing/LB_ServerRequestInterceptor.h"
#include "orbsvcs/LoadBalancing/LB_LoadAlert.h"
#include "tao/debug.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_ServerRequestInterceptor::TAO_LB_ServerRequestInterceptor (
  TAO_LB_LoadAlert & load_alert)
  : load_alert_ (load_alert)
{
}

TAO_LB_ServerRequestInterceptor::~TAO_LB_ServerRequestInterceptor (void)
{
}

char *
TAO_LB_ServerRequestInterceptor::name (void)
{
  return CORBA::string_dup ("TAO_LB_ServerRequestInterceptor");
}

void
TAO_LB_ServerRequestInterceptor::destroy (void)
{
}

void
TAO_LB_ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  if (this->load_alert_.alerted ())
    {
      CORBA::String_var op = ri->operation ();

      if (ACE_OS::strcmp (op.in (), "_get_loads") == 0        // LoadMonitor
          || ACE_OS::strcmp (op.in (), "disable_alert") == 0  // LoadAlert
          || ACE_OS::strcmp (op.in (), "enable_alert") == 0)  // LoadAlert
        return;  // Do not redirect.

#if 0
      try
        {
          IOP::ServiceContext_var service_context =
            ri->get_request_service_context (CosLoadBalancing::LOAD_MANAGED);

          /*
          // Use TAO-specific "compiled marshaling" instead of
          // standard interpretive marshaling via a CDR encapsulation
          // Codec for efficiency reasons.
          const char * buf =
            reinterpret_cast<const char *> (service_context->context_data.get_buffer ());
          TAO_InputCDR cdr (buf,
                            service_context->context_data.length ());

          CORBA::Boolean byte_order;
          if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
            throw CORBA::BAD_PARAM ();

          cdr.reset_byte_order (static_cast<int> (byte_order));

          CORBA::Object_var object_group;
          if (cdr >> object_group.out ())
            throw PortableInterceptor::ForwardRequest (object_group.in (,
                                                            0));
          else
            throw CORBA::BAD_PARAM ();
          */

          // A ServiceContext of the given ServiceId exists.  This
          // means that the target is load balanced.  Force the client
          // to try another profile since this location is currently
          // overloaded.
          throw CORBA::TRANSIENT ();
        }
      catch (const CORBA::BAD_PARAM& ex)
        {
          // No CosLoadBalancing::LB_GROUP_REF ServiceContext.  This
          // probably means that the target object is not
          // LoadBalanced.

          // There is a huge DoS attack vulnerability with this load
          // shedding model.  The model relies on the client behaving
          // correctly.  In particular, it relies on the client to
          // send the forward object group reference in the
          // ServiceContextList.  Any "misbehaving" client can avoid
          // that, in which case no load will ever be shed!


          // Make sure we get a CORBA::BAD_PARAM for the right
          // reason.
          if (ex.minor () != (CORBA::OMGVMCID | 26))
            throw;
        }
#else
      // Force the client to try another profile since this location
      // is currently overloaded.
      //
      // NOTE: This applies to both load balanced and non-load
      // balanced targets.
      throw CORBA::TRANSIENT ();
#endif  /* 0 */
    }
}

void
TAO_LB_ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO_LB_ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
}

void
TAO_LB_ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  if (TAO_debug_level > 0)
    {
      // A CORBA::TRANSIENT is thrown when the LoadManager informs the
      // LoadAlert object that its member is overloaded, for example.

      ORBSVCS_DEBUG ((LM_INFO,
                  ACE_TEXT ("TAO_LB_ServerRequestInterceptor -- ")
                  ACE_TEXT ("Exception thrown.\n")));
    }
}

void
TAO_LB_ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    {
      // A location forward occurs when the LoadManager informs the
      // LoadAlert object that its member is overloaded, for example.

      const PortableInterceptor::ReplyStatus status =
        ri->reply_status ();

      if (status == PortableInterceptor::LOCATION_FORWARD)
        ORBSVCS_DEBUG ((LM_INFO,
                    ACE_TEXT ("TAO_LB_ServerRequestInterceptor -- ")
                    ACE_TEXT ("LOCATION FORWARDED\n")));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
