// -*- C++ -*-
// $Id$

#include "Timer_Helper.h"
#include "TAO_Time_Service_Clerk.h"
#include "ace/OS.h"

// Constructor.
Timer_Helper::Timer_Helper (void)
{
}

// Constructor that sets the clerk.
Timer_Helper::Timer_Helper (TAO_Time_Service_Clerk *clerk)
  : clerk_ (clerk)
{

}

// Destructor.
Timer_Helper::~Timer_Helper (void)
{
  delete this;
}

int
Timer_Helper::handle_timeout (const ACE_Time_Value &,
			      const void *)
{
  int no_of_servers = 0;
  CORBA::ULongLong sum = 0;
  
  // The following are used to keep a track of the inaccuracy
  // in synchronization.

#if defined (ACE_LACKS_LONGLONG_T)
  CORBA::ULongLong lowest_time (0xFFFFFFFF, 0xFFFFFFFF);
#else
  CORBA::ULongLong lowest_time = ACE_UINT64_LITERAL(0xFFFFFFFFFFFFFFFF);
#endif
  
  CORBA::ULongLong highest_time  = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      IORS::TYPE* value;
      for (IORS::ITERATOR server_iterator (this->clerk_->server_);
	   server_iterator.next (value) != 0;
	   server_iterator.advance ())
	{
	  
	  // This is a remote call.
	  CosTime::UTO_var UTO_server =
	    (*value)->universal_time (ACE_TRY_ENV);
	  
	  ACE_TRY_CHECK;
	  
          #if defined (ACE_LACKS_LONGLONG_T)
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q\n"
		      "struct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
		      ACE_U64_TO_U32 (UTO_server->time (ACE_TRY_ENV)),
		      ACE_U64_TO_U32 (UTO_server->inaccuracy (ACE_TRY_ENV)),
		      UTO_server->tdf (ACE_TRY_ENV),
		      ACE_U64_TO_U32 ((UTO_server->utc_time ()).time),
		      (UTO_server->utc_time ()).inacclo,
		      (UTO_server->utc_time ()).inacchi,
		      (UTO_server->utc_time ()).tdf));
	  
          #else

	  ACE_DEBUG ((LM_DEBUG,
		      "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q\n"
		      "struct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
		      UTO_server->time (ACE_TRY_ENV),
		      UTO_server->inaccuracy (ACE_TRY_ENV),
		      UTO_server->tdf (ACE_TRY_ENV),
		      (UTO_server->utc_time ()).time,
		      (UTO_server->utc_time ()).inacclo,
		      (UTO_server->utc_time ()).inacchi,
		      (UTO_server->utc_time ()).tdf));
          #endif
	  
	  CORBA::ULongLong curr_server_time = UTO_server->time (ACE_TRY_ENV);
	  
	  ACE_TRY_CHECK;

	  sum += curr_server_time;
	  
	  ++no_of_servers;
	  
	  // Set the highest time to the largest time seen so far.
	  if (curr_server_time > highest_time)
	    highest_time = curr_server_time;
	  
	  // Set the lowest time to the smallest time seen so far.
	  if (curr_server_time < lowest_time)
	    lowest_time = curr_server_time;
	  
	}

      ACE_DEBUG ((LM_DEBUG,
		  "\nUpdated time from %d servers in the network",
		  no_of_servers));
      
      // Return the average of the times retrieved from the various
      // servers.
      clerk_->time_ = sum / no_of_servers ;

      // Set the Time Displacement Factor. The TZ environment variable is 
      // read to set the time zone. We convert the timezone value from seconds 
      // to minutes.

      ACE_OS::tzset();
      clerk_->time_displacement_factor (ACE_OS::timezone () / 60);

      // Set the inaccuracy.
      if (highest_time > lowest_time)
	clerk_->inaccuracy (highest_time - lowest_time);
      else
	clerk_->inaccuracy (0);
      
      // Record the current time in a timestamp to know when global
      // updation of time was done.
      clerk_->update_timestamp_ = ACE_OS::gettimeofday ().sec () * 10000000
        + ACE_OS::gettimeofday ().usec () * 10;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in the handle_timeout ()\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base <CosTime::TimeService_var>;
template class ACE_Array_Iterator <CosTime::TimeService_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base <CosTime::TimeService_var>
#pragma instantiate ACE_Array_Iterator <CosTime::TimeService_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
