/* $Id$ -*- C -*- */

/* ===================================================================== */
/*                                                                       */
/* = FILENAME                                                            */
/*    Cond_Var_Test.c                                                    */
/*                                                                       */
/* =  DESCRIPTION                                                        */
/*     Testing the platform for POSIX condition variables. This is not   */
/*     meant to be an exhaustive test at this point but more a sanity    */
/*     check that PACE works (at least somewhat) as advertised.          */
/*     This program simply creates some threads, waits on a condition    */
/*     variable, joins the threads, and then exits.                      */
/*                                                                       */
/*     This test is largely taken from the O'Reilly _Pthreads            */
/*     Programming_ book and accompanying example code.                  */
/*                                                                       */
/* = AUTHOR                                                              */
/*    Joe Hoffert <joeh@cs.wustl.edu>                                    */
/*                                                                       */
/* ===================================================================== */

#include "pace/stdio.h"
#include "pace/stdlib.h"
#include "pace/pthread.h"

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
#include "vxworks_stub.c"
#endif /* VXWORKS */

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_THRES 12

int count = 0;
int thread_ids[3] = {0,1,2};
#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
/* VxWorks does not handle the *_ININITALIZER values and we
 * can not emulate it since it involves memory allocation for
 * VxWorks.
 */
pace_pthread_mutex_t count_lock;
pace_pthread_cond_t count_hit_threshold;
#else
pace_pthread_mutex_t count_lock = PACE_PTHREAD_MUTEX_INITIALIZER;
pace_pthread_cond_t count_hit_threshold = PACE_PTHREAD_COND_INITIALIZER;
#endif

void *inc_count(void *idp)
{
  int i = 0;
  /*int *my_id = idp;*/
  PACE_UNUSED_ARG (idp);

  pace_sleep(1);

  for (i = 0; i < TCOUNT; i++) {
    pace_pthread_mutex_lock(&count_lock);
    count++;
    /* pace_printf("inc_counter(): thread %d, count = %d, unlocking mutex\n", 
	   *my_id, count); */
    if (count == COUNT_THRES) {
      /* pace_printf("inc_count(): Thread %d, count %d\n", *my_id, count); */
      pace_pthread_cond_signal(&count_hit_threshold);
    }
    pace_pthread_mutex_unlock(&count_lock);
  }
  
  return(NULL);
}

void *watch_count(void *idp)
{
  /*int *my_id = idp;*/
  PACE_UNUSED_ARG (idp);

  /* pace_printf("watch_count(): thread %d\n", *my_id); */

  pace_pthread_mutex_lock(&count_lock);

  while (count < COUNT_THRES) {
    pace_pthread_cond_wait(&count_hit_threshold, &count_lock);
    /* pace_printf("watch_count(): thread %d, count %d\n", *my_id, count); */
  }

  pace_pthread_mutex_unlock(&count_lock);

  return(NULL);
}

int
main()
{
  int i;
  pace_pthread_t threads[3];

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
  /* VxWorks does not handle the *_ININITALIZER values and we
   * can not emulate it since it involves memory allocation for
   * VxWorks.
   */
  pace_pthread_mutex_init(&count_lock, 0);
  pace_pthread_cond_init(&count_hit_threshold, 0);
#endif

  pace_pthread_create(&threads[0], NULL, inc_count, (void *)&thread_ids[0]);
  pace_pthread_create(&threads[1], NULL, inc_count, (void *)&thread_ids[1]);
  pace_pthread_create(&threads[2], NULL, watch_count, (void *)&thread_ids[2]);

  for (i = 0; i < NUM_THREADS; i++) {
    pace_pthread_join(threads[i], NULL);
  }

  if (count < COUNT_THRES)
    {
      pace_printf ("### ERROR ###: count should be >= %d.\n",
                   COUNT_THRES);
      pace_printf ("### ERROR ###: However, count == %d.\n",
                   count);
      pace_exit (-1);
    }

  /* pace_printf ("main(): This should be the last print statement.\n"); */

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
  /* VxWorks does not handle the *_ININITALIZER values and we
   * can not emulate it since it involves memory allocation for
   * VxWorks. We need to explicitly delete the mutex and condition
   * variable allocated by the init calls.
   */
  pace_pthread_mutex_destroy(&count_lock);
  pace_pthread_cond_destroy(&count_hit_threshold);
#endif /* PACE_VXWORKS && PACE_VXWORKS != 0 */

  return 0;
}
