// $Id$

#if !defined (ACE_TIMEPROBE_H)
#define ACE_TIMEPROBE_H

// This class is compiled only when ACE_COMPILE_TIMEPROBES is defined
#if defined (ACE_COMPILE_TIMEPROBES)

#include "ace/Containers.h"

// = Event Descriptions
struct ACE_Event_Descriptions
{  
  const char **descriptions_;
  // Event descriptions

  u_long minimum_id_;
  // Minimum id of this description set

  int operator== (const ACE_Event_Descriptions &rhs) const
  {
    return 
      this->minimum_id_ == rhs.minimum_id_ &&
      this->descriptions_ == rhs.descriptions_;
  }
  // Comparison
};

// = Time probe record
struct ACE_timeprobe_t
{
  union event
  {
    u_long event_number_;
    const char *event_description_;
  };
  // Events are record as strings or numbers

  enum event_type
  {
    NUMBER,
    STRING
  };
  // Type of event

  event event_;
  // Event

  event_type event_type_;
  // Event type

  ACE_hrtime_t time_;
  // Timestamp

  ACE_thread_t thread_;
  // Id of thread posting the time probe
};

template <class ACE_LOCK>
class ACE_Timeprobe
{
  // = TITLE
  //
  //     This class is used to instrument code.  This is accomplished
  //     by inserting time probes at different location in the code.
  //     ACE_Timeprobe then measures the time difference between two
  //     time probes.
  //
  // = DESCRIPTION
  //
  //     This class provides a lightweight implementation for
  //     measuring the time required to execute code between two time
  //     probes.  When a time probe executes, it records the time, the
  //     id of the calling thread, and an event description.  The
  //     event description can either be an unsigned long or a string
  //     (char *). If string are used, care must be taken cause only
  //     pointer copies are done and the string data is *not* copied.
  //     
  //     The recorded time probes can then be printed by calling
  //     print_times().  If you have used unsigned longs as event
  //     descriptions in any of your time probes, you must have
  //     provided an event description table that maps the unsigned
  //     longs to readable strings.  This map is a simple array of
  //     strings, and the event number is used as the index into the
  //     array when looking for the event description.  If you have
  //     only used strings for the event description, this map is not
  //     necessary.
  //
  //     Multiple maps can also be used to chunk up the time probes.
  //     Each one can be added by calling event_descriptions().
  //     Different tables are used internally by consulting the
  //     minimum_id for each table.  It is up to the user to make sure
  //     that multiple tables do not share the same event id range.
public:

  enum
  {
    ACE_DEFAULT_TABLE_SIZE = 4 * 1024
    // Default size of the slots in Timeprobe
  };

  ACE_Timeprobe (u_long size = ACE_DEFAULT_TABLE_SIZE);
  // Create Timeprobes with <size> slots

  ~ACE_Timeprobe (void);
  // Destructor

  void timeprobe (u_long event);
  // Record a time. <event> is used to describe this time probe.

  void timeprobe (const char *id);
  // Record a time. <id> is used to describe this time probe.

  int event_descriptions (const char **descriptions,
                          u_long minimum_id);
  // Record event descriptions.

  void print_times (void);
  // Print the time probes. 

  void reset (void);
  // Reset the slots.  All old time probes will be lost.

protected:

  typedef ACE_Timeprobe<ACE_LOCK> SELF;
  // Self

  typedef ACE_Unbounded_Set<ACE_Event_Descriptions> EVENT_DESCRIPTIONS;
  // We can hold multiple event description tables

  EVENT_DESCRIPTIONS event_descriptions_;
  // Event Descriptions

  EVENT_DESCRIPTIONS sorted_event_descriptions_;
  // Sorted Event Descriptions

  u_int *current_slot_vme_address_;
  // Added sections below here to make compatible with the VMETRO
  // board test.

  ACE_Timeprobe (const ACE_Timeprobe &);
  // Not implemented.

  const char *find_description_i (u_long i);
  // Find description of event <i>

  void sort_event_descriptions_i (void);
  // Sort event descriptions

  ACE_timeprobe_t *timeprobes_;
  // Time probe slots

  ACE_LOCK lock_;
  // Synchronization variable.

  u_long max_size_;
  // Max size of timestamp table

  u_long current_size_;
  // Current size of timestamp table
};

template <class Timeprobe> 
class ACE_Function_Timeprobe 
{
  // = TITLE
  //
  //     Auto pointer like time probes. It will record <event> on
  //     construction and <event + 1> on destruction.
  //
public:
  ACE_Function_Timeprobe (Timeprobe &timeprobe, 
                          u_long event);

  ~ACE_Function_Timeprobe (void);

protected:

  Timeprobe &timeprobe_;

  u_long event_;
};

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

typedef ACE_Timeprobe<ACE_TIMEPROBE_MUTEX> ACE_TIMEPROBE_WITH_LOCKING;

// If ACE_TSS_TIMEPROBES is defined, store the ACE_Timeprobe singleton
// in thread specific storage.  This allows multiple threads to use
// their own instance of ACE_Timerprobe, without interfering with each
// other.
#  if defined (ACE_TSS_TIMEPROBES)
typedef ACE_TSS_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_NULL_MUTEX> ACE_TIMEPROBE_SINGLETON;
#  else /* ACE_TSS_TIMEPROBES */
typedef ACE_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_MUTEX> ACE_TIMEPROBE_SINGLETON;
#  endif /* ACE_TSS_TIMEPROBES */

#if defined (__ACE_INLINE__)
#include "ace/Timeprobe.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timeprobe.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timeprobe.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_COMPILE_TIMEPROBES */

// If ACE_ENABLE_TIMEPROBES is defined, the macros below will
// work. Otherwise, they just vanish.  Using this macro, you can
// control which files/libraries are probed.
#if defined (ACE_ENABLE_TIMEPROBES)

#  define ACE_TIMEPROBE_RESET ACE_TIMEPROBE_SINGLETON::instance ()->reset ()
#  define ACE_TIMEPROBE(id) ACE_TIMEPROBE_SINGLETON::instance ()->timeprobe (id)
#  define ACE_TIMEPROBE_PRINT ACE_TIMEPROBE_SINGLETON::instance ()->print_times ()
#  define ACE_TIMEPROBE_EVENT_DESCRIPTIONS(descriptions, minimum_id) static int ace_timeprobe_##descriptions##_return = ACE_TIMEPROBE_SINGLETON::instance ()->event_descriptions (descriptions, minimum_id)
#  define ACE_FUNCTION_TIMEPROBE(X) ACE_Function_Timeprobe<ACE_TIMEPROBE_WITH_LOCKING> function_timeprobe (*ACE_TIMEPROBE_SINGLETON::instance (), X)

#else /* ACE_ENABLE_TIMEPROBES */

#  define ACE_TIMEPROBE_RESET
#  define ACE_TIMEPROBE(id)
#  define ACE_TIMEPROBE_PRINT
#  define ACE_TIMEPROBE_EVENT_DESCRIPTIONS(descriptions, minimum_id)
#  define ACE_FUNCTION_TIMEPROBE(X)

#endif /* ACE_ENABLE_TIMEPROBES */

#endif /* ACE_TIMEPROBE_H */
