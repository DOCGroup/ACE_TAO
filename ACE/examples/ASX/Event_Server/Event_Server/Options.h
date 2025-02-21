/* -*- C++ -*- */
#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"

class Options
{
  // = TITLE
  //     Option Singleton for Event Server.
public:
  static Options *instance ();
  // Singleton access point.

  void parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the command-line arguments and set the options.

  // = Timer management.
  void stop_timer ();
  void start_timer ();

  // = Set/get the number of threads.
  void thr_count (size_t count);
  size_t thr_count ();

  // = Set/get the size of the queue.
  void initial_queue_length (size_t length);
  size_t initial_queue_length ();

  // = Set/get the high water mark.
  void high_water_mark (size_t size);
  size_t high_water_mark ();

  // = Set/get the high water mark.
  void low_water_mark (size_t size);
  size_t low_water_mark ();

  // = Set/get the size of a message.
  void message_size (size_t size);
  size_t message_size ();

  // = Set/get the number of iterations.
  void iterations (size_t n);
  size_t iterations ();

  // Set/get threading flags.
  void t_flags (long flag);
  long t_flags ();

  // Set/get supplier port number.
  void supplier_port (u_short port);
  u_short supplier_port ();

  // Set/get consumer port number.
  void consumer_port (u_short port);
  u_short consumer_port ();

  // Enabled if we're in debugging mode.
  int debug ();

  // Enabled if we're in verbose mode.
  int verbose ();

  // Print the results to the STDERR.
  void print_results ();

private:
  // = Ensure we're a Singleton.
  Options ();
  ~Options ();

  ACE_Profile_Timer itimer_;
  // Time the process.

  size_t thr_count_;
  // Number of threads to spawn.

  long t_flags_;
  // Flags to <thr_create>.

  size_t high_water_mark_;
  // ACE_Task high water mark.

  size_t low_water_mark_;
  // ACE_Task low water mark.

  size_t message_size_;
  // Size of a message.

  size_t initial_queue_length_;
  // Initial number of items in the queue.

  size_t iterations_;
  // Number of iterations to run the test program.

  int debugging_;
  // Extra debugging info.

  int verbosity_;
  // Extra verbose messages.

  u_short consumer_port_;
  // Port that the Consumer_Router is using.

  u_short supplier_port_;
  // Port that the Supplier_Router is using.

  static Options *instance_;
  // Static Singleton.
};

#include "Options.inl"
#endif /* OPTIONS_H */
