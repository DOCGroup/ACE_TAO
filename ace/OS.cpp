// $Id$

// OS.cpp
#define ACE_BUILD_DLL
#include "ace/OS.h"

#include "ace/ARGV.h"

// Perhaps we should *always* include ace/OS.i in order to make sure
// we can always link against the OS symbols?
#if !defined (ACE_HAS_INLINED_OSCALLS)
#include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OS_CALLS */

void 
ACE_OS::flock_t::dump (void) const
{
// ACE_TRACE ("ACE_OS::flock_t::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "handle_ = %u", this->handle_));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG, "\nInternal = %d", this->overlapped_.Internal));
  ACE_DEBUG ((LM_DEBUG, "\nInternalHigh = %d", this->overlapped_.InternalHigh));
  ACE_DEBUG ((LM_DEBUG, "\nOffsetHigh = %d", this->overlapped_.OffsetHigh));
  ACE_DEBUG ((LM_DEBUG, "\nhEvent = %d", this->overlapped_.hEvent));
#else
  ACE_DEBUG ((LM_DEBUG, "\nl_whence = %d", this->lock_.l_whence));
  ACE_DEBUG ((LM_DEBUG, "\nl_start = %d", this->lock_.l_start));
  ACE_DEBUG ((LM_DEBUG, "\nl_len = %d", this->lock_.l_len));
  ACE_DEBUG ((LM_DEBUG, "\nl_type = %d", this->lock_.l_type));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_OS::mutex_lock_cleanup (void *mutex)
{
// ACE_TRACE ("ACE_OS::mutex_lock_cleanup");
#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
  ACE_mutex_t *p_lock = (ACE_mutex_t *) mutex;
  ACE_OS::mutex_unlock (p_lock);
#endif /* ACE_HAS_DCETHREADS */
#endif /* ACE_HAS_THREADS */
}

// = Static initialization.

// This is necessary to deal with POSIX pthreads insanity.  This
// guarantees that we've got a "zero'd" thread id even when ACE_thread_t
// is implemented as a structure...
ACE_thread_t ACE_OS::NULL_thread;
ACE_hthread_t ACE_OS::NULL_hthread;

ACE_OS::ACE_OS (void)
{
// ACE_TRACE ("ACE_OS::ACE_OS");
}

#if defined (ACE_WIN32)

// = Static initialization.

// Keeps track of whether we've initialized the WinSock DLL.
int ACE_OS::socket_initialized_;

// We need this to initialize the WinSock DLL.

BOOL WINAPI
DllMain (HINSTANCE, // DLL module handle
	 DWORD fdwReason, // Reason called
	 LPVOID) // Reserved
{
  switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
      if (ACE_OS::socket_init (ACE_WSOCK_VERSION) != 0)
	return FALSE;
      break;

    case DLL_PROCESS_DETACH:
      if (ACE_OS::socket_fini () != 0)
        return FALSE;
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "Sock.DLL DllMain called with unknown fdwReason = %u\n.", 
			 fdwReason), FALSE);
      /* NOTREACHED */
    }

  return TRUE;
}

#include "ace/Synch.h"
#include "ace/Set.h"

class ACE_TSS_Ref
  // = TITLE
  //     "Reference count" for thread-specific storage keys.  
  //
  // = DESCRIPTION
  //     Since the ACE_Unbounded_Set doesn't allow duplicates, the
  //     "reference count" is the identify of the thread_id.
{
public:
  ACE_TSS_Ref (ACE_thread_t id);
  // Constructor

  ACE_TSS_Ref (void);
  // Default constructor

  int operator== (const ACE_TSS_Ref &);
  // Check for equality.

// private:

  ACE_thread_t tid_;
  // ID of thread using a specific key.
};

