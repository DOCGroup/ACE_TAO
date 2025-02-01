
//=============================================================================
/**
 *  @file    Sched_Params.cpp
 *
 *  @author David Levine
 */
//=============================================================================

#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
#include "ace/Sched_Params.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

int
ACE_Sched_Params::priority_min (const Policy policy,
                                const int scope)
{
#if defined(ACE_HAS_PTHREADS) && !defined(ACE_LACKS_SETSCHED)
  switch (scope)
    {
    case ACE_SCOPE_THREAD:
      switch (policy)
        {
          case ACE_SCHED_FIFO:
            return ACE_THR_PRI_FIFO_MIN;
          case ACE_SCHED_RR:
            return ACE_THR_PRI_RR_MIN;
          case ACE_SCHED_OTHER:
          default:
            return ACE_THR_PRI_OTHER_MIN;
        }

    case ACE_SCOPE_PROCESS:
    default:
      switch (policy)
        {
          case ACE_SCHED_FIFO:
            return ACE_PROC_PRI_FIFO_MIN;
          case ACE_SCHED_RR:
            return ACE_PROC_PRI_RR_MIN;
          case ACE_SCHED_OTHER:
          default:
            return ACE_PROC_PRI_OTHER_MIN;
        }
    }

#elif defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  return THREAD_PRIORITY_IDLE;
#elif defined (ACE_VXWORKS)
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
# if defined (VX_TASK_PRIORITY_MAX)
  return VX_TASK_PRIORITY_MAX;
# else
  return 255;
# endif
#else
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_PTHREADS */
}

int
ACE_Sched_Params::priority_max (const Policy policy,
                                const int scope)
{
#if defined(ACE_HAS_PTHREADS) && \
      (!defined(ACE_LACKS_SETSCHED) || \
       defined (ACE_HAS_PTHREAD_SCHEDPARAM))

  switch (scope)
    {
    case ACE_SCOPE_THREAD:
      switch (policy)
        {
          case ACE_SCHED_FIFO:
            return ACE_THR_PRI_FIFO_MAX;
          case ACE_SCHED_RR:
            return ACE_THR_PRI_RR_MAX;
          case ACE_SCHED_OTHER:
          default:
            return ACE_THR_PRI_OTHER_MAX;
        }

    case ACE_SCOPE_PROCESS:
    default:
      switch (policy)
        {
          case ACE_SCHED_FIFO:
            return ACE_PROC_PRI_FIFO_MAX;
          case ACE_SCHED_RR:
            return ACE_PROC_PRI_RR_MAX;
          case ACE_SCHED_OTHER:
          default:
            return ACE_PROC_PRI_OTHER_MAX;
        }
    }

#elif defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  return THREAD_PRIORITY_TIME_CRITICAL;
#elif defined (ACE_VXWORKS)
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
# if defined (VX_TASK_PRIORITY_MIN)
  return VX_TASK_PRIORITY_MIN;
# else
  return 0;
# endif
#else
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_PTHREADS */
}

int
ACE_Sched_Params::next_priority (const Policy policy,
                                 const int priority,
                                 const int scope)
{
#if defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  switch (priority)
    {
      case THREAD_PRIORITY_IDLE:
        return THREAD_PRIORITY_LOWEST;
      case THREAD_PRIORITY_LOWEST:
        return THREAD_PRIORITY_BELOW_NORMAL;
      case THREAD_PRIORITY_BELOW_NORMAL:
        return THREAD_PRIORITY_NORMAL;
      case THREAD_PRIORITY_NORMAL:
        return THREAD_PRIORITY_ABOVE_NORMAL;
      case THREAD_PRIORITY_ABOVE_NORMAL:
        return THREAD_PRIORITY_HIGHEST;
      case THREAD_PRIORITY_HIGHEST:
        return THREAD_PRIORITY_TIME_CRITICAL;
      case THREAD_PRIORITY_TIME_CRITICAL:
        return THREAD_PRIORITY_TIME_CRITICAL;
      default:
        return priority;  // unknown priority:  should never get here
    }
#elif defined(ACE_HAS_THREADS) && \
      (!defined(ACE_LACKS_SETSCHED) || \
       defined (ACE_HAS_PTHREAD_SCHEDPARAM))
  // including STHREADS, and PTHREADS
  int const max = priority_max (policy, scope);
  return priority < max  ?  priority + 1  :  max;
#elif defined (ACE_VXWORKS)
  int const max = priority_max (policy, scope);
  return priority > max ?  priority - 1 :  max;
#else
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  ACE_UNUSED_ARG (priority);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_Sched_Params::previous_priority (const Policy policy,
                                     const int priority,
                                     const int scope)
{
#if defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  switch (priority)
    {
      case THREAD_PRIORITY_IDLE:
        return THREAD_PRIORITY_IDLE;
      case THREAD_PRIORITY_LOWEST:
        return THREAD_PRIORITY_IDLE;
      case THREAD_PRIORITY_BELOW_NORMAL:
        return THREAD_PRIORITY_LOWEST;
      case THREAD_PRIORITY_NORMAL:
        return THREAD_PRIORITY_BELOW_NORMAL;
      case THREAD_PRIORITY_ABOVE_NORMAL:
        return THREAD_PRIORITY_NORMAL;
      case THREAD_PRIORITY_HIGHEST:
        return THREAD_PRIORITY_ABOVE_NORMAL;
      case THREAD_PRIORITY_TIME_CRITICAL:
        return THREAD_PRIORITY_HIGHEST;
      default:
        return priority;  // unknown priority:  should never get here
    }
#elif defined(ACE_HAS_THREADS) && \
      (!defined(ACE_LACKS_SETSCHED) || \
       defined (ACE_HAS_PTHREAD_SCHEDPARAM))
  // including STHREADS and PTHREADS
  int const min = priority_min (policy, scope);
  return priority > min ? priority - 1 : min;
#elif defined (ACE_VXWORKS)
  int const min = priority_min (policy, scope);
  return priority < min ?  priority + 1 :  min;
#else
  ACE_UNUSED_ARG (policy);
  ACE_UNUSED_ARG (scope);
  ACE_UNUSED_ARG (priority);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

ACE_END_VERSIONED_NAMESPACE_DECL
