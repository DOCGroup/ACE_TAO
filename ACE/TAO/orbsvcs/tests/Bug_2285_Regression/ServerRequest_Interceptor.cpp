// -*- C++ -*-

#include "ServerRequest_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOP_IORC.h"
#include "tao/ORB_Constants.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CDR.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "Hello.h"

ACE_RCSID (FaultTolerance,
           TAO249_ServerRequest_Interceptor,
           "$Id$")

TAO249_ServerRequest_Interceptor::TAO249_ServerRequest_Interceptor (void)
: orb_ (0),
  client_id_ (0)
{
}

TAO249_ServerRequest_Interceptor::~TAO249_ServerRequest_Interceptor (void)
{
}


char *
TAO249_ServerRequest_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO_TAO249_ServerRequest_Interceptor");
}

void
TAO249_ServerRequest_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO249_ServerRequest_Interceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
}

void
TAO249_ServerRequest_Interceptor::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  ACE_TRY
  {
    IOP::ServiceContext_var sc =
      ri->get_request_service_context (IOP::FT_REQUEST
                                       ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    TAO_InputCDR cdr (reinterpret_cast <const char*>
                     (sc->context_data.get_buffer ()),
                     sc->context_data.length ());

    CORBA::Boolean byte_order;

    if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
      {
        ACE_THROW (CORBA::BAD_PARAM (CORBA::OMGVMCID | 28,
                               CORBA::COMPLETED_NO));
      }
    ACE_TRY_CHECK;

    cdr.reset_byte_order (static_cast <int> (byte_order));

    FT::FTRequestServiceContext ftrsc;

    if ((cdr >> ftrsc) == 0)
      ACE_THROW (CORBA::BAD_PARAM (CORBA::OMGVMCID | 28,
                               CORBA::COMPLETED_NO));
    ACE_TRY_CHECK;
    
    ACE_DEBUG ((LM_DEBUG, "TAO249_ServerRequest_Interceptor::receive_request (%P|%t) called for method: %s ... client retention id is: %d\n", op.in (), ftrsc.retention_id ));    
                            
    if (client_id_ == 0)
      {
        client_id_ = ftrsc.retention_id;
      }
    else
      {
        if (client_id_ != ftrsc.retention_id)
          {
            client_id_ = ftrsc.retention_id;
          }
        else
          {
            ACE_DEBUG ((LM_ERROR, "Test Failed - REGRESSION !!! Same client retention id has been used for TWO consecutive independent invocations!!\n"));
            Hello::ids_differ_ = 0;
          }
      }      
  }
ACE_CATCHANY
  {
    ACE_DEBUG ((LM_ERROR, "Unexpected (non regression) error - test failed\n"));
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Exception in TAO249_ServerRequest_Interceptor::receive_request \n");  
    ACE_RE_THROW;
  }
ACE_ENDTRY;
ACE_CHECK;

}

void
TAO249_ServerRequest_Interceptor::send_reply (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO249_ServerRequest_Interceptor::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
}

void
TAO249_ServerRequest_Interceptor::send_other (
  PortableInterceptor::ServerRequestInfo_ptr
  ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                 PortableInterceptor::ForwardRequest))
{
}
  
