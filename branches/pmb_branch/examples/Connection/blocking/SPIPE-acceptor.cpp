// $Id$

#if !defined (SPIPE_ACCEPTOR_C)
#define SPIPE_ACCEPTOR_C

#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/Proactor.h"
#include "ace/Get_Opt.h"
#include "SPIPE-acceptor.h"

ACE_RCSID(blocking, SPIPE_acceptor, "$Id$")

Svc_Handler::Svc_Handler (void)
  : mb_ (BUFSIZ + 1)
{
  // An extra byte for null termination.
  this->mb_.size (BUFSIZ);
}

Svc_Handler::~Svc_Handler (void)
{
}

int
Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              "client connected on handle %d\n",
	      this->peer ().get_handle ()));
  if (this->ar_.open (*this,
                      this->peer ().get_handle ()) == -1)
    return -1;
  return this->ar_.read (this->mb_,
                         this->mb_.size ());
}

void
Svc_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  if (result.success () && result.bytes_transferred () > 0)
    {
      result.message_block ().rd_ptr ()[result.message_block ().length ()] = '\0';

      // Print out the message received from the server.
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) message size %d.\n",
                  result.message_block ().length ()));
      ACE_DEBUG ((LM_DEBUG,
                  "%s",
                  result.message_block ().rd_ptr ()));
      // Reset the message block here to make sure multiple writes to
      // the pipe don't keep appending to the message_block!
      this->mb_.reset ();

      this->ar_.read (this->mb_, this->mb_.size ());
    }
  else
    ACE_Proactor::end_event_loop ();
}

IPC_Server::IPC_Server (void)
  : n_threads_ (1),
    shutdown_ (0)
{
  ACE_OS::strcpy (rendezvous_, ACE_TEXT ("acepipe"));
}

IPC_Server::~IPC_Server (void)
{
}

int 
IPC_Server::handle_signal (int signum,
                           siginfo_t *,
                           ucontext_t *)
{
  ACE_LOG_MSG->log (LM_INFO,
                    "IPC_Server::handle_signal().\n");

  // Flag the main <svc> loop to shutdown.
  this->shutdown_ = 1;

  this->acceptor ().close (); // Close underlying acceptor.
  // This should cause the <accept> to fail, which will "bounce"
  // us out of the loop in <svc>.
  return 0;
}

int
IPC_Server::init (int argc, char *argv[])
{
  if (this->parse_args (argc,
                        argv) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              "Opening %s\n",
              rendezvous_));

  // Initialize named pipe listener.
  if (this->open (ACE_SPIPE_Addr (rendezvous_)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"), 1);

  // Register to receive shutdowns using this handler.
  else if (ACE_Reactor::instance ()->register_handler
      (SIGINT, this) == -1)
    return -1;
  else
    return 0;
}

int
IPC_Server::fini (void)
{
  return 0;
}

int
IPC_Server::parse_args (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, "ut:r:");

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'r':
	  ACE_OS::strncpy (rendezvous_,
			   ACE_TEXT_CHAR_TO_TCHAR (get_opt.opt_arg ()),
			   sizeof rendezvous_ / sizeof ACE_TCHAR);
	  break;
	case 't':
	  n_threads_ = ACE_OS::atoi (get_opt.opt_arg ());
	  ACE_DEBUG ((LM_DEBUG, "%s == %d.\n",
		      get_opt.opt_arg (),
		      n_threads_));
	  ACE_Proactor::instance (2 * n_threads_);
	  // This is a lame way to tell the proactor how many threads
	  // we'll be using.
	  break;
	case 'u':
	default:
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "usage: %n -t <threads>\n"
			     "-r <rendezvous>\n"), -1);
	  break;
	}
    }

  return 0;
}

static ACE_THR_FUNC_RETURN
run_reactor_event_loop (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) worker thread starting\n"));

  ACE_Proactor::run_event_loop ();
  return 0;
}

int
IPC_Server::svc (void)
{
  // Performs the iterative server activities.
  while (this->shutdown_ == 0)
    {
      Svc_Handler sh;
		
      // Create a new SH endpoint, which performs all processing in
      // its open() method (note no automatic restart if errno ==
      // EINTR).
      if (this->accept (&sh, 0) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "accept"),
                          1);
		
      // SH's destructor closes the stream implicitly but the
      // listening endpoint stays open.
      else
        {
          // Run single-threaded.
          if (n_threads_ <= 1)
            run_reactor_event_loop (0);
          else if (ACE_Thread_Manager::instance ()->spawn_n 
                   (n_threads_,
                    run_reactor_event_loop,
                    0,
                    THR_NEW_LWP) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%p\n",
                                 "spawn_n"),
                                1);
				
              ACE_Thread_Manager::instance ()->wait ();
            }
			
          ACE_DEBUG ((LM_DEBUG, 
                      "(%t) main thread exiting.\n"));

          // Reset the Proactor so another accept will work.  
          ACE_Proactor::reset_event_loop();

          // Must use some other method now to terminate this thing
          // instead of the ACE_Signal_Adapter just running
          // ACE_Proactor::end_event_loop()...  Since this is an
          // ACE_Event_Handler, doesn't it seem possible to implement
          // a handle_signal() hook, and catch the signal there?
        }
    }
	
  /* NOTREACHED */
  return 0;
}

#endif /* SPIPE_ACCEPTOR_C */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_Oneshot_Acceptor<Svc_Handler, ACE_SPIPE_ACCEPTOR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Concurrency_Strategy<Svc_Handler>
#pragma instantiate ACE_Oneshot_Acceptor<Svc_Handler, ACE_SPIPE_ACCEPTOR>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

