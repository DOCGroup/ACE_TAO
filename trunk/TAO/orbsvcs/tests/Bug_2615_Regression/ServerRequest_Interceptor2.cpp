// -*- C++ -*-

#include "ServerRequest_Interceptor2.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOP_IORC.h"
#include "tao/ORB_Constants.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CDR.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "Hello.h"
#include "ace/OS_NS_sys_time.h"
#include "tao/PI/PIForwardRequestC.h"

ACE_RCSID (FaultTolerance,
           ServerRequest_Interceptor2,
           "$Id$")

CORBA::Boolean
ServerRequest_Interceptor2::has_ft_request_sc_ = false;

ServerRequest_Interceptor2::ServerRequest_Interceptor2 (void)
: orb_ (0)
{
}

ServerRequest_Interceptor2::~ServerRequest_Interceptor2 (void)
{
}

char *
ServerRequest_Interceptor2::name (void)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ServerRequest_Interceptor2");
}

void
ServerRequest_Interceptor2::destroy (void)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerRequest_Interceptor2::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
}

void
ServerRequest_Interceptor2::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "has_ft_request_service_context"))
  {
    // bail if not the op we are interested in -
    // avoid excess spurious error clutter when client calls ::shutdown; ::ping etc..
    return;
  }

  ACE_TRY
    {
      IOP::ServiceContext_var sc =
        ri->get_request_service_context (IOP::FT_REQUEST
                                         ACE_ENV_ARG_PARAMETER);

      // No exception therefore there was a context
      has_ft_request_sc_ = true;
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      ACE_UNUSED_ARG (ex);
      // No group version context
      has_ft_request_sc_ = false;
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
ServerRequest_Interceptor2::send_reply (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerRequest_Interceptor2::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
}

void
ServerRequest_Interceptor2::send_other (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
}

