#include "LB_ServerRequestInterceptor.h"
#include "LB_LoadAlert.h"

#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           LB_ServerRequestInterceptor,
           "$Id$")


TAO_LB_ServerRequestInterceptor::TAO_LB_ServerRequestInterceptor (
  TAO_LB_LoadAlert & load_alert)
  : load_alert_ (load_alert)
{
}

TAO_LB_ServerRequestInterceptor::~TAO_LB_ServerRequestInterceptor (void)
{
}

char *
TAO_LB_ServerRequestInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO_LB_ServerRequestInterceptor");
}

void
TAO_LB_ServerRequestInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (this->load_alert_.alerted ())
    {
#if 0
      ACE_TRY
        {
          IOP::ServiceContext_var service_context =
            ri->get_request_service_context (CosLoadBalancing::LOAD_MANAGED
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          /*
          // Use TAO-specific "compiled marshaling" instead of
          // standard interpretive marshaling via a CDR encapsulation
          // Codec for efficiency reasons.
          const char * buf =
            ACE_reinterpret_cast (const char *,
                                  service_context->context_data.get_buffer ());
          TAO_InputCDR cdr (buf,
                            service_context->context_data.length ());

          CORBA::Boolean byte_order;
          if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
            ACE_THROW (CORBA::BAD_PARAM ());

          cdr.reset_byte_order (ACE_static_cast (int, byte_order));

          CORBA::Object_var object_group;
          if (cdr >> object_group.out ())
            ACE_THROW (PortableInterceptor::ForwardRequest (object_group.in (),
                                                            0));
          else
            ACE_THROW (CORBA::BAD_PARAM ());
          */

          // A ServiceContext of the given ServiceId exists.  This
          // means that the target is load balanced.  Force the client
          // to try another profile since this location is currently
          // overloaded.
          ACE_THROW (CORBA::TRANSIENT ());
        }
      ACE_CATCH (CORBA::BAD_PARAM, ex)
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
            ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
#else
      // Force the client to try another profile since this location
      // is currently overloaded.
      //
      // NOTE: This applies to both load balanced and non-load
      // balanced targets.
      ACE_THROW (CORBA::TRANSIENT ());
#endif  /* 0 */
    }
}

void
TAO_LB_ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_LB_ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (TAO_debug_level > 0)
    {
      // A CORBA::TRANSIENT is thrown when the LoadManager informs the
      // LoadAlert object that its member is overloaded, for example.

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("TAO_LB_ServerRequestInterceptor -- ")
                  ACE_TEXT ("Exception thrown.\n")));
    }
}

void
TAO_LB_ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (TAO_debug_level > 0)
    {
      // A location forward occurs when the LoadManager informs the
      // LoadAlert object that its member is overloaded, for example.

      const PortableInterceptor::ReplyStatus status =
        ri->reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (status == PortableInterceptor::LOCATION_FORWARD
          || status == PortableInterceptor::LOCATION_FORWARD_PERMANENT)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("TAO_LB_ServerRequestInterceptor -- ")
                    ACE_TEXT ("LOCATION FORWARDED\n")));
    }
}
