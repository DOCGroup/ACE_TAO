// -*- C++ -*-

#include "Server_Request_Interceptor.h"
#include "testS.h"

ACE_RCSID (ForwardRequest,
           Server_Request_Interceptor,
           "$Id$")

Server_Request_Interceptor::Server_Request_Interceptor (void)
  : reply_ (0)
  , last_id_ (-45)
{

}

Server_Request_Interceptor::~Server_Request_Interceptor (void)
{
}

void
Server_Request_Interceptor::tao_ft_interception_point (
    PortableInterceptor::ServerRequestInfo_ptr ri,
    CORBA::OctetSeq_out ocs
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext_var svc =
    ri->get_request_service_context (Replay_Reply::SPECIFIC_CONTEXT
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          svc->context_data.get_buffer ()),
                    svc->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    {
      return;
    }

  cdr.reset_byte_order (ACE_static_cast (int,byte_order));

  Replay_Reply::tagged_component group_component;

  cdr >> group_component;

  if (!cdr.good_bit ())
    return;

  if (group_component.id == this->last_id_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Replaying reply ..\n"));

      // CORBA::OctetSeq_var o;
      ACE_NEW (ocs,
               CORBA::OctetSeq (this->reply_.in ()));

      // (ocs) = o.in ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Done copying reply ..\n"));
    }
  else
    this->last_id_ = group_component.id;

  return;
}

char *
Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Any_var result =
    ri->result (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Short retval = 0;

  result >>= retval;

  TAO_OutputCDR cdr;

  cdr << retval;

  Dynamic::ParameterList_var pl =
    ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong len =
    pl->length ();

  for (CORBA::ULong index = 0; index != len ; ++index)
    {
      // No chance for PARAM_OUT
      if ((*pl)[index].mode == CORBA::PARAM_INOUT)
        {
          const char *str;
          (*pl)[index].argument >>= str;

          cdr.write_string (str);
        }
    }

  CORBA::OctetSeq *p = 0;

  ACE_NEW (p,
           CORBA::OctetSeq (cdr.total_length ()));

  this->reply_ = p;

  this->reply_->length (cdr.total_length ());

  CORBA::Octet *buf =
    this->reply_->get_buffer ();

  // @@ What if this throws an exception??  We don't have anyway to
  // check whether this succeeded
  for (const ACE_Message_Block *mb = cdr.begin ();
       mb != 0;
       mb = mb->cont ())
    {
      ACE_OS::memcpy (buf, mb->rd_ptr (), mb->length ());
      buf += mb->length ();
    }

}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
