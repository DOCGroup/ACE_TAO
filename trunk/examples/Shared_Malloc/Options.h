/* -*- C++ -*- */
// $Id$

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
  static Options *instance (void);
  // Return Singleton.

  void parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the arguments.

  // = Accessor methods.
  const char *program_name (void);
  const char *slave_name (void);
  int debug (void);
  int exec_slave (void);
  size_t iteration_count (void);
  int use_sbrk (void);
  int use_shmem (void);
  size_t max_msg_size (void);
  size_t spawn_count (void);
  int spawn_threads (void);
  int use_mmap (void);
  int child (void);

private:
  Options (void);
  // Ensure Singleton.

  static Options *instance_;
  // Singleton.

  void print_usage_and_die (void);
  // Explain usage and exit.

  char program_name_[MAXPATHLEN];
  // Name of the program.

  char slave_name_[MAXPATHLEN];
  // Name of slave process.

  int debug_;
  // Flag to indicate if we are debugging.

  int exec_slave_;
  // Flag to indicate if we should exec after forking.

  size_t iteration_count_;
  // Number of iterations to call malloc_recurse().

  int use_sbrk_;
  // Should we use sbrk(2)?

  int use_shmem_;
  // Should we use Shared Memory?

  size_t max_msg_size_;
  // Maximum number of bytes to malloc.

  size_t spawn_count_;
  // Number of threads.

  int spawn_threads_;
  // Spawn threads vs. processes.

  int use_mmap_;
  // Use mmap() as the backing store.

  int child_;
  // We're a child process.
};

#endif /* _OPTIONS */
