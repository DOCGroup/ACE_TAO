
#include "server.h"

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
        this->rttag_ = 1;
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

  ComInitServer(VCR_TCP_PORT, VCR_UNIX_PORT, VCR_ATM_PORT);
  
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
	fprintf(stderr, "Remote client has closed connection.\n");
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
	VideoServer(cfd, dfd, this->rttag_, max_pkt_size);
	fprintf(stderr, "Weird: video server returned.\n");
      }
      else
      {
	/*
	fprintf(stderr, "Server forked a AudioServer process.\n");
	*/
	if (Mpeg_Global::live_video) LeaveLiveVideo();
        AudioServer(cfd, dfd,this->rttag_, max_pkt_size);
	fprintf(stderr, "Weird: audio server returned.\n");
      }
      exit(1);
    }
  }
  return 0;
}

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
