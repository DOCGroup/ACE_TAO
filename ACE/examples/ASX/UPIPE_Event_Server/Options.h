/* -*- C++ -*- */
// Option manager for Event Server.

#ifndef DEVICE_OPTIONS_H
#define DEVICE_OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"

#if defined (ACE_HAS_THREADS)

class Options
{
public:
  Options ();
  ~Options ();
  void parse_args (int argc, ACE_TCHAR *argv[]);

  void   stop_timer ();
  void   start_timer ();

  void   thr_count (size_t count);
  size_t thr_count ();

  void   initial_queue_length (size_t length);
  size_t initial_queue_length ();

  void   high_water_mark (size_t size);
  size_t high_water_mark ();

  void   low_water_mark (size_t size);
  size_t low_water_mark ();

  void   message_size (size_t size);
  size_t message_size ();

  void   iterations (size_t n);
  size_t iterations ();

  void   t_flags (long flag);
  long   t_flags ();

  void   supplier_port (const ACE_TCHAR *port);
  const ACE_TCHAR  *supplier_port ();

  void   consumer_port (const ACE_TCHAR *port);
  const ACE_TCHAR  *consumer_port ();

  void   supplier_file (const ACE_TCHAR *file);
  const ACE_TCHAR  *supplier_file ();

  void   consumer_file (const ACE_TCHAR *file);
  const ACE_TCHAR  *consumer_file ();

  int    debug ();
  int    verbose ();

  void   print_results ();

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
  const ACE_TCHAR *consumer_port_;      // Port that the Consumer_Router is using.
  const ACE_TCHAR *supplier_port_;      // Port that the Supplier_Router is using.
  const ACE_TCHAR *consumer_file_;      // file that the Consumer_Router is using.
  const ACE_TCHAR *supplier_file_;      // file that the Supplier_Router is using.
};

extern Options options;

#include "Options.inl"
#endif /* ACE_HAS_THREADS */
#endif /* DEVICE_OPTIONS_H */
