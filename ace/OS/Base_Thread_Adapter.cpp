// $Id$

#include "ace/OS/Base_Thread_Adapter.h"
#include "ace/OS/OS.h"

ACE_RCSID(ace, Base_Thread_Adapter, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "Base_Thread_Adapter.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

ACE_INIT_LOG_MSG_HOOK     ACE_Base_Thread_Adapter::init_log_msg_hook_ = 0;
ACE_INHERIT_LOG_MSG_HOOK  ACE_Base_Thread_Adapter::inherit_log_msg_hook_ = 0;
ACE_CLOSE_LOG_MSG_HOOK    ACE_Base_Thread_Adapter::close_log_msg_hook_ = 0;
ACE_SYNC_LOG_MSG_HOOK     ACE_Base_Thread_Adapter::sync_log_msg_hook_ = 0;
ACE_THR_DESC_LOG_MSG_HOOK ACE_Base_Thread_Adapter::thr_desc_log_msg_hook_ = 0;

ACE_Base_Thread_Adapter::ACE_Base_Thread_Adapter (
     ACE_THR_FUNC user_func,
     void *arg,
     ACE_THR_C_FUNC entry_point,
     ACE_OS_Thread_Descriptor *td
#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
     , ACE_SEH_EXCEPT_HANDLER selector
     , ACE_SEH_EXCEPT_HANDLER handler
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
   )
  : user_func_ (user_func)
  , arg_ (arg)
  , entry_point_ (entry_point)
  , thr_desc_ (td)
{
  ACE_OS_TRACE ("ACE_Base_Thread_Adapter::ACE_Base_Thread_Adapter");

  if (ACE_Base_Thread_Adapter::init_log_msg_hook_ != 0)
    (*ACE_Base_Thread_Adapter::init_log_msg_hook_) (
          this->log_msg_attributes_
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
          , selector
          , handler
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
          );
}

ACE_Base_Thread_Adapter::~ACE_Base_Thread_Adapter (void)
{
}

void
ACE_Base_Thread_Adapter::inherit_log_msg (void)
{
  if (ACE_Base_Thread_Adapter::inherit_log_msg_hook_ != 0)
    (*ACE_Base_Thread_Adapter::inherit_log_msg_hook_)(
           this->thr_desc_,
           this->log_msg_attributes_);
}

void
ACE_Base_Thread_Adapter::close_log_msg (void)
{
  if (ACE_Base_Thread_Adapter::close_log_msg_hook_ != 0)
    (*ACE_Base_Thread_Adapter::close_log_msg_hook_) ();
}

void
ACE_Base_Thread_Adapter::sync_log_msg (const ACE_TCHAR *prg)
{
  if (ACE_Base_Thread_Adapter::sync_log_msg_hook_ != 0)
    (*ACE_Base_Thread_Adapter::sync_log_msg_hook_) (prg);
}

ACE_OS_Thread_Descriptor *
ACE_Base_Thread_Adapter::thr_desc_log_msg (void)
{
  if (ACE_Base_Thread_Adapter::thr_desc_log_msg_hook_ != 0)
    return (*ACE_Base_Thread_Adapter::thr_desc_log_msg_hook_) ();
  return 0;
}

// Run the thread entry point for the <ACE_Thread_Adapter>.  This must
// be an extern "C" to make certain compilers happy...

#if defined (ACE_PSOS)
extern "C" void
ace_thread_adapter (unsigned long args)
{
  ACE_OS_TRACE ("ace_thread_adapter");

#if defined (ACE_HAS_TSS_EMULATION)
  // As early as we can in the execution of the new thread, allocate
  // its local TS storage.  Allocate it on the stack, to save dynamic
  // allocation/dealloction.
  void *ts_storage[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
  ACE_TSS_Emulation::tss_open (ts_storage);
#endif /* ACE_HAS_TSS_EMULATION */

  ACE_Base_Thread_Adapter *thread_args =
    ACE_static_cast(ACE_Base_Thread_Adapter *, args);

  // Invoke the user-supplied function with the args.
  thread_args->invoke ();
}
#else /* ! defined (ACE_PSOS) */
extern "C" void *
ace_thread_adapter (void *args)
{
  ACE_OS_TRACE ("ace_thread_adapter");

#if defined (ACE_HAS_TSS_EMULATION)
  // As early as we can in the execution of the new thread, allocate
  // its local TS storage.  Allocate it on the stack, to save dynamic
  // allocation/dealloction.
  void *ts_storage[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
  ACE_TSS_Emulation::tss_open (ts_storage);
#endif /* ACE_HAS_TSS_EMULATION */

  ACE_Base_Thread_Adapter *thread_args =
    ACE_static_cast (ACE_Base_Thread_Adapter *, args);

  // Invoke the user-supplied function with the args.
  void *status = thread_args->invoke ();

  return status;
}
#endif /* ACE_PSOS */
