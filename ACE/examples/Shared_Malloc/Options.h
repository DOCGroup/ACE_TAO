/* -*- C++ -*- */
#ifndef _OPTIONS
#define _OPTIONS

#include "ace/os_include/os_stddef.h"
#include "ace/os_include/os_limits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Options
  // = TITLE
  //   Options Singleton.
{
public:
  static Options *instance ();
  // Return Singleton.

  void parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the arguments.

  // = Accessor methods.
  const char *program_name () const;
  const char *slave_name () const;
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
  Options ();
  // Ensure Singleton.

  Options (const Options &) = delete;
  Options (Options &&) = delete;
  Options &operator= (const Options &) = delete;
  Options &operator= (Options &&) = delete;

  static Options *instance_;
  // Singleton.

  void print_usage_and_die ();
  // Explain usage and exit.

  char program_name_[MAXPATHLEN];
  // Name of the program.

  char slave_name_[MAXPATHLEN];
  // Name of slave process.

  bool debug_;
  // Flag to indicate if we are debugging.

  bool exec_slave_;
  // Flag to indicate if we should exec after forking.

  size_t iteration_count_;
  // Number of iterations to call malloc_recurse().

  bool use_sbrk_;
  // Should we use sbrk(2)?

  bool use_shmem_;
  // Should we use Shared Memory?

  size_t max_msg_size_;
  // Maximum number of bytes to malloc.

  size_t spawn_count_;
  // Number of threads.

  bool spawn_threads_;
  // Spawn threads vs. processes.

  bool use_mmap_;
  // Use mmap() as the backing store.

  bool child_;
  // We're a child process.
};

#endif /* _OPTIONS */