ACE_TSS_Ref::ACE_TSS_Ref (ACE_thread_t id)
  : tid_(id)
{
// ACE_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

ACE_TSS_Ref::ACE_TSS_Ref (void)
{
// ACE_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

// Check for equality.
int
ACE_TSS_Ref::operator== (const ACE_TSS_Ref &info)
{
// ACE_TRACE ("ACE_TSS_Ref::operator==");

  return this->tid_ == info.tid_;
}

typedef ACE_Unbounded_Set<ACE_TSS_Ref> ACE_TSS_REF_TABLE;
typedef ACE_Unbounded_Set_Iterator<ACE_TSS_Ref> ACE_TSS_REF_TABLE_ITERATOR;

class ACE_TSS_Info
  // = TITLE
  //     Thread Specific Key management.
  //
  // = DESCRIPTION
  //     This class maps a key to a "destructor."
{
public:
  ACE_TSS_Info (ACE_thread_key_t key, 
		void (*dest)(void *) = 0, 
		void *tss_inst = 0);
  // Constructor

  ACE_TSS_Info (void);
  // Default constructor

  int operator== (const ACE_TSS_Info &);
  // Check for equality.

  void dump (void);
  // Dump the state.

// private:
  ACE_thread_key_t key_;
  // Key to the thread-specific storage item.

  void (*destructor_)(void *);
  // "Destructor" that gets called when the item is finally released.

  void *tss_obj_;
  // Pointer to ACE_TSS<xxx> instance that has/will allocate the key.
 
  ACE_TSS_REF_TABLE ref_table_;
  // Table of thread IDs that are using this key.
};

ACE_TSS_Info::ACE_TSS_Info (ACE_thread_key_t key, 
			    void (*dest)(void *),
			    void *tss_inst)
  : key_ (key),
    destructor_ (dest),
    tss_obj_ (tss_inst)
{
// ACE_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

ACE_TSS_Info::ACE_TSS_Info (void)
{
// ACE_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

// Check for equality.
int 
ACE_TSS_Info::operator== (const ACE_TSS_Info &info)
{
// ACE_TRACE ("ACE_TSS_Info::operator==");

  return this->key_ == info.key_;
}

void 
ACE_TSS_Info::dump (void)
{
//  ACE_TRACE ("ACE_TSS_Info::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "key_ = %u", this->key_));
  ACE_DEBUG ((LM_DEBUG, "\ndestructor_ = %u", this->destructor_));
  ACE_DEBUG ((LM_DEBUG, "\ntss_obj_ = %u", this->tss_obj_));
  ACE_DEBUG ((LM_DEBUG, "\nref_table_.size_ = %u", this->ref_table_.size ()));

  ACE_TSS_Ref *tid_info = 0;

  ACE_DEBUG ((LM_DEBUG, "\nThread_usage_list\n[\n"));

  for (ACE_TSS_REF_TABLE_ITERATOR iter (this->ref_table_);
       iter.next (tid_info) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG, "\ntid_ = %d", tid_info->tid_));

  ACE_DEBUG ((LM_DEBUG, "\n]\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Create a set of <ACE_TSS_Info> objects that will reside
// within thread-specific storage.
typedef ACE_Unbounded_Set<ACE_TSS_Info> ACE_TSS_TABLE;
typedef ACE_Unbounded_Set_Iterator<ACE_TSS_Info> ACE_TSS_TABLE_ITERATOR;

class ACE_TSS_Cleanup
  // = TITLE
  //     Singleton that knows how to clean up all the thread-specific
  //     resources for Win32.
  // 
  // = DESCRIPTION
  //     All this nonsense is required since Win32 doesn't
  //     automatically cleanup thread-specific storage on thread exit,
  //     unlike real operating systems... ;-)
{
public:
  static ACE_TSS_Cleanup *instance (void);

  void exit (void *status);
  // Cleanup the thread-specific objects and exit with <status>.

  int insert (ACE_thread_key_t key, void (*destructor)(void *), void *inst);
  // Insert a <key, destructor> tuple into the table.

  int remove (ACE_thread_key_t key);
  // Remove a <key, destructor> tuple from the table.

  int detach (void *inst);
  // Detaches a tss_instance from its key.
 
  int detach (ACE_thread_key_t key, ACE_thread_t tid);
  // Detaches a thread from the key.
 
  int key_used (ACE_thread_key_t key);
  // Mark a key as being used by this thread.

protected:
  int mark_cleanup_i (void);
  // Mark a thread for actually performing cleanup.

  int check_cleanup_i (void);
  // Check if given thread is performing cleanup.

  int exit_cleanup_i (void);
  // Indicate that a thread has finished cleanup.

  void dump (void);

  ACE_TSS_Cleanup (void);
  // Ensure singleton.

private:
  ACE_TSS_TABLE table_;
  // Table of <ACE_TSS_Info>'s.

  ACE_TSS_REF_TABLE ref_table_;
  // Table of thread IDs that are performing cleanup activities.

  // = Static data.
  static ACE_TSS_Cleanup *instance_;
  // Pointer to the singleton instance.

public:
  static ACE_Thread_Mutex lock_;
  // Serialize initialization of <key_>.
};

// = Static object initialization.

// Pointer to the singleton instance.
ACE_TSS_Cleanup *ACE_TSS_Cleanup::instance_ = 0;

// Serialize initialization of <key_>.
ACE_Thread_Mutex ACE_TSS_Cleanup::lock_;

int 
ACE_TSS_Cleanup::mark_cleanup_i (void)
{
  return this->ref_table_.insert (ACE_TSS_Ref (ACE_OS::thr_self ()));
}
  
int 
ACE_TSS_Cleanup::check_cleanup_i (void)
{
  return this->ref_table_.find (ACE_TSS_Ref (ACE_OS::thr_self ()));
}

int 
ACE_TSS_Cleanup::exit_cleanup_i (void)
{
  return this->ref_table_.remove (ACE_TSS_Ref (ACE_OS::thr_self ()));
}

void 
ACE_TSS_Cleanup::exit (void *status)
{
// ACE_TRACE ("ACE_TSS_Cleanup::exit");

  ACE_thread_key_t key_arr[TLS_MINIMUM_AVAILABLE];
  int index = 0;

  ACE_TSS_Info *key_info = 0;
  ACE_TSS_Info info_arr[TLS_MINIMUM_AVAILABLE];
  int info_ix = 0;

  // While holding the lock, we only collect the ACE_TSS_Info objects
  // in an array without invoking the according destructors.

  {
    ACE_GUARD (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_);

    // Prevent recursive deletions

    if (this->check_cleanup_i ()) // Are we already performing cleanup?
      return;

    // If we can't insert our thread_id into the list, we will not be
    // able to detect recursive invocations for this thread. Therefore
    // we better risk memory and key leakages, resulting also in
    // missing close() calls as to be invoked recursively.

    if (this->mark_cleanup_i () != 0) // Insert our thread_id in list
      return;

    // Iterate through all the thread-specific items and free them all
    // up.

    for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
	 iter.next (key_info) != 0;
	 iter.advance ())
      {
	void *tss_info = 0;

	int val = key_info->ref_table_.remove (ACE_TSS_Ref (ACE_OS::thr_self ()));

	if ((ACE_OS::thr_getspecific (key_info->key_, &tss_info) == 0)
	    && (key_info->destructor_) 
	    && tss_info)
          info_arr[info_ix++] = *key_info; // copy this information into array

 	if (key_info->ref_table_.size () == 0 
	    && key_info->tss_obj_ == 0)
	  key_arr[index++] = key_info->key_;
      }
   }

   // Now we have given up the ACE_TSS_Cleanup::lock_ and we start
   // invoking destructors.

   for (int i = 0; i < info_ix; i++)
     {
       void *tss_info = 0;

       ACE_OS::thr_getspecific (info_arr[i].key_, &tss_info);

       (*info_arr[i].destructor_)(tss_info);
     }

   // Acquiring ACE_TSS_Cleanup::lock_ to free TLS keys and remove
   // entries from ACE_TSS_Info table.
   {
    ACE_GUARD (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_);

    for (int i = 0; i < index; i++)
      {
	::TlsFree (key_arr[i]);
	this->table_.remove (ACE_TSS_Info (key_arr[i]));
      }

    this->exit_cleanup_i (); // remove thread id from reference list.
   }

#if defined (ACE_HAS_MFC)	
  // allow CWinThread-destructor to be invoked from AfxEndThread
  // _endthreadex will be called from AfxEndThread so don't exit the
  // thread now if we are running an MFC thread.
  CWinThread *pThread = ::AfxGetThread ();
  if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
#endif /* ACE_HAS_MFC */
    ::_endthreadex ((DWORD) status);
#if 0 
  ::ExitThread ((DWORD) status);
#endif 

  /* NOTREACHED */
}

ACE_TSS_Cleanup::ACE_TSS_Cleanup (void)
{
// ACE_TRACE ("ACE_TSS_Cleanup::ACE_TSS_Cleanup");
}

ACE_TSS_Cleanup *
ACE_TSS_Cleanup::instance (void)
{
// ACE_TRACE ("ACE_TSS_Cleanup::instance");

  // Create and initialize thread-specific key.
  if (ACE_TSS_Cleanup::instance_ == 0)
    {
      // Insure that we are serialized!
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_, 0);

      // Now, use the Double-Checked Locking pattern to make sure we
      // only create the key once.
      if (instance_ == 0)
	ACE_NEW_RETURN (ACE_TSS_Cleanup::instance_, ACE_TSS_Cleanup, 0);
    }

  return ACE_TSS_Cleanup::instance_;
}

int 
ACE_TSS_Cleanup::insert (ACE_thread_key_t key, 
			 void (*destructor)(void *),
			 void *inst)
{
// ACE_TRACE ("ACE_TSS_Cleanup::insert");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_, -1);

  return this->table_.insert (ACE_TSS_Info (key, destructor, inst));
}

int
ACE_TSS_Cleanup::remove (ACE_thread_key_t key)
{
// ACE_TRACE ("ACE_TSS_Cleanup::remove");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_, -1);

  return this->table_.remove (ACE_TSS_Info (key));
}

int 
ACE_TSS_Cleanup::detach (void *inst)
{ 
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_, -1);
  
  ACE_TSS_Info *key_info = 0;
  int success = 0;
  int ref_cnt = 0;
  
  for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
       iter.next (key_info) != 0;
       iter.advance ())
    {
      if (key_info->tss_obj_ == inst)
	{
	  key_info->tss_obj_ = 0;
	  ref_cnt = key_info->ref_table_.size ();
	  success = 1;
	  break;
	}
    }
  
  if (success == 0)
    return -1;
  else if (ref_cnt == 0)
    {
      ::TlsFree (key_info->key_);
      return this->table_.remove (ACE_TSS_Info (key_info->key_));
    }

  return 0;
}
  
int 
ACE_TSS_Cleanup::detach (ACE_thread_key_t key, ACE_thread_t tid)
{
  return -1;
}
  
int 
ACE_TSS_Cleanup::key_used (ACE_thread_key_t key)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ACE_TSS_Cleanup::lock_, -1);

  ACE_TSS_Info *key_info = 0;

  for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
       iter.next (key_info) != 0;
       iter.advance ())
    if (key_info->key_ == key)
      return key_info->ref_table_.insert (ACE_TSS_Ref (ACE_OS::thr_self ()));

  return -1;
}
  
