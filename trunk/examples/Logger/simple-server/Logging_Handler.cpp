
// $Id$

#include "Logging_Handler.h"
#include "Reactor_Singleton.h"

// Default constructor.

Logging_Handler::Logging_Handler (void)
{
}

Logging_Handler::~Logging_Handler (void)
{
  REACTOR::instance ()->cancel_timer (this);
  this->cli_stream_.close ();
}

// Extract the underlying ACE_SOCK_Stream (e.g., for purposes of
// accept()).

Logging_Handler::operator ACE_SOCK_Stream &()
{
  return this->cli_stream_;
}

int
Logging_Handler::handle_timeout (const ACE_Time_Value &,
				const void *arg)
{
  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
  return 0;
}

// Perform the logging record receive. 

int
Logging_Handler::handle_input (ACE_HANDLE)
{
  ssize_t n; 
  size_t len;

  // Perform two recv's to emulate record-oriented semantics.  Note
  // that this code is not entirely portable since it relies on the
  // fact that sizeof (ssize_t) is the same on both the sender and
  // receiver side.  To correctly handle this is painful, and we leave
  // it as an exercise for the reader ;-).

  switch (n = this->cli_stream_.recv ((void *) &len, sizeof len))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client logger", this->host_name_), -1);      
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) closing log daemon at host %s (fd = %d)\n",
			this->host_name_, this->get_handle ()), -1);
      /* NOTREACHED */
    case sizeof (size_t):
      {
	ACE_Log_Record lp;

        len = ntohl (len);
	n = this->cli_stream_.recv_n ((void *) &lp, len);
	if (n != (ssize_t) len)
	  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			    "client logger", this->host_name_), -1);
	/* NOTREACHED */
	  
	lp.decode ();

	if (lp.length () == n)
	  {
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) "));
	    lp.print (this->host_name_, 1, cerr);
	  }
	else
	  ACE_ERROR ((LM_ERROR, "(%P|%t) error, lp.length = %d, n = %d\n",
		     lp.length (), n));
	break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client logger", this->host_name_), -1);
      /* NOTREACHED */
    }

  return 0;
}

// Extract underlying device descriptor.

ACE_HANDLE
Logging_Handler::get_handle (void) const
{
  return this->cli_stream_.get_handle ();
}

int
Logging_Handler::open (void)
{
  ACE_INET_Addr addr;
  
  if (this->cli_stream_.get_remote_addr (addr) == -1)
    return -1;
  else
    {
      ACE_OS::strncpy (this->host_name_, 
		       addr.get_host_name (), 
		       MAXHOSTNAMELEN + 1);

      if (REACTOR::instance ()->register_handler 
	  (this, ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "(%P|%t) can't register with reactor\n"), 
			   -1);
      else if (REACTOR::instance ()->schedule_timer
	  (this, (const void *) this, 
	   ACE_Time_Value (2), 
	   ACE_Time_Value (2)) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "can'(%P|%t) t register with reactor\n"), 
			  -1);
      else
      	ACE_DEBUG ((LM_DEBUG, 
		    "(%P|%t) connected with %s\n", 
		    this->host_name_));
      return 0;
    }
}

// Perform termination activities when deregistered from the
// ACE_Reactor.

int
Logging_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask mask)
{
  // Must be allocated dynamically!
  if (mask == ACE_Event_Handler::READ_MASK)
    delete this;
  return 0;
}

// Perform termination activities when close fails.

int
Logging_Handler::close (void)
{
  return this->handle_close (ACE_INVALID_HANDLE, 
			     ACE_Event_Handler::READ_MASK);
}
