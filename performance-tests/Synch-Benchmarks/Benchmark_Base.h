/* -*- C++ -*- */
// $Id$

/* Defines the base class used to dynamically link in the benchmark tests */

#if !defined (ACE_BENCHMARK_BASE_H)
# define ACE_BENCHMARK_BASE_H

# include "ace/Service_Config.h"
# include "ace/Synch.h"
# include "ace/Service_Types.h"

# if defined (ACE_HAS_THREADS)

#   if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)

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
#   endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */

class ACE_Svc_Export Benchmark_Base : public ACE_Service_Object
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

  Benchmark_Base (void);
  // Default ctor.

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
  int benchmark_type_;
  // Store the RTTI info of this module.

#   if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
  ACE_TSS <Thr_ID> id_;
  // Keeps track of our "virtual" thread id...
#   endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
};
# endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_H */
