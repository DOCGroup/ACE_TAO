/* -*- C++ -*- */
// $Id$

// Option manager for Event Server.

#ifndef DEVICE_OPTIONS_H
#define DEVICE_OPTIONS_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"

#if defined (ACE_HAS_THREADS)

class Options
{
public:
  Options (void);
  ~Options (void);
  void parse_args (int argc, char *argv[]);

  void   stop_timer (void);
  void   start_timer (void);

  void   thr_count (size_t count);
  size_t thr_count (void);

  void   initial_queue_length (size_t length);
  size_t initial_queue_length (void);

  void   high_water_mark (size_t size);
  size_t high_water_mark (void);

  void   low_water_mark (size_t size);
  size_t low_water_mark (void);

  void   message_size (size_t size);
  size_t message_size (void);

  void   iterations (size_t n);
  size_t iterations (void);

  void   t_flags (long flag);
  long   t_flags (void);

  void   supplier_port (char *port);
  char   *supplier_port (void);

  void   consumer_port (char *port);
  char   *consumer_port (void);

  void   supplier_file (char *file);
  char   *supplier_file (void);

  void   consumer_file (char *file);
  char   *consumer_file (void);

  int    debug (void);
  int    verbose (void);

  void   print_results (void);

private:
  ACE_Profile_Timer itimer_;            // Time the process.
  size_t        thr_count_;             // Number of threads to spawn.
  long          t_flags_;               // Flags to thr_create().
  size_t        high_water_mark_;       // ACE_Task high water mark.
  size_t        low_water_mark_;        // ACE_Task low water mark.
  size_t        message_size_;          // Size of a message.
  size_t        initial_queue_length_;  // Initial number of items in the queue.
  size_t        iterations_;            // Number of iterations to run the test program.
  int           debugging_;             // Extra debugging info.
  int           verbosity_;             // Extra verbose messages.
  char          *consumer_port_;        // Port that the Consumer_Router is using.
  char          *supplier_port_;        // Port that the Supplier_Router is using.
  char          *consumer_file_;        // file that the Consumer_Router is using.
  char          *supplier_file_;        // file that the Supplier_Router is using.
};

extern Options options;

#include "Options.i"
#endif /* ACE_HAS_THREADS */
#endif /* DEVICE_OPTIONS_H */
