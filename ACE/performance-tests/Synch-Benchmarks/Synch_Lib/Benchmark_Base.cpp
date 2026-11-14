#if !defined (SYNCHLIB_BUILD_DLL)
#define  SYNCHLIB_BUILD_DLL
#endif /* SYNCHLIB_BUILD_DLL */

#include "ace/Log_Msg.h"
#include "Benchmark_Base.h"

#if defined (ACE_HAS_THREADS)

// Initialize the static variables.
/* static */

Benchmark_Base::Benchmark_Base (int const type)
  : benchmark_type_ (type)
{
}

int
Benchmark_Base::benchmark_type () const
{
  return this->benchmark_type_;
}

int
Benchmark_Base::thr_id () const
{
#if defined (ACE_HAS_PTHREADS) || defined (VXWORKS)
  // This invokes the thread-specific storage smart pointer.
  return this->id_->thr_id ();
#else
  return ACE_Thread::self ();
#endif /* ACE_HAS_PTHREADS || VXWORKS */
}

Benchmark_Method_Base::Benchmark_Method_Base ()
  : Benchmark_Base (Benchmark_Base::METHOD)
{
}

int
Benchmark_Method_Base::exec (ACE_Service_Repository_Iterator *sri)
{
  sri->advance ();
  for (ACE_Service_Type const *sr;
       sri->next (sr) != 0;
       sri->advance ())
    {
      // This would greatly benefit from RTTI typesafe downcasting...
      ACE_Service_Type_Impl const *type = sr->type ();
      void * const obj = type->object ();
      ACE_Service_Object * const so = static_cast<ACE_Service_Object *> (obj);
      Benchmark_Base * const bp = static_cast<Benchmark_Base *> (so);

      if (this->valid_test_object (bp))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nstarting up %s\n"), sr->name ()));

          if (this->pre_run_test (bp) == 0 && this->run_test () == 0)
            {
              this->post_run_test ();
            }
        }
      else
        return 0;
    }
  return 0;
}

#if defined (ACE_HAS_PTHREADS) || defined (VXWORKS)
/* static */
MT_INT Thr_ID::thread_id_ (0);

Thr_ID::Thr_ID ()
  : thr_id_ (++Thr_ID::thread_id_)
{
}

int
Thr_ID::thr_id () const
{
  return this->thr_id_;
}

void
Thr_ID::thr_id (int const i)
{
  this->thr_id_ = i;
}

#endif /* ACE_HAS_PTHREADS || VXWORKS */
#endif /* ACE_HAS_THREADS */
