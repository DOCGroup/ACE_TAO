#if !defined (SPIPE_CONNECTOR_C)
// $Id$

#define SPIPE_CONNECTOR_C


#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Get_Opt.h"
#include "SPIPE-connector.h"

Peer_Handler::Peer_Handler (int iterations)
  : iterations_ (iterations)
{
}

Peer_Handler::~Peer_Handler ()
{
}

int
Peer_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "activating %d\n", this->get_handle ()));

  // If iterations_ has not been set, read from stdin.
  if (iterations_ == 0)
    {
      this->display_menu ();
      if (ACE::register_stdin_handler (this,
				      ACE_Reactor::instance (),
					  ACE_Thread_Manager::instance ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_stdin_handler"), -1);
      else
	return 0;
    }
  else // If iterations_ has been set, send iterations_ buffers.
    {
      char *buffer =
	"Oh give me a home\n"
	"Where the buffalo roam,\n"
	"And the deer and the antelope play.\n"
	"Where seldom is heard\n"
	"A discouraging word,\n"
	"And the skies are not cloudy all day.\n";
      int length = ACE_OS::strlen (buffer);

      while (iterations_-- > 0
	     && this->peer ().send_n (buffer, length) == length)
	continue;

      this->peer ().close ();
      ACE_Reactor::end_event_loop();
      return 0;
    }
}

int
Peer_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ssize_t n = ACE_OS::read (ACE_STDIN, buf, sizeof buf);

  if (n > 0)
    if (this->peer ().send (buf, n) != n)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "write failed"), -1);
    else if (n == 0) /* Explicitly close the connection. */
      {
	if (this->peer ().close () == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);
	return -1;
      }
    else
      this->display_menu ();
  return 0;
}

int
Peer_Handler::handle_close (ACE_HANDLE,
			    ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Shutting down\n"));
  return 0;
}

ACE_HANDLE
Peer_Handler::get_handle (void) const
{
  return this->peer ().get_handle ();
}

void
Peer_Handler::display_menu (void)
{
  ACE_DEBUG ((LM_DEBUG, "\nplease enter input..: "));
}

IPC_Client::IPC_Client (void)
  : iterations_ (0),
    done_handler_ (ACE_Sig_Handler_Ex (ACE_Proactor::end_event_loop))
{
  ACE_OS::strcpy (rendezvous_, __TEXT ("acepipe"));
}

IPC_Client::~IPC_Client (void)
{
}

// Dynamic linking hooks.

int
IPC_Client::init (int argc, char *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    return -1;
  // Handle signals through the ACE_Reactor.
  else if (ACE_Reactor::instance ()->register_handler
	   (SIGINT, &this->done_handler_) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, "Opening %s\n", ACE_MULTIBYTE_STRING (rendezvous_)));

  Peer_Handler *ph;

  ACE_NEW_RETURN (ph, Peer_Handler (iterations_), -1);

  // Connect to the peer, reusing the local addr if necessary.
  if (this->connect (ph,
		     ACE_SPIPE_Addr (rendezvous_),
		     ACE_Synch_Options::defaults,
		     *((ACE_SPIPE_Addr *) &ACE_Addr::sap_any),
		     0,
		     O_RDWR | FILE_FLAG_OVERLAPPED,
		     0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect"), -1);

  return 0;
}

int
IPC_Client::fini (void)
{
  return 0;
}

int
IPC_Client::svc (void)
{
  ACE_Reactor::run_event_loop ();
  return 0;
}

int
IPC_Client::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return 0;
}

int
IPC_Client::parse_args (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, "ui:r:");

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'r':
	  ACE_OS::strncpy (rendezvous_,
			   ACE_WIDE_STRING (get_opt.optarg),
			   sizeof rendezvous_ / sizeof TCHAR);
	  break;
	case 'i':
	  iterations_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'u':
	default:
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "usage: %n -i <iterations>\n"
			     "-r <rendezvous>\n"), -1);
	  break;
	}
    }

  return 0;
}


#endif /* SPIPE_CONNECTOR */


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<Peer_Handler, ACE_SPIPE_CONNECTOR>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Peer_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Peer_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Tuple<Peer_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Peer_Handler, ACE_SPIPE_CONNECTOR>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Peer_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Peer_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Svc_Tuple<Peer_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

