#include "server.h"
#include "../mpeg_server/Video_Server.h"

// creates a svc handler by passing "this", i.e. 
// a reference to the acceptor that created it
// this is needed by the svc_handler to remove the
// acceptor handle from the reactor
// called by the acceptor to create a new svc_handler to 
// handle the new connection.
int
Mpeg_Acceptor::make_svc_handler (Mpeg_Svc_Handler *&sh)
{
  ACE_NEW_RETURN (sh,
                  Mpeg_Svc_Handler (ACE_Reactor::instance (),
                                    this),
                  -1);
  return 0;
}

// initialize the svc_handler, and the acceptor. 
Mpeg_Svc_Handler::Mpeg_Svc_Handler (ACE_Reactor *reactor,
                                    Mpeg_Acceptor *acceptor)
  : ACE_Svc_Handler <ACE_SOCK_STREAM, 
                     ACE_NULL_SYNCH> (0, 0, reactor),
    acceptor_ (acceptor)
{
  
}

// Client connected to our control port
// called by the reactor (acceptor)
int
Mpeg_Svc_Handler::open (void *)
{

  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Mpeg_Svc_Handler::open"
              "called\n"));
  // Lets use threads at a later point. The current scheme works fine
  // with fork..  
  // this will activate a thread 
  //  this->activate (THR_BOUND);
  switch (ACE_OS::fork ("child"))
    {
    case -1:
      // fork failed!!
      ACE_ERROR_RETURN ((LM_ERROR, 
                         "(%P|%t) fork failed\n"),
                        -1);
    case 0:
      // I am the child. i should handle this connection
      // close down the "listen-mode" socket
      ACE_Reactor::instance ()->remove_handler
        (this->acceptor_->get_handle (),
         ACCEPT_MASK);
      
      // handle this connection in the same thread
      this->svc ();

      ACE_DEBUG ((LM_DEBUG,
                  "Parent returning from Mpeg_Svc_handler::open\n"));
      return 0;
      
    default:
      // i am the parent. i should go back and listen for more
      // connections

      // (1) this will commit suicide, because this svc_handler is not required
      // in the parent. otherwise, a new mpeg_svc_handler will be created
      // for each connection, and will never go away, i.e. a memory leak
      // will result. 
      // (2) also, this closes down the "connected socket" in the
      // parent, so that when the child closes down its connected
      // socket the connection is actually closed. otherwise, the
      // connection would remain open forever because the parent still
      // has a connected socket.
      this->destroy ();
      ACE_DEBUG ((LM_DEBUG,
                  "Child Returning from Mpeg_Svc_Handler::open\n"));
      return 0;
    }
  return 0;
}


// this will handle the connection
int
Mpeg_Svc_Handler::svc (void)
{
  int result;
  // %% this needs to be renamed, the name is misleading!
  result = this->handle_input ();

  if (result != 0)

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Mpeg_Svc_Handler::svc exiting\n"));

  return result;
}

// handles the connection
int
Mpeg_Svc_Handler::handle_input (ACE_HANDLE)
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
               
        this->vs_.init (this->peer ().get_handle (),
                        this->dgram_.get_handle (),
                        Mpeg_Global::rttag,
                        -INET_SOCKET_BUFFER_SIZE);

        // enters the 
        result = this->vs_.run ();

        if (result != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) handle_input: "),
                             result);
        return result;
                 
      }
      break;
    default:
      if (Mpeg_Global::live_audio) LeaveLiveAudio();
      result = AudioServer (this->peer ().get_handle (), 
                            this->dgram_.get_handle (), 
                            Mpeg_Global::rttag, 
                            -INET_SOCKET_BUFFER_SIZE);
      ACE_Reactor::instance ()->end_event_loop ();
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t)handle_input : "),
                          result);
      return result;
     
    }

  return 0;
}

int
Mpeg_Svc_Handler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)Mpeg_Svc_Handler::close called \n"));
  return 0;
}

int
Mpeg_Svc_Handler::handle_timeout (const ACE_Time_Value &,
                                  const void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)Mpeg_Svc_Handler::handle_timeout called \n"));
  return 0;
}

// Default Constructor
Mpeg_Server::Mpeg_Server ()
{

}

//  Cluttering the code with various signal handlers here.

//  ctrl-c handler
void
Mpeg_Server::int_handler (int sig)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) killed by signal %d",
              sig));
  exit (0);
}

void
Mpeg_Server::on_exit_routine (void)
{
  // %% what does the following do
  if (Mpeg_Global::parentpid != ACE_OS::getpid ()) 
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "(%P|%t) exiting"));
      return;
    }
  
  // %% what does the following do
  if (Mpeg_Global::live_audio > 1) ExitLiveAudio ();
  if (Mpeg_Global::live_video > 1) ExitLiveVideo ();
  //  ComCloseServer();
}

// SIGCHLD handler
void
Mpeg_Server::clear_child (int sig)
{
  int pid;
  int status;
  
  // reap the children
  while ((pid = ACE_OS::waitpid (-1, 
                                 &status, 
                                 WNOHANG)) > 0)
  {
    // decrement the count of number of active children
    Mpeg_Global::session_num --;
    
    if (status == 0) 
      {
        continue;
      }
    
    ACE_DEBUG ((LM_DEBUG, 
                "(%P|%t) VCRS: child %d (status %d) ", 
                pid, 
                status));

    // %% what does the following do
    if (WIFEXITED(status)) 
      {
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
      {
        fprintf(stderr, "stopped at signal %d\n", WSTOPSIG(status));
      }
    
  }
}

// Parses the command line arguments
int
Mpeg_Server::parse_args (int argc,
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

// sets the handlers for the various signals
int
Mpeg_Server::set_signals ()
{
  setsignal (SIGCHLD, clear_child);
  setsignal (SIGPIPE, SIG_IGN);    
  setsignal (SIGBUS, int_handler); 
  setsignal (SIGINT, int_handler); 
  setsignal (SIGTERM, int_handler);
  //  setsignal(SIGALRM, SIG_IGN);
  return 0;
}

        
// Initializes the mpeg server
int
Mpeg_Server::init (int argc,
                   char **argv)
{
  int result;

  result = this->parse_args (argc, argv);
  if (result < 0)
    return result;

  this->set_signals ();
  Mpeg_Global::parentpid = ACE_OS::getpid ();
  
  ::atexit (on_exit_routine);
  
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
Mpeg_Server::run ()
{
  int result;
  this->server_control_addr_.set (VCR_TCP_PORT);

  // "listen" on the socket
  if (this->acceptor_.open (this->server_control_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  ACE_Reactor::instance ()->run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P)Mpeg_Server::run () "
              "came out of the (acceptor) "
              "event loop %p\n",
              "run_event_loop\n"));
}

Mpeg_Server::~Mpeg_Server (void)
{
}

int
main (int argc, char **argv)
{
  Mpeg_Server vcr_server;
  
  // parses the arguments, and initializes the server
  if (vcr_server.init (argc, argv) < 0)
    return 1;
  
  // runs the reactor event loop
  vcr_server.run ();
  
  return 0;
}
