// $Id$

#include "server.h"
#include "ace/Process.h"

// Creates a svc handler by passing "this", i.e.  a reference to the
// acceptor that created it this is needed by the svc_handler to
// remove the acceptor handle from the reactor called by the acceptor
// to create a new svc_handler to handle the new connection.

int
AV_Acceptor::make_svc_handler (AV_Svc_Handler *&sh)
{
  ACE_NEW_RETURN (sh,
                  //                AV_Svc_Handler (ACE_Reactor::instance (),
                  AV_Svc_Handler (TAO_ORB_Core_instance ()->reactor (),
                                  this),
                  -1);
  return 0;
}

// Initialize the svc_handler, and the acceptor. 

AV_Svc_Handler::AV_Svc_Handler (ACE_Reactor *reactor,
                                AV_Acceptor *acceptor)
  : ACE_Svc_Handler <ACE_SOCK_STREAM, 
                     ACE_NULL_SYNCH> (0, 0, reactor),
    acceptor_ (acceptor)
{
}

// Client connected to our control port called by the reactor
// (acceptor).

int
AV_Svc_Handler::open (void *)
{
  // Lets use threads at a later point. The current scheme works fine
  // with fork..  this will activate a thread this->activate
  // (THR_BOUND);
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) AV_Svc_Handler::open called\n"));
  return this->svc ();
}

// this will handle the connection
int
AV_Svc_Handler::svc (void)
{
  int result;
  result = this->handle_connection ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Svc_Handler::svc exiting\n"));

  return result;
}

// handles the connection

int
AV_Svc_Handler::handle_connection (ACE_HANDLE)
{
  // Client is sending us a command
  u_char cmd;
  if (this->peer ().recv_n (&cmd,
                            1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%P|%t, Command recieve failed: %p"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Command recieved is %d",
              cmd));
  // Change these CMD's to enums and put them in a "appropriate" namespace
  switch (cmd)
    {
    case CmdINITvideo:
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Spawning the video server process\n"));
        
        // %% what does this do ?!!
        if (Mpeg_Global::live_audio) LeaveLiveAudio ();
        /* result = VideoServer (this->peer ().get_handle (), 
           this->dgram_.get_handle (), 
           Mpeg_Global::rttag, 
           -INET_SOCKET_BUFFER_SIZE); 
        */

        ACE_Process_Options video_process_options;
        video_process_options.command_line ("./vs -ORBport 0");

        // Create the semaphore
        ACE_SV_Semaphore_Simple semaphore (4242,
                                           ACE_SV_Semaphore_Simple::ACE_CREATE,
                                           0);
 
        
        ACE_Process video_process;
        video_process.spawn (video_process_options);
        // %% need to close down the orb fd's
        // in the child process!!

        // %% wait until the child finishes booting
        semaphore.acquire ();
        // Wait until a ACE_SV_Semaphore's value is greater than 0, the
        // decrement it by 1 and return. Dijkstra's P operation, Tannenbaums
        // DOWN operation.
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
        int ack = 42;
        if (this->peer ().send_n (&ack,
                                  sizeof (ack)) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t), ACK send failed: %p",
                             "AV_Svc_Handler::handle_connection"),
                            -1);
      }
      this->destroy ();
      break;
    default:
      // %% need to fork here
      if (Mpeg_Global::live_audio)
        LeaveLiveAudio ();
      int result = AudioServer (this->peer ().get_handle (), 
                                this->dgram_.get_handle (), 
                                Mpeg_Global::rttag, 
                                -INET_SOCKET_BUFFER_SIZE);
      //    ACE_Reactor::instance ()->end_event_loop ();
      TAO_ORB_Core_instance ()->reactor ()->end_event_loop ();
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t)handle_connection : "),
                          result);
      return result;
     
    }

  return 0;
}

int
AV_Svc_Handler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)AV_Svc_Handler::close called \n"));
  return 0;
}

int
AV_Svc_Handler::handle_timeout (const ACE_Time_Value &,
                                  const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)AV_Svc_Handler::handle_timeout called \n"));
  return 0;
}

// AV_Server_Sig_Handler routines

AV_Server_Sig_Handler::AV_Server_Sig_Handler (void)
{
}

int
AV_Server_Sig_Handler::register_handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != -1);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) sig_handler == %d\n",
              this->handle_));

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.

  if (TAO_ORB_Core_instance ()->reactor ()->register_handler
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);

  // handles these signals.
  this->sig_set.sig_add (SIGCHLD);  
  this->sig_set.sig_add (SIGBUS);
  this->sig_set.sig_add (SIGINT);
  this->sig_set.sig_add (SIGTERM);

  // Register the signal handler object to catch the signals.  if
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler 
      (this->sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);
  return 0;
}

// Called by the ACE_Reactor to extract the fd.
ACE_HANDLE
AV_Server_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int 
AV_Server_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int 
AV_Server_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.
//
// There are several advantages to using this approach.  First, 
// the behavior triggered by the signal is handled in the main event
// loop, rather than in the signal handler.  Second, the ACE_Reactor's 
// signal handling mechanism eliminates the need to use global signal 
// handler functions and data. 

int
AV_Server_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGCHLD:
      // Handle the death of child signal.
      this->clear_child (SIGCHLD);
      break;
    case SIGBUS:
      // Handle the Bus error signal
    case SIGINT:
      // Handle the interrupt signal
    case SIGTERM:
      // Handle the process termination signal.
      this->int_handler (signum);
      break;
    default:
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) %S: not handled, returning to program\n", 
                  signum));
      break;
    }
  return 0;
}

