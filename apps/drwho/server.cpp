// $Id$
/* Driver program for the server.  Note that it is easy to reuse the
   server for other distributed programs.  Pretty much all that must
   change are the functions registered with the communciations
   manager. */ 

#include "ace/OS.h"
#include "Options.h"
#include "new.h"
#include "SMR_Server.h"

static char *
tstamp (void)
{
  long    time_now;
  char   *temp;

  time_now = time (0);
  temp = asctime (localtime (&time_now));
  temp[12] = 0;
  return temp;
}

/* Catch the obvious signals and die with dignity... */

static void
exit_server (int sig)
{
  char buffer[80];
  sprintf (buffer, "%s exiting on signal", tstamp ());
  psignal (sig, buffer);
  monitor (0);
  exit (0);
}

/* Returns TRUE if the program was started by INETD. */

static int
started_by_inetd (void)
{
  /* SUPPRESS 175 */
  sockaddr_in 	sin;
  int 		size = sizeof sin;

  return getsockname (0, (sockaddr *) &sin, &size) == 0;
}

/* Does the drwho service. */

static void
do_drwho (SMR_Server &smr_server)
{
  if (smr_server.receive () == -1)
    perror (Options::program_name);
	  
  if (smr_server.send () == -1)
    perror (Options::program_name);
}

/* If the server is started with any argument at all then it doesn't
   fork off a child process to do the work.  This is useful when
   debugging! */

int
main (int argc, char *argv[])
{
  signal (SIGTERM, SIG_PF (exit_server));
  signal (SIGINT, SIG_PF (exit_server));
  signal (SIGQUIT, SIG_PF (exit_server));

  Options::set_options (argc, argv);
  Options::set_opt (Options::STAND_ALONE_SERVER);

  int inetd_controlled = started_by_inetd ();

  if (!inetd_controlled && Options::get_opt (Options::BE_A_DAEMON))
    daemon_start (1);

  mark_memory ();

  SMR_Server smr_server (Options::port_number);

  /* I'm not sure what this should do yet... */

  if (inetd_controlled)
    do_drwho (smr_server);
  else
    {

      for (;;)
	do_drwho (smr_server);

      /* NOTREACHED */
    }
  exit (0);
}
