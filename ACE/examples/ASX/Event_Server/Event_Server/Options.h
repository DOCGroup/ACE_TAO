/* -*- C++ -*- */
#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"
#include "ace/Default_Constants.h"

class Options
{
  // = TITLE
  //     Option Singleton for Event Server.
public:
  static Options &instance ();
  // Singleton access point.

  void parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the command-line arguments and set the options.

  // = Timer management.
  void stop_timer ();
  void start_timer ();

  // = Set/get the number of threads.
  void thr_count (size_t count);
  size_t thr_count () const;

  // = Set/get the size of the queue.
  void initial_queue_length (size_t length);
  size_t initial_queue_length () const;

  // = Set/get the high water mark.
  void high_water_mark (size_t size);
  size_t high_water_mark () const;

  // = Set/get the high water mark.
  void low_water_mark (size_t size);
  size_t low_water_mark () const;

  // = Set/get the size of a message.
  void message_size (size_t size);
  size_t message_size () const;

  // = Set/get the number of iterations.
  void iterations (size_t n);
  size_t iterations () const;

  // Set/get threading flags.
  void t_flags (long flag);
  long t_flags () const;

  // Set/get supplier port number.
  void supplier_port (u_short port);
  u_short supplier_port () const;

  // Set/get consumer port number.
  void consumer_port (u_short port);
  u_short consumer_port () const;

  // Enabled if we're in debugging mode.
  bool debug () const;

  // Enabled if we're in verbose mode.
  bool verbose () const;

  // Print the results to the STDERR.
  void print_results ();

private:
  // = Ensure we're a Singleton.
  Options () = default;
  ~Options () = default;
  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  ACE_Profile_Timer itimer_;
  // Time the process.

  size_t thr_count_ {4};
  // Number of threads to spawn.

  long t_flags_ {};
  // Flags to <thr_create>.

  size_t high_water_mark_ {8 * 1024};
  // ACE_Task high water mark.

  size_t low_water_mark_ {1024};
  // ACE_Task low water mark.

  size_t message_size_ {128};
  // Size of a message.

  size_t initial_queue_length_ {};
  // Initial number of items in the queue.

  size_t iterations_ {100000};
  // Number of iterations to run the test program.

  bool debugging_ {};
  // Extra debugging info.

  bool verbosity_ {};
  // Extra verbose messages.

  u_short consumer_port_ {ACE_DEFAULT_SERVER_PORT};
  // Port that the Consumer_Router is using.

  u_short supplier_port_ {ACE_DEFAULT_SERVER_PORT + 1};
  // Port that the Supplier_Router is using.

};

#include "Options.inl"
#endif /* OPTIONS_H */
