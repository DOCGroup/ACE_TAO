/* -*- C++ -*- */
// $Id$

/* Defines the base class used to dynamically link in the benchmark tests */

#ifndef ACE_BENCHMARK_BASE_H
# define ACE_BENCHMARK_BASE_H

# include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/Service_Repository.h"
# include "ace/Service_Types.h"
# include "ace/Atomic_Op.h"
# include "export_mac.h"
# include "ace/TSS_T.h"

# if defined (ACE_HAS_THREADS)

#   if defined (ACE_HAS_PTHREADS) || defined (VXWORKS)

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> MT_INT;

class Thr_ID
  // TITLE
  //   A simple class that provides a thread-specific value in order
  //   to compensate for POSIX Pthreads.
  //
  // DESCRIPTION
  //   Pthreads are too lame to have a sensible scalar values for the
  //   thread id (unlike Solaris threads).   Therefore, we have to
  //   emulate this ourselves with this class (gag).
{
public:
  Thr_ID (void);
  int thr_id (void);
  void thr_id (int);

private:
  int thr_id_;
  static MT_INT thread_id_;
};
#   endif /* ACE_HAS_PTHREADS || VXWORKS */

class SYNCHLIB_Export Benchmark_Base : public ACE_Service_Object
{
  // = TITLE
  //     Base class for all benchmarking objects.
  //
  // = DESCRIPTION
  //     This class is the base class for all benchmarking
  //     classes.  Its major functionalities are to privide RTTI
  //     information and to define other common methods all
  //     benchmarking classes should support.
public:
  enum {
    BENCHMARK_BASE,
    METHOD,
    BASELINE,
    PERFORMANCE
  };

  int benchmark_type (void);
  // RTTI information of this module.

  int thr_id (void);
  // Returns our thread id;

protected:
  Benchmark_Base (int type = BENCHMARK_BASE);
  // Default ctor.

  int benchmark_type_;
  // Store the RTTI info of this module.

#   if defined (ACE_HAS_PTHREADS) || defined (VXWORKS)
  ACE_TSS <Thr_ID> id_;
  // Keeps track of our "virtual" thread id...
#   endif /* ACE_HAS_PTHREADS || VXWORKS */
};

class SYNCHLIB_Export Benchmark_Method_Base : public Benchmark_Base
{
  // = TITLE
  //     This class identifies itself as Benmarking Method class.
  //     It defines a method as of how the test is setup and measured.
public:
  int exec (ACE_Service_Repository_Iterator *sri);
  // Run the test and advanced the service repository iterator

  virtual int pre_run_test (Benchmark_Base *bp) = 0;
  // Before running the real test.  Subclasses implement this method
  // to dictate how the test is performed.

  virtual int run_test (void) = 0;
  // Run the real test.  Subclasses implement this method to
  // dictate how the test is performed.

  virtual int post_run_test (void) = 0;
  // After running the real test.  Subclasses implement this method to
  // dictate how the test is performed.

  virtual int valid_test_object (Benchmark_Base *) = 0;
  // Check if we got a valid test to perform.

protected:
  Benchmark_Method_Base (void);
};

# endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_BASE_H */
