// $Id$

#include "server.h"
#include "Video_Server.h"

// creates a svc handler by passing "this", i.e.  a reference to the
// acceptor that created it this is needed by the svc_handler to
// remove the acceptor handle from the reactor called by the acceptor
// to create a new svc_handler to handle the new connection.

int
AV_Acceptor::make_svc_handler (AV_Svc_Handler *&sh)
{
  ACE_NEW_RETURN (sh,
                  AV_Svc_Handler (ACE_Reactor::instance (),
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
  switch (ACE_OS::fork ("child"))
    {
    case -1:
      // fork failed!!
      ACE_ERROR_RETURN ((LM_ERROR, 
                         "(%P|%t) fork failed\n"),
                        -1);
    case 0:
      // I am the child. i should handle this connection close down
      // the "listen-mode" socket
      ACE_Reactor::instance ()->remove_handler
        (this->acceptor_->get_handle (),
         ACCEPT_MASK);
      
      // Handle this connection in the same thread.
      this->svc ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Child returning from AV_Svc_handler::open\n"));
      return 0;
      
    default:
      // i am the parent. i should go back and listen for more
      // connections

      // (1) "this" will commit suicide, because this svc_handler is not required
      // in the parent. otherwise, a new AV_Svc_handler will be created
      // for each connection, and will never go away, i.e. a memory leak
      // will result. 
      // (2) also, this closes down the "connected socket" in the
      // parent, so that when the child closes down its connected
      // socket the connection is actually closed. otherwise, the
      // connection would remain open forever because the parent still
      // has a connected socket.
      this->destroy ();
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Parent Returning from AV_Svc_Handler::open\n"));
      return 0;
    }
  return 0;
}

// this will handle the connection
int
AV_Svc_Handler::svc (void)
{
  int result;
  result = this->handle_connection ();

  if (result != 0)

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) AV_Svc_Handler::svc exiting\n"));

  return result;
}

// handles the connection

int
AV_Svc_Handler::handle_connection (ACE_HANDLE)
{
  int junk;
  int result;
  u_short port;
  ACE_UINT32 ip;

  // Client is sending us JUNK
  this->peer ().recv_n (&junk, sizeof junk);
  
  // Client is sending us it's port number
  this->peer ().recv_n (&port, sizeof port);
  
  // Client is sending us it's IP addr
  this->peer ().recv_n (&ip, sizeof ip);
  
  this->client_data_addr_.set (port,
                               ip,
                               0);
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Client IP == %s, "
              "Client Port == %d\n",
              client_data_addr_.get_host_name (),
              client_data_addr_.get_port_number ()));

  this->server_data_addr_.set ((unsigned short)0);

  // "Connect" our dgram to the client endpoint.
  if (this->dgram_.open (client_data_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP open failed: %p\n"),
                      -1);
  
  if (this->dgram_.get_local_addr (this->server_data_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) UDP get_local_addr failed: %p\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) UDP IP address is %s, and the port number is %d\n",
              this->server_data_addr_.get_host_addr (),
              this->server_data_addr_.get_port_number ()));

  port = this->server_data_addr_.get_port_number ();

  // %% we need to fix this ?
  // XXX this is a hack to get my IP address set correctly! By default,
  // get_ip_address is returning 0.0.0.0, even after calling
  // get_local_addr () !!
  this->server_data_addr_.set (port,
                               this->server_data_addr_.get_host_name ());

  ip = this->server_data_addr_.get_ip_address ();

  port = htons (port);
  ip = htonl (ip);
  // Client wants us to send the port number first
  this->peer ().send_n (&port,
                        (int) sizeof (u_short));

  // Client wants us to send it the IP address
  this->peer ().send_n (&ip,
                        (int) sizeof (ACE_UINT32));

  // Client is sending us a command
  u_char cmd;
  if (this->peer ().recv_n (&cmd,
                            1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%P|%t, Command recieve failed: %p"),
                       -1);
  // Change these CMD's to enums and put them in a "appropriate" namespace
  switch (cmd)
    {
    case CmdINITvideo:
      {
        // %% what does this do ?!!
        if (Mpeg_Global::live_audio) LeaveLiveAudio ();
        /* result = VideoServer (this->peer ().get_handle (), 
                              this->dgram_.get_handle (), 
                              Mpeg_Global::rttag, 
                              -INET_SOCKET_BUFFER_SIZE); 
        */
               
        ACE_NEW_RETURN (this->vs_,
                        Video_Server (this->peer ().get_handle (),
                                      this->dgram_.get_handle (),
                                      Mpeg_Global::rttag,
                                      -INET_SOCKET_BUFFER_SIZE),
                        -1);

        // registers the video server handlers.
        result = this->vs_->register_handlers ();

        if (result != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) handle_connection: "),
                             result);
        return result;
                 
      }
      break;
    default:
      if (Mpeg_Global::live_audio)
        LeaveLiveAudio ();
      result = AudioServer (this->peer ().get_handle (), 
                            this->dgram_.get_handle (), 
                            Mpeg_Global::rttag, 
                            -INET_SOCKET_BUFFER_SIZE);
      ACE_Reactor::instance ()->end_event_loop ();
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

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.

  //  if (ACE_Reactor::instance ()->register_handler 
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);

  // Create a sigset_t corresponding to the signals we want to catch.
  ACE_Sig_Set sig_set;

  // handles these signals.
  sig_set.sig_add (SIGCHLD);  
  sig_set.sig_add (SIGBUS);
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGTERM);

  // Register the signal handler object to catch the signals.
  //  if (ACE_Reactor::instance ()->register_handler (sig_set, 
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (sig_set,
                                                  this) == -1)
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
        fprintf(stderr, "terminated at signal %d%s.\n", WTERMSIG(status),
                WCOREDUMP(status) ? ", core dumped" : "");
