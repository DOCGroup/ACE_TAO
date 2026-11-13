/* -*- C++ -*- */
#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"
#include "ace/Default_Constants.h"

/**
 * @class Options
 * @brief Singleton storing Event Server configuration and run statistics.
 */
class Options
{
public:
  /// Return the singleton instance.
  static Options &instance ();

  /// Parse command-line arguments and set the options.
  void parse_args (int argc, ACE_TCHAR *argv[]);

  /// @name Timer management
  /// @{

  void stop_timer ();
  void start_timer ();

  /// @}

  /// @name Thread count
  /// @{

  void thr_count (size_t count);
  size_t thr_count () const;

  /// @}

  /// @name Queue length
  /// @{

  void initial_queue_length (size_t length);
  size_t initial_queue_length () const;

  /// @}

  /// @name Queue water marks
  /// @{

  void high_water_mark (size_t size);
  size_t high_water_mark () const;

  void low_water_mark (size_t size);
  size_t low_water_mark () const;

  /// @}

  /// @name Message size
  /// @{

  void message_size (size_t size);
  size_t message_size () const;

  /// @}

  /// @name Iteration count
  /// @{

  void iterations (size_t n);
  size_t iterations () const;

  /// @}

  /// @name Thread flags
  /// @{

  void t_flags (long flag);
  long t_flags () const;

  /// @}

  /// @name Router ports
  /// @{

  void supplier_port (u_short port);
  u_short supplier_port () const;

  void consumer_port (u_short port);
  u_short consumer_port () const;

  /// @}

  /// Return true when debugging is enabled.
  bool debug () const;

  /// Return true when verbose output is enabled.
  bool verbose () const;

  /// Print the run results to standard error.
  void print_results ();

private:
  Options () = default;
  ~Options () = default;
  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  /// Time the process.
  ACE_Profile_Timer itimer_;

  /// Number of threads to spawn.
  size_t thr_count_ {4};

  /// Flags to <thr_create>.
  long t_flags_ {};

  /// ACE_Task high water mark.
  size_t high_water_mark_ {8 * 1024};

  /// ACE_Task low water mark.
  size_t low_water_mark_ {1024};

  /// Size of a message.
  size_t message_size_ {128};

  /// Initial number of items in the queue.
  size_t initial_queue_length_ {};

  /// Number of iterations to run the test program.
  size_t iterations_ {100000};

  /// Extra debugging info.
  bool debugging_ {};

  /// Extra verbose messages.
  bool verbosity_ {};

  /// Port that the Consumer_Router is using.
  u_short consumer_port_ {ACE_DEFAULT_SERVER_PORT};

  /// Port that the Supplier_Router is using.
  u_short supplier_port_ {ACE_DEFAULT_SERVER_PORT + 1};

};

#include "Options.inl"
#endif /* OPTIONS_H */
