// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Sched_Params.cpp
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#define ACE_BUILD_DLL

#include "ace/ACE.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
#include "ace/Sched_Params.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_STHREADS)
#include <sys/rtpriocntl.h>
#include <sys/tspriocntl.h>
#endif /* ACE_HAS_STHREADS */

int
ACE_Sched_Params::priority_min (const Policy policy)
{
#if defined (ACE_HAS_STHREADS)
  // Assume that ACE_SCHED_OTHER indicates Solaris TS class, and that
  // other policies indicate Solaris RT class.

  if (policy == ACE_SCHED_OTHER)
    {
      // Get the priority class ID and attributes.
      pcinfo_t pcinfo;
      ACE_OS::strcpy (pcinfo.pc_clname, "TS");
      // The following is just to avoid Purify warnings about unitialized
      // memory reads.
      ACE_OS::memset (pcinfo.pc_clinfo, 0, PC_CLINFOSZ);

      if (::priocntl (P_ALL /* ignored */,
                      P_MYID /* ignored */,
                      PC_GETCID,
                      (char *) &pcinfo) == -1)
        {
          return -1;  // just hope that priority range wasn't configured
                      // from -1 .. 1
        }

      // OK, now we've got the class ID in pcinfo.pc_cid.  In addition,
      // the maximum configured real-time priority is in ((tsinfo_t *)
      // pcinfo.pc_clinfo)->ts_maxupri.

      return -((tsinfo_t *) pcinfo.pc_clinfo)->ts_maxupri;
    }
  else
    {
      return 0;
    }
#elif defined (ACE_HAS_DCETHREADS) || defined(ACE_HAS_PTHREADS)
  return ::sched_get_priority_min (policy);
#elif defined (ACE_HAS_WTHREADS)
  return THREAD_PRIORITY_IDLE;
#elif defined (VXWORKS)
  return 255;
#elif
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STHREADS */
}

int
ACE_Sched_Params::priority_max (const Policy policy)
{
#if defined (ACE_HAS_STHREADS)
  // Assume that ACE_SCHED_OTHER indicates Solaris TS class, and that
  // other policies indicate Solaris RT class.

  // Get the priority class ID and attributes.
  pcinfo_t pcinfo;
  ACE_OS::strcpy (pcinfo.pc_clname,
                  policy == ACE_SCHED_OTHER  ?  "TS"  :  "RT");
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (pcinfo.pc_clinfo, 0, PC_CLINFOSZ);

  if (::priocntl (P_ALL /* ignored */,
                  P_MYID /* ignored */,
                  PC_GETCID,
                  (char *) &pcinfo) == -1)
    {
      return -1;
    }

  // OK, now we've got the class ID in pcinfo.pc_cid.  In addition,
  // the maximum configured real-time priority is in ((rtinfo_t *)
  // pcinfo.pc_clinfo)->rt_maxpri, or similarly for the TS class.

  return policy == ACE_SCHED_OTHER
      ? ((tsinfo_t *) pcinfo.pc_clinfo)->ts_maxupri
      : ((rtinfo_t *) pcinfo.pc_clinfo)->rt_maxpri;
#elif defined (ACE_HAS_DCETHREADS) || defined(ACE_HAS_PTHREADS)
  return ::sched_get_priority_max (policy);
#elif defined (ACE_HAS_WTHREADS)
  return THREAD_PRIORITY_TIME_CRITICAL;
#elif defined (VXWORKS)
  return 0;
#elif
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STHREADS */
}

int
ACE_Sched_Params::next_priority (const Policy policy, const int priority)
{
#if defined (VXWORKS)
  ACE_UNUSED_ARG (policy);
  return priority > priority_max (policy)  ?  priority - 1
                                           :  priority_max (policy);
#elif defined (ACE_HAS_THREADS)
  const int max = priority_max (policy);
  return priority < max  ?  priority + 1  :  max;
#elif
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_Sched_Params::previous_priority (const Policy policy,
                                          const int priority)
{
#if defined (VXWORKS)
  ACE_UNUSED_ARG (policy);
  return priority < priority_min (policy)  ?  priority + 1
                                           :  priority_min (policy);
#elif defined (ACE_HAS_THREADS)
  const int min = priority_min (policy);
  return priority > min  ?  priority - 1  :  min;
#elif
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}
