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
      CORBA::Object_var forward = this->load_alert_.forward ();

      ACE_THROW (PortableInterceptor::ForwardRequest (forward.in (), 0));
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
