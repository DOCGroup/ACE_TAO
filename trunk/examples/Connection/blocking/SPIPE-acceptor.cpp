#if !defined (SPIPE_ACCEPTOR_C)
// $Id$

#define SPIPE_ACCEPTOR_C

#include "ace/Log_Msg.h"
#include "ace/SPIPE_Addr.h"
#include "ace/Time_Value.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/Get_Opt.h"
#include "SPIPE-acceptor.h"

Svc_Handler::Svc_Handler (void)
{
}

Svc_Handler::~Svc_Handler (void)
{
}

int 
Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "client connected on handle %d\n",
	      this->peer ().get_handle ()));
  return ACE_Service_Config::proactor ()->initiate 
    (this, ACE_Event_Handler::READ_MASK);
}

ACE_Message_Block *
Svc_Handler::get_message (void)
{
  // An extra byte for null termination.
  ACE_Message_Block *message = 
    new ACE_Message_Block (BUFSIZ + 1);
  message->size (BUFSIZ);
  return message;
}

int 
Svc_Handler::handle_input_complete (ACE_Message_Block *msg, 
				    long bytes_transfered)
{
  if (bytes_transfered > 0)
    {
      msg->base ()[msg->length ()] = '\0';
      // Print out the message received from the server.
      ACE_DEBUG ((LM_DEBUG, "(%t) message size %d.\n", msg->length ()));
      ACE_DEBUG ((LM_DEBUG, "%s", msg->rd_ptr ()));
      
      return 1; // Reinvoke a recv() operation.
    }
  else
    return -1; // Close down.
}

IPC_Server::IPC_Server (void)
  : n_threads_ (1),
    rendezvous_ ("acepipe"),
    done_handler_ (ACE_Sig_Handler_Ex (ACE_Service_Config::end_proactor_event_loop))
{
}

IPC_Server::~IPC_Server (void)
{
}

int 
IPC_Server::init (int argc, char *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, "Opening %s\n", rendezvous_));

  // Initialize named pipe listener.
  if (this->open (ACE_SPIPE_Addr (rendezvous_)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  // Register to receive shutdowns.
  else if (ACE_Service_Config::reactor ()->register_handler
      (SIGINT, &this->done_handler_) == -1)
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
	  rendezvous_ = get_opt.optarg;
	  break;
	case 't':
	  n_threads_ = ACE_OS::atoi (get_opt.optarg);
	  ACE_DEBUG ((LM_DEBUG, "%s == %d.\n", 
		      get_opt.optarg,
		      n_threads_));
	  ACE_Service_Config::proactor (2*n_threads_);
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

static void *
run_reactor_event_loop (void *)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  ACE_DEBUG ((LM_DEBUG, "(%t) worker thread starting\n"));

  ACE_Service_Config::run_proactor_event_loop ();
  return 0;
}

int 
IPC_Server::svc (void)
{
  // Performs the iterative server activities.
  while (ACE_Service_Config::reactor_event_loop_done () == 0)
    {
      Svc_Handler sh;
      
      // Create a new SH endpoint, which performs all processing in
      // its open() method (note no automatic restart if errno ==
      // EINTR).
      if (this->accept (&sh, 0) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "accept"), 1);
      // SH's destructor closes the stream implicitly but the
      // listening endpoint stays open.
      else
	{
	  // Run single-threaded.
	  if (n_threads_ <= 1)
	    run_reactor_event_loop (0);
	  else
	    {
	      // Run thread pool.
	      if (ACE_Service_Config::thr_mgr ()->spawn_n (n_threads_,
							   run_reactor_event_loop,
							   0, THR_NEW_LWP)
		  == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

	      ACE_Service_Config::thr_mgr ()->wait ();
	    }

	  ACE_DEBUG ((LM_DEBUG, "(%t) main thread exiting.\n"));
	}
    }
  
  /* NOTREACHED */
  return 0;
}

#endif /* SPIPE_ACCEPTOR_C */
