/* $Id$ -*- C -*- */

/* ===================================================================== */
/*                                                                       */
/* = FILENAME                                                            */
/*    Pthreads_Test.c                                                    */
/*                                                                       */
/* =  DESCRIPTION                                                        */
/*     Testing the platform for POSIX threads. This is not meant         */
/*     to be an exhaustive test at this point but more a sanity check    */
/*     that PACE works (at least somewhat) as advertised.                */
/*     This program simply creates some threads, joins them, and then    */
/*     exits.                                                            */
/*                                                                       */
/* = AUTHOR                                                              */
/*    Joe Hoffert <joeh@cs.wustl.edu>                                    */
/*                                                                       */
/* ===================================================================== */

#include "pace/stdio.h"
#include "pace/pthread.h"

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0

# define main ace_main
# define PACE_NEEDS_HUGE_THREAD_STACKSIZE 64000
# include /**/ <usrLib.h>   /* for ::sp() */

/* This global function can be used from the VxWorks shell to pass
 * arguments to a C main () function.
 *
 * usage: -> spa main, "arg1", "arg2"
 *
 * All arguments must be quoted, even numbers.
 */
int
spa (FUNCPTR entry, ...)
{
  const unsigned int MAX_ARGS = 10;
  char *argv[MAX_ARGS];
  va_list pvar;
  unsigned int argc;
  int ret;

  /* Hardcode a program name because the real one isn't available
   * through the VxWorks shell.
   */
  argv[0] = "ace_main";

  /* Peel off arguments to spa () and put into argv.  va_arg () isn't
   * necessarily supposed to return 0 when done, though since the
   * VxWorks shell uses a fixed number (10) of arguments, it might 0
   * the unused ones.  This function could be used to increase that
   * limit, but then it couldn't depend on the trailing 0.  So, the
   * number of arguments would have to be passed.
   */
  va_start (pvar, entry);

  for (argc = 1; argc <= MAX_ARGS; ++argc)
    {
      argv[argc] = va_arg (pvar, char *);

      if (argv[argc] == 0)
        break;
    }

  if (argc > MAX_ARGS  &&  argv[argc-1] != 0)
    {
      /* try to read another arg, and warn user if the limit was exceeded */
      if (va_arg (pvar, char *) != 0)
        pace_fprintf (stderr, "spa(): number of arguments limited to %d\n",
                      MAX_ARGS);
    }
  else
    {
      /* fill unused argv slots with 0 to get rid of leftovers
       * from previous invocations
       */
      unsigned int i;
      for (i = argc; i <= MAX_ARGS; ++i)
        argv[i] = 0;
    }

  /* The hard-coded options are what ::sp () uses, except for the
   * larger stack size (instead of ::sp ()'s 20000).
   */
  ret = taskSpawn (argv[0],    /* task name */
                   100,        /* task priority */
                   VX_FP_TASK, /* task options */
                   PACE_NEEDS_HUGE_THREAD_STACKSIZE, /* stack size */
                   entry,      /* entry point */
                   argc,       /* first argument to main () */
                   (int) argv, /* second argument to main () */
                   0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  /* taskSpawn () returns the taskID on success: return 0 instead if
   * successful
   */
  return ret > 0 ? 0 : ret;
}
#endif /* VXWORKS */

void
func1 (int *pnum_times)
{
  int i;
  int j;
  int x;

  for (i = 0; i < 4; i++) 
    {
      pace_printf("func1\n");
      for (j = 0; j < 100000; j++)
        x += i;

      (*pnum_times)++;
    }
}

void
func2 (int *pnum_times)
{
  int i;
  int j;
  int x;

  for (i = 0; i < 4; i++) 
    {
      pace_printf("func2\n");
      for (j = 0; j < 100; j++)
        x += i;

      (*pnum_times)++;
    }
}

void
join_point (int func1_times, int func2_times)
{
  int total;

  total = func1_times + func2_times;
  pace_printf("join_point: func1 times %d, func2 times %d, total %d\n",
              func1_times, func2_times, total);
}

int
main (int argc, char **argv)
{
  /* Test creating a thread. */
  pace_pthread_t thread1;
  pace_pthread_t thread2;
  int func1_ret = 0;
  int func2_ret = 0;

  pace_pthread_create(&thread1,
                      NULL,
                      (void *) func1,
                      (void *) &func1_ret);

  pace_pthread_create(&thread2,
                      NULL,
                      (void *) func2,
                      (void *) &func2_ret);

  pace_pthread_join(thread1, NULL);
  pace_pthread_join(thread2, NULL);

  join_point(func1_ret, func2_ret);

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);
  return 0;
}