void 
ACE_TSS_Cleanup::dump (void)
{
  ACE_TSS_Info *key_info = 0;

  // Iterate through all the thread-specific items and dump them all.

  for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
       iter.next (key_info) != 0;
       iter.advance ())
    key_info->dump ();
}

#endif // WIN32

class ACE_Thread_Adapter
  // = TITLE
  //     Converts a C++ function into a function <ace_thread_adapter>
  //     function that can be called from a thread creation routine
  //     (e.g., pthread_create() or _beginthreadex()) that expects an
  //     extern "C" entry point.
  // 
  // = DESCRIPTION
  //     This is used below in <ACE_OS::thr_create> for Win32 and
  //     MVS. 
{
public:
  ACE_Thread_Adapter (ACE_THR_FUNC f, void *a);
  // Constructor.

  // private:
  // = Arguments to thread startup.
  ACE_THR_FUNC func_;
  // Thread startup function (C++ linkage).

  void *arg_;
  // Argument to thread startup function.
};

// Run the thread exit point.  This must be an extern "C" to make
// certain compilers happy...

extern "C" void *
ace_thread_adapter (void *args)
{
  // ACE_TRACE ("ACE_Thread_Adapter::svc_run");
  ACE_Thread_Adapter *thread_args = (ACE_Thread_Adapter *) args;

  ACE_THR_FUNC func = thread_args->func_;
  void *arg = thread_args->arg_;
  delete thread_args;

#if defined (ACE_WIN32)
  void *status;

  ACE_SEH_TRY {
    status = (*func) (arg);  // Call thread entry point.
  }
  ACE_SEH_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
    ACE_DEBUG ((LM_DEBUG, "(%t) Win32 structured exception exiting outermost svc_run\n"));
    // Here's where we might want to provide a hook to report this...
    // As it stands now, we just catch all Win32 structured exceptions
    // so that we can make sure to clean up correctly when the thread
    // exits.
  }

  // If dropped off end, call destructors for thread-specific storage
  // and exit.
  ACE_TSS_Cleanup::instance ()->exit (status);
  /* NOTREACHED */
  return status;
