// -*- C++ -*-
// $Id$

#include "Timer_Helper.h"
#include "TAO_Time_Service_Clerk.h"

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

  TAO_TRY
    {
      IORS::TYPE* value;
      for (IORS::ITERATOR server_iterator (this->clerk_->server_);
	   server_iterator.next (value) != 0;
	   server_iterator.advance ())
	{

	  printf("Making a remote Call\n");
	  // This is a remote call.
	  CosTime::UTO_var UTO_server =
	    (*value)->universal_time (TAO_TRY_ENV);

	  TAO_CHECK_ENV;

	  #if defined (ACE_LACKS_LONGLONG_T)

	  ACE_DEBUG ((LM_DEBUG,
		      "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q\n"
		      "struct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
		      ACE_U64_TO_U32 (UTO_server->time (TAO_TRY_ENV)),
		      ACE_U64_TO_U32 (UTO_server->inaccuracy (TAO_TRY_ENV)),
		      UTO_server->tdf (TAO_TRY_ENV),
		      ACE_U64_TO_U32 ((UTO_server->utc_time ()).time),
		      (UTO_server->utc_time ()).inacclo,
		      (UTO_server->utc_time ()).inacchi,
		      (UTO_server->utc_time ()).tdf));

          #else

	  ACE_DEBUG ((LM_DEBUG,
		      "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q\n"
		      "struct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
		      UTO_server->time (TAO_TRY_ENV),
		      UTO_server->inaccuracy (TAO_TRY_ENV),
		      UTO_server->tdf (TAO_TRY_ENV),
		      (UTO_server->utc_time ()).time,
		      (UTO_server->utc_time ()).inacclo,
		      (UTO_server->utc_time ()).inacchi,
		      (UTO_server->utc_time ()).tdf));
          #endif

	  // This is a remote call.
	  sum += (CORBA::ULongLong) UTO_server->time (TAO_TRY_ENV);

	  ++no_of_servers;
	}

      ACE_DEBUG ((LM_DEBUG,
		  "\nUpdated time from %d servers in the network",
		  no_of_servers));

      // Return the average of the times retrieved from the various
      // servers.
      clerk_->time_ = sum / no_of_servers ;

      // Record the current time in a timestamp to know when global
      // updation of time was done.
      clerk_->update_timestamp_ = ACE_OS::gettimeofday ().sec () * 10000000
        + ACE_OS::gettimeofday ().usec () * 10;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in the handle_timeout ()\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base <CosTime::TimeService_var>;
template class ACE_Array_Iterator <CosTime::TimeService_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base <CosTime::TimeService_var>
#pragma instantiate ACE_Array_Iterator <CosTime::TimeService_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
