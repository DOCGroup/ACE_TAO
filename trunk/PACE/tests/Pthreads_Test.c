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
#include "vxworks_stub.c"
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
