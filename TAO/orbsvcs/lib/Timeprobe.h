//
// $Id$
//

#ifndef ACE_TIMEPROBE_H
#define ACE_TIMEPROBE_H

class ACE_Timeprobe
{
public:
  static ACE_Timeprobe &instance ();

  void timeprobe (const char *id);

  void print_times () const;

  void reset();

  void destroy ();

private:
  static ACE_Timeprobe *instance_;

  enum { SLOTS = 8192 };

  u_int current_slot_;

  typedef struct timeprobe_info {
    const char *id_;
    ACE_hrtime_t time_;
    ACE_thread_t thread_;
  } timeprobe_t;
  timeprobe_t timeprobes [SLOTS];

  ACE_Timeprobe ();
  ~ACE_Timeprobe ();

  friend class null_friend_to_avoid_compiler_warning_about_no_friends;

  ACE_Timeprobe (const ACE_Timeprobe &); // not implemented
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
