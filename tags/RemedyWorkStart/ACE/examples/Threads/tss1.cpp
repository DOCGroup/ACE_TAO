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
//    Detlef Becker <Detlef.Becker@med.siemens.de>
//
// ============================================================================

#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"



#if defined (ACE_HAS_THREADS)

#include "thread_specific.h"

// (Sun C++ 4.2 with -O3 won't link if the following is not const.)
static const int iterations = 100;

// Static variables.
ACE_MT (ACE_Thread_Mutex Errno::lock_);
int Errno::flags_;

// This is our thread-specific error handler...
// (Sun C++ 4.2 with -O3 won't link if the following is static.)
ACE_TSS<Errno> TSS_Error;

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

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *args = 0);
  // Activate the thread.

  virtual int close (u_long args = 0);
  //FUZZ: enable check_for_lack_ACE_OS
};

template <ACE_SYNCH_DECL> int
Tester<ACE_SYNCH_USE>::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) svc: setting error code to 1\n"));
  TSS_Error->error (1);

  for (int i = 0; i < iterations; i++)
    // Print out every tenth iteration.
    if ((i % 10) == 1)
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) error = %d\n",
                  TSS_Error->error ()));
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
  ACE_DEBUG ((LM_DEBUG,
              "(%t) close running\n"));
  close_started = 1;
  ACE_DEBUG ((LM_DEBUG,
              "(%t) close: setting error code to 7\n"));
  TSS_Error->error (7);
  ACE_DEBUG ((LM_DEBUG,
              "(%t) close: error = %d\n",
              TSS_Error->error ()));
  //close_started = 0;
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Tester<ACE_MT_SYNCH> tester;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) main: setting error code to 3\n"));
  TSS_Error->error (3);
  ACE_DEBUG ((LM_DEBUG,
              "(%t) main: error = %d\n",
              TSS_Error->error ()));

  // Spawn off a thread and make test an Active Object.
  tester.open ();

  // Keep looping until <Tester::close> is called.
  for (int i = 0; !close_started; i++) {
  // while (!close_started)
    if ((i % 100) == 0) {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) error = %d\n",
                  TSS_Error->error ()));
    }
  }
  ACE_DEBUG ((LM_DEBUG,
              "(%t) main: setting error code to 4\n"));
  TSS_Error->error (4);
  ACE_DEBUG ((LM_DEBUG,
              "(%t) main: error = %d\n",
              TSS_Error->error ()));

  // Keep looping until <Tester::close> finishes.
  while (close_started != 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) error = %d\n",
                TSS_Error->error ()));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "ACE doesn't support support threads on this platform (yet)\n"),
                    -1);
}
#endif /* ACE_HAS_THREADS */
