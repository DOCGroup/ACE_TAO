// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Thread_Priority.cpp
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#define ACE_BUILD_DLL

#include "ace/Thread_Priority.h"

#if !defined (__ACE_INLINE__)
#include "ace/Thread_Priority.i"
#endif /* __ACE_INLINE__ */

int
ACE_Thread_Priority::increment ()
{
  if (default_thread_priority_ == ACE_PRIORITY_MAX)
    {
      if (priority_class_ == ACE_REALTIME_PRIORITY_CLASS)
        {
          // Overflow: no more priority levels!
          return -1;
        }
      else
        {
          priority_class_ = (Priority_Class) (priority_class_ + 1);
          default_thread_priority_ = ACE_PRIORITY_MIN;
          return 1;
        }
    }
  else
    {
      // not at maximum Thread_Priority: just increment the thread priority.
      default_thread_priority_ = (Thread_Priority) (default_thread_priority_ + 1);
      return 0;
    }
}


int
ACE_Thread_Priority::decrement ()
{
  if (default_thread_priority_ == ACE_PRIORITY_MIN)
    {
      if (priority_class_ == ACE_LOW_PRIORITY_CLASS)
        {
          // Underflow: no more priority levels!
          return -1;
        }
      else
        {
          priority_class_ = (Priority_Class) (priority_class_ - 1);
          default_thread_priority_ = ACE_PRIORITY_MAX;
          return 1;
        }
    }
  else
    {
      // not at maximum Thread_Priority: just decrement the thread priority.
      default_thread_priority_ = (Thread_Priority) (default_thread_priority_ - 1);
      return 0;
    }
}


// convert (priority_class_,default_thread_priority_) -> 
//              (os_priority_class_, os_default_thread_priority_)
// using whatever mapping is appropriate.

#if defined (ACE_HAS_STHREADS)
#include <sys/priocntl.h>
#include <sys/rtpriocntl.h>
#include <sys/tspriocntl.h>

/* mapping of
      ACE_Thread_Priority::                    to        Solaris
   Priority_Class             Thread_Priority      class         priority
   ==============             ===============      =====         ========
   ACE_LOW_PRIORITY_CLASS         0 .. 6            TS            0 .. 6
   ACE_NORMAL_PRIORITY_CLASS      0 .. 6            TS            7 .. 13
   ACE_HIGH_PRIORITY_CLASS        0 .. 6            RT            0 .. 6
   ACE_REALTIME_PRIORITY_CLASS    0 .. 6            RT            7 .. 13
 */

int
ACE_Thread_Priority::convert_to_os_priority (void)
{
  // Get the priority class ID and attributes.
  pcinfo_t pcinfo;
  ACE_OS::strcpy (pcinfo.pc_clname,
                  priority_class_ == ACE_HIGH_PRIORITY_CLASS
                  || priority_class_ == ACE_REALTIME_PRIORITY_CLASS
                  ?  "RT"
                  :  "TS");

  if (::priocntl (P_ALL /* ignored */,
                  P_MYID /* ignored */,
                  PC_GETCID,
                  (char *) &pcinfo) == -1)
    return -1;

  // OK, now we've got the class ID in pcinfo.pc_cid.  In addition,
  // the maximum configured real-time priority is in ((rtinfo_t *)
  // pcinfo.pc_clinfo)->rt_maxpri.

  os_priority_class_ = pcinfo.pc_cid;

  if (ACE_PRIORITY_MIN <= default_thread_priority_
      && default_thread_priority_ <= ACE_PRIORITY_MAX)
    {
      os_default_thread_priority_ =
        priority_class_ == ACE_NORMAL_PRIORITY_CLASS
        || priority_class_ == ACE_REALTIME_PRIORITY_CLASS
        ?  default_thread_priority_ + 7
        :  default_thread_priority_;
    }
  else
    // The user specified a thread priority outside the enum range, so
    // use it without modification.
    os_default_thread_priority_ = default_thread_priority_;

  return 0;
}


#elif defined (ACE_WIN32)

/* mapping of
      ACE_Thread_Priority::                    to        Win32
   Priority_Class             Thread_Priority      class      THREAD_PRIORITY_
   ==============             ===============      =====      ================
   ACE_LOW_PRIORITY_CLASS         0 .. 6           IDLE_...   IDLE...
                                                                 TIME_CRITICAL
   ACE_NORMAL_PRIORITY_CLASS      0 .. 6           NORMAL_...      ""
   ACE_HIGH_PRIORITY_CLASS        0 .. 6           HIGH_...        ""
   ACE_REALTIME_PRIORITY_CLASS    0 .. 6           REALTIME_...    ""
 */

int
ACE_Thread_Priority::convert_to_os_priority (void)
{
  switch (priority_class_)
  {
    case ACE_LOW_PRIORITY_CLASS :
      os_priority_class_ = IDLE_PRIORITY_CLASS;
      break;
    case ACE_NORMAL_PRIORITY_CLASS :
      os_priority_class_ = NORMAL_PRIORITY_CLASS;
      break;
    case ACE_HIGH_PRIORITY_CLASS :
      os_priority_class_ = HIGH_PRIORITY_CLASS;
      break;
    case ACE_REALTIME_PRIORITY_CLASS :
      os_priority_class_ = REALTIME_PRIORITY_CLASS;
      break;
  }

  if (ACE_PRIORITY_MIN <= default_thread_priority_
      && default_thread_priority_ <= ACE_PRIORITY_MAX)
    {
      switch (default_thread_priority_)
      {
        case ACE_PRIORITY_0 :
          os_default_thread_priority_ = THREAD_PRIORITY_IDLE;
          break;
        case ACE_PRIORITY_1 :
          os_default_thread_priority_ = THREAD_PRIORITY_LOWEST;
          break;
        case ACE_PRIORITY_2 :
          os_default_thread_priority_ = THREAD_PRIORITY_BELOW_NORMAL;
          break;
        case ACE_PRIORITY_3 :
          os_default_thread_priority_ = THREAD_PRIORITY_NORMAL;
          break;
        case ACE_PRIORITY_4 :
          os_default_thread_priority_ = THREAD_PRIORITY_ABOVE_NORMAL;
          break;
        case ACE_PRIORITY_5 :
          os_default_thread_priority_ = THREAD_PRIORITY_HIGHEST;
          break;
        case ACE_PRIORITY_6 :
          os_default_thread_priority_ = THREAD_PRIORITY_TIME_CRITICAL;
          break;
      }
    }
  else
    // The user specified a thread priority outside the enum range, so
    // use it without modification.  The user had better know what they're
    // doing on Win32 platforms.
    os_default_thread_priority_ = default_thread_priority_;

  return 0;
}


#elif defined (VXWORKS)

/* mapping of
      ACE_Thread_Priority::                    to        VxWorks
   Priority_Class             Thread_Priority      class         priority
   ==============             ===============      =====         ========
   ACE_LOW_PRIORITY_CLASS        0 .. 6             N/A           27 .. 21
   ACE_NORMAL_PRIORITY_CLASS     0 .. 6             N/A           20 .. 14
   ACE_HIGH_PRIORITY_CLASS       0 .. 6             N/A           13 .. 7
   ACE_REALTIME_PRIORITY_CLASS   0 .. 6             N/A            6 .. 0
 */

int
ACE_Thread_Priority::convert_to_os_priority (void)
{
  os_priority_class_ = -1;  /* unused on this platform */

  if (ACE_PRIORITY_MIN <= default_thread_priority_
      && default_thread_priority_ <= ACE_PRIORITY_MAX)
    {
      switch (priority_class_)
      {
        case ACE_LOW_PRIORITY_CLASS :
          os_default_thread_priority_ =
            ACE_PRIORITY_MAX - default_thread_priority_ + 21;
          break;
        case ACE_NORMAL_PRIORITY_CLASS :
          os_default_thread_priority_ =
            ACE_PRIORITY_MAX - default_thread_priority_ + 14;
          break;
        case ACE_HIGH_PRIORITY_CLASS :
          os_default_thread_priority_ =
            ACE_PRIORITY_MAX - default_thread_priority_ + 7;
          break;
        case ACE_REALTIME_PRIORITY_CLASS :
          os_default_thread_priority_ =
            ACE_PRIORITY_MAX - default_thread_priority_;
          break;
      }
    }
  else
    // The user specified a thread priority outside the enum range, so
    // use it without modification.
    os_default_thread_priority_ = default_thread_priority_;

  return 0;
}


#elif defined (DIGITAL_UNIX)

// Thanks to Thilo Kielmann <kielmann@informatik.uni-siegen.de> for
// figuring this out for Digital UNIX and providing this code . . .
//
// might be adapted to other flavours of POSIX threads...
//
// Please note that this has only been tested for POSIX 1003.1c threads,
// and may cause problems with other PThreads flavors!

/* mapping of
      ACE_Thread_Priority::                    to        POSIX 1003.1c
   Priority_Class             Thread_Priority      class      THREAD_PRIORITY_
   ==============             ===============      =====      ================
   ACE_LOW_PRIORITY_CLASS         0 .. 6           BG_NP          0 .. 6
   ACE_NORMAL_PRIORITY_CLASS      0 .. 6           OTHER          8 .. 14
   ACE_HIGH_PRIORITY_CLASS        0 .. 6           RR             16 .. 22
   ACE_REALTIME_PRIORITY_CLASS    0 .. 6           FIFO           25 .. 31
 */
// Man pages don't talk about "real time", but at least, FIFO threads are
// not preempted (except for threads with higher priority).

int
ACE_Thread_Priority::convert_to_os_priority (void)
{
  switch (priority_class_)
  {
    case ACE_LOW_PRIORITY_CLASS :
      os_priority_class_ = SCHED_BG_NP;
      break;
    case ACE_NORMAL_PRIORITY_CLASS :
      os_priority_class_ = SCHED_OTHER;
      break;
    case ACE_HIGH_PRIORITY_CLASS :
      os_priority_class_ = SCHED_RR;
      break;
    case ACE_REALTIME_PRIORITY_CLASS :
      os_priority_class_ = SCHED_FIFO;
      break;
  }

  if (ACE_PRIORITY_MIN <= default_thread_priority_
      && default_thread_priority_ <= ACE_PRIORITY_MAX)
    {
      switch (priority_class_)
      {
        case ACE_LOW_PRIORITY_CLASS :
          os_default_thread_priority_ = default_thread_priority_;
          break;
        case ACE_NORMAL_PRIORITY_CLASS :
          os_default_thread_priority_ = default_thread_priority_ + 8;
          break;
        case ACE_HIGH_PRIORITY_CLASS :
          os_default_thread_priority_ = default_thread_priority_ + 16;
          break;
        case ACE_REALTIME_PRIORITY_CLASS :
          os_default_thread_priority_ = default_thread_priority_ + 25;
          break;
      }
    }
  else
    // The user specified a thread priority outside the enum range, so
    // use it without modification.
    os_default_thread_priority_ = default_thread_priority_;

  return 0;
}


#elif defined(linux) \
   && defined(ACE_HAS_PTHREADS) // not sure 
/* mapping of
      ACE_Thread_Priority::                    to        POSIX 1003.1c
   Priority_Class             Thread_Priority      class      THREAD_PRIORITY_
   ==============             ===============      =====      ================
   ACE_LOW_PRIORITY_CLASS         0 .. 6           OTHER          0
   ACE_NORMAL_PRIORITY_CLASS      0 .. 6           OTHER          0
   ACE_HIGH_PRIORITY_CLASS        0 .. 6           RR             1..7
   ACE_REALTIME_PRIORITY_CLASS    0 .. 6           FIFO           21..27
 */

int
ACE_Thread_Priority::convert_to_os_priority (void)
{
  switch (priority_class_)
  {
    case ACE_LOW_PRIORITY_CLASS :
      os_priority_class_ = SCHED_OTHER;
      break;
    case ACE_NORMAL_PRIORITY_CLASS :
      os_priority_class_ = SCHED_OTHER;
      break;
    case ACE_HIGH_PRIORITY_CLASS :
      os_priority_class_ = SCHED_RR;
      break;
    case ACE_REALTIME_PRIORITY_CLASS :
      os_priority_class_ = SCHED_FIFO;
      break;
  }

  if (ACE_PRIORITY_MIN <= default_thread_priority_
      && default_thread_priority_ <= ACE_PRIORITY_MAX)
    {
      switch (priority_class_)
      {
        case ACE_LOW_PRIORITY_CLASS :
          os_default_thread_priority_ = 0;
          break;
        case ACE_NORMAL_PRIORITY_CLASS :
          os_default_thread_priority_ = 0;
          break;
        case ACE_HIGH_PRIORITY_CLASS :
          os_default_thread_priority_ = default_thread_priority_ + 1;
          break;
        case ACE_REALTIME_PRIORITY_CLASS :
          os_default_thread_priority_ = default_thread_priority_ + 21;
          break;
      }
    }
  else
    {
      // The user specified a thread priority outside the enum range,
      // so use it without modification UNLESS the class is
      // SCHED_OTHER, in which case the only valid value is zero, or
      // unless it's < 1, in which case it's rounded up to 1.
      os_default_thread_priority_ = 
	(os_priority_class_ == SCHED_OTHER) ? 0 : 
	(default_thread_priority_ < 1) ? 1 : default_thread_priority_;
    }

  return 0;
}


#else

int
ACE_Thread_Priority::convert_to_os_priority (void)
{
  ACE_NOTSUP_RETURN (-1);
}


#endif /* ACE_HAS_STHREADS */
