// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Sigset_Ops_Test.cpp
//
// = DESCRIPTION
//     This program tests the correctness of following functions.
//     sigfillset(), sigemptyset(), sigaddset(), sigdelset(),
//     sigismember().
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"

ACE_RCSID(tests, Sigset_Ops_Test, "$Id$")

void
siglistset(sigset_t x, int *sigset)
{
  int empty = 1 ;
  int retv = 0 ;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Signal(s) in the set = %08x:\n    "), x)) ;
  for (int i = 1; i < ACE_NSIG; i++) {
    if ((retv = ACE_OS::sigismember (&x, i)) > 0) {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" %d"), i)) ;
      empty = 0 ;
    }
    ACE_ASSERT ((sigset [i] ? retv > 0 : retv <= 0)) ;
  }
  if (empty) {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Empty!!\n\n"))) ;
  }
  else {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\n"))) ;
  }
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Sigset_Ops_Test"));

#if defined (ACE_LACKS_SIGSET)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%n uses ACE implementation of sigset*() functions.\n\n"))) ;
#else
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%n uses platform's native sigset*() functions.\n\n"))) ;
#endif

  sigset_t x ;                  // examined sigset
  int sigset [ACE_NSIG] ;       // a comparison sigset
  int i ;
  int status = 0;               // 0 is success, else fail code

  // Two test signal numbers.  I choose these low value signals to
  // avoid exceeding the ACE_NSIG range.
  const int tsig1 = 5 ;
  const int tsig2 = 8 ;


  // Testing sigfillset
  ACE_OS::sigfillset (&x) ;

  // fill the comparison set
  for (i = 0 ; i < ACE_NSIG ; i++) {
    sigset [i] = 1 ;
  }
  siglistset (x, sigset) ;

  // testing sigemptyset
  ACE_OS::sigemptyset (&x) ;

  // empty the comparison set
  for (i = 0 ; i < ACE_NSIG ; i++) {
    sigset [i] = 0 ;
  }
  siglistset (x, sigset) ;

  // add the first signal into set
  ACE_OS::sigaddset (&x, tsig1) ;
  sigset [tsig1] = 1 ;
  siglistset (x, sigset) ;

  // add the second signal into set
  ACE_OS::sigaddset (&x, tsig2) ;
  sigset [tsig2] = 1 ;
  siglistset (x, sigset) ;

  // then remove it
  ACE_OS::sigdelset(&x, tsig1) ;
  sigset [tsig1] = 0 ;
  siglistset(x, sigset) ;

  // remove the second one
  ACE_OS::sigdelset(&x, tsig2) ;
  sigset [tsig2] = 0 ;
  siglistset(x, sigset) ;

  // Now testing out of bound signal
  if (ACE_OS::sigismember (&x, ACE_NSIG) >= 0) {
    ACE_ERROR((LM_ERROR, ACE_TEXT ("Platform doesn't check for valid signal number.\n")));
    status = 1;
  }
  else if (ACE_OS::last_error() != EINVAL) {
    ACE_ERROR((LM_ERROR, ACE_TEXT ("%p.\n"), ACE_TEXT ("Expected status EINVAL; got")));
    status = 1;
  }

  /* Skip this test at this moment
  // Test if platform can catch invalid sigset error
  // Currently, I can only think of passing a NULL ptr
  // If you know other situations that fall into this
  // catagory, please let me know.  Thanks.
  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Now testing invalid sigset.  If your platform gets a \nsegmentation fault, then it doesn't check the error properly.\n"))) ;

  ACE_ASSERT (ACE_OS::sigfillset (NULL) < 0 && ACE_OS::last_error() == EFAULT) ;
  */

  ACE_END_TEST;
  return status;
}
