// $Id$

#include "ace/Thread_Adapter.h"
#include "ace/OS.h"
#include "ace/Thread_Manager.h"
#include "ace/Thread_Exit.h"
#include "ace/Thread_Hook.h"

ACE_RCSID (ace,
           Thread_Adapter,
           "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/Thread_Adapter.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

ACE_Thread_Adapter::ACE_Thread_Adapter (ACE_THR_FUNC user_func,
                                        void *arg,
                                        ACE_THR_C_FUNC entry_point,
                                        ACE_Thread_Manager *tm,
                                        ACE_Thread_Descriptor *td
#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
                                        , ACE_SEH_EXCEPT_HANDLER selector,
                                        ACE_SEH_EXCEPT_HANDLER handler
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
                                        )
  : ACE_Base_Thread_Adapter (
        user_func
        , arg
        , entry_point
        , td
#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
        , selector
        , handler
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
        )
  , thr_mgr_ (tm)
{
  ACE_OS_TRACE ("ACE_Thread_Adapter::ACE_Thread_Adapter");
}

ACE_Thread_Adapter::~ACE_Thread_Adapter (void)
{
}

void *
ACE_Thread_Adapter::invoke (void)
{
  // Inherit the logging features if the parent thread has an
  // ACE_Log_Msg instance in thread-specific storage.
  this->inherit_log_msg ();

#if !defined(ACE_USE_THREAD_MANAGER_ADAPTER)
  // NOTE: this preprocessor directive should match the one in above
  // ACE_Thread_Exit::instance ().  With the Xavier Pthreads package,
  // the exit_hook in TSS causes a seg fault.  So, this works around
  // that by creating exit_hook on the stack.
# if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)
  // Obtain our thread-specific exit hook and make sure that it knows
  // how to clean us up!  Note that we never use this pointer directly
  // (it's stored in thread-specific storage), so it's ok to
  // dereference it here and only store it as a reference.

  // Except if it is null, then the thr_mgr() method crashes.
  // -jxh

  ACE_Thread_Exit *exit_hook_instance = ACE_Thread_Exit::instance ();
  ACE_Thread_Exit_Maybe exit_hook_maybe (exit_hook_instance == 0);
  ACE_Thread_Exit *exit_hook_ptr = exit_hook_instance
                                   ? exit_hook_instance
                                   : exit_hook_maybe.instance ();
  ACE_Thread_Exit &exit_hook = *exit_hook_ptr;

  if (this->thr_mgr () != 0)
    {
      // Keep track of the <Thread_Manager> that's associated with this
      // <exit_hook>.
      exit_hook.thr_mgr (this->thr_mgr ());
    }
# else
  // Without TSS, create an <ACE_Thread_Exit> instance.  When this
  // function returns, its destructor will be called because the
  // object goes out of scope.  The drawback with this appraoch is
  // that the destructor _won't_ get called if <thr_exit> is called.
  // So, threads shouldn't exit that way.  Instead, they should return
  // from <svc>.
  ACE_Thread_Exit exit_hook;
  exit_hook.thr_mgr (this->thr_mgr ());
# endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION */

#endif /* ! ACE_USE_THREAD_MANAGER_ADAPTER */

  return this->invoke_i ();
}

void *
ACE_Thread_Adapter::invoke_i (void)
{
  // Extract the arguments.
  ACE_THR_FUNC_INTERNAL func = ACE_reinterpret_cast (ACE_THR_FUNC_INTERNAL,
                                                     this->user_func_);
  void *arg = this->arg_;

#if defined (ACE_WIN32) && defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
  ACE_OS_Thread_Descriptor *thr_desc = this->thr_desc_;
#endif /* ACE_WIN32 && ACE_HAS_MFC && (ACE_HAS_MFC != 0) */

  // Delete ourselves since we don't need <this> anymore.  Make sure
  // not to access <this> anywhere below this point.
  delete this;

#if defined (ACE_NEEDS_LWP_PRIO_SET)
  // On SunOS, the LWP priority needs to be set in order to get
  // preemption when running in the RT class.  This is the ACE way to
  // do that . . .
  ACE_hthread_t thr_handle;
  ACE_OS::thr_self (thr_handle);
  int prio;

  // thr_getprio () on the current thread should never fail.
  ACE_OS::thr_getprio (thr_handle, prio);

  // ACE_OS::thr_setprio () has the special logic to set the LWP priority,
  // if running in the RT class.
  ACE_OS::thr_setprio (prio);

#endif /* ACE_NEEDS_LWP_PRIO_SET */

  void *status = 0;

  ACE_SEH_TRY
    {
      ACE_SEH_TRY
        {
          ACE_Thread_Hook *hook =
            ACE_OS_Object_Manager::thread_hook ();

          if (hook)
            // Invoke the start hook to give the user a chance to
            // perform some initialization processing before the
            // <func> is invoked.
            status = hook->start (ACE_reinterpret_cast (ACE_THR_FUNC, func),
                                  arg);
          else
            {
              // Call thread entry point.
#if defined (ACE_PSOS)
              (*func) (arg);
#else /* ! ACE_PSOS */
              status = ACE_reinterpret_cast (void *, (*func) (arg));
#endif /* ACE_PSOS */
            }
#if defined (ACE_PSOS)
          // pSOS task functions do not return a value.
          status = 0;
#endif /* ACE_PSOS */
        }

#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
      ACE_SEH_EXCEPT (ACE_OS_Object_Manager::seh_except_selector ()(
                          (void *) GetExceptionInformation ()))
        {
          ACE_OS_Object_Manager::seh_except_handler ()(0);
        }
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
    }

  ACE_SEH_FINALLY
    {
      // If we changed this to 1, change the respective if in
      // Task::svc_run to 0.
#if 0
      // Call the <Task->close> hook.
      if (func == ACE_reinterpret_cast (ACE_THR_FUNC_INTERNAL,
                                        ACE_Task_Base::svc_run))
        {
          ACE_Task_Base *task_ptr = (ACE_Task_Base *) arg;
          ACE_Thread_Manager *thr_mgr_ptr = task_ptr->thr_mgr ();

          // This calls the Task->close () hook.
          task_ptr->cleanup (task_ptr, 0);

          // This prevents a second invocation of the cleanup code
          // (called later by <ACE_Thread_Manager::exit>.
          thr_mgr_ptr->at_exit (task_ptr, 0, 0);
        }
#endif /* 0 */

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
# if defined (ACE_WIN32) && defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
      int using_afx = -1;
      if (thr_desc)
        using_afx = ACE_BIT_ENABLED (thr_desc->flags (), THR_USE_AFX);
# endif /* ACE_WIN32 && ACE_HAS_MFC && (ACE_HAS_MFC != 0) */
          // Call TSS destructors.
      ACE_OS::cleanup_tss (0 /* not main thread */);

# if defined (ACE_WIN32)
      // Exit the thread.  Allow CWinThread-destructor to be invoked
      // from AfxEndThread.  _endthreadex will be called from
      // AfxEndThread so don't exit the thread now if we are running
      // an MFC thread.
#   if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
      if (using_afx != -1)
        {
          if (using_afx)
            ::AfxEndThread ((DWORD) status);
          else
            ACE_ENDTHREADEX (status);
        }
      else
        {
          // Not spawned by ACE_Thread_Manager, use the old buggy
          // version.  You should seriously consider using
          // ACE_Thread_Manager to spawn threads.  The following code
          // is know to cause some problem.
          CWinThread *pThread = ::AfxGetThread ();

          if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
            ACE_ENDTHREADEX (status);
          else
            ::AfxEndThread ((DWORD)status);
        }
#   else

      ACE_ENDTHREADEX (status);
#   endif /* ACE_HAS_MFC && ACE_HAS_MFS != 0*/
# endif /* ACE_WIN32 */
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */

#if defined (ACE_PSOS)
      // This sequence of calls is documented by ISI as the proper way to
      // clean up a pSOS task. They affect different components, so only
      // try the ones for components that are built with ACE.
#  if defined (SC_PREPC) && (SC_PREPC == YES)
      ::fclose (0);   // Return pREPC+ resources
#  endif /* SC_PREPC */
#  if defined (SC_PHILE) && (SC_PHILE == YES)
      ::close_f (0);  // Return pHILE+ resources
#  endif /* SC_PHILE */
#  if defined (SC_PNA) && (SC_PNA == YES)
      ::close (0);    // Return pNA+ resources
#  endif /* SC_PNA */
#  if defined (SC_SC_PREPC) && (SC_PREPC == YES)
      ::free (-1);    // Return pREPC+ memory
#  endif /* SC_PREPC */
      status = ::t_delete (0); // Suicide - only returns on error
#endif /* ACE_PSOS */
    }

  return status;
}
