// $Id$

#include "ace/Proactor.h"

#include "JAWS/Waiter.h"
#include "JAWS/IO_Handler.h"

ACE_RCSID(JAWS, Waiter, "$Id$")

JAWS_Waiter::JAWS_Waiter (void)
  : iter_ (*this)
{
}

JAWS_Waiter::~JAWS_Waiter (void)
{
}

JAWS_Waiter_Base_Iterator &
JAWS_Waiter::iter (void)
{
  return this->iter_;
}

int
JAWS_Waiter::index (void)
{
#if 0
  // A future version of ACE will support this.
  ACE_Thread_ID tid = ACE_OS::thr_self ();
#else
  // Do it this way for now
  ACE_thread_t thr_name;
  thr_name = ACE_OS::thr_self ();

  JAWS_Thread_ID tid (thr_name);
#endif /* 0 */

  return JAWS_Waiter_Base::index (tid);
}

JAWS_IO_Handler *
JAWS_Waiter::wait_for_completion (int i)
{
  JAWS_IO_Handler *ioh;
  JAWS_IO_Handler **iohptr;

  iohptr = (i >= 0) ? this->find (i) : this->find (this->index ());

  while (*iohptr == 0)
    if (ACE_Proactor::instance ()->handle_events () == -1)
      {
        ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_Waiter::wait_for_completion"));
        return 0;
      }

  ioh = *iohptr;
  *iohptr = 0;

  ioh->lock ();
  ioh->release ();
  return ioh;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class JAWS_Assoc_Array<JAWS_Thread_ID, JAWS_IO_Handler *>;
template class JAWS_Assoc_Array_Iterator<JAWS_Thread_ID, JAWS_IO_Handler *>;
template class ACE_Singleton<JAWS_Waiter, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate JAWS_Assoc_Array<JAWS_Thread_ID, JAWS_IO_Handler *>
#pragma instantiate JAWS_Assoc_Array_Iterator<JAWS_Thread_ID, JAWS_IO_Handler *>
#pragme instantiate ACE_Singleton<JAWS_Waiter, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
