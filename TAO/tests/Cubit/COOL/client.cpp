//**************************************************************************
//
// NAME :   client.C 
// DESCRIPTION:  
//
// Client for the Cubit example
//
//****************************************************************************

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "api/api.H"
#include "cubit.H"
#include "Task_Client.h"

int LOOP_COUNT=100;
char SERVER_HOST [1024];
unsigned int THREAD_COUNT=1;

// = TITLE
//     Parses the command line arguments and returns an error status
//
// = DESCRIPTION
//     This method parses the command line arguments
int parse_args(int argc, char *argv[])
{
  ACE_OS::strcpy (SERVER_HOST, "localhost");
  ACE_Get_Opt opts (argc, argv, "n:t:");
  int			c;
  
  while ((c = opts ()) != -1)
    switch (c) {
    case 'h':
      ACE_OS::strcpy (SERVER_HOST, opts.optarg);
      continue;
    case 'n':			// loop count
      LOOP_COUNT = (unsigned) ACE_OS::atoi (opts.optarg);
      continue;
    case 't':
      THREAD_COUNT = (unsigned) ACE_OS::atoi (opts.optarg);
      continue;
    case '?':
    default:
      ACE_OS::fprintf (stderr, "usage:  %s"
                       " [-d]"
                       " [-n loopcount]"
                       " [-h SERVER_HOST]"
                       " [-x]"
                       " [-t num_threads]"
                       "\n", argv [0]
                       );
      return 1;
    }
  
  return 0;  // Indicates successful parsing of command line
}


//
// Mainline
//
int
main (int argc, char *argv [])
{
  CORBA_Environment env;
  CORBA_ORB_ptr orb = CORBA_ORB_init (argc, argv, 0, env);

  if (parse_args (argc, argv) != 0)
    return -1;

  if (env.exception ()) {
    ACE_ERROR ((LM_ERROR, "ORB_init failed..\n"));
    return -1;
  }

  Client clients (SERVER_HOST, THREAD_COUNT, LOOP_COUNT);
  //  ACE_Thread_Manager::instance ()->wait ();
  ACE_OS::printf ("Test done.\n"
                  "High priority client latency : %f msec\n"
                  "Low priority client latency : %f msec\n",
                  clients.get_high_priority_latency (),
                  clients.get_low_priority_latency ());

}
