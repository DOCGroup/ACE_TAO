// $Id$

#define ACE_BUILD_SVC_DLL

#include "Baseline_Test.h"

# if defined (ACE_HAS_THREADS)

#include "ace/Log_Msg.h"
#include "ace/Token.h"
#include "ace/Process_Mutex.h"
#include "ace/RW_Mutex.h"
#include "ace/RW_Process_Mutex.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Semaphore.h"
#include "ace/Null_Semaphore.h"
#include "ace/Process_Semaphore.h"


template<class LOCK>
class ACE_Svc_Export Baseline_Lock_Test : public Baseline_Test_Base
{
public:
  Baseline_Lock_Test ();
  virtual ~Baseline_Lock_Test ();

  virtual int acquire ();
  virtual int release ();
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release ();
  virtual int test_try_lock ();
  // Real test methods.

private:
  LOCK lock_;
  //
};

template<class LOCK>
Baseline_Lock_Test<LOCK>::Baseline_Lock_Test ()
{
}

template<class LOCK>
Baseline_Lock_Test<LOCK>::~Baseline_Lock_Test ()
{
}

template<class LOCK> int
Baseline_Lock_Test<LOCK>::acquire ()
{
  int retv = 0;
  switch (this->what_)
    {
    case TEST_READLOCK:
      retv = this->lock_.acquire_read ();
      break;
    case TEST_WRITELOCK:
      retv = this->lock_.acquire_write ();
      break;
    case TEST_LOCK:
    default:
      retv = this->lock_.acquire ();
      break;
    }

  return retv;
}

template<class LOCK> int
Baseline_Lock_Test<LOCK>::release ()
{
  return this->lock_.release ();
}

template<class LOCK> int
Baseline_Lock_Test<LOCK>::test_acquire_release ()
{
  baseline_options.start_inc_timer ();

  switch (this->what_)
    {
    case TEST_READLOCK:
      for (; baseline_options.inc_loop_counter () ; )
        {
          this->lock_.acquire_read ();
          this->lock_.release ();
        }
      break;
    case TEST_WRITELOCK:
      for (; baseline_options.inc_loop_counter () ; )
        {
          this->lock_.acquire_write ();
          this->lock_.release ();
        }
      break;
    case TEST_LOCK:
    default:
      for (; baseline_options.inc_loop_counter () ; )
        {
          this->lock_.acquire ();
          this->lock_.release ();
        }
      break;
    }

  baseline_options.stop_inc_timer ();

  return 0;
}

template<class LOCK> int
Baseline_Lock_Test<LOCK>::test_try_lock ()
{
  baseline_options.start_inc_timer ();
  switch (this->what_)
    {
    case TEST_READLOCK:
      for (; baseline_options.inc_loop_counter () ; )
        this->lock_.tryacquire_read (); // This should always fail.
      break;
    case TEST_WRITELOCK:
      for (; baseline_options.inc_loop_counter () ; )
        this->lock_.tryacquire_write (); // This should always fail.
      break;
    case TEST_LOCK:
    default:
      for (; baseline_options.inc_loop_counter () ; )
        this->lock_.tryacquire (); // This should always fail.
      break;
    }

  for (; baseline_options.inc_loop_counter () ; )
    this->lock_.tryacquire (); // This should always fail.

  baseline_options.stop_inc_timer ();
  return 0;
}

typedef Baseline_Lock_Test<ACE_Thread_Mutex> Baseline_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Mutex_Test)

typedef Baseline_Lock_Test<ACE_RW_Thread_Mutex> Baseline_RW_Thread_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_RW_Thread_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_RW_Thread_Mutex_Test)

typedef Baseline_Lock_Test<ACE_RW_Mutex> Baseline_RW_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_RW_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_RW_Mutex_Test)

typedef Baseline_Lock_Test<ACE_Process_Mutex> Baseline_Process_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Process_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Process_Mutex_Test)

typedef Baseline_Lock_Test<ACE_RW_Process_Mutex> Baseline_RW_Process_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_RW_Process_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_RW_Process_Mutex_Test)

typedef Baseline_Lock_Test<ACE_Null_Mutex> Baseline_Null_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Null_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Null_Mutex_Test)

typedef Baseline_Lock_Test< ACE_Lock_Adapter<ACE_Null_Mutex> > Baseline_Adaptive_Null_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Adaptive_Null_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Adaptive_Null_Mutex_Test)

typedef Baseline_Lock_Test<ACE_Recursive_Thread_Mutex> Baseline_Recursive_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Recursive_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Recursive_Mutex_Test)

typedef Baseline_Lock_Test< ACE_Lock_Adapter<ACE_Thread_Mutex> > Baseline_Adaptive_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Adaptive_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Adaptive_Mutex_Test)

typedef Baseline_Lock_Test< ACE_Lock_Adapter<ACE_RW_Mutex> > Baseline_Adaptive_RW_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Adaptive_RW_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Adaptive_RW_Mutex_Test)

typedef Baseline_Lock_Test< ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex> > Baseline_Adaptive_Recursive_Mutex_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Adaptive_Recursive_Mutex_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Adaptive_Recursive_Mutex_Test)

typedef Baseline_Lock_Test<ACE_Semaphore> Baseline_Semaphore_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Semaphore_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Semaphore_Test)

typedef Baseline_Lock_Test<ACE_Semaphore> Baseline_Process_Semaphore_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Process_Semaphore_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Process_Semaphore_Test)

typedef Baseline_Lock_Test<ACE_Null_Semaphore> Baseline_Null_Semaphore_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Null_Semaphore_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Null_Semaphore_Test)

typedef Baseline_Lock_Test<ACE_Token> Baseline_Token_Test;

ACE_SVC_FACTORY_DECLARE (Baseline_Token_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Token_Test)

#endif  /* ACE_HAS_THREADS */
