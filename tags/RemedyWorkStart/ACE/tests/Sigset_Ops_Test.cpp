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
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_errno.h"



void
siglistset (sigset_t x, int *sigset, int can_miss = 0)
{
  bool empty = true;
  int result = 0;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Signal (s) in the set = %08x:\n"), x)) ;

  for (int i = 1; i < ACE_NSIG; i++)
    {
      result = ACE_OS::sigismember (&x, i);

      if (result > 0)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" %d\n"), i)) ;
          empty = false;
        }
      else if (can_miss)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("Be careful... Signal %d is not valid\n"),
            i));
          result = 1;
        }
      ACE_TEST_ASSERT ((sigset [i] ? result > 0 : result <= 0)) ;
    }

  if (empty)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Empty!!\n\n"))) ;
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n\n"))) ;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Sigset_Ops_Test"));

#if defined (ACE_LACKS_SIGSET)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%n uses ACE implementation of sigset* () functions.\n\n"))) ;
#else
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%n uses platform's native sigset* () functions.\n\n"))) ;
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
  for (i = 0 ; i < ACE_NSIG ; i++)
    sigset [i] = 1 ;

  // There's no guarantee that the valid signals are sequential without
  // missed spots. For example, Red Hat Enterprise Linux 3 (any version
  // with NPTL) does not include signal 32 in sigfillset() since it's
  // reserved for kernel/nptl use. So, allow a miss in this check, but
  // be prepared to check the log file for misses if signal capability seems
  // odd if the test passes.
  siglistset (x, sigset, 1) ;

  // testing sigemptyset
  ACE_OS::sigemptyset (&x) ;

  // empty the comparison set
  for (i = 0 ; i < ACE_NSIG ; i++)
    sigset [i] = 0 ;

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
  ACE_OS::sigdelset (&x, tsig1) ;
  sigset [tsig1] = 0 ;
  siglistset (x, sigset) ;

  // remove the second one
  ACE_OS::sigdelset (&x, tsig2) ;
  sigset [tsig2] = 0 ;
  siglistset (x, sigset) ;

  // Now testing out of bound signal
  if (ACE_OS::sigismember (&x, ACE_NSIG) >= 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Platform doesn't check for valid signal number.\n")));
      status = 1;
    }
  else if (ACE_OS::last_error () != EINVAL)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p.\n"), ACE_TEXT ("Expected status EINVAL; got")));
      status = 1;
    }

  /* Skip this test at this moment
  // Test if platform can catch invalid sigset error Currently, I can
  // only think of passing a NULL ptr If you know other situations
  // that fall into this catagory, please let me know.  Thanks.
  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Now testing invalid sigset.  If your platform gets a \nsegmentation fault, then it doesn't check the error properly.\n"))) ;

  ACE_TEST_ASSERT (ACE_OS::sigfillset (0) < 0 && ACE_OS::last_error () == EFAULT) ;
  */

  ACE_END_TEST;
  return status;
}
