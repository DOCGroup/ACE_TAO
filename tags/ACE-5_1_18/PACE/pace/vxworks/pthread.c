/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/pthread.c
 *
 * = AUTHOR
 *    Joe Hoffert. The *VAST* majority of the pthread code for VxWorks
 *    has been supplied by Hughes Network Systems via Braeton Taylor.
 *
 * ============================================================================= */

#include "pace/pthread.h"

/* pthread queue */
static pthread_q_entry pthread_queue[PTHEAD_QUEUE_MAX_LEN];
static pthread_q_entry * first, *current;
static int pthread_count = 0;
/*static pthread_q_entry * pthread_queue_get_entry();*/
static initialized = 0;

static pthread_key_entry keyList[PTHREAD_KEYS_MAX];

/* 
 *  The defaut pthread attr structure, if calling task 
 *  does not pass attr, this default will be used 
 */
static struct _PTHREAD_ATTR _pthread_attr_default_s =
{
  10000,          /* stacksize */
  { '\0' },       /* name */
  {100},          /* priority */
  PTHREAD_SCOPE_SYSTEM,  /* scope */
  SCHED_FIFO,   /* indirect function */
  PTHREAD_CREATE_DETACHED  /* detached */
};

/* global default access */
pace_pthread_attr_t  pthread_attr_default = &_pthread_attr_default_s;


#if !defined (PACE_HAS_INLINE)
# include "pace/vxworks/pthread.inl"
#endif /* ! PACE_HAS_INLINE */

/*
 * VxWorks Helper Functions
 */