#else
  return (void *) (*func) (arg);  // Call thread entry point.
#endif /* ACE_WIN32 */
}

ACE_Thread_Adapter::ACE_Thread_Adapter (ACE_THR_FUNC f, void *a)
  : func_(f), 
    arg_(a) 
{
// ACE_TRACE ("ACE_Thread_Adapter::ACE_Thread_Adapter");
}

int
ACE_OS::thr_create (ACE_THR_FUNC func,
		    void *args,
		    long flags,
		    ACE_thread_t *thr_id,
		    ACE_hthread_t *thr_handle,
                    u_int priority,
		    void *stack,
		    size_t stacksize)
{
// ACE_TRACE ("ACE_OS::thr_create");

#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
  int result;
  pthread_attr_t attr;
  ACE_thread_t tmp_thr;
  ACE_thread_t *p_thr;

#if defined (ACE_HAS_SETKIND_NP)
  if (::pthread_attr_create (&attr) != 0)
#else /* ACE_HAS_SETKIND_NP */
  if (::pthread_attr_init (&attr) != 0)
#endif /* ACE_HAS_SETKIND_NP */
    return -1;
#if !defined (ACE_LACKS_SETSCHED)
  else if (priority != 0)
    {
      struct sched_param sparam;

      ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);

#if defined (ACE_HAS_DCETHREADS) && !defined (ACE_HAS_SETKIND_NP)
      sparam.sched_priority = priority > PRIORITY_MAX ? PRIORITY_MAX : priority;
#elif defined(ACE_HAS_IRIX62_THREADS)
      sparam.sched_priority = priority > PTHREAD_MAX_PRIORITY ? PTHREAD_MAX_PRIORITY : priority;
#elif defined (PTHREAD_MAX_PRIORITY) // For MIT pthreads...
      sparam.prio = priority > PTHREAD_MAX_PRIORITY ? PTHREAD_MAX_PRIORITY : priority;
#endif /* ACE_HAS_DCETHREADS */

#if !defined (ACE_HAS_FSU_PTHREADS)
#if defined (ACE_HAS_SETKIND_NP)
      if (::pthread_attr_setsched (&attr, SCHED_OTHER) != 0)
#else /* ACE_HAS_SETKIND_NP */
      if (::pthread_attr_setschedparam (&attr, &sparam) != 0)
#endif /* ACE_HAS_SETKIND_NP */
	{
#if defined (ACE_HAS_SETKIND_NP)
	  ::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
	  ::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
	  return -1;
	}
#else
      if ((sparam.sched_priority >= PTHREAD_MIN_PRIORITY)
	  && (sparam.sched_priority <= PTHREAD_MAX_PRIORITY))
	attr.prio = sparam.sched_priority;
      else
	{
	  pthread_attr_destroy (&attr);
	  return -1;
	}
#endif	/* ACE_HAS_FSU_PTHREADS */
    }
#endif /* ACE_LACKS_SETSCHED */

#if defined (ACE_NEEDS_HUGE_THREAD_STACKSIZE)
    if (stacksize < ACE_NEEDS_HUGE_THREAD_STACKSIZE)
      stacksize = ACE_NEEDS_HUGE_THREAD_STACKSIZE;
#endif /* ACE_NEEDS_HUGE_THREAD_STACKSIZE */

    if (stacksize != 0)
      {
	size_t size = stacksize;

#if defined (PTHREAD_STACK_MIN)
	if (size < PTHREAD_STACK_MIN)
	  size = PTHREAD_STACK_MIN;
#endif /* PTHREAD_STACK_MIN */

	if (::pthread_attr_setstacksize (&attr, size) != 0)
	  {
#if defined (ACE_HAS_SETKIND_NP)
	    ::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
	    ::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
	    return -1;
	  }
      }

