/* -*- C++ -*- */
// $Id$

#ifndef ACE_TIMEPROBE_T_H
#define ACE_TIMEPROBE_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_COMPILE_TIMEPROBES)

#include "ace/Containers.h"

template <class ACE_LOCK>
class ACE_Timeprobe
{
  // = TITLE
  //     This class is used to instrument code.  This is accomplished
  //     by inserting time probes at different location in the code.
  //     ACE_Timeprobe then measures the time difference between two
  //     time probes.
  //
  // = DESCRIPTION
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

  typedef ACE_Timeprobe<ACE_LOCK>
          SELF;
  // Self

  typedef ACE_Unbounded_Set<ACE_Event_Descriptions>
          EVENT_DESCRIPTIONS;
  // We can hold multiple event description tables.

  ACE_Timeprobe (u_long size = ACE_DEFAULT_TIMEPROBE_TABLE_SIZE);
  // Create Timeprobes with <size> slots

  ~ACE_Timeprobe (void);
  // Destructor.

  void timeprobe (u_long event);
  // Record a time. <event> is used to describe this time probe.

  void timeprobe (const char *id);
  // Record a time. <id> is used to describe this time probe.

  int event_descriptions (const char **descriptions,
                          u_long minimum_id);
  // Record event descriptions.

  void print_times (void);
  // Print the time probes.

  void print_absolute_times (void);
  // Print the time probes.

  void reset (void);
  // Reset the slots.  All old time probes will be lost.

  ACE_Timeprobe (const ACE_Timeprobe<ACE_LOCK> &);
  // Not implemented (stupid MSVC won't let it be protected).

  // = (Somewhat private) Accessors

  ACE_Unbounded_Set<ACE_Event_Descriptions> &event_descriptions (void);
  // Event Descriptions

  ACE_Unbounded_Set<ACE_Event_Descriptions> &sorted_event_descriptions (void);
  // Sorted Event Descriptions.

  u_int *current_slot_vme_address (void);
  // VME slot address.

  const char *find_description_i (u_long i);
  // Find description of event <i>

  void sort_event_descriptions_i (void);
  // Sort event descriptions

  ACE_timeprobe_t *timeprobes (void);
  // Time probe slots

  ACE_LOCK &lock (void);
  // Synchronization variable.

  u_long max_size (void);
  // Max size of timestamp table

  u_long current_size (void);
  // Current size of timestamp table

protected:

  EVENT_DESCRIPTIONS event_descriptions_;
  // Event Descriptions

  EVENT_DESCRIPTIONS sorted_event_descriptions_;
  // Sorted Event Descriptions.

  u_int *current_slot_vme_address_;
  // Added sections below here to make compatible with the VMETRO
  // board test.

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
  //     Auto pointer like time probes. It will record <event> on
  //     construction and <event + 1> on destruction.
public:
  ACE_Function_Timeprobe (Timeprobe &timeprobe,
                          u_long event);
  // Constructor.

  ~ACE_Function_Timeprobe (void);
  // Destructor.

protected:
  Timeprobe &timeprobe_;
  // Reference to timeprobe.

  u_long event_;
  // Event.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timeprobe_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timeprobe_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_COMPILE_TIMEPROBES */
#endif /* ACE_TIMEPROBE_T_H */
