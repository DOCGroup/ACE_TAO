// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TSS_Test.cpp
//
// = DESCRIPTION
//     This program tests thread specific storage of data. The ACE_TSS
//     wrapper transparently ensures that the objects of this class
//     will be placed in thread-specific storage. All calls on
//     ACE_TSS::operator->() are delegated to the appropriate method
//     in the Errno class.  Note that each thread of control has its
//     own unique TSS object.
//
// = AUTHOR
//    Detlef Becker
//
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Task.h"

#if defined (ACE_HAS_THREADS)

// (Sun C++ 4.2 with -O3 won't link if the following is not const.)
static const int iterations = 100;

class Errno
{
public:
  int error (void) { return this->errno_; }
  void error (int i) { this->errno_ = i; }

  int line (void) { return this->lineno_; }
  void line (int l) { this->lineno_ = l; }

  // Errno::flags_ is a static variable, so we've got to protect it
  // with a mutex since it isn't kept in thread-specific storage.
  int flags (void) {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_Mon, Errno::lock_, -1));

    return Errno::flags_;
  }
  int flags (int f)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Errno::lock_, -1));

    Errno::flags_ = f;
    return 0;
  }

private:
  // = errno_ and lineno_ will be thread-specific data so they don't
  // need a lock.
  int errno_;
  int lineno_;

  static int flags_;
#if defined (ACE_HAS_THREADS)
  // flags_ needs a lock.
  static ACE_Thread_Mutex lock_;
#endif /* ACE_HAS_THREADS */
};

// Static variables.
ACE_MT (ACE_Thread_Mutex Errno::lock_);
int Errno::flags_;

// This is our thread-specific error handler...
// (Sun C++ 4.2 with -O3 won't link if the following is static.)
ACE_TSS<Errno> TSS_Error;

#if defined (ACE_HAS_THREADS)
  typedef ACE_TSS_Guard<ACE_Thread_Mutex> GUARD;
#else
  typedef ACE_Guard<ACE_Null_Mutex> GUARD;
#endif /* ACE_HAS_THREADS */

// Keeps track of whether Tester::close () has started.
// (Sun C++ 4.2 with -O3 won't link if the following is static.)
int close_started = 0;

template <ACE_SYNCH_DECL>
class Tester: public ACE_Task<ACE_SYNCH_USE>
{
public:
  Tester (void) {}
  ~Tester (void) {}

  virtual int svc (void);

  virtual int open (void *args = 0);
  // Activate the thread.

  virtual int close (u_long args = 0);
};

template <ACE_SYNCH_DECL> int
Tester<ACE_SYNCH_USE>::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) svc: setting error code to 1\n"));
  TSS_Error->error (1);

  for (int i = 0; i < iterations; i++)
    // Print out every tenth iteration.
    if ((i % 10) == 1)
      ACE_DEBUG ((LM_DEBUG, "(%t) error = %d\n", TSS_Error->error ()));

  this->close ();

  return 0;
}

template <ACE_SYNCH_DECL> int
Tester<ACE_SYNCH_USE>::open (void *)
{
  // Make this an Active Object.
  return this->activate ();
}

template <ACE_SYNCH_DECL>
int Tester<ACE_SYNCH_USE>::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) close running\n"));
  close_started = 1;
  ACE_DEBUG ((LM_DEBUG, "(%t) close: setting error code to 7\n"));
  TSS_Error->error (7);
  ACE_DEBUG ((LM_DEBUG, "(%t) close: error = %d\n", TSS_Error->error ()));
  close_started = 0;
  return 0;
}

int
main (int, char *[])
{
  Tester<ACE_MT_SYNCH> tester;

  ACE_DEBUG ((LM_DEBUG, "(%t) main: setting error code to 3\n"));
  TSS_Error->error (3);
  ACE_DEBUG ((LM_DEBUG, "(%t) main: error = %d\n", TSS_Error->error ()));

  // Spawn off a thread and make test an Active Object.
  tester.open ();

  // Keep looping until <Tester::close> is called.
  while (!close_started)
    ACE_DEBUG ((LM_DEBUG, "(%t) error = %d\n", TSS_Error->error ()));

  ACE_DEBUG ((LM_DEBUG, "(%t) main: setting error code to 4\n"));
  TSS_Error->error (4);
  ACE_DEBUG ((LM_DEBUG, "(%t) main: error = %d\n", TSS_Error->error ()));

  // Keep looping until <Tester::close> finishes.
  while (close_started != 0)
    ACE_DEBUG ((LM_DEBUG, "(%t) error = %d\n", TSS_Error->error ()));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<Errno>;
template class Tester<ACE_MT_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Errno>
#pragma instantiate Tester<ACE_MT_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "ACE doesn't support support threads on this platform (yet)\n"),
                    -1);
}
#endif /* ACE_HAS_THREADS */
