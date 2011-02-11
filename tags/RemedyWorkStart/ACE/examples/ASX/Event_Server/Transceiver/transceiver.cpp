// $Id$

// Test program for the event transceiver.  This program can play the
// role of either Consumer or Supplier.  You can terminate this
// program by typing ^C....

#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Service_Config.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"
#include "ace/OS_NS_unistd.h"

#include "transceiver.h"

// Handle the command-line arguments.

int
Event_Transceiver::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("Ch:p:S"));

  this->port_number_ = ACE_DEFAULT_SERVER_PORT;
  this->host_name_ = ACE_DEFAULT_SERVER_HOST;
  this->role_ = ACE_TEXT ("Supplier");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'C':
        this->role_ = ACE_TEXT ("Consumer");
        break;
      case 'h':
        this->host_name_ = get_opt.opt_arg ();
        break;
      case 'p':
        this->port_number_ = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'S':
        this->role_ = ACE_TEXT ("Supplier");
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %n [-CS] [-h host_name] [-p portnum]\n")),
                          -1);
        /* NOTREACHED */
        break;
      }

  // Increment by 1 if we're the supplier to mirror the default
  // behavior of the Event_Server (which sets the Consumer port to
  // ACE_DEFAULT_SERVER_PORT and the Supplier port to
  // ACE_DEFAULT_SERVER_PORT + 1).  Note that this is kind of a
  // hack...
  if (ACE_OS::strcmp (this->role_, ACE_TEXT ("Supplier")) == 0
      && this->port_number_ == ACE_DEFAULT_SERVER_PORT)
    this->port_number_++;
  return 0;
}

int
Event_Transceiver::handle_close (ACE_HANDLE,
                                 ACE_Reactor_Mask)
{
  ACE_Reactor::instance ()->end_reactor_event_loop ();
  return 0;
}

// Close down via SIGINT or SIGQUIT.

int
Event_Transceiver::handle_signal (int,
                                  siginfo_t *,
                                  ucontext_t *)
{
  ACE_Reactor::instance ()->end_reactor_event_loop ();
  return 0;
}

Event_Transceiver::Event_Transceiver (void)
{
}

Event_Transceiver::Event_Transceiver (int argc, ACE_TCHAR *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("parse_args")));
  else
    {
      ACE_Sig_Set sig_set;

      sig_set.sig_add (SIGINT);
      sig_set.sig_add (SIGQUIT);

      // Register to handle the SIGINT and SIGQUIT signals.
      if (ACE_Reactor::instance ()->register_handler
          (sig_set,
           this) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("register_handler")));

      // We need to register <this> here before we're connected since
      // otherwise <get_handle> will return the connection socket
      // handle for the peer.
      else if (ACE_Event_Handler::register_stdin_handler (this,
                                                          ACE_Reactor::instance (),
                                                          ACE_Thread_Manager::instance ()) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("register_stdin_handler")));

      // Address of the server.
      ACE_INET_Addr server_addr (this->port_number_,
                                 this->host_name_);

      ACE_Connector<Event_Transceiver, ACE_SOCK_CONNECTOR> connector;

      // We need a pointer here because connect takes a reference to a
      // pointer!
      Event_Transceiver *etp = this;

      // Establish the connection to the Event Server.
      if (connector.connect (etp,
                             server_addr) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      this->host_name_));
          ACE_Reactor::instance()->remove_handler (sig_set);
          ACE_Event_Handler::remove_stdin_handler (ACE_Reactor::instance(),
                                                   ACE_Thread_Manager::instance());
        }
    }
}

int
Event_Transceiver::open (void *)
{
  // Register ourselves to be notified when there's data to read on
  // the socket.
  if (ACE_Reactor::instance ()->register_handler
      (this,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("register_handler")),
                       -1);
  return 0;
}

int
Event_Transceiver::handle_input (ACE_HANDLE handle)
{
  // Determine whether we play the role of a consumer or a supplier.
  if (handle == ACE_STDIN)
    return this->transmitter ();
  else
    return this->receiver ();
}

int
Event_Transceiver::transmitter (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) entering %s transmitter\n"),
              this->role_));

  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (ACE_STDIN, buf, sizeof buf);
  int result = 0;

  if (n <= 0 || this->peer ().send_n (buf, n) != n)
    result = -1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) leaving %s transmitter\n"),
              this->role_));
  return result;
}

int
Event_Transceiver::receiver (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) entering %s receiver\n"),
              this->role_));

  char buf[BUFSIZ];

  ssize_t n = this->peer ().recv (buf, sizeof buf);
  int result = 0;

  if (n <= 0
      || ACE_OS::write (ACE_STDOUT, buf, n) != n)
    result = -1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) leaving %s receiver\n"),
              this->role_));
  return result;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (ACE_Service_Config::open (argv[0]) == -1
      && errno != ENOENT)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                       -1);

  // Create and initialize the transceiver.
  Event_Transceiver transceiver (argc, argv);

  // Demonstrate how we can check if a constructor failed...
  if (ACE_LOG_MSG->op_status () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Event_Transceiver constructor failed")),
                       -1);


  // Run event loop until either the event server shuts down or we get
  // a SIGINT.
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}

