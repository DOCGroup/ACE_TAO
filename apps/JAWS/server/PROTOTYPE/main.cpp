// $Id$

#include "ace/Get_Opt.h"

#include "HTTP_Policy.h"

// Parse arguments

struct SERVER_FLAGS
{
  int port;            // port to listen on
  int concurrency;     // 0 => pool, 1 => per
  int nthreads;        // number of threads
  int maxthreads;      // maximum number of threads
  long flags;          // thread creation flags
};

static SERVER_FLAGS parse_args (int argc, char *argv[])
{
  int c;
  SERVER_FLAGS sflags;

  sflags.port = 5432;
  sflags.concurrency = 0;
  sflags.nthreads = 5;
  sflags.maxthreads = 20;
  sflags.flags = THR_NEW_LWP;

  ACE_Get_Opt getopt (argc, argv, "p:c:n:m:f:");
  while ((c = getopt ()) != -1)
    switch (c)
      {
      case 'p':
        sflags.port = ACE_OS::atoi (getopt.optarg);
        break;
      case 'c':
        sflags.concurrency =
          (ACE_OS::strcmp (getopt.optarg, "PER_REQUEST") == 0);
        break;
      case 'n':
        sflags.nthreads = ACE_OS::atoi (getopt.optarg);
        break;
      case 'm':
        sflags.maxthreads = ACE_OS::atoi (getopt.optarg);
        break;
      case 'f':
        if (ACE_OS::strcmp (getopt.optarg, "THR_BOUND") == 0)
          sflags.flags |= THR_BOUND;
        else if (ACE_OS::strcmp (getopt.optarg, "THR_DAEMON") == 0)
          sflags.flags |= THR_DAEMON;
        else if (ACE_OS::strcmp (getopt.optarg, "THR_DETACHED") == 0)
          sflags.flags |= THR_DETACHED;
        break;
      }

  return sflags;
}

int
main (int argc, char *argv[])
{


  return 0;
}
