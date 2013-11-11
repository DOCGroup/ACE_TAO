// $Id$

#if !defined (SPIPE_CONNECTOR_C)

#define SPIPE_CONNECTOR_C

#include "ace/OS_NS_string.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Get_Opt.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_unistd.h"
#include "SPIPE-connector.h"



Peer_Handler::Peer_Handler (int iterations)
  : iterations_ (iterations)
{
}

Peer_Handler::~Peer_Handler (void)
{
}

int
Peer_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("activating %d\n"), this->get_handle ()));

  // If iterations_ has not been set, read from stdin.
  if (iterations_ == 0)
    {
      this->display_menu ();

      if (ACE_Event_Handler::register_stdin_handler (
            this,
            ACE_Reactor::instance (),
            ACE_Thread_Manager::instance ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("register_stdin_handler")),
                            -1);
        }
      else
        {
          return 0;
        }
    }
  else // If iterations_ has been set, send iterations_ buffers.
    {
      const char *buffer =
        "Oh give me a home\n"
        "Where the buffalo roam,\n"
        "And the deer and the antelope play.\n"
        "Where seldom is heard\n"
        "A discouraging word,\n"
        "And the skies are not cloudy all day.\n";
      int length = ACE_Utils::truncate_cast<int> (ACE_OS::strlen (buffer));

      while (iterations_-- > 0
             && this->peer ().send_n (buffer, length) == length)
        {
          continue;
        }

      this->peer ().close ();
      ACE_Reactor::instance ()->end_reactor_event_loop();
      return 0;
    }
}

int
Peer_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ssize_t n = ACE_OS::read (ACE_STDIN,
                            buf,
                            sizeof buf);

  if (n > 0)
    {
      if (this->peer ().send (buf, n) != n)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("write failed")),
                            -1);
        }
      else if (n == 0) // Explicitly close the connection.
        {
          if (this->peer ().close () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("%p\n"),
                                 ACE_TEXT ("close")),
                                1);
            }

          return -1;
        }
      else
        {
          this->display_menu ();
        }
    }

  return 0;
}

int
Peer_Handler::handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Shutting down\n")));
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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nplease enter input..: ")));
}

IPC_Client::IPC_Client (void)
  : iterations_ (0),
    done_handler_ (ACE_Sig_Handler_Ex (ACE_Reactor::end_event_loop))
{
  ACE_OS::strcpy (rendezvous_, ACE_TEXT ("acepipe"));
}

IPC_Client::~IPC_Client (void)
{
}

// Dynamic linking hooks.

int
IPC_Client::init (int argc, ACE_TCHAR *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    return -1;
  // Handle signals through the ACE_Reactor.
  else if (ACE_Reactor::instance ()->register_handler
           (SIGINT,
            &this->done_handler_) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Opening %s\n"), rendezvous_));

  Peer_Handler *ph;

  ACE_NEW_RETURN (ph,
                  Peer_Handler (iterations_),
                  -1);

  // Connect to the peer, reusing the local addr if necessary.
  if (this->connect (ph,
                     ACE_SPIPE_Addr (rendezvous_),
                     ACE_Synch_Options::defaults,
                     ACE_sap_any_cast (ACE_SPIPE_Addr &),
                     0,
                     O_RDWR | FILE_FLAG_OVERLAPPED,
                     0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")),
                      -1);

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
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}

int
IPC_Client::handle_close (ACE_HANDLE,
                          ACE_Reactor_Mask)
{
  return 0;
}

int
IPC_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("ui:r:"));

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 'r':
          ACE_OS::strncpy (rendezvous_,
                           get_opt.opt_arg (),
                           sizeof (rendezvous_) / sizeof (ACE_TCHAR));
          break;
        case 'i':
          iterations_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %n -i <iterations>\n")
                             ACE_TEXT ("-r <rendezvous>\n")),
                            -1);
        }
    }

  return 0;
}


#endif /* SPIPE_CONNECTOR */