#if !defined (ACE_LACKS_THREAD_STACK_ADDR)
    if (stack != 0)
      {
	if (::pthread_attr_setstackaddr (&attr, stack) != 0)
	  {
#if defined (ACE_HAS_SETKIND_NP)
	    ::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
	    ::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
	    return -1;
	  }
      }
#endif /* !ACE_LACKS_THREAD_STACK_ADDR */
    if (flags != 0)
      {
#if !defined (ACE_LACKS_SETDETACH)
	if (ACE_BIT_ENABLED (flags, THR_DETACHED) 
	    || ACE_BIT_ENABLED (flags, THR_JOINABLE))
	  {
	    int dstate = PTHREAD_CREATE_JOINABLE; 

	    if (ACE_BIT_ENABLED (flags, THR_DETACHED))
	      dstate = PTHREAD_CREATE_DETACHED;

#if defined (ACE_HAS_SETKIND_NP)
	    if (::pthread_attr_setdetach_np (&attr, dstate) != 0)
#else /* ACE_HAS_SETKIND_NP */
#if defined (ACE_HAS_PTHREAD_DSTATE_PTR)
	    if (::pthread_attr_setdetachstate (&attr, &dstate) != 0)
#else
	    if (::pthread_attr_setdetachstate (&attr, dstate) != 0)
#endif /* ACE_HAS_PTHREAD_DSTATE_PTR */
#endif /* ACE_HAS_SETKIND_NP */
	      {
#if defined (ACE_HAS_SETKIND_NP)
		::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
		::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
		return -1;
	      }
	  }
#endif /* ACE_LACKS_SETDETACH */
#if !defined (ACE_LACKS_SETSCHED)
	if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO)
	    || ACE_BIT_ENABLED (flags, THR_SCHED_RR)
	    || ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
	  {
	    int spolicy;

	    if (ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
	      spolicy = SCHED_OTHER;
	    else if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
	      spolicy = SCHED_FIFO;
	    else
	      spolicy = SCHED_RR;

#if !defined (ACE_HAS_FSU_PTHREADS)
#if defined (ACE_HAS_SETKIND_NP)
	    if (::pthread_attr_setsched (&attr, spolicy) != 0)
#else /* ACE_HAS_SETKIND_NP */
	    if (::pthread_attr_setschedpolicy (&attr, spolicy) != 0)
#endif /* ACE_HAS_SETKIND_NP */
	      {
#if defined (ACE_HAS_SETKIND_NP)
		::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
		::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
		return -1;
	      }
#else
	    int ret;
	    switch (spolicy)
	      {
	      case SCHED_FIFO:
	      case SCHED_RR:
		ret = 0;
		break;
	      default:
		ret = 22;
		break;
	      }
	    if (ret != 0)
	      {
#if defined (ACE_HAS_SETKIND_NP)
		::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
		::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
		return -1;
	      }
#endif	//  ACE_HAS_FSU_PTHREADS
	  }

	if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED)
	    || ACE_BIT_ENABLED (flags, THR_EXPLICIT_SCHED))
	  {
#if defined (ACE_HAS_SETKIND_NP)
	    int sched = PTHREAD_DEFAULT_SCHED;
#else /* ACE_HAS_SETKIND_NP */
	    int sched = PTHREAD_EXPLICIT_SCHED;
#endif /* ACE_HAS_SETKIND_NP */
	    if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED))
	      sched = PTHREAD_INHERIT_SCHED;
	    if (::pthread_attr_setinheritsched (&attr, sched) != 0)
	      {
#if defined (ACE_HAS_SETKIND_NP)
		::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
		::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
		return -1;
	      }
	  }
#endif /* ACE_LACKS_SETSCHED */
#if !defined (ACE_LACKS_THREAD_PROCESS_SCOPING)
	if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM)
	    || ACE_BIT_ENABLED (flags, THR_SCOPE_PROCESS))
	  {
	    int scope = PTHREAD_SCOPE_PROCESS;
	    if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM))
	      scope = PTHREAD_SCOPE_SYSTEM;

	    if (::pthread_attr_setscope (&attr, scope) != 0)
	      {
#if defined (ACE_HAS_SETKIND_NP)
		::pthread_attr_delete (&attr);
#else /* ACE_HAS_SETKIND_NP */
		::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
		return -1;
	      }
	  }
#endif /* !ACE_LACKS_THREAD_PROCESS_SCOPING */

	if (ACE_BIT_ENABLED (flags, THR_NEW_LWP))
	  {
	    // Increment the number of LWPs by one to emulate the
	    // Solaris semantics.
	    int lwps = ACE_OS::thr_getconcurrency ();
	    ACE_OS::thr_setconcurrency (lwps + 1);
	  }
      }

      p_thr = thr_id == 0 ? &tmp_thr : thr_id;

#if defined (ACE_HAS_SETKIND_NP)
      ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (p_thr, attr, func, args), 
				    result),
		  int, -1, result);
      ::pthread_attr_delete (&attr);
#else /* !ACE_HAS_SETKIND_NP */
#if defined (ACE_HAS_THR_C_FUNC)
      ACE_Thread_Adapter *thread_args;
      ACE_NEW_RETURN (thread_args, ACE_Thread_Adapter (func, args), -1);

      ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (p_thr, &attr,
						      ACE_THR_C_FUNC (&ace_thread_adapter), 
						      thread_args),
				    result),
		  int, -1, result);
