/* -*- C++ -*- */
#ifndef _OPTIONS
#define _OPTIONS

#include "ace/os_include/os_stddef.h"
#include "ace/os_include/os_limits.h"
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Options singleton.
class Options
{
public:
  /// Returns the singleton.
  static Options *instance ();

  /// Parses the command-line arguments.
  void parse_args (int argc, ACE_TCHAR *argv[]);

  std::string const &program_name () const;
  std::string const &slave_name () const;
  bool debug () const;
  bool exec_slave () const;
  size_t iteration_count () const;
  bool use_sbrk () const;
  bool use_shmem () const;
  size_t max_msg_size () const;
  size_t spawn_count () const;
  bool spawn_threads () const;
  bool use_mmap () const;
  bool child () const;

private:
  /// Singleton-only constructor.
  Options () = default;

  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  /// Prints usage information and exits.
  void print_usage_and_die ();

  /// Program name.
  std::string program_name_ {};

  /// Slave process name.
  std::string slave_name_ { "slave" };

  /// Whether debugging is enabled.
  bool debug_ {};

  /// Whether to exec after forking.
  bool exec_slave_ {};

  /// Number of iterations to call malloc_recurse().
  size_t iteration_count_ { 100 };

  /// Whether to use sbrk(2).
  bool use_sbrk_ {};

  /// Whether to use shared memory.
  bool use_shmem_ {};

  /// Maximum number of bytes to allocate.
  size_t max_msg_size_ { 127 };

  /// Number of threads.
  size_t spawn_count_ { 1 };

  /// Whether to spawn threads instead of processes.
  bool spawn_threads_ { true };

  /// Whether to use mmap() as the backing store.
  bool use_mmap_ {};

  /// Whether this is a child process.
  bool child_ {};
};

#endif /* _OPTIONS */
