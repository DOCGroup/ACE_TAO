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
//     in the Errno class. 
//
// = AUTHOR
//    Detlef Becker
// 
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Task.h"

#if defined (ACE_HAS_THREADS)

static int iterations = 100;

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
static ACE_TSS<Errno> TSS_Error;

#if defined (ACE_HAS_THREADS)
// Serializes output via cout.
static ACE_Thread_Mutex lock;

typedef ACE_TSS_Guard<ACE_Thread_Mutex> GUARD;
#else
// Serializes output via cout.
static ACE_Null_Mutex lock;

typedef ACE_Guard<ACE_Null_Mutex> GUARD;
#endif /* ACE_HAS_THREADS */

// Keeps track of whether Tester::close () has started.
static int close_started = 0;

template <ACE_SYNCH_1>
class Tester: public ACE_Task<ACE_SYNCH_2>
{
public:
  Tester (void) {}
  ~Tester (void) {}

  virtual int open (void *theArgs = 0);
  virtual int close (u_long theArg = 0);
};

template <ACE_SYNCH_1> int 
Tester<ACE_SYNCH_2>::open (void *)
{
  return this->activate ();
}

template <ACE_SYNCH_1>
int Tester<ACE_SYNCH_2>::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "close running\n!"));
  close_started = 1;
  ACE_OS::sleep (2);
  ACE_DEBUG ((LM_DEBUG, "close: trying to log error code 7!\n"));
  TSS_Error->error (7);
  ACE_DEBUG ((LM_DEBUG, "close: logging succeeded!\n"));
  return 0;
}

int 
main (int, char *[])
{
  Tester<ACE_MT_SYNCH> tester;

  tester.open ();

  while (!close_started)
    continue;

  ACE_DEBUG ((LM_DEBUG, "main: trying to log error code 7!\n"));

  TSS_Error->error (3);

  ACE_DEBUG ((LM_DEBUG, "main: logging succeeded!\n"));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_TSS<Errno>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int 
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, 
		     "ACE doesn't support support threads on this platform (yet)\n"),
		    -1);
}
#endif /* ACE_HAS_THREADS */