#else
      ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (p_thr, &attr, func, args), 
				    result),
		  int, -1, result);
#endif /* ACE_HAS_THR_C_FUNC */
      ::pthread_attr_destroy (&attr);
#endif /* ACE_HAS_SETKIND_NP */
#if defined (ACE_HAS_STHREADS)
      // This is the Solaris implementation of pthreads, where
      // ACE_thread_t and ACE_hthread_t are the same.
      if (result == 0 && thr_handle != 0)
	*thr_handle = thr_id == 0 ? 0 : *thr_id;
#else
      if (thr_handle != 0)
	*thr_handle = ACE_OS::NULL_hthread;
#endif /* ACE_HAS_STHREADS */
      return result;
#elif defined (ACE_HAS_STHREADS)
      int result;
      ACE_OSCALL (ACE_ADAPT_RETVAL (::thr_create (stack, stacksize, func, args,
						  flags, thr_id), result), 
		  int, -1, result);
      if (result == 0 && thr_handle != 0)
	*thr_handle = *thr_id;
      return result;
#elif defined (ACE_HAS_WTHREADS)
      ACE_thread_t t;
      ACE_hthread_t handle;

      if (thr_id == 0)
	thr_id = &t;
      if (thr_handle == 0)
	thr_handle = &handle;

      ACE_Thread_Adapter *thread_args;
      ACE_NEW_RETURN (thread_args, ACE_Thread_Adapter (func, args), -1);
#if defined (ACE_HAS_MFC)
      if (ACE_BIT_ENABLED (flags, THR_USE_AFX))
  	{
  	  CWinThread *cwin_thread = 
  	    ::AfxBeginThread (ACE_THR_C_FUNC (&ace_thread_adapter)),
 			      thread_args, 0, 0, flags | THR_SUSPENDED);
 	  // Have to duplicate the handle because
 	  // CWinThread::~CWinThread() closes the original handle.
 	  *thr_handle = ::DuplicateHandle (::GetCurrentProcess (), 
 					   cwin_thread->m_hThread,
 					   ::GetCurrentProcess (),
 					   thr_handle,
 					   0, 
 					   TRUE,
 					   DUPLICATE_SAME_ACCESS);
 
  	  *thr_id = cwin_thread->m_nThreadID;
 	  if (ACE_BIT_ENABLED (flags, THR_SUSPENDED) == 0)
	    cwin_thread->ResumeThread ();
	      // cwin_thread will be deleted in AfxThreadExit()
	      // Warning: If AfxThreadExit() is called from within the
	      // thread, ACE_TSS_Cleanup->exit() never gets called !

	}
      else
#endif /* ACE_HAS_MFC */
	*thr_handle = (void *) ::_beginthreadex 
	  (NULL,
	   stacksize,
	   ACE_THR_C_FUNC (&ace_thread_adapter),
	   thread_args,
	   flags,
	   (unsigned int *) thr_id);

#if 0
      *thr_handle = ::CreateThread 
	 (NULL, stacksize,
	  LPTHREAD_START_ROUTINE (ACE_THR_C_FUNC (ace_thread_adapter),
	  thread_args, flags, thr_id);
#endif /* 0 */

      // Close down the handle if no one wants to use it.
      if (thr_handle == &handle)
	::CloseHandle (handle);

      if (*thr_handle != 0)
	return 0;
      else
	ACE_FAIL_RETURN (-1);
      /* NOTREACHED */
#elif defined (VXWORKS)
      // If thr_id points to NULL (or is 0), the call below causes
      // VxWorks to assign a unique task name of the form: "t" + an
      // integer.

      // args must be an array of _exactly_ 10 ints.

      // stack arg is ignored:  if there's a need for it, we'd have to use
      // taskInit ()/taskActivate () instead of taskSpawn ()

      // The hard-coded arguments are what ::sp() would use.  ::taskInit()
      // is used instead of ::sp() so that we can set the priority, flags,
      // and stacksize.  (::sp() also hardcodes priority to 100, flags
      // to VX_FP_TASK, and stacksize to 20,000.)  stacksize should be
      // an even integer.

      // if called with thr_create() defaults, use same default values as ::sp()
      if (stacksize == 0) stacksize = 20000;
      if (priority == 0) priority = 100;

      ACE_hthread_t tid = ::taskSpawn (thr_id == 0 ? NULL : *thr_id, priority,
                                       (int) flags, (int) stacksize, func,
                                       ((int *) args)[0], ((int *) args)[1],
                                       ((int *) args)[2], ((int *) args)[3],
                                       ((int *) args)[4], ((int *) args)[5],
                                       ((int *) args)[6], ((int *) args)[7],
                                       ((int *) args)[8], ((int *) args)[9]);

      if (tid == ERROR)
        return -1;
      else
        {
          // return the thr_id and thr_handle, if addresses were provided for them
          if (thr_id != 0)
            // taskTcb (int tid) returns the address of the WIND_TCB
            // (task control block).  According to the taskSpawn()
            // documentation, the name of the new task is stored at
            // pStackBase, but is that of the current task?  If so, it
            // would be a bit quicker than this extraction of the tcb . . .
            *thr_id = taskTcb (tid)->name;
          if (thr_handle != 0)
            *thr_handle = tid;
          return 0;
        }

#endif /* ACE_HAS_STHREADS */
#else
      ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */		
}

