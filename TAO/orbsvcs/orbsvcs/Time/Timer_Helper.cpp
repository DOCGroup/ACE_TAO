// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Time/Timer_Helper.h"
#include "orbsvcs/Time/TAO_Time_Service_Clerk.h"

#include "tao/debug.h"

#include "ace/OS_NS_time.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Timer_Helper::Timer_Helper (void)
  : clerk_ (0)
{
}

Timer_Helper::Timer_Helper (TAO_Time_Service_Clerk *clerk)
  : clerk_ (clerk)
{

}

Timer_Helper::~Timer_Helper (void)
{
}

int
Timer_Helper::handle_timeout (const ACE_Time_Value &,
                              const void *)
{
  int no_of_servers = 0;
  CORBA::ULongLong sum = 0;

  // The following are used to keep a track of the inaccuracy
  // in synchronization.
  CORBA::ULongLong lowest_time = ACE_UINT64_LITERAL (0xFFFFFFFFFFFFFFFF);
  CORBA::ULongLong highest_time  = 0;

  try
    {
      IORS::TYPE* value;
      for (IORS::ITERATOR server_iterator (this->clerk_->server_);
           server_iterator.next (value) != 0;
           server_iterator.advance ())
        {
          // This is a remote call.
          CosTime::UTO_var UTO_server =
            (*value)->universal_time ();

          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q\n"
                        "struct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
                        UTO_server->time (),
                        UTO_server->inaccuracy (),
                        UTO_server->tdf (),
                        (UTO_server->utc_time ()).time,
                        (UTO_server->utc_time ()).inacclo,
                        (UTO_server->utc_time ()).inacchi,
                        (UTO_server->utc_time ()).tdf));

          CORBA::ULongLong curr_server_time =
            UTO_server->time ();

          sum += curr_server_time;

          ++no_of_servers;

          // Set the highest time to the largest time seen so far.
          if (curr_server_time > highest_time)
            highest_time = curr_server_time;

          // Set the lowest time to the smallest time seen so far.
          if (curr_server_time < lowest_time)
            lowest_time = curr_server_time;

        }

      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "\nUpdated time from %d servers in the network",
                    no_of_servers));

      // Return the average of the times retrieved from the various
      // servers.
      clerk_->time_ = sum / no_of_servers ;

      // Set the Time Displacement Factor. The TZ environment variable is
      // read to set the time zone. We convert the timezone value from seconds
      // to minutes.

      ACE_OS::tzset ();
      long arg = ACE_OS::timezone () / 60;
      CORBA::Short goodarg = static_cast<CORBA::Short> (arg);
      clerk_->time_displacement_factor (goodarg);

      // Set the inaccuracy.
      if (highest_time > lowest_time)
        clerk_->inaccuracy (highest_time - lowest_time);
      else
        clerk_->inaccuracy (0);

      const ACE_Time_Value timeofday = ACE_OS::gettimeofday ();

      // Record the current time in a timestamp to know when global
      // updation of time was done.
      clerk_->update_timestamp_ =
        static_cast<CORBA::ULongLong> (timeofday.sec ()) *
        static_cast<ACE_UINT32> (10000000) +
        static_cast<CORBA::ULongLong> (timeofday.usec () * 10);
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("Exception in handle_timeout()\n");

      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
