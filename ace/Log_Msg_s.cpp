// $Id$

#define ACE_BUILD_DLL

#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Manager.h"

// Default per-process priority mask
// By default, no priorities are enabled.
u_long ACE_Log_Msg::process_priority_mask_ = 0;

// Records the program name.
const ASYS_TCHAR *ACE_Log_Msg::program_name_ = 0;

// Name of the local host.
const ASYS_TCHAR *ACE_Log_Msg::local_host_ = 0;

// Process id of the current process.
pid_t ACE_Log_Msg::pid_ = -1;

// Default is to use stderr.
u_long ACE_Log_Msg::flags_ = ACE_Log_Msg::STDERR;

// Current offset of msg_[].
int ACE_Log_Msg::msg_off_ = 0;

// Instance count for Log_Msg - used to know when dynamically
// allocated storage (program name and host name) can be safely
// deleted.
int ACE_Log_Msg::instance_count_ = 0;

// Default per-thread priority mask
// By default, all priorities are enabled.
u_long ACE_Log_Msg::default_priority_mask_ = LM_SHUTDOWN
                                           | LM_TRACE
                                           | LM_DEBUG
                                           | LM_INFO
                                           | LM_NOTICE
                                           | LM_WARNING
                                           | LM_STARTUP
                                           | LM_ERROR
                                           | LM_CRITICAL
                                           | LM_ALERT
                                           | LM_EMERGENCY;

/* static */
ACE_LOG_MSG_IPC_STREAM *ACE_Log_Msg_Manager::message_queue_ = 0;

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)

#include "ace/Synch.h"

ACE_Recursive_Thread_Mutex *ACE_Log_Msg_Manager::lock_ = 0;

int ACE_Log_Msg::key_created_ = 0;

# if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || \
     defined (ACE_HAS_TSS_EMULATION)
// This can cause a static constructor (but not destructor) call.
ACE_thread_key_t ACE_Log_Msg::log_msg_tss_key_ = ACE_OS::NULL_key;
# endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION */

#endif /* ACE_MT_SAFE */
