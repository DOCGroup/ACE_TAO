// $Id$

#define ACE_BUILD_DLL

#include "ace/Log_Msg_Manager.h"

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)

#include "ace/Synch.h"

ACE_Recursive_Thread_Mutex *
ACE_Log_Msg_Manager::get_lock (void)
{
  // This function is called by the first thread to create an ACE_Log_Msg
  // instance.  It makes the call while holding a mutex, so we don't have
  // to grab another one here.

  if (ACE_Log_Msg_Manager::lock_ == 0)
    {
      ACE_NO_HEAP_CHECK;

      ACE_NEW_RETURN_I (ACE_Log_Msg_Manager::lock_,
                        ACE_Recursive_Thread_Mutex,
                        0);

      // Allocate the ACE_Log_Msg IPC instance.
      ACE_NEW_RETURN (message_queue_, ACE_LOG_MSG_IPC_STREAM, 0);
    }

  return ACE_Log_Msg_Manager::lock_;
}

void
ACE_Log_Msg_Manager::close (void)
{
#if defined (ACE_HAS_STHREADS) && !defined (ACE_HAS_TSS_EMULATION) && \
    !defined (ACE_HAS_EXCEPTIONS)
  // Delete the (main thread's) Log_Msg instance.  I think that this
  // is only necessary if exception handling is not enabled.
  // Without exception handling, main thread TSS destructors don't
  // seem to be called.  It's not really necessary anyways, because
  // this one leak is harmless on Solaris.
  delete ACE_Log_Msg::instance ();
#endif /* ACE_HAS_STHREADS && ! TSS_EMULATION && ! ACE_HAS_EXCEPTIONS */

  // Ugly, ugly, but don't know a better way.
  delete ACE_Log_Msg_Manager::lock_;
  ACE_Log_Msg_Manager::lock_ = 0;
}

#endif /* ACE_MT_SAFE */