void 
ACE_OS::thr_exit (void *status)
{
// ACE_TRACE ("ACE_OS::thr_exit");
#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
  ::pthread_exit (status);
#elif defined (ACE_HAS_STHREADS)
  ::thr_exit (status);
#elif defined (ACE_HAS_WTHREADS)
  // Cleanup the thread-specific resources and exit.
  ACE_TSS_Cleanup::instance ()->exit (status);
#elif defined (VXWORKS)
  ACE_hthread_t tid;
  ACE_OS::thr_self (tid);

  *((int *) status) = ::taskDelete (tid);
#endif /* ACE_HAS_STHREADS */
#else
  ;
#endif /* ACE_HAS_THREADS */		     
}

int 
ACE_OS::thr_setspecific (ACE_thread_key_t key, void *data)
{
// ACE_TRACE ("ACE_OS::thr_setspecific");
#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
#if defined (ACE_HAS_FSU_PTHREADS)
// Call pthread_init() here to initialize threads package.  FSU
// threads need an initialization before the first thread constructor.
// This seems to be the one; however, a segmentation fault may
// indicate that another pthread_init() is necessary, perhaps in
// Synch.cpp or Synch_T.cpp.  FSU threads will not reinit if called
// more than once, so another call to pthread_init will not adversely
// affect existing threads.
  pthread_init ();
#endif 	//  ACE_HAS_FSU_PTHREADS
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setspecific (key, data), ace_result_), 
		     int, -1);
#elif defined (ACE_HAS_STHREADS)
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_setspecific (key, data), ace_result_), int, -1);
#elif defined (ACE_HAS_WTHREADS)
  ::TlsSetValue (key, data);
  ACE_TSS_Cleanup::instance ()->key_used (key);
  return 0;
#elif defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STHREADS */
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int 
ACE_OS::thr_keyfree (ACE_thread_key_t key)
{
// ACE_TRACE ("ACE_OS::thr_keyfree");
#if defined (ACE_HAS_THREADS)
#if defined (ACE_LACKS_KEYDELETE)
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_HAS_PTHREADS) && !defined (ACE_HAS_FSU_PTHREADS)
  return ::pthread_key_delete (key);
#elif defined (ACE_HAS_DCETHREADS) 
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_HAS_STHREADS)
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_HAS_WTHREADS)
  // Extract out the thread-specific table instance and and free up
  // the key and destructor.
  ACE_TSS_Cleanup::instance ()->remove (key);
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::TlsFree (key), ace_result_), int, -1);
#elif defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STHREADS */
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int 
ACE_OS::thr_keycreate (ACE_thread_key_t *key, 
		       ACE_THR_DEST dest,
		       void *inst)
{
// ACE_TRACE ("ACE_OS::thr_keycreate");
  inst = inst;
#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
#if defined (ACE_HAS_SETKIND_NP)
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_keycreate (key, dest), 
                                       ace_result_), 
                     int, -1);
#else /* ACE_HAS_SETKIND_NP */
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest), 
                                       ace_result_), 
                     int, -1);
#endif /* ACE_HAS_SETKIND_NP */
#elif defined (ACE_HAS_STHREADS)
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest), 
				       ace_result_), 
		     int, -1);
#elif defined (ACE_HAS_WTHREADS)
  *key = ::TlsAlloc ();

  if (*key != ACE_SYSCALL_FAILED)
    // Extract out the thread-specific table instance and stash away
    // the key and destructor so that we can free it up later on...
    return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
  else
    ACE_FAIL_RETURN (-1);
    /* NOTREACHED */

#elif defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STHREADS */
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */		     
}

int 
ACE_OS::thr_key_used (ACE_thread_key_t key)
{
#if defined (ACE_WIN32)
  return ACE_TSS_Cleanup::instance ()->key_used (key);
#else
  key = key;
  ACE_NOTSUP_RETURN (-1);  
#endif /* ACE_WIN32 */
}

int 
ACE_OS::thr_key_detach (void *inst)
{
#if defined (ACE_WIN32)
  return ACE_TSS_Cleanup::instance()->detach (inst);
#else
  inst = inst;
  ACE_NOTSUP_RETURN (-1);  
#endif /* ACE_WIN32 */
}

// Create a contiguous command-line argument buffer with each arg
// separated by spaces.

pid_t
ACE_OS::fork_exec (char *argv[])
{
#if defined (ACE_WIN32)
  ACE_ARGV argv_buf (argv);

  char *buf = argv_buf.buf ();

  if (buf != 0)
    {
      PROCESS_INFORMATION process_info;
      STARTUPINFO startup_info;
      ACE_OS::memset ((void *) &startup_info, 0, sizeof startup_info);
      startup_info.cb = sizeof startup_info;

      if (::CreateProcess (NULL,
			   buf,
			   NULL, // No process attributes.
			   NULL,  // No thread attributes.
			   TRUE, // Allow handle inheritance.
			   CREATE_NEW_CONSOLE, // Create a new console window.
			   NULL, // No environment.
			   NULL, // No current directory.
			   &startup_info,
			   &process_info))
	{
	  // Free resources allocated in kernel.
	  ACE_OS::close (process_info.hThread);
	  ACE_OS::close (process_info.hProcess);
	  // Return new process id.
	  return process_info.dwProcessId;
	}
    }

  // CreateProcess failed.
  return -1; 
#else
      pid_t result = ACE_OS::fork ();

      switch (result)
	{
	case -1:
	  // Error.
	  return -1;
	case 0:
	  // Child process.
	  if (ACE_OS::execv (argv[0], argv) == -1)
	    {
	      ACE_ERROR ((LM_ERROR, "%p Exec failed\n"));

	      // If the execv fails, this child needs to exit.
	      ACE_OS::exit (errno);
	    }
	default:
	  // Server process.  The fork succeeded.
	  return result;
	}
#endif /* ACE_WIN32 */
    }

