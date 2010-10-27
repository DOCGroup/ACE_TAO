// $Id$

#if !defined (SYNCHLIB_BUILD_DLL)
#define  SYNCHLIB_BUILD_DLL
#endif /* SYNCHLIB_BUILD_DLL */

#include "ace/Log_Msg.h"
#include "Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

// Initialize the static variables.
/* static */

Benchmark_Base::Benchmark_Base (int type)
  : benchmark_type_ (type)
{
}

int
Benchmark_Base::benchmark_type (void)
{
  return this->benchmark_type_;
}

int
Benchmark_Base::thr_id (void)
{
#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
  // This invokes the thread-specific storage smart pointer.
  return this->id_->thr_id ();
#else
  return ACE_Thread::self ();
#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
}

Benchmark_Method_Base::Benchmark_Method_Base (void)
  : Benchmark_Base (Benchmark_Base::METHOD)
{
}

int
Benchmark_Method_Base::exec (ACE_Service_Repository_Iterator *sri)
{
  sri->advance ();
  for (const ACE_Service_Type *sr;
       sri->next (sr) != 0;
       sri->advance ())
    {
      // This would greatly benefit from RTTI typesafe downcasting...
      const ACE_Service_Type_Impl *type = sr->type ();
      const void *obj = type->object ();
      ACE_Service_Object *so = (ACE_Service_Object *) obj;
      Benchmark_Base *bp = (Benchmark_Base *) so;

      if (this->valid_test_object (bp))
        {

          ACE_DEBUG ((LM_DEBUG, "\nstarting up %s\n", sr->name ()));

          int notused = this->pre_run_test (bp) == 0 && this->run_test () == 0 &&
            this->post_run_test () == 0;
          notused = notused;
        }
      else
        return 0;
    }
  return 0;
}

#if defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_DCETHREADS) || defined (VXWORKS)
/* static */
MT_INT Thr_ID::thread_id_ (0);

Thr_ID::Thr_ID (void)
  : thr_id_ (++Thr_ID::thread_id_)
{
}

int
Thr_ID::thr_id (void)
{
  return this->thr_id_;
}

void
Thr_ID::thr_id (int i)
{
  this->thr_id_ = i;
}

#endif /* ACE_HAS_PTHREADS || ACE_HAS_DCETHREADS || VXWORKS */
#endif /* ACE_HAS_THREADS */
