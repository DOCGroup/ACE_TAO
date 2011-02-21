// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Bug_3539_Regression_Test.cpp
//
// = DESCRIPTION
//    This test is similar to TSS_Test but it's a bit simpler and it uses
//    ACE_TSS in a main() thread. It uses TSS_Test_Errno.h.
//
// = AUTHOR
//    Vladimir Zykov <vz@prismtech.com>
//
// ============================================================================

#include "test_config.h"
#include "TSS_Test_Errno.h"
#include "ace/Auto_Ptr.h"
#include "ace/TSS_T.h"
#include "ace/Thread_Manager.h"
#include "ace/Atomic_Op_T.h"

ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> status;

#if defined (ACE_HAS_THREADS)

struct ObjectWithTSS
{
public:
  ACE_TSS_TYPE (Errno) tss_;
};

// Static variables.
int Errno::flags_;
int Errno::created_;
int Errno::deleted_;
ACE_Thread_Mutex *Errno::lock_ = 0;

ACE_THR_FUNC_RETURN
check_tss (ObjectWithTSS *o)
{
  if (0 == o)
    {
      ACE_ERROR ((LM_ERROR,
                  "Nil object."));
      status = 1;
    }

  Errno *e = o->tss_.ts_object (0);
  if (0 != e)
    {
      ACE_ERROR ((LM_ERROR,
                  "TSS should not have any value yet."));
      status = 2;
    }

  e = ACE_TSS_GET (&o->tss_, Errno);
  if (0 == e)
    {
      ACE_ERROR ((LM_ERROR,
                  "TSS should now have a value."));
      status = 3;
    }

  return 0;
}

// This worker function is the entry point for each thread.
ACE_THR_FUNC_RETURN
worker (void *c)
{
  return check_tss (reinterpret_cast<ObjectWithTSS *> (c));
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3539_Regression_Test"));

#if defined (ACE_HAS_THREADS)
  Errno::allocate_lock ();

  const u_int threads = ACE_MAX_THREADS;

  {
    ObjectWithTSS *o = 0;
    ACE_NEW_RETURN (o, ObjectWithTSS, 1);
    ACE_Auto_Ptr<ObjectWithTSS> owner (o);

    if (ACE_Thread_Manager::instance ()->spawn_n
        (threads,
         ACE_THR_FUNC (worker),
         o,
         THR_BOUND) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n")),
                        1);

    check_tss (o);

    ACE_Thread_Manager::instance ()->wait ();
  }

  Errno::deallocate_lock ();

  if (Errno::created () != Errno::deleted ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Number created (%d) ")
                         ACE_TEXT ("!= number deleted (%d)\n"),
                         Errno::created (), Errno::deleted ()),
                        1);
    }

#else /* ACE_HAS_THREADS */
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads are not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return status.value ();
}