#if defined (ACE_NEEDS_WRITEV)

// "Fake" writev for sites without it.  Note that this is totally
// broken for multi-threaded applications since the <send_n> calls are
// not atomic...

extern "C" int
writev (ACE_HANDLE handle, ACE_WRITEV_TYPE *vp, int vpcount)
{
// ACE_TRACE ("::writev");

  int count;

  for (count = 0; --vpcount >= 0; count += vp->iov_len, vp++)
    if (ACE::send_n (handle, vp->iov_base, vp->iov_len) < 0)
      return -1;

  return count;
}
#endif /* ACE_NEEDS_WRITEV */

#if defined (ACE_NEEDS_READV)

// "Fake" readv for sites without it.  Note that this is totally
// broken for multi-threaded applications since the <send_n> calls are
// not atomic...

extern "C" int
readv (ACE_HANDLE handle, struct iovec *vp, int vpcount)
{
// ACE_TRACE ("::readv");

  int count;

  for (count = 0; --vpcount >= 0; count += vp->iov_len, vp++)
    if (ACE::recv_n (handle, vp->iov_base, vp->iov_len) < 0)
      return -1;

  return count;
}
#endif /* ACE_NEEDS_READV */

#if defined (ACE_NEEDS_FTRUNCATE)
extern "C" int
ftruncate (ACE_HANDLE handle, long len)
{
  struct flock fl;
  fl.l_whence = 0;
  fl.l_len = 0;
  fl.l_start = len;
  fl.l_type = F_WRLCK;

  return ::fcntl (handle, F_FREESP, &fl);
}
#endif /* ACE_NEEDS_FTRUNCATE */

char *
ACE_OS::mktemp (char *s)
{
  // ACE_TRACE ("ACE_OS::mktemp");
#if defined (ACE_LACKS_MKTEMP)
  if (s == 0) 
    // check for null template string failed!
    return 0;
  else
    {
      char *xxxxxx = ACE_OS::strstr (s, "XXXXXX");

      if (xxxxxx == 0)
        // the template string doesn't contain "XXXXXX"!
        return s;
      else
        {
          char unique_letter = 'a';
          struct stat sb;

          // Find an unused filename for this process.  It is assumed
          // that the user will open the file immediately after
          // getting this filename back (so, yes, there is a race
          // condition if multiple threads in a process use the same
          // template).  This appears to match the behavior of the
          // Solaris 2.5 mktemp().
          ::sprintf (xxxxxx, "%05d%c", getpid (), unique_letter);
          while (::stat (s, &sb) >= 0)
            {
	      if (++unique_letter <= 'z')
		::sprintf (xxxxxx, "%05d%c", getpid (), unique_letter);
	      else
		{
		  // maximum of 26 unique files per template, per process
		  ::sprintf (xxxxxx, "%s", "");
		  return s;
		}
            }
        }
      return s;
    }
  
#else
  return ::mktemp (s);
#endif /* ACE_LACKS_MKTEMP */
}

int
ACE_OS::socket_init (int version_high, int version_low)
{
#if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ == 0)
    {
      WORD version_requested = MAKEWORD (version_high, version_low);
      WSADATA wsa_data;
      int error = ::WSAStartup (version_requested, &wsa_data);

      if (error != 0)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "WSAStartup failed, WSAGetLastError returned %u.\n",
			   error), -1);

      ACE_OS::socket_initialized_ = 1;
    }
#else
  version_high = version_high;
  version_low = version_low;
#endif /* ACE_WIN32 */
  return 0;
}

int
ACE_OS::socket_fini (void)
{
#if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ != 0)
    {
      if (::WSACleanup () != 0)
	{
	  int error = ::WSAGetLastError ();
	  ACE_ERROR_RETURN ((LM_ERROR, 
			     "WSACleanup failed, WSAGetLastError returned %u.\n", 
			     error), -1);
	}
      ACE_OS::socket_initialized_ = 0;
    }
#endif /* ACE_WIN32 */
  return 0;
}

#if defined (VXWORKS)
int sys_nerr = ERRMAX + 1;

#endif /* VXWORKS */

#if !defined (ACE_HAS_SIGINFO_T)
siginfo_t::siginfo_t (ACE_HANDLE handle)
  : si_handle_ (handle) 
{
}
#endif /* ACE_HAS_SIGINFO_T */

// This is necessary to work around nasty problems with MVS C++.

extern "C" void
ace_mutex_lock_cleanup_adapter (void *args)
{
  ACE_OS::mutex_lock_cleanup (args);
}