// SIGCHLD handler
void
AV_Server_Sig_Handler::clear_child (int sig)
{
  int pid;
  int status;
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Reaping the children\n"));
  // reap the children
  while ((pid = ACE_OS::waitpid (-1, 
                                 &status, 
                                 WNOHANG)) > 0)
  {
    // decrement the count of number of active children
    Mpeg_Global::session_num --;
    
    if (status == 0) 
      continue;
    
    ACE_DEBUG ((LM_DEBUG, 
                "(%P|%t) VCRS: child %d (status %d) ", 
                pid, 
                status));

    // %% what does the following do
    if (WIFEXITED(status)) 
      {
        // @@ Can you replace these fprintfs with the appropriate
        // ACE_DEBUG/ACE_ERROR macros?!
        fprintf(stderr, "exited with status %d\n", WEXITSTATUS(status));
      }
    else if (WIFSIGNALED(status)) 
      {
        // %% can we remove the below ?
#if defined(_HPUX_SOURCE) || defined(__svr4__) || defined(IRIX)
        fprintf (stderr, "terminated at signal %d%s.\n", WTERMSIG(status),
                 WCOREDUMP(status) ? ", core dumped" : "");
#else
        fprintf (stderr, "terminated at signal %d.\n", WTERMSIG(status));
#endif /* defined(_HPUX_SOURCE) || defined(__svr4__) || defined(IRIX) */
      }
    else if (WIFSTOPPED(status)) 
      fprintf(stderr, "stopped at signal %d\n", WSTOPSIG(status));
  }
}

//  ctrl-c, Bus error, interrupt sig handler
void
AV_Server_Sig_Handler::int_handler (int sig)
{
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) killed by signal %d\n",
              sig));
}

AV_Server_Sig_Handler::~AV_Server_Sig_Handler (void)
{
  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->sig_set);
}

// AV_Server routines

// Default Constructor
AV_Server::AV_Server (void)
{
}

// %% move to the destructor or sig handler
void
AV_Server::on_exit_routine (void)
{
  // %% what does the following do
  if (Mpeg_Global::parentpid != ACE_OS::getpid ()) 
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "(%P|%t) Process is exiting\n"));
      return;
    }
  
  // %% what does the following do
  if (Mpeg_Global::live_audio > 1)
    ExitLiveAudio ();
  if (Mpeg_Global::live_video > 1)
    ExitLiveVideo ();
  //  ComCloseServer();
}


// Parses the command line arguments
int
AV_Server::parse_args (int argc,
                         char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "rd:s:vamh");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'r': // real time flag
        Mpeg_Global::rttag = 1;
        break;
      case 'd': // clock drift in ppm
        Mpeg_Global::drift_ppm = ACE_OS::atoi (get_opts.optarg);
        break;
      case 's':// limit on number of sessions
        Mpeg_Global::session_limit = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'v':// live video flag
        Mpeg_Global::live_video = 1;
        break;
      case 'a':// live audio flag
        Mpeg_Global::live_audio = 1;
        break;
      case 'm':// remove flag
        ACE_OS::unlink (VCR_UNIX_PORT);
        break;
      case '?':
      case 'h':// help flag
        ACE_DEBUG ((LM_DEBUG,
                    "Usage: %s [-r ] [-m]\n"
                    "          [-d#int(clock drift in ppm)]\n"
                    "          [-s#int(limit on number of sessions)]\n"
                    "          [-v] [-a] [-?] [-h]",
                    argv [0]));
        return -1;
      }
  return 0;
}

        
// Initializes the mpeg server
int
AV_Server::init (int argc,
                   char **argv,
                 CORBA::Environment& env)
{
  int result;

  // Initialize the orb_manager
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);
  
  CORBA::ORB_var orb = 
    this->orb_manager_.orb ();

  PortableServer::POA_var child_poa = 
    this->orb_manager_.child_poa ();
  // Initialize the Naming Server
  this->naming_server_.init (orb,
                             child_poa);

  result = this->parse_args (argc, argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error parsing arguments"),
                      -1);
  
  // Register the various signal handlers with the reactor.
  result = this->signal_handler_.register_handler ();

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error registering signal handler"),
                      -1);

  ::atexit (on_exit_routine);
  return 0;
}

// Runs the mpeg server
int
AV_Server::run (CORBA::Environment& env)
{
  int result;
  this->server_control_addr_.set (VCR_TCP_PORT);

  // "listen" on the socket
  if (this->acceptor_.open (this->server_control_addr_,
                            TAO_ORB_Core_instance ()->reactor ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) acceptor_handler == %d\n",
              this->acceptor_.get_handle ()));

  // Run the ORB event loop
  this->orb_manager_.run (env);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Server::run () "
              "came out of the (acceptor) "
              "event loop %p\n",
              "run_event_loop"));
  return 0;
  
}

AV_Server::~AV_Server (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Server: Removing handlers from the Reactor\n"));
  
  TAO_ORB_Core_instance ()->reactor ()->remove_handler 
    (this->acceptor_.get_handle (),
     ACE_Event_Handler::ACCEPT_MASK);

  TAO_ORB_Core_instance ()->reactor ()->remove_handler
    (&this->signal_handler_,
     ACE_Event_Handler::NULL_MASK);
}

int
main (int argc, char **argv)
{
  AV_Server vcr_server; 

  TAO_TRY
    {
      // Parses the arguments, and initializes the server.
      if (vcr_server.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
  
      // Runs the reactor event loop.
      vcr_server.run (TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}
