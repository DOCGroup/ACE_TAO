// $Id$

#if !defined (ACE_TIMEPROBE_H)
#define ACE_TIMEPROBE_H

#include "ace/Synch.h"

class ACE_Timeprobe
{
  // @@ Please comment me.
public:
  static ACE_Timeprobe &instance (void);

  void timeprobe (const char *id);

  void print_times (void) const;

  void reset (void);

  void destroy (void);

private:
  ACE_Timeprobe (void);
  ~ACE_Timeprobe (void);

  ACE_Timeprobe (const ACE_Timeprobe &); 
  // Not implemented.

private:
  static ACE_Timeprobe *instance_;

  enum { SLOTS = 8192 };

  ACE_SYNCH_MUTEX mutex_;

  u_int current_slot_;

  struct timeprobe_t
  {
    const char *id_;
    ACE_hrtime_t time_;
    ACE_thread_t thread_;
  };

  timeprobe_t timeprobes [SLOTS];

  // @@ Don't we have a macro that does this?
  friend class null_friend_to_avoid_compiler_warning_about_no_friends;
};

#if defined (ACE_ENABLE_TIMEPROBES)
#  define ACE_TIMEPROBE_RESET ACE_Timeprobe::instance ().reset ()
#  define ACE_TIMEPROBE(id) ACE_Timeprobe::instance ().timeprobe (id)
#  define ACE_TIMEPROBE_PRINT ACE_Timeprobe::instance ().print_times ()
#  define ACE_TIMEPROBE_FINI ACE_Timeprobe::instance ().destroy ()
#else
#  define ACE_TIMEPROBE_RESET
#  define ACE_TIMEPROBE(id)
#  define ACE_TIMEPROBE_PRINT
#  define ACE_TIMEPROBE_FINI
#endif /* ACE_ENABLE_TIMEPROBES */

#if defined (__ACE_INLINE__)
#include "Timeprobe.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TIMEPROBE_H */
