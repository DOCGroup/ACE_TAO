// -*- C++ -*-
#include "ServerRequest_Interceptor2.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "tao/Exception.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"
#include "tao/PI/PIForwardRequestC.h"
#include <cstring>

char *
TAO249_ServerRequest_Interceptor2::name ()
{
  return CORBA::string_dup ("TAO_TAO249_ServerRequest_Interceptor2");
}

void
TAO249_ServerRequest_Interceptor2::destroy ()
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

  if (std::strcmp (op.in (), "throw_location_forward") != 0)
    return;

  IOP::ServiceContext_var const sc =
    ri->get_request_service_context (IOP::FT_REQUEST);

  TAO_InputCDR cdr (
    reinterpret_cast<char const *> (sc->context_data.get_buffer ()),
    sc->context_data.length ());

  CORBA::Boolean byte_order {};
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FT::FTRequestServiceContext ftrsc;
  if ((cdr >> ftrsc) == 0)
    throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

  TimeBase::TimeT const now = this->get_now ();
  if (now > ftrsc.expiration_time)
    {
      if (this->expired_.exchange (true))
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Test Failed - REGRESSION !!! Client ORB keeps retrying "
                                "LOCATION_FORWARDs / TRANSIENTs after expiration on "
                                "invocation # %u !!\n"),
                      this->invocation_count_.load () + 1));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expiration time  : %Q\n"), ftrsc.expiration_time));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Time now         : %Q\n"), now));
          return;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("The expiration time has passed; the next exception must not "
                            "prompt reinvocation.\n")));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expiration time  : %Q\n"), ftrsc.expiration_time));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Time now         : %Q\n"), now));
    }

  unsigned int const invocation = ++this->invocation_count_;
  if (invocation == 1)
    {
      if (this->expired_.load ())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("On invocation #%u throwing a ForwardRequest back to the client\n"),
                    invocation));
      throw PortableInterceptor::ForwardRequest (this->server_iogr_.in ());
    }

  if (this->expired_.load ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("On invocation #%u throwing a TRANSIENT back to the client\n"),
                invocation));
  throw CORBA::TRANSIENT (0, CORBA::COMPLETED_NO);
}

void
TAO249_ServerRequest_Interceptor2::set_server_iogr (CORBA::Object_ptr obj)
{
  this->server_iogr_ = CORBA::Object::_duplicate (obj);
}

TimeBase::TimeT
TAO249_ServerRequest_Interceptor2::get_now () const
{
  TimeBase::TimeT const time_offset = ACE_UINT64_LITERAL (0x1B21DD213814000);
  ACE_Time_Value const time_value = ACE_OS::gettimeofday ();
  TimeBase::TimeT const sec_part = time_value.sec () * 10000000;
  TimeBase::TimeT const usec_part = time_value.usec () * 10;
  return sec_part + usec_part + time_offset;
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
