// Service_Manager.cpp
// $Id$


#define ACE_BUILD_DLL
#include "ace/Get_Opt.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Config.h"
#include "ace/Service_Manager.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Manager)

void
ACE_Service_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Service_Manager::dump");
}

/* Static variables. */

u_short ACE_Service_Manager::DEFAULT_PORT_ = 10000;

ACE_Service_Manager::ACE_Service_Manager (void)
  : debug_ (0), 
    signum_ (SIGHUP)
{
  ACE_TRACE ("ACE_Service_Manager::ACE_Service_Manager");
}

int
ACE_Service_Manager::suspend (void)
{
  ACE_TRACE ("ACE_Service_Manager::suspend");
  return ACE_Service_Config::reactor ()->suspend_handler (this);
}
     
int
ACE_Service_Manager::resume (void)
{
  ACE_TRACE ("ACE_Service_Manager::resume");
  return ACE_Service_Config::reactor ()->resume_handler (this);
}

int
ACE_Service_Manager::open (const ACE_INET_Addr &sia)
{
  ACE_TRACE ("ACE_Service_Manager::open");
  // Reuse the listening address, even if it's already in use!
  if (this->acceptor_.open (sia, 1) == -1)
    return -1;
  return 0;
}

int 
ACE_Service_Manager::info (char **strp, size_t length) const
{
  ACE_TRACE ("ACE_Service_Manager::info");
  ACE_INET_Addr sa;
  char buf[BUFSIZ];

  if (this->acceptor_.get_local_addr (sa) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%d/%s %s", sa.get_port_number (), "tcp", 
	     "# lists all services in the daemon\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

int
ACE_Service_Manager::init (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Service_Manager::init");
  ACE_INET_Addr local_addr (ACE_Service_Manager::DEFAULT_PORT_);
  ACE_Get_Opt	getopt (argc, argv, "dp:s:", 0); // Start at argv[0]

  for (int c; (c = getopt ()) != -1; )
     switch (c)
       {
       case 'd':
	 this->debug_ = 1;
	 break;
       case 'p': 
	 local_addr.set (ACE_OS::atoi (getopt.optarg));
	 break;
       case 's':
	 this->signum_ = ACE_OS::atoi (getopt.optarg);
	 break;
       default:
	 break;
       }
  
  if (this->open (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Service_Config::reactor ()->register_handler 
	   (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);
  return 0;
}

int
ACE_Service_Manager::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Service_Manager::handle_close");
  return this->acceptor_.close ();
}

int
ACE_Service_Manager::fini (void)
{
  ACE_TRACE ("ACE_Service_Manager::fini");
  return ACE_Service_Config::reactor ()->remove_handler 
    (this, ACE_Event_Handler::READ_MASK);
}
  
ACE_HANDLE
ACE_Service_Manager::get_handle (void) const
{ 
  ACE_TRACE ("ACE_Service_Manager::get_handle");
  return this->acceptor_.get_handle (); 
}

int
ACE_Service_Manager::handle_signal (int sig, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Service_Manager::handle_signal");
  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG, "got %S\n", sig));
  return 0;
}

// Determine all the services offered by this daemon and return the
// information back to the client. 

int
ACE_Service_Manager::list_services (void)
{
  ACE_TRACE ("ACE_Service_Manager::list_services");
  ACE_Service_Repository_Iterator sri (*ACE_Service_Config::svc_rep ());

  for (const ACE_Service_Record *sr; 
       sri.next (sr) != 0; 
       sri.advance ())
    {
      int len = ACE_OS::strlen (sr->name ()) + 1;
      char buf[BUFSIZ], *p = buf + len;

      ACE_OS::strcpy (buf, sr->name ());
      p[-1] = ' ';
      p[0]  = '\0';

      len += sr->type ()->info (&p, sizeof buf - len);

      if (this->debug_)
	ACE_DEBUG ((LM_DEBUG, "len = %d, info = %s%s",
		   len, buf, buf[len - 1] == '\n' ? "" : "\n"));

      if (len > 0)
	{
	  ssize_t n = this->client_stream_.send_n (buf, len);

	  if (n != len || (n == -1 && errno != EPIPE))
	    ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));
	}
    }

  return 0;
}

// Trigger a remote reconfiguration of the Service Configurator.

int
ACE_Service_Manager::reconfigure_services (void)
{
  ACE_TRACE ("ACE_Service_Manager::reconfigure_services");

#if 0
// Send ourselves a signal!  ACE_OS::kill (ACE_OS::getpid (),
// this->signum_);
#endif /* 0 */

  // Flag the main event loop that a reconfiguration should occur.
  // The next trip through the ACE_Service_Config::run_reactor_event_loop()
  // should pick this up and cause a reconfiguration!
  ACE_Service_Config::reconfig_occurred ((sig_atomic_t) 1);
  return this->client_stream_.send_n ("done\n", sizeof ("done\n"));
}

// Accept new connection from client and carry out the service they
// request.

int 
ACE_Service_Manager::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Service_Manager::handle_input");

  if (this->acceptor_.accept (this->client_stream_) == -1)
    return -1;

  if (this->debug_)
    {
      ACE_DEBUG ((LM_DEBUG, "client_stream fd = %d\n",
		 this->client_stream_.get_handle ()));
      ACE_INET_Addr sa;
      if (this->client_stream_.get_remote_addr (sa) == -1)
	return -1;

      ACE_DEBUG ((LM_DEBUG, "accepted from host %s at port %d\n", 
		sa.get_host_name (), sa.get_port_number ()));
    }

  char request[BUFSIZ];

  // Read service request from client.

  switch (client_stream_.recv (request, sizeof request))
    {
    case -1:
      if (this->debug_)
	ACE_DEBUG ((LM_ERROR, "%p\n", "recv"));
      break;
    case 0:
      return 0;
      /* NOTREACHED */
    default:
      {
	char *p;

	// Kill trailing newlines.

	for (p = request; 
	     (*p != '\0') && (*p != '\r') && (*p != '\n'); 
	     p++)
	  continue;

	*p = '\0';

	ACE_Event_Handler *old_signal_handler = 0;
	ACE_Service_Config::reactor ()->register_handler (SIGPIPE, this, 0,
							  &old_signal_handler);

	if (ACE_OS::strcmp (request, "help") == 0)
	  this->list_services ();
	else if (ACE_OS::strcmp (request, "reconfigure") == 0)
	  this->reconfigure_services ();

	// Additional management services may be handled here... 

	// Restore existing SIGPIPE handler
	ACE_Service_Config::reactor ()->register_handler 
	  (SIGPIPE, old_signal_handler);
      }
    }
  if (this->client_stream_.close () == -1 && this->debug_)
    ACE_DEBUG ((LM_ERROR, "%p\n", "close"));

  return 0;
}