#else
        fprintf(stderr, "terminated at signal %d.\n", WTERMSIG(status));
#endif
      }
    else if (WIFSTOPPED(status)) 
      fprintf(stderr, "stopped at signal %d\n", WSTOPSIG(status));
  }
}

//  ctrl-c, Bus error, interrupt sig handler
void
AV_Server_Sig_Handler::int_handler (int sig)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) killed by signal %d",
              sig));
  exit (0);
}

// AV_Server routines

// Default Constructor
AV_Server::AV_Server ()
{
  this->signal_handler_ = new AV_Server_Sig_Handler ;
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
  if (Mpeg_Global::live_audio > 1) ExitLiveAudio ();
  if (Mpeg_Global::live_video > 1) ExitLiveVideo ();
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
                   char **argv)
{
  int result;

  TAO_TRY
    {
  // Initialize the orb_manager
      this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->orb_manager_.activate_under_child_poa ("Video_Control",
                                                   this->video_control_,
                                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  
  result = this->parse_args (argc, argv);
  if (result < 0)
    return result;

  // Register the various signal handlers with the reactor.
  result = this->signal_handler_->register_handler ();

  if (result < 0)
    return result;

  Mpeg_Global::parentpid = ACE_OS::getpid ();
  
  ::atexit (on_exit_routine);

  // %%
  if (Mpeg_Global::live_audio) 
    {
      if (InitLiveAudio (argc, argv) == -1)
        Mpeg_Global::live_audio = 0;
      else
        Mpeg_Global::live_audio = 2;
    }

  if (Mpeg_Global::live_video) 
    {
      if (InitLiveVideo (argc, argv) == -1)
        Mpeg_Global::live_video = 0;
      else
        Mpeg_Global::live_video = 2;
    }
  
  // open LOG_DIR/vcrsSession.log as the stdout
  // if not, use /dev/null
  {
    char buf [100];
    ACE_OS::sprintf (buf, 
                     "%s%s", 
                     LOG_DIR, 
                     "vcrsSession.log");

    if (::freopen (buf, 
                   "a", 
                   stdout) == NULL) 
      {
        ::freopen ("/dev/null", 
                   "w", 
                   stdout);
      }
  }
  return 0;
}

// Runs the mpeg server
int
AV_Server::run ()
{
  int result;
    this->server_control_addr_.set (VCR_TCP_PORT);

  // "listen" on the socket
  if (this->acceptor_.open (this->server_control_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

    //  ACE_Reactor::instance ()->run_event_loop (); 

  // Run the ORB event loop
  this->orb_manager_->run ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P)AV_Server::run () "
              "came out of the (acceptor) "
              "event loop %p\n",
              "run_event_loop\n"));
}

AV_Server::~AV_Server (void)
{
  if (this->signal_handler_ != 0)
    delete this->signal_handler_;
}

int
main (int argc, char **argv)
{
  AV_Server vcr_server;
  
  // parses the arguments, and initializes the server
  if (vcr_server.init (argc, argv) < 0)
    return 1;
  
  // runs the reactor event loop
  vcr_server.run ();
  
  return 0;
}
