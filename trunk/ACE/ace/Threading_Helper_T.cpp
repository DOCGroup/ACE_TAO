#ifndef ACE_THREADING_HELPER_T_CPP
#define ACE_THREADING_HELPER_T_CPP

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_RCSID(ace,
          Threading_Helper_T,
          "$Id$")

template <>
ACE_Threading_Helper<ACE_Thread_Mutex>::~ACE_Threading_Helper ()
{
  ACE_OS::thr_key_detach (this->key_, 0);
  ACE_OS::thr_keyfree (this->key_);
}

template <>
ACE_Threading_Helper<ACE_Thread_Mutex>::ACE_Threading_Helper ()
  :  key_ (ACE_OS::NULL_key)
{
# if defined (ACE_HAS_TSS_EMULATION)
  ACE_Object_Manager::init_tss ();
# endif

  if (ACE_Thread::keycreate (&key_, 0, 0) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failed to create thread key: %p\n"),
                  ""));
    }
}

template <> void
ACE_Threading_Helper<ACE_Thread_Mutex>::set (void* p)
{
  if (ACE_Thread::setspecific (key_, p) == -1)
    ACE_ERROR ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Service Config failed to set thread key value: %p\n"),
                       ""));
}

template <> void*
ACE_Threading_Helper<ACE_Thread_Mutex>::get (void)
{
  void* temp = 0;
  if (ACE_Thread::getspecific (key_, &temp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Service Config failed to get thread key value: %p\n"),
                       ""),
                      0);
  return temp;
}

template <>
ACE_Threading_Helper<ACE_Null_Mutex>::~ACE_Threading_Helper ()
{
}

template <>
ACE_Threading_Helper<ACE_Null_Mutex>::ACE_Threading_Helper ()
  :  key_ (ACE_OS::NULL_key)
{
}

template <> void
ACE_Threading_Helper<ACE_Null_Mutex>::set (void*)
{
}

template <> void*
ACE_Threading_Helper<ACE_Null_Mutex>::get (void)
{
  return ACE_Service_Config::singleton()->instance_.get ();
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_THREADING_HELPER_T_CPP */

