// Implementation of the logger object.
// $Id$


#include "ace/OS.h"
#include <iostream.h>
#include "logger_i.h"

// Select non-verbose logging by default.

logger_i::logger_i (int verbose)
  : verbose_value_ (verbose)
{ 
  if (ACE_OS::getenv ("NO_MESSAGES") == 0)
    this->verbose_message_ = 1;
  else
    this->verbose_message_ = 0;
}

// Implement the log method.

void 
logger_i::log (const logger::Log_Record &log_rec, CORBA::Environment &IT_env) 
{
  if (this->verbose_value_) // If verbose mode is on
    {
      char *tm;

      // Convert time
      if ((tm = ACE_OS::ctime (&log_rec.time)) == 0)
	cerr << "ctime failed" << endl;
      else
	{
	  hostent *hp;

	  /* 01234567890123456789012345 */
	  /* Wed Oct 18 14:25:36 1989n0 */
	  tm[24] = '@';
	  cout << tm;
      
	  // Get host name of client

	  if ((hp = gethostbyaddr((char *) &log_rec.host_addr, 
				  sizeof log_rec.host_addr, AF_INET)) == NULL) 
	    {
	      cerr << "server: error in calling gethostbyaddr" << endl;
	      cerr << "h_errno = " << h_errno << endl;
	      return;
	    } 
	  else // Output client hostname.
	    cout << hp->h_name << "@";

	  // Output PID of client
	  cout << log_rec.app_id << "@";   

	  // Output priority

	  switch (log_rec.type) 
	    {
	    case logger::LM_DEBUG:
	      cout << "LM_DEBUG";
	      break;
	    case logger::LM_WARNING:
	      cout << "LM_WARNING";
	      break;
	    case logger::LM_ERROR:
	      cout << "LM_ERROR";
	      break;
	    case logger::LM_EMERG:
	      cout << "LM_EMERG";
	      break;
	    }
	}
    }
  if (this->verbose_message_)
    {
      cout << "::";
      // Output message
      cout.write (log_rec.msg_data._buffer, log_rec.msg_data._length) << flush;   
    }
}

// Enable/disable verbosity.

void 
logger_i::verbose (char verbose, CORBA::Environment &IT_env)
{
  this->verbose_value_ = verbose;
}

// Report current verbosity level.

char
logger_i::verbose (CORBA::Environment &IT_env) 
{
  return this->verbose_value_;
}

// Profile_Logger_i

void
profile_logger_i::start_timer (CORBA::Environment &IT_env) 
{
  this->pt_.start ();
}

void
profile_logger_i::stop_timer (profile_logger::Elapsed_Time& et, 
			      CORBA::Environment &IT_env) 
{
  this->pt_.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time e;

  this->pt_.elapsed_time (e);

  et.real_time   = e.real_time;
  et.user_time   = e.user_time;
  et.system_time = e.system_time;
}
