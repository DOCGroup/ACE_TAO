/* $Id$ -*- C -*- */

/* ===================================================================== */
/*                                                                       */
/* = FILENAME                                                            */
/*    Pthreads_Test.c                                                    */
/*                                                                       */
/* =  DESCRIPTION                                                        */
/*     Testing the platform for POSIX threads thread specific storage.   */
/*     This is not meant to be an exhaustive test at this point but more */
/*     a sanity check that PACE works (at least somewhat) as advertised. */
/*     This program creates some threads, stores some thread specific    */
/*     information in each one (some time information) and then checks   */
/*     that the different threads contained different values.            */
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
#include "pace/time.h"
#include "pace/pthread.h"

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
#include "vxworks_stub.c"
#endif /* VXWORKS */

#define NUM_THREADS  3
/*pace_pthread_key_t saved_time_key;*/
pace_pthread_key_t saved_ID_key;

/*void free_time (void *arg)*/
void free_ID (void *arg)
{
  /*pace_timespec *timep = (pace_timespec *)arg; 
  pace_printf("free_time:\n");
  pace_free(timep);*/

  pace_pthread_t *threadp = (pace_pthread_t *)arg; 
  /* pace_printf("free_ID:\n"); */
  pace_free(threadp);
}

/*void save_the_time (void)*/
long save_the_ID (void)
{
  /*pace_timespec *timep;

  timep = (pace_timespec *)malloc(sizeof(pace_timespec));
  clock_gettime(1, timep);
  pace_printf("save_the_time: \t\t%ld %ld\n",timep->tv_sec, timep->tv_nsec);
  pace_pthread_setspecific(saved_time_key, (void *)timep);*/

  pace_pthread_t *pace_thread =
    (pace_pthread_t *)malloc(sizeof(pace_pthread_t));

  *pace_thread = pace_pthread_self();
  /* pace_printf("save_the_ID: \t\t%ld\n", (long)*pace_thread); */
  pace_pthread_setspecific(saved_ID_key, (void *)pace_thread);

  return ((long)*pace_thread);
}

/*void what_time_did_i_save (void)*/
long what_ID_did_i_save (void)
{
  /*pace_timespec *timep;

  timep = pace_pthread_getspecific(saved_time_key);
  printf("what_time_did_i_save: \t%ld %ld\n",timep->tv_sec, timep->tv_nsec);*/

  pace_pthread_t *pace_thread;

  pace_thread = pace_pthread_getspecific(saved_ID_key);
  /* pace_printf("what_ID_did_i_save: \t%ld\n", (long)*pace_thread); */
  return ((long)*pace_thread);
}  

void *thread_routine (void *arg)
{
  long saved_id;
  long retrieved_id;
  /*int *my_id=(int *)arg;*/
  PACE_UNUSED_ARG (arg);

  /* pace_printf("thread_routine %d\n", *my_id); */
  /*save_the_time();
    what_time_did_i_save();*/
  saved_id = save_the_ID();
  pace_sleep (1);
  retrieved_id = what_ID_did_i_save();

  if (saved_id != retrieved_id)
    {
      pace_printf ("### ERROR ###: saved id %ld does not equal retrieved
                   id %ld.\n", saved_id, retrieved_id);
      pace_exit (-1);
    }

  return (NULL); 
}

int
main (int argc, char *argv[])
{
  int i;
  int *id_arg;
  pace_pthread_t threads[NUM_THREADS];

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);

  id_arg = (int *) pace_malloc (NUM_THREADS * sizeof(int));

  /* pace_printf("main : initializing the key\n"); */
  /*pace_pthread_key_create(&saved_time_key, free_time);*/
  pace_pthread_key_create(&saved_ID_key, free_ID);

  /* pace_printf("main : spawning the threads\n"); */
  for (i = 0; i < NUM_THREADS; i++)
    {
      id_arg[i] = i;

      pace_pthread_create(&(threads[i]), 
                          NULL,
                          thread_routine,
                          (void *) &(id_arg[i]));
    }

  for (i = 0; i < NUM_THREADS; i++)
    {
      pace_pthread_join(threads[i], NULL);
      /* pace_printf("main : thread %d has finished. \n", i); */
    }

  /* pace_printf("main : goodbye\n"); */

  return 0;
}
