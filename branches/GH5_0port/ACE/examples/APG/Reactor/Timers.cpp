// $Id$

#include "ace/config-all.h"
#include "ace/OS_main.h"

#if !defined (ACE_LACKS_FORK)

#include "ace/streams.h"

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

typedef void (*timerTask_t)(void);

// Listing 1 code/ch07
pid_t timerTask (int initialDelay,
                 int interval,
                 timerTask_t task)
{
  if (initialDelay < 1 && interval < 1)
    return -1;

  pid_t pid = fork ();

  if (pid < 0)
    return -1;

  if (pid > 0)
    return pid;

  if (initialDelay > 0)
    sleep (initialDelay);

  if (interval < 1)
    return 0;

  while (1)
    {
      (*task) ();
      sleep (interval);
    }

  ACE_NOTREACHED (return 0);
}
// Listing 1

// Listing 2 code/ch07
void foo ()
{
  time_t now = time (0);
  cerr << "The time is " << ctime (&now) << endl;
}
// Listing 2

void programMainLoop (void)
{
  sleep (30);
}

// Listing 3 code/ch07
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  pid_t timerId = timerTask (3, 5, foo);
  programMainLoop ();
  kill (timerId, SIGINT);
  return 0;
}
// Listing 3

#else
#include <stdio.h>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  puts ("This very unportable example requires fork().\n");
  return 0;
}

#endif /* ACE_LACKS_FORK */
