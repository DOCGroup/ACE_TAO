// -*- C++ -*-
#include "ServerRequest_Interceptor2.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"
#include "tao/CDR.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <cstring>

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

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
  CORBA::String_var const op = ri->operation ();

  if (std::strcmp (op.in (), "drop_down_dead") != 0)
    return;

  try
    {
      IOP::ServiceContext_var const sc =
        ri->get_request_service_context (IOP::FT_REQUEST);

      TAO_InputCDR cdr (reinterpret_cast<char const *> (
                          sc->context_data.get_buffer ()),
                        sc->context_data.length ());

      CORBA::Boolean byte_order {};

      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

      cdr.reset_byte_order (static_cast<int> (byte_order));

      FT::FTRequestServiceContext ftrsc;

      if ((cdr >> ftrsc) == 0)
        throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

      File_Ptr last_exp_time_file (
        std::fopen ("last_expiration_time", "r+"), std::fclose);
      TimeBase::TimeT last_exp_time {};

      if (!last_exp_time_file)
        {
          // The first server creates the file containing the expiration time.
          last_exp_time_file.reset (std::fopen ("last_expiration_time", "w+"));
          if (!last_exp_time_file)
            throw CORBA::NO_RESOURCES ();
          last_exp_time = ftrsc.expiration_time;
        }
      else
        {
          // Subsequent servers read the previous server's expiration time.
          if (std::fread (&last_exp_time,
                          1,
                          sizeof (TimeBase::TimeT),
                          last_exp_time_file.get ()) != sizeof (TimeBase::TimeT))
            throw CORBA::NO_RESOURCES ();
          ACE_OS::sleep (1);
        }

      if (last_exp_time != ftrsc.expiration_time)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Test Failed - REGRESSION !!! Different expiration times used on consecutive "
                                "retries of the same op!!\n")));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Previous expiration: %Q\n"), last_exp_time));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Current expiration : %Q\n"), ftrsc.expiration_time));
          return;
        }

      // A value below this UTC offset indicates that POSIX time was used.
      TimeBase::TimeT const the_seventies = ACE_UINT64_LITERAL (0x1B21DD213814000);

      // @warning This test is fragile and will need updating in 2358 A.D.
      if (last_exp_time <= the_seventies)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Test Failed - REGRESSION !!! Expiration time is before 1970 A.D.\n")));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expiration time: %Q\n"), last_exp_time));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("1970 A.D.      : %Q\n"), the_seventies));
          return;
        }
      else if (last_exp_time > the_seventies * 2)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Test Failed - REGRESSION !!! Expiration time is after 2358 A.D.\n")));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expiration time: %Q\n"), last_exp_time));
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("2358 A.D.      : %Q\n"), the_seventies * 2));
          return;
        }

      TimeBase::TimeT const now = this->get_now ();

      if (now > last_exp_time)
        {
          // No retries should be received after the expiration time.
          File_Ptr no_more_retries (
            std::fopen ("no_more_retries", "r+"), std::fclose);
          if (no_more_retries)
            {
              TimeBase::TimeT last_expired_attempt {};
              if (std::fread (&last_expired_attempt,
                              1,
                              sizeof (TimeBase::TimeT),
                              no_more_retries.get ()) != sizeof (TimeBase::TimeT))
                throw CORBA::NO_RESOURCES ();

              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("Test Failed - REGRESSION !!! Client ORB is still retrying after "
                                    "the expiration time!!\n")));
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expiration time  : %Q\n"), last_exp_time));
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Previous attempt : %Q\n"), last_expired_attempt));
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Time now         : %Q\n"), now));
              return;
            }
          else
            {
              no_more_retries.reset (std::fopen ("no_more_retries", "w+"));
              if (!no_more_retries)
                throw CORBA::NO_RESOURCES ();

              if (std::fwrite (&now, 1, sizeof (TimeBase::TimeT), no_more_retries.get ()) !=
                    sizeof (TimeBase::TimeT) ||
                  std::fflush (no_more_retries.get ()) != 0)
                throw CORBA::NO_RESOURCES ();
            }
          }

      // Write the expiration time for the next server to check.
      if (std::fwrite (&ftrsc.expiration_time,
                       1,
                       sizeof (TimeBase::TimeT),
                       last_exp_time_file.get ()) != sizeof (TimeBase::TimeT) ||
          std::fflush (last_exp_time_file.get ()) != 0)
        throw CORBA::NO_RESOURCES ();
      last_exp_time_file.reset ();

      std::abort ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Unexpected (non regression problem) error - test failed\n")));
      ex._tao_print_exception (
        "Exception in TAO249_ServerRequest_Interceptor2::receive_request\n");
      throw;
    }
}

TimeBase::TimeT
TAO249_ServerRequest_Interceptor2::get_now ()
{
  // 1582...
  TimeBase::TimeT const time_offset = ACE_UINT64_LITERAL (0x1B21DD213814000);

  // Now in posix
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

