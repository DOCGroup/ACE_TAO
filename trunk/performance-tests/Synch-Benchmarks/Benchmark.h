/* -*- C++ -*- */
// $Id$

/* Defines the class used to dynamically link in the benchmark tests */

#if !defined (ACE_BENCHMARK_H)
#define ACE_BENCHMARK_H

#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Service_Record.h"

#if defined (ACE_HAS_THREADS)

extern int buffer;
extern ACE_Svc_Export int synch_count;

#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)

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
#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */

class ACE_Svc_Export Benchmark : public ACE_Service_Object
  // TITLE
  //   Base class for all the timing tests.
{
public:
  // = Hooks inherited from ACE_Service_Object.
  virtual int svc (void);
  virtual int init (int, char *[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);
  static void *svc_run (Benchmark *bp);

  int thr_id (void);
  // Returns our thread id;

  // = Set/get flag that controls how the tests are shut down
  // gracefully.
  static void done (sig_atomic_t);
  static sig_atomic_t done (void);

protected:
  static sig_atomic_t done_;
  // Keeps track if we are finished or not.

#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
  ACE_TSS <Thr_ID> id_;
  // Keeps track of our "virtual" thread id...
#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_BENCHMARK_H */