/*
 * Get a free entry from the pthread queue.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
pthread_q_entry *
pacevx_pthread_queue_get_entry()
{
  int i;
  for (i = 0; i<PTHEAD_QUEUE_MAX_LEN; i++)
    {
      if (pthread_queue[i].status == FALSE)
        return &pthread_queue[i];
    }

  /* error condition, can alloc more in future */
  return NULL;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/* 
 * Run the destructor functions for the specific data key
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
void
pacevx_pthread_destructor_key(pace_pthread_key_t key, void * arg)
{
  if (pacevx_pthread_key_validate(key))
    {
      if (keyList[key].destructor != NULL)
        (*(keyList[key].destructor))(arg);
    }
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/*
 * Run the destructor functions for the thread
 * For each key value, if there is a non-NULL destructor pointer, 
 * and the thread has a non-NULL value associated with that key, 
 * the function pointed to is called with the current associated 
 * value as its sole argument.
 *
 * return: none.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
void
pacevx_pthread_destructor_thread(pace_pthread_t pthread)
{
  int i;
 
  for (i = 0; i < PTHREAD_KEYS_MAX; i ++)
    {
      if (pthread->keyvaluelist[i] != NULL)
        pacevx_pthread_destructor_key(i, pthread->keyvaluelist[i]);
    }
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/*
 * General exit processing.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
void
pacevx_pthread_proc_exit(pace_pthread_t pthread, void *value_ptr)
{
  int key;
  int needgive = 0;

  pacevx_pthread_cleanup_popall(pthread);

  /* thread storage data cleanup is automatically*/
  pacevx_pthread_destructor_thread(pthread);

  /* joinable or detached */
  if(pthread->detachflag == PTHREAD_CREATE_DETACHED)
    {
      free(pthread);
    }
  else
    {
      key = intLock();

      /* pass the value */
      pthread->joinvalue = value_ptr;

      switch(pthread->joinstate)
        {
        case JOIN_PENDING:
          pthread->joinstate = JOIN_TERMINATED;
          needgive = 1;
          break;
        case JOIN_NORMAL:
        default:
          pthread->joinstate = JOIN_TERMINATED;
          break;
        }
      intUnlock(key);

      /* unblock the calling thread */
      if(needgive)
        semGive(((pace_pthread_t)(pthread->jointhread))->joinSem);
    }
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pacevx_pthread_verify(pace_pthread_t pthread) 
{
  int key;
  pthread_q_entry * entry;

  key = intLock();

  /* if queue is empty */
  if (first == NULL)
    {
      intUnlock(key);
      return ERROR;
    }

  entry = first; /* get the first one */

  while (entry != NULL)
    {
     if (entry->pthread == pthread)
       {
         intUnlock(key);
         return TRUE;
       }
     entry = entry->next;
    } 

  intUnlock(key);

  return FALSE;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/*
 * The cleanup routine that will be called if the task get deleted.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
void
pacevx_pthread_run_cleanup (WIND_TCB *pTcb) 
/* pointer to deleted task's WIND_TCB */
{
  pace_pthread_t pthread;

  /* free thread tcb only*/
  if (pTcb->_USER_SPARE4 != 0)
    {
      pthread = (pace_pthread_t) pTcb->_USER_SPARE4;

      pacevx_pthread_proc_exit(pthread, NULL);
    }
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/*
 * Add to the pthread queue.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pacevx_pthread_queue_add(pace_pthread_t pthread)
{
  int key;
  pthread_q_entry * entry;

  key = intLock();

  if ((entry = pacevx_pthread_queue_get_entry()) != NULL) 
    {
      entry->status = TRUE;
      entry->pthread = pthread;

      if (first == NULL)
        {
          first = entry;
          current = entry;
        }
      else
        {
          current->next = entry;
          current = entry;
        }
      pthread_count ++;
      intUnlock(key);
      return OK;
    }

  intUnlock(key);
  return ERROR;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/*
 * Remove an entry to the pthread queue.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pacevx_pthread_queue_remove(pace_pthread_t pthread)
{
  int key;
  pthread_q_entry * entry1;
  pthread_q_entry * entry2;

  key = intLock();

  if (first == NULL)
    {
      intUnlock(key);
      return ERROR;
    }

  /* if it is the first one, simple */
  if (first->pthread == pthread)
    {
      first->status = FALSE;
      first->pthread = NULL;

      entry1 = first->next;

      first->next = NULL;

      /* if it is the only one */
      if (current == first)
        current = entry1;

      first = entry1;

      intUnlock(key);
      return OK;
    }
     
  /* else */

  entry1 = first;
  entry2 = entry1->next;
  
  while (entry2 != NULL)
    {
      if (entry2->pthread == pthread)
      {
         if (current == entry2)
           current = entry1;

         entry1->next = entry2->next; /* remove the node */

         /* set invalid and put back to the list */
         entry2->status = FALSE;
         entry2->pthread = NULL;
         entry2->next = NULL;
         intUnlock(key);
         return OK;
      }
      entry1 = entry2;
      entry2 = entry2->next;
   }

   intUnlock(key);
   return ERROR;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/* 
 * Run the rest of cleanup routines left in the stack.
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
void
pacevx_pthread_cleanup_popall(pace_pthread_t thread)
{
  int count;
  int i;

  count = thread->rtnCount - 1;
  for (i = count; i > 0 ; i--)
    {
      thread->rtnCount --;
      (*(thread->cleanupRtn[i].routine))(thread->cleanupRtn[i].args);
    }
}
#endif /* PACE_HAS_NONUOF_FUNCS */

/*
 * Validate the key to see if the key is already created (valid)
 */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pacevx_pthread_key_validate(pace_pthread_key_t key)
{
  int intkey;

  intkey = intLock();

  if (keyList[key].valid)
    {
      intUnlock(intkey);
      return TRUE;
    }
  else
    {
      intUnlock(intkey);
      return FALSE;
    }
}
#endif /* PACE_HAS_NONUOF_FUNCS */


/*
 * PACE - POSIX Functions
 */


#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pace_pthread_cond_timedwait (pace_pthread_cond_t * cond,
                             pace_pthread_mutex_t * mutex,
                             const pace_timespec * abstime)
{
  STATUS status;
  int timeval = 0;
  int errornumber, returnval = 0;

  if (pace_pthread_mutex_unlock(mutex) != OK) return ERROR;

  /* convert the abstime to timeval */
  status = semTake(*cond, timeval);

  if (status != OK)
    {
      errornumber = errnoGet();
      if (errornumber == S_objLib_OBJ_ID_ERROR)
        returnval = EINVAL;
      else if (errornumber == S_objLib_OBJ_TIMEOUT) 
        returnval = ETIMEDOUT;
      else 
        returnval = ERROR;
    }

  pace_pthread_mutex_lock(mutex);

  return returnval;
}
#endif /* PACE_HAS_NONUOF_FUNCS */


#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pace_pthread_cond_wait (pace_pthread_cond_t * cond,
                        pace_pthread_mutex_t * mutex)
{
  STATUS status;
  int errornumber;
  int returnval = 0;

  if (pace_pthread_mutex_unlock(mutex) != OK)
    return ERROR;

  status = semTake(*cond, WAIT_FOREVER);

  if(status != OK)
    {
      errornumber = errnoGet();
      if (errornumber == S_objLib_OBJ_ID_ERROR) 
          returnval = EINVAL;
      else
          returnval = ERROR;
    }

  pace_pthread_mutex_lock(mutex);

  return returnval;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
int
pace_pthread_create (pace_pthread_t * thread,
                     const pace_pthread_attr_t * attr,
                     pace_create_pf start_routine,
                     void * arg)
{
  pace_pthread_attr_t pattr;
  char * pname;
  int taskid;
  pace_pthread_t pthread;
  WIND_TCB * pTcb;

  if (attr == 0) 
    pattr = pthread_attr_default;
  else
    if (*attr == 0)
       pattr = pthread_attr_default;
    else
       pattr = *attr;

  if (pattr->name[0] != '\0')   /* name is provided */
    pname = pattr->name;
  else
    pname = (char *)0;

  taskid = taskSpawn(pname, 
                     (SCHED_FIFO_HIGH_PRI - pattr->schedule.sched_priority), 
                     VX_FP_TASK, pattr->stacksize, (FUNCPTR)start_routine, 
                     (int)arg, 0,0,0,0,0,0,0,0,0);
  if (taskid == ERROR)
     return ERROR;

  if ((pTcb = taskTcb(taskid)) == NULL)
    {
      taskDelete(taskid);
      return ERROR;
    }

  pthread = (pace_pthread_t )malloc(sizeof(struct _PTHREAD_T));

  if(pthread == NULL)
    {
      taskDelete(taskid);
      return ERROR;
    }

  /* construct pace_pthread_t structure */

  bzero((char *)pthread, sizeof(struct _PTHREAD_T));
  pthread->tid = taskid;
  pthread->stateflag = PTHREAD_CANCEL_ENABLE;
  pthread->canceltype = PTHREAD_CANCEL_ASYNCHRONOUS;
  pthread->detachflag = pattr->dstate;

  /* initialize the join semaphore also */
  if ((pthread->joinSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY)) == NULL)
    {
      free((void *)pthread);
      taskDelete(taskid);
      return ERROR;
    }

  /* pass it to the caller */
  *thread = pthread;   

  /* save to the WIND_TCB for reference afterward */
  pTcb->_USER_SPARE4 = (int) pthread;

  taskDeleteHookAdd((FUNCPTR)pacevx_pthread_run_cleanup);

  pacevx_pthread_queue_add(pthread);

  return OK;
}
# else /* ! PACE_HAS_CPLUSPLUS */
int
pace_pthread_create (pace_pthread_t * thread,
                     const pace_pthread_attr_t * attr,
                     void * (*start_routine) (void*),
                     void * arg)
{
  pace_pthread_attr_t pattr;
  char * pname;
  int taskid;
  pace_pthread_t pthread;
  WIND_TCB * pTcb;

  if (attr == 0) 
    pattr = pthread_attr_default;
  else
    if (*attr == 0)
       pattr = pthread_attr_default;
    else
       pattr = *attr;

  if (pattr->name[0] != '\0')   /* name is provided */
    pname = pattr->name;
  else
    pname = (char *)0;

  taskid = taskSpawn(pname, 
                     (SCHED_FIFO_HIGH_PRI - pattr->schedule.sched_priority), 
                     VX_FP_TASK, pattr->stacksize, (FUNCPTR)start_routine, 
                     (int)arg, 0,0,0,0,0,0,0,0,0);
  if (taskid == ERROR)
     return ERROR;

  if ((pTcb = taskTcb(taskid)) == NULL)
    {
      taskDelete(taskid);
      return ERROR;
    }

  pthread = (pace_pthread_t )malloc(sizeof(struct _PTHREAD_T));

  if(pthread == NULL)
    {
      taskDelete(taskid);
      return ERROR;
    }

  /* construct pace_pthread_t structure */

  bzero((char *)pthread, sizeof(struct _PTHREAD_T));
  pthread->tid = taskid;
  pthread->stateflag = PTHREAD_CANCEL_ENABLE;
  pthread->canceltype = PTHREAD_CANCEL_ASYNCHRONOUS;
  pthread->detachflag = pattr->dstate;

  /* initialize the join semaphore also */
  if ((pthread->joinSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY)) == NULL)
    {
      free((void *)pthread);
      taskDelete(taskid);
      return ERROR;
    }

  /* pass it to the caller */
  *thread = pthread;   

  /* save to the WIND_TCB for reference afterward */
  pTcb->_USER_SPARE4 = (int) pthread;

  taskDeleteHookAdd((FUNCPTR)pacevx_pthread_run_cleanup);

  pacevx_pthread_queue_add(pthread);

  return OK;
}
# endif /*! PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pace_pthread_detach (pace_pthread_t thread)
{
  int key;
  int needfree;
  needfree = 0;

  if (!pacevx_pthread_verify(thread))
    return EINVAL;

  key = intLock();

  switch (thread->joinstate)
    {
      /* task is joined, or detached, but still running, do nothing */
      case JOIN_PENDING:
      case JOIN_DETATCHED:
        intUnlock(key);
        return OK;
        break;
      case JOIN_NORMAL:      /* task is running */
        thread->joinstate = JOIN_DETATCHED;
        break;
      case JOIN_TERMINATED:
        needfree = 1;
        break;
      default:
        break;
    }

  intUnlock(key);

  if (needfree) 
    {
      pacevx_pthread_queue_remove(thread);
      free(thread);
    }
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pace_pthread_join (pace_pthread_t thread, void ** value_ptr)
{
  /*
   * The pthread_join() function suspends execution of the calling
   * thread until the target thread terminates, unless the target 
   * thread has already terminated.
   * The terminating thread can pass value to the caller by
   * pthread_exit() and the calling thread gets it from "value_ptr"
   * The application must verify the "value_ptr" value before using it.
   */
  pace_pthread_t pthread;
  int needfree;
  int key;

  if (!pacevx_pthread_verify(thread))
    return ERROR;

  if (thread->detachflag != PTHREAD_CREATE_JOINABLE)
     return ERROR;

  if ((pthread = pace_pthread_self()) == NULL)
    return ERROR;

  needfree = 0;

  key = intLock();
  switch (thread->joinstate)
    {
      case JOIN_NORMAL:
        thread->jointhread = pthread;
        thread->joinstate = JOIN_PENDING;
        break;
      case JOIN_TERMINATED:
        needfree = 1;
        break;
      case JOIN_PENDING:
      default:
        intUnlock(key);
        return ERROR;
    }
  intUnlock(key);

  if (needfree)
  {
     *value_ptr = thread->joinvalue;
     pacevx_pthread_queue_remove(thread);
     free(thread);
     return OK;
  }

  /* if we are here, thread is not terminated yet */

  semTake(pthread->joinSem, WAIT_FOREVER);

  /* cleanup */
  *value_ptr = thread->joinvalue;
  pacevx_pthread_queue_remove(thread);
  free(thread);
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
int
pace_pthread_key_create (pace_pthread_key_t * key,
                         pace_keycreate_pf destructor)
{
  /*
   * Create a thread-specific data key. Also initialize the 
   * data structure if it is called first time.
   */
  int i;
  int intkey;

  /* do the initialization if it is the first time */
  if (initialized == 0)
    {
      initialized = 1;

      /* initialize the data structure */
      for (i = 0; i < PTHREAD_KEYS_MAX; i++)
        {
          keyList[i].index = i;
          keyList[i].valid = FALSE;
          keyList[i].destructor = NULL;
        } 
    }
       
  /* find first available position */
  intkey = intLock();
  for (i = 0; i < PTHREAD_KEYS_MAX; i++)
    {
      if(keyList[i].valid == FALSE)
        {
          *key = (pace_pthread_key_t)keyList[i].index;
          keyList[i].valid = TRUE;
          keyList[i].destructor = destructor;
          intUnlock(intkey);
          return OK;
        }
    }

  intUnlock(intkey);
  return ERROR;
}
#else /* ! PACE_HAS_CPLUSPLUS */
int
pace_pthread_key_create (pace_pthread_key_t * key,
                         void (*destructor)(void*))
{
  /*
   * Create a thread-specific data key. Also initialize the 
   * data structure if it is called first time.
   */
  int i;
  int intkey;

  /* do the initialization if it is the first time */
  if (initialized == 0)
    {
      initialized = 1;

      /* initialize the data structure */
      for (i = 0; i < PTHREAD_KEYS_MAX; i++)
        {
          keyList[i].index = i;
          keyList[i].valid = FALSE;
          keyList[i].destructor = NULL;
        } 
    }
       
  /* find first available position */
  intkey = intLock();
  for (i = 0; i < PTHREAD_KEYS_MAX; i++)
    {
      if(keyList[i].valid == FALSE)
        {
          *key = (pace_pthread_key_t)keyList[i].index;
          keyList[i].valid = TRUE;
          keyList[i].destructor = destructor;
          intUnlock(intkey);
          return OK;
        }
    }

  intUnlock(intkey);
  return ERROR;
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pace_pthread_key_delete (pace_pthread_key_t key)
{
  int intkey;

  if ((key < 0) || (key >= PTHREAD_KEYS_MAX))
    return ERROR;

  intkey = intLock();

  keyList[key].valid = FALSE;
  keyList[key].destructor = NULL;

  intUnlock(intkey);

  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
int
pace_pthread_once (pace_pthread_once_t * once_control,
                   pace_once_pf void_routine)
{
  /*
   * Once function allows the function to be executed exact only once
   * Subsequent calls of pthread_once() with the same once_control will 
   * not call the void_routine(). 
   */
  int i;
  int key;
  pace_pthread_t pthread;

  if ((pthread = pace_pthread_self()) == NULL)
    return ERROR;

  /* make it atomic */
  key = intLock();

  for (i = 0; i < pthread->onceCount; i++)
    {
      if (*once_control == pthread->onceList[i].once_ctl)
        {
          /* do nothing, already called */
          intUnlock(key);
          return OK;
        }
    }

  /* if we are here, no match is found */
  pthread->onceList[pthread->onceCount].once_ctl = *once_control;
  pthread->onceCount++;
  intUnlock(key);

  /* run the init routine */
  (*void_routine)();
  return OK;
}
# else /* ! PACE_HAS_CPLUSPLUS */
int
pace_pthread_once (pace_pthread_once_t * once_control,
                   void (*void_routine) (void))
{
  /*
   * Once function allows the function to be executed exact only once
   * Subsequent calls of pthread_once() with the same once_control will 
   * not call the void_routine(). 
   */
  int i;
  int key;
  pace_pthread_t pthread;

  if ((pthread = pace_pthread_self()) == NULL)
    return ERROR;

  /* make it atomic */
  key = intLock();

  for (i = 0; i < pthread->onceCount; i++)
    {
      if (*once_control == pthread->onceList[i].once_ctl)
        {
          /* do nothing, already called */
          intUnlock(key);
          return OK;
        }
    }

  /* if we are here, no match is found */
  pthread->onceList[pthread->onceCount].once_ctl = *once_control;
  pthread->onceCount++;
  intUnlock(key);

  /* run the init routine */
  (*void_routine)();
  return OK;
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */
