// $Id$

#if !defined (ACE_TIMEPROBE_H)
#define ACE_TIMEPROBE_H

#include "ace/Containers.h"

class ACE_Export ACE_Event_Descriptions
{  
  // = TITLE
  // Event Descriptions.
public:
  const char **descriptions_;
  // Event descriptions

  u_long minimum_id_;
  // Minimum id of this description set

  int operator== (const ACE_Event_Descriptions &rhs) const;
  // Comparison
};

class ACE_Export ACE_timeprobe_t
{
  // = TITLE
  //   Time probe record.
public:
  // = Events are record as strings or numbers.
  union event
  {
    u_long event_number_;
    const char *event_description_;
  };

  // = Type of event.
  enum event_type
  {
    NUMBER,
    STRING
  };

  event event_;
  // Event.

  event_type event_type_;
  // Type of event.

  ACE_hrtime_t time_;
  // Timestamp.

  ACE_thread_t thread_;
  // Id of thread posting the time probe.
};

#if defined (__ACE_INLINE__)
#include "ace/Timeprobe.i"
#endif /* __ACE_INLINE__ */

#include "ace/Timeprobe_T.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"

// If ACE_MT_TIMEPROBES is defined, use a Thread_Mutex to lock the
// internal state of ACE_Timerprobe.  This allows multiple threads to
// use the same ACE_Timerprobe.
#  if defined (ACE_MT_TIMEPROBES)
typedef ACE_SYNCH_MUTEX ACE_TIMEPROBE_MUTEX;
#  else /* ACE_MT_TIMEPROBES */
typedef ACE_SYNCH_NULL_MUTEX ACE_TIMEPROBE_MUTEX;
#  endif /* ACE_MT_TIMEPROBES */

typedef ACE_Timeprobe<ACE_TIMEPROBE_MUTEX> 
        ACE_TIMEPROBE_WITH_LOCKING;

// If ACE_TSS_TIMEPROBES is defined, store the ACE_Timeprobe singleton
// in thread specific storage.  This allows multiple threads to use
// their own instance of ACE_Timerprobe, without interfering with each
// other.
#  if defined (ACE_TSS_TIMEPROBES)
typedef ACE_TSS_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_NULL_MUTEX> 
        ACE_TIMEPROBE_SINGLETON;
#  else /* ACE_TSS_TIMEPROBES */
typedef ACE_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_MUTEX> 
        ACE_TIMEPROBE_SINGLETON;
#  endif /* ACE_TSS_TIMEPROBES */

// If ACE_ENABLE_TIMEPROBES is defined, the macros below will
// work. Otherwise, they just vanish.  Using this macro, you can
// control which files/libraries are probed.
#if defined (ACE_ENABLE_TIMEPROBES)

#  define ACE_TIMEPROBE_RESET ACE_TIMEPROBE_SINGLETON::instance ()->reset ()
#  define ACE_TIMEPROBE(id) ACE_TIMEPROBE_SINGLETON::instance ()->timeprobe (id)
#  define ACE_TIMEPROBE_PRINT ACE_TIMEPROBE_SINGLETON::instance ()->print_times ()
#  define ACE_TIMEPROBE_PRINT_ABSOLUTE ACE_TIMEPROBE_SINGLETON::instance ()->print_absolute_times ()
#  define ACE_TIMEPROBE_EVENT_DESCRIPTIONS(descriptions, minimum_id) static int ace_timeprobe_##descriptions##_return = ACE_TIMEPROBE_SINGLETON::instance ()->event_descriptions (descriptions, minimum_id);
#  define ACE_FUNCTION_TIMEPROBE(X) ACE_Function_Timeprobe<ACE_TIMEPROBE_WITH_LOCKING> function_timeprobe (*ACE_TIMEPROBE_SINGLETON::instance (), X)

#else /* ACE_ENABLE_TIMEPROBES */

#  define ACE_TIMEPROBE_RESET
#  define ACE_TIMEPROBE(id)
#  define ACE_TIMEPROBE_PRINT
#  define ACE_TIMEPROBE_PRINT_ABSOLUTE
#  define ACE_TIMEPROBE_EVENT_DESCRIPTIONS(descriptions, minimum_id)
#  define ACE_FUNCTION_TIMEPROBE(X)

#endif /* ACE_ENABLE_TIMEPROBES */
#endif /* ACE_TIMEPROBE_H */
