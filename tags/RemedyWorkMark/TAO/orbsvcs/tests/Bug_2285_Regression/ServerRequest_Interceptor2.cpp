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

TAO249_ServerRequest_Interceptor2::TAO249_ServerRequest_Interceptor2 (void)
: orb_ (0),
  client_id_ (0)
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

  if (ACE_OS::strcmp (op.in (), "drop_down_dead"))
  {
    // bail if not the op we are interested in -
    // avoid excess spurious error clutter when client calls ::shutdown
    return;
  }

  try
  {
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

    FILE* last_exp_time_file = ACE_OS::fopen ("last_expiration_time", "r+");
    TimeBase::TimeT last_exp_time = 0;

    if (!last_exp_time_file)
      {
        // file does not exist ... we're the first server
        // create new file for the expiration time
        last_exp_time_file = ACE_OS::fopen ("last_expiration_time", "w+");
        last_exp_time = ftrsc.expiration_time;
      }
    else
      {
        // We're a subsequent server... read in the previous server's expiration time
        ACE_OS::fread (&last_exp_time,
                       1,
                       sizeof (TimeBase::TimeT),
                       last_exp_time_file);
        ACE_OS::sleep (1);
      }

    if (last_exp_time != ftrsc.expiration_time)
      {
        ACE_DEBUG ((LM_DEBUG, "Test Failed - REGRESSION !!! Different expiration times used on consecutive retries of the same op!!\n"));
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Previous expiration: %Q\n"), last_exp_time));
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Current expiration : %Q\n"), ftrsc.expiration_time));
        return;
      }


    // This is the offset from UTC to posix - a value less than than this will
    // indicate posix is (mistakenly) being used
    TimeBase::TimeT the_seventies = ACE_UINT64_LITERAL (0x1B21DD213814000);

    // @warning - this test is fragile. It will break in 2358 A.D. :-)
    if (last_exp_time <= the_seventies)
      {
        ACE_DEBUG ((LM_ERROR, "Test Failed - REGRESSION !!! Expiration time is in wrong format - it is before 1970 A.D. !!\n"));
        ACE_DEBUG ((LM_DEBUG, "Expiration time: %Q\n", last_exp_time));
        ACE_DEBUG ((LM_DEBUG, "1970 A.D.      : %Q\n", the_seventies));
        return;
      }
    else if ( last_exp_time > (the_seventies * 2))
      {
        ACE_DEBUG ((LM_ERROR, "Test Failed - REGRESSION !!! Expiration time is garbage - it is after 2358 A.D. !!\n"));
        ACE_DEBUG ((LM_DEBUG, "Expiration time: %Q\n", last_exp_time));
        ACE_DEBUG ((LM_DEBUG, "2358 A.D.      : %Q\n", (the_seventies * 2)));
        return;
      }

    TimeBase::TimeT now = get_now ();

    if (now > last_exp_time)
      {
        // We have passed the exp time... there should be no more retries received after this point...
        FILE* no_more_retries = ACE_OS::fopen ("no_more_retries", "r+");
        if (no_more_retries)
          {
            TimeBase::TimeT last_expired_attempt = 0;
            ACE_OS::fread (&last_expired_attempt, 1, sizeof (TimeBase::TimeT), no_more_retries);
            ACE_OS::fclose (no_more_retries);

            // There has already been an attempt after the last server passed the expiration time
            ACE_DEBUG ((LM_ERROR, "Test Failed - REGRESSION !!! Client ORB is still retrying after the expiration time!!\n"));
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Expiration time  : %Q\n"), last_exp_time));
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Previous attempt : %Q\n"), last_expired_attempt));
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Time now         : %Q\n"), now));
            return;
          }
        else
          {
            // Tell the next server we've passed the point of no return
            no_more_retries = ACE_OS::fopen ("no_more_retries", "w+");
            ACE_OS::fwrite (&now, 1, sizeof (TimeBase::TimeT), no_more_retries);
            ACE_OS::fflush (no_more_retries);
            ACE_OS::fclose (no_more_retries);
          }
      }

    // Everthing is fine so far. Write out the expiration time for the next server to check
    ACE_OS::fwrite (&ftrsc.expiration_time, 1, sizeof (TimeBase::TimeT), last_exp_time_file);
    ACE_OS::fflush (last_exp_time_file);
    ACE_OS::fclose (last_exp_time_file);

    // Goodbye cruel world !!
    ACE_OS::abort ();
  }
catch (const CORBA::Exception& ex)
  {
    ACE_DEBUG ((LM_ERROR, "Unexpected (non regression problem) error - test failed\n"));
    ex._tao_print_exception (
      "Exception in TAO249_ServerRequest_Interceptor2::receive_request\n");
    throw;
  }

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

