#include "server.h"

Mpeg_Svc_Handler::Mpeg_Svc_Handler (ACE_Reactor *r)
  : ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> (0, 0, r)
{

}

// Client connected to our control port
int
Mpeg_Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)Mpeg_Svc_Handler::open called\n"));
  this->activate ();
  return 0;
}


int
Mpeg_Svc_Handler::svc (void)
{
  return this->handle_input ();
}

int
Mpeg_Svc_Handler::handle_input (ACE_HANDLE)
{  
  int junk;
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
              "(%P|%t) Client IP == %s, Client Port == %d\n",
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
  switch (cmd)
    {
    case CmdINITvideo:
      if (Mpeg_Global::live_audio) LeaveLiveAudio();
      VideoServer (this->peer ().get_handle (), 
                   this->dgram_.get_handle (), 
                   Mpeg_Global::rttag, 
                   -INET_SOCKET_BUFFER_SIZE);
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t): video server returned.\n"),
                        -1);
      break;
    default:
      if (Mpeg_Global::live_audio) LeaveLiveAudio();
      AudioServer (this->peer ().get_handle (), 
                   this->dgram_.get_handle (), 
                   Mpeg_Global::rttag, 
                   -INET_SOCKET_BUFFER_SIZE);
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t): video server returned.\n"),
                        -1);
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
  :fdTable_ (0),
   size_ (0)
{

}

//  Cluttering the code with various signal handlers here.

 void
Mpeg_Server::int_handler(int sig)
{
  /*
  fprintf(stderr, "process %d killed by sig %d\n", getpid(), sig);
  */
  exit(0);
}

 void
Mpeg_Server::on_exit_routine(void)
{
  if (Mpeg_Global::parentpid != getpid()) {
    /*
    fprintf(stderr, "process %d exiting...\n", getpid());
    */
    return;
  }
  /*
  fprintf(stderr, "deamon exiting . . .\n");
  */
  if (Mpeg_Global::live_audio > 1) ExitLiveAudio();
  if (Mpeg_Global::live_video > 1) ExitLiveVideo();
  ComCloseServer();
}

 void
Mpeg_Server::clear_child(int sig)
{
  int pid;
  int status;
  
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    Mpeg_Global::session_num --;
    
    if (status == 0) {
      continue;
    }
    fprintf(stderr, "VCRS: child %d (status %d) ", pid, status);
    if (WIFEXITED(status)) {
      fprintf(stderr, "exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
#if defined(_HPUX_SOURCE) || defined(__svr4__) || defined(IRIX)
      fprintf(stderr, "terminated at signal %d%s.\n", WTERMSIG(status),
	      WCOREDUMP(status) ? ", core dumped" : "");
#else
      fprintf(stderr, "terminated at signal %d.\n", WTERMSIG(status));
#endif
    }
    else if (WIFSTOPPED(status)) {
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
        unlink(VCR_UNIX_PORT);        
        break;
      case '?':
      case 'h':// help flag
        fprintf(stderr, "Usage: %s [-rt] [-nrt] [-rm]\n", argv[0]);
        fprintf(stderr, "          [-d#int(clock drift in ppm)]\n");
        fprintf(stderr, "          [-s#int(limit on number of sessions)]\n");
        return -1;
      }
  return 0;
}

// sets the handlers for the various signals
int
Mpeg_Server::set_signals ()
{
  setsignal(SIGCHLD,clear_child);
  setsignal(SIGPIPE, SIG_IGN);
  setsignal(SIGBUS, int_handler);
  setsignal(SIGINT, int_handler);
  setsignal(SIGTERM, int_handler);
  setsignal(SIGALRM, SIG_IGN);
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
  Mpeg_Global::parentpid = getpid();

  atexit(on_exit_routine);

  if (Mpeg_Global::live_audio) {
    if (InitLiveAudio(argc, argv) == -1)
      Mpeg_Global::live_audio = 0;
    else
      Mpeg_Global::live_audio = 2;
  }

  if (Mpeg_Global::live_video) {
    if (InitLiveVideo(argc, argv) == -1)
      Mpeg_Global::live_video = 0;
    else
      Mpeg_Global::live_video = 2;
  }

  /*  ComInitServer(VCR_TCP_PORT, VCR_UNIX_PORT, VCR_ATM_PORT); */
  
  /*
  setpgrp();
  */
  {
    char buf[100];
    sprintf(buf, "%s%s", LOG_DIR, "vcrsSession.log");
    if (freopen(buf, "a", stdout) == NULL) {
      freopen("/dev/null", "w", stdout);
    }
  }
  return 0;
}

// Runs the mpeg server
int
Mpeg_Server::run ()
{
  this->server_control_addr_.set (VCR_TCP_PORT);

  this->server_control_addr_.dump ();
  // "listen" on the socket
  if (this->acceptor_.open (this->server_control_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  // enter the reactor event loop
  ACE_Reactor::run_event_loop ();
}

#if 0
{
  for (;;)
  {
    int val;
    unsigned char cmd;
    int serverpid = -1;
    int cfd, dfd;
    int max_pkt_size;

    if (ComGetConnPair(&cfd, &dfd, &max_pkt_size) == -1) continue;
    
    if ((serverpid = fork()) == -1)
    {
      perror("VCRS error on creating service process");
      exit(1);
    }
    Mpeg_Global::session_num ++;
    if (serverpid > 0)  /* parent process for forking servers */
    {
      ComCloseFd(cfd);
      ComCloseFd(dfd);
      continue;
    }
    else
    {
      if (Mpeg_Global::session_num > Mpeg_Global::session_limit)
        {
          time_t t;
          char *buf;
          t = time(NULL);
          buf = ctime(&t);
          buf[strlen(buf) - 1] = 0;
          fprintf(stderr, "VCRS: %s, session_limit %d, session_number %d\n",
                  buf, Mpeg_Global::session_limit, Mpeg_Global::session_num);
        }
      
      if ((val = read(cfd, &cmd, 1)) < 0)
        {
          perror("VCRS fails to read command from service socket");
          exit(1);
        }
      if (val == 0) {
	fprintf(stderr, "Client has closed connection.\n");
	ComCloseConn(cfd);
	ComCloseConn(dfd);
	/* continue; -- I don't know why I wrote this line? scen 5-12-96 */
        exit (0);
      }
      ComCloseListen();
      if (cmd == CmdINITvideo)
      {
	/*
	fprintf(stderr, "Server forked a VideoServer process.\n");
	*/
	if (Mpeg_Global::live_audio) LeaveLiveAudio();
	VideoServer(cfd, dfd, Mpeg_Global::rttag, max_pkt_size);
	fprintf(stderr, "Weird: video server returned.\n");
      }
      else
      {
	/*
	fprintf(stderr, "Server forked a AudioServer process.\n");
	*/
	if (Mpeg_Global::live_video) 
          LeaveLiveVideo ();

        AudioServer (cfd, 
                     dfd, 
                     Mpeg_Global::rttag, 
                     max_pkt_size);

	fprintf (stderr, 
                 "Weird: audio server returned.\n");

      }
      exit(1);
    }
  }
  return 0;
}
#endif

Mpeg_Server::~Mpeg_Server (void)
{
}

int
main (int argc, char **argv)
{
  int result;

  Mpeg_Server vcr_server;

  result = vcr_server.init (argc, argv);

  if (result < 0)
    exit (1);
  
  vcr_server.run ();

  return 0;
}
