// -*- C++ -*-
// $Id$

#include "ServerRequest_Interceptor2.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"
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

TAO249_ServerRequest_Interceptor2::TAO249_ServerRequest_Interceptor2 (void)
: orb_ (0),
  expired_ (0)
{
}

TAO249_ServerRequest_Interceptor2::~TAO249_ServerRequest_Interceptor2 (void)
{
}

char *
TAO249_ServerRequest_Interceptor2::name (void)
{
  return CORBA::string_dup ("TAO_TAO249_ServerRequest_Interceptor2");
}

void
TAO249_ServerRequest_Interceptor2::destroy (void)
{
}

void
TAO249_ServerRequest_Interceptor2::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO249_ServerRequest_Interceptor2::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "throw_location_forward"))
  {
    // bail if not the op we are interested in -
    // avoid excess spurious error clutter when client calls ::shutdown
    return;
  }


  IOP::ServiceContext_var sc =
    ri->get_request_service_context (IOP::FT_REQUEST);

  TAO_InputCDR cdr (reinterpret_cast <const char*>
                                     (sc->context_data.get_buffer ()
                                      ),
                                     sc->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    {
      throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);
    }

  cdr.reset_byte_order (static_cast <int>(byte_order));

  FT::FTRequestServiceContext ftrsc;

  if ((cdr >> ftrsc) == 0)
    throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

  TimeBase::TimeT now = get_now ();

  if (now > ftrsc.expiration_time)
    {
      // We have passed the exp time... there should be no more retries received after this point...
      if (expired_)
        {
          // The client has retried after the expiration time. This is a regression
          ACE_DEBUG ((LM_ERROR, "Test Failed - REGRESSION !!! Client ORB is still retrying LOCATION_FORWARDs after the expiration time!!\n"));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Expiration time  : %Q\n"), ftrsc.expiration_time));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Time now         : %Q\n"), now));

          // Let the request 'succeed' rather than throwing a forward exception.
          return;
        }
      else
        {
          // A request has been recioved after the expiration time.
          // This could legitimately happen - it is only definitely a problem if
          // the client keeps on retrying after now. We set a flag so we can check for this.
          expired_ = 1;
          ACE_DEBUG ((LM_DEBUG, "The expiration time has now passed !!\n"));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Expiration time  : %Q\n"), ftrsc.expiration_time));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Time now         : %Q\n"), now));
        }
    }

  // Let's forward the client back to us again. I would like to be able to make this a PERM
  // but there's no such animal in the PortableInterceptor module. Plus as we (currently) transform
  // and marshal *all* forward requests as vanilla LOCATION_FORWARD it doesn't really matter.
  throw PortableInterceptor::ForwardRequest (server_iogr_.in ());
}

TimeBase::TimeT
TAO249_ServerRequest_Interceptor2::get_now (void)
{
  // 1582...
  const TimeBase::TimeT timeOffset = ACE_UINT64_LITERAL (0x1B21DD213814000);

  // Now in posix
  ACE_Time_Value time_value = ACE_OS::gettimeofday ();

  TimeBase::TimeT sec_part  = time_value.sec ();
  sec_part = sec_part  * 10000000;
  TimeBase::TimeT usec_part = time_value.usec ();
  usec_part = usec_part * 10;
  return (sec_part + usec_part + timeOffset);
}

void
TAO249_ServerRequest_Interceptor2::send_reply (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO249_ServerRequest_Interceptor2::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO249_ServerRequest_Interceptor2::send_other (
  PortableInterceptor::ServerRequestInfo_ptr)
{
}

