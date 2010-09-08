// $Id$
// -*- C++ -*-

#include "ServerRequest_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"
#include "tao/ORB_Constants.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CDR.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "Hello.h"

TAO249_ServerRequest_Interceptor::TAO249_ServerRequest_Interceptor (void)
: orb_ (0),
  client_id_ (0)
{
}

TAO249_ServerRequest_Interceptor::~TAO249_ServerRequest_Interceptor (void)
{
}


char *
TAO249_ServerRequest_Interceptor::name (void)
{
  return CORBA::string_dup ("TAO_TAO249_ServerRequest_Interceptor");
}

void
TAO249_ServerRequest_Interceptor::destroy (void)
{
}

void
TAO249_ServerRequest_Interceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO249_ServerRequest_Interceptor::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();
  try
  {
    IOP::ServiceContext_var sc =
      ri->get_request_service_context (IOP::FT_REQUEST);

    TAO_InputCDR cdr (reinterpret_cast <const char*>
                     (sc->context_data.get_buffer ()),
                     sc->context_data.length ());

    CORBA::Boolean byte_order;

    if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
      {
        throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);
      }

    cdr.reset_byte_order (static_cast <int> (byte_order));

    FT::FTRequestServiceContext ftrsc;

    if ((cdr >> ftrsc) == 0)
      throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

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
catch (const CORBA::Exception& ex)
  {
    ACE_DEBUG ((LM_ERROR, "Unexpected (non regression) error - test failed\n"));
    ex._tao_print_exception (
      "Exception in TAO249_ServerRequest_Interceptor::receive_request\n");
    throw;
  }

}

void
TAO249_ServerRequest_Interceptor::send_reply (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO249_ServerRequest_Interceptor::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO249_ServerRequest_Interceptor::send_other (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

