// -*- C++ -*-
// $Id$

#include "ace/OS_NS_Thread.h"

ACE_RCSID(ace, OS_NS_Thread, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_Thread.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#include "ace/OS_NS_stdio.h"
#include "ace/Sched_Params.h"
#include "ace/OS_Memory.h"
#include "ace/OS_Thread_Adapter.h"
#include "ace/Min_Max.h"
#include "ace/OS_NS_errno.h"

// This is necessary to work around nasty problems with MVS C++.

extern "C" void
ace_mutex_lock_cleanup_adapter (void *args)
{
  ACE_OS::mutex_lock_cleanup (args);
}

/*****************************************************************************/

ACE_Thread_ID::ACE_Thread_ID (ACE_thread_t thread_id,
                              ACE_hthread_t thread_handle)
  : thread_id_ (thread_id),
    thread_handle_ (thread_handle)
{
}

ACE_Thread_ID::ACE_Thread_ID (const ACE_Thread_ID &id)
  : thread_id_ (id.thread_id_),
    thread_handle_ (id.thread_handle_)
{
}

ACE_thread_t
ACE_Thread_ID::id (void)
{
  return this->thread_id_;
}

void
ACE_Thread_ID::id (ACE_thread_t thread_id)
{
  this->thread_id_ = thread_id;
}

ACE_hthread_t
ACE_Thread_ID::handle (void)
{
  return this->thread_handle_;
}

void
ACE_Thread_ID::handle (ACE_hthread_t thread_handle)
{
  this->thread_handle_ = thread_handle;
}

void
ACE_Thread_ID::to_string (char* thr_id)
{

  char format[128]; // Converted format string
  char *fp;         // Current format pointer
  fp = format;
  *fp++ = '%';   // Copy in the %

#if defined (ACE_WIN32)
  ACE_OS::strcpy (fp, "u");
  ACE_OS::sprintf (thr_id,
		   format,
		   ACE_static_cast(unsigned,
				   ACE_OS::thr_self ()));
#elif defined (ACE_AIX_VERS) && (ACE_AIX_VERS <= 402)
                  // AIX's pthread_t (ACE_hthread_t) is a pointer, and it's
                  // a little ugly to send that through a %u format.  So,
                  // get the kernel thread ID (tid_t) via thread_self() and
                  // display that instead.
                  // This isn't conditionalized on ACE_HAS_THREAD_SELF because
                  // 1. AIX 4.2 doesn't have that def anymore (it messes up
                  //    other things)
                  // 2. OSF/1 V3.2 has that def, and I'm not sure what affect
                  //   this would have on that.
                  // -Steve Huston, 19-Aug-97
                  ACE_OS::strcpy (fp, "u");
                  ACE_OS::sprintf (thr_id, format, thread_self());
#elif defined (DIGITAL_UNIX)
                  ACE_OS::strcpy (fp, "u");
                  ACE_OS::sprintf (thr_id, format,
#  if defined (ACE_HAS_THREADS)
                                   pthread_getselfseq_np ()
#  else
                                   ACE_Thread::self ()
#  endif /* ACE_HAS_THREADS */
                                          );
#else
                  ACE_hthread_t t_id;
                  ACE_OS::thr_self (t_id);

#  if defined (ACE_HAS_PTHREADS_DRAFT4) && defined (HPUX_10)
                  ACE_OS::strcpy (fp, "u");
                  // HP-UX 10.x DCE's thread ID is a pointer.  Grab the
                  // more meaningful, readable, thread ID.  This will match
                  // the one seen in the debugger as well.
                  ACE_OS::sprintf (thr_id, format,
                                   pthread_getunique_np(&t_id));
#  elif defined (ACE_MVS)
                  // MVS's pthread_t is a struct... yuck. So use the ACE 5.0
                  // code for it.
                  ACE_OS::strcpy (fp, "u");
                  ACE_OS::sprintf (thr_id, format, t_id);
#  else
                  // Yes, this is an ugly C-style cast, but the correct
                  // C++ cast is different depending on whether the t_id
                  // is an integral type or a pointer type. FreeBSD uses
                  // a pointer type, but doesn't have a _np function to
                  // get an integral type, like the OSes above.
                  ACE_OS::strcpy (fp, "lu");
                  ACE_OS::sprintf (thr_id, format, (unsigned long)t_id);
#  endif /* ACE_HAS_PTHREADS_DRAFT4 && HPUX_10 */

#endif /* ACE_WIN32 */
}

int
ACE_Thread_ID::operator== (const ACE_Thread_ID &rhs) const
{
  return ACE_OS::thr_cmp (this->thread_handle_, rhs.thread_handle_) == 0
    && ACE_OS::thr_equal (this->thread_id_, rhs.thread_id_) == 0;
}

int
ACE_Thread_ID::operator!= (const ACE_Thread_ID &rhs) const
{
  return !(*this == rhs);
}

/*****************************************************************************/

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))

#if defined (ACE_HAS_TSS_EMULATION)
u_int ACE_TSS_Emulation::total_keys_ = 0;

ACE_TSS_Keys ACE_TSS_Emulation::tss_keys_used_;

ACE_TSS_Emulation::ACE_TSS_DESTRUCTOR
ACE_TSS_Emulation::tss_destructor_[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX]
 = { 0 };

#  if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)

int ACE_TSS_Emulation::key_created_ = 0;

ACE_OS_thread_key_t ACE_TSS_Emulation::native_tss_key_;

/* static */
#    if defined (ACE_HAS_THR_C_FUNC)
extern "C"
void
ACE_TSS_Emulation_cleanup (void *ptr)
{
   ACE_UNUSED_ARG (ptr);
   // Really this must be used for ACE_TSS_Emulation code to make the TSS
   // cleanup
}
#    else
void
ACE_TSS_Emulation_cleanup (void *ptr)
{
   ACE_UNUSED_ARG (ptr);
   // Really this must be used for ACE_TSS_Emulation code to make the TSS
   // cleanup
}
#    endif /* ACE_HAS_THR_C_FUNC */

void **
ACE_TSS_Emulation::tss_base (void* ts_storage[], u_int *ts_created)
{
  // TSS Singleton implementation.

  // Create the one native TSS key, if necessary.
  if (key_created_ == 0)
    {
      // Double-checked lock . . .
      ACE_TSS_BASE_GUARD

      if (key_created_ == 0)
        {
          ACE_NO_HEAP_CHECK;
          if (ACE_OS::thr_keycreate (&native_tss_key_,
                                     &ACE_TSS_Emulation_cleanup) != 0)
            {
              return 0; // Major problems, this should *never* happen!
            }
          key_created_ = 1;
        }
    }

  void **old_ts_storage = 0;

  // Get the tss_storage from thread-OS specific storage.
  if (ACE_OS::thr_getspecific (native_tss_key_,
                               (void **) &old_ts_storage) == -1)
    return 0; // This should not happen!

  // Check to see if this is the first time in for this thread.
  // This block can also be entered after a fork () in the child process,
  // at least on Pthreads Draft 4 platforms.
  if (old_ts_storage == 0)
    {
      if (ts_created)
        *ts_created = 1u;

      // Use the ts_storage passed as argument, if non-zero.  It is
      // possible that this has been implemented in the stack. At the
      // moment, this is unknown.  The cleanup must not do nothing.
      // If ts_storage is zero, allocate (and eventually leak) the
      // storage array.
      if (ts_storage == 0)
        {
          ACE_NO_HEAP_CHECK;

          ACE_NEW_RETURN (ts_storage,
                          void*[ACE_TSS_THREAD_KEYS_MAX],
                          0);

          // Zero the entire TSS array.  Do it manually instead of
          // using memset, for optimum speed.  Though, memset may be
          // faster :-)
          void **tss_base_p = ts_storage;

          for (u_int i = 0;
               i < ACE_TSS_THREAD_KEYS_MAX;
               ++i)
            *tss_base_p++ = 0;
        }

       // Store the pointer in thread-specific storage.  It gets
       // deleted via the ACE_TSS_Emulation_cleanup function when the
       // thread terminates.
       if (ACE_OS::thr_setspecific (native_tss_key_,
                                    (void *) ts_storage) != 0)
          return 0; // Major problems, this should *never* happen!
    }
  else
    if (ts_created)
      ts_created = 0;

  return ts_storage  ?  ts_storage  :  old_ts_storage;
}
#  endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */

u_int
ACE_TSS_Emulation::total_keys ()
{
  ACE_OS_Recursive_Thread_Mutex_Guard (
    *ACE_static_cast (ACE_recursive_thread_mutex_t *,
                      ACE_OS_Object_Manager::preallocated_object[
                        ACE_OS_Object_Manager::ACE_TSS_KEY_LOCK]));

  return total_keys_;
}

int
ACE_TSS_Emulation::next_key (ACE_thread_key_t &key)
{
  ACE_OS_Recursive_Thread_Mutex_Guard (
    *ACE_static_cast (ACE_recursive_thread_mutex_t *,
                      ACE_OS_Object_Manager::preallocated_object[
                        ACE_OS_Object_Manager::ACE_TSS_KEY_LOCK]));

  if (total_keys_ < ACE_TSS_THREAD_KEYS_MAX)
    {
       u_int counter = 0;
       // Loop through all possible keys and check whether a key is free
       for ( ;counter < ACE_TSS_THREAD_KEYS_MAX; counter++)
         {
            ACE_thread_key_t localkey;
#  if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
              ACE_OS::memset (&localkey, 0, sizeof (ACE_thread_key_t));
              ACE_OS::memcpy (&localkey, &counter_, sizeof (u_int));
#  else
              localkey = counter;
#  endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */
            // If the key is not set as used, we can give out this key, if not
            // we have to search further
            if (tss_keys_used_.is_set(localkey) == 0)
            {
               tss_keys_used_.test_and_set(localkey);
               key = localkey;
               break;
            }
         }

      ++total_keys_;
      return 0;
    }
  else
    {
      key = ACE_OS::NULL_key;
      return -1;
    }
}

int
ACE_TSS_Emulation::release_key (ACE_thread_key_t key)
{
  ACE_OS_Recursive_Thread_Mutex_Guard (
    *ACE_static_cast (ACE_recursive_thread_mutex_t *,
                      ACE_OS_Object_Manager::preallocated_object[
                        ACE_OS_Object_Manager::ACE_TSS_KEY_LOCK]));

  if (tss_keys_used_.test_and_clear (key) == 0)
  {
    --total_keys_;
    return 0;
  }
  return 1;
}

void *
ACE_TSS_Emulation::tss_open (void *ts_storage[ACE_TSS_THREAD_KEYS_MAX])
{
#  if defined (ACE_PSOS)
  u_long tss_base;

  // Use the supplied array for this thread's TSS.
  tss_base = (u_long) ts_storage;
  t_setreg (0, PSOS_TASK_REG_TSS, tss_base);

  // Zero the entire TSS array.
  void **tss_base_p = ts_storage;
  for (u_int i = 0; i < ACE_TSS_THREAD_KEYS_MAX; ++i, ++tss_base_p)
    {
      *tss_base_p = 0;
    }

  return (void *) tss_base;
#  else  /* ! ACE_PSOS */
#    if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
        // On VxWorks, in particular, don't check to see if the field
        // is 0.  It isn't always, specifically, when a program is run
        // directly by the shell (without spawning a new task) after
        // another program has been run.

  u_int ts_created = 0;
  tss_base (ts_storage, &ts_created);
  if (ts_created)
    {
#    else  /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */
      tss_base () = ts_storage;
#    endif

      // Zero the entire TSS array.  Do it manually instead of using
      // memset, for optimum speed.  Though, memset may be faster :-)
      void **tss_base_p = tss_base ();
      for (u_int i = 0; i < ACE_TSS_THREAD_KEYS_MAX; ++i, ++tss_base_p)
        {
          *tss_base_p = 0;
        }

      return tss_base ();
#    if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
    }
  else
    {
      return 0;
    }
#    endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
#  endif /* ! ACE_PSOS */
}

void
ACE_TSS_Emulation::tss_close ()
{
#  if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  // Free native_tss_key_ here.
#  endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
}

#endif /* ACE_HAS_TSS_EMULATION */

#endif /* WIN32 || ACE_HAS_TSS_EMULATION */

/*****************************************************************************/

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))

// Moved class ACE_TSS_Ref declaration to OS.h so it can be visible to
// the single file of template instantiations.

ACE_TSS_Ref::ACE_TSS_Ref (ACE_thread_t id)
  : tid_(id)
{
  ACE_OS_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

ACE_TSS_Ref::ACE_TSS_Ref (void)
{
  ACE_OS_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

// Check for equality.
int
ACE_TSS_Ref::operator== (const ACE_TSS_Ref &info) const
{
  ACE_OS_TRACE ("ACE_TSS_Ref::operator==");

  return this->tid_ == info.tid_;
}

// Check for inequality.
ACE_SPECIAL_INLINE
int
ACE_TSS_Ref::operator != (const ACE_TSS_Ref &tss_ref) const
{
  ACE_OS_TRACE ("ACE_TSS_Ref::operator !=");

  return !(*this == tss_ref);
}

// moved class ACE_TSS_Info declaration
// to OS.h so it can be visible to the
// single file of template instantiations

ACE_TSS_Info::ACE_TSS_Info (ACE_thread_key_t key,
                            void (*dest)(void *),
                            void *tss_inst)
  : key_ (key),
    destructor_ (dest),
    tss_obj_ (tss_inst),
    thread_count_ (-1)
{
  ACE_OS_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

ACE_TSS_Info::ACE_TSS_Info (void)
  : key_ (ACE_OS::NULL_key),
    destructor_ (0),
    tss_obj_ (0),
    thread_count_ (-1)
{
  ACE_OS_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

# if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
static inline int operator== (const ACE_thread_key_t &lhs,
                              const ACE_thread_key_t &rhs)
{
  return ! ACE_OS::memcmp (&lhs, &rhs, sizeof (ACE_thread_key_t));
}

static inline int operator!= (const ACE_thread_key_t &lhs,
                              const ACE_thread_key_t &rhs)
{
  return ! (lhs == rhs);
}
# endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */

// Check for equality.
int
ACE_TSS_Info::operator== (const ACE_TSS_Info &info) const
{
  ACE_OS_TRACE ("ACE_TSS_Info::operator==");

  return this->key_ == info.key_;
}

// Check for inequality.
int
ACE_TSS_Info::operator != (const ACE_TSS_Info &info) const
{
  ACE_OS_TRACE ("ACE_TSS_Info::operator !=");

  return !(*this == info);
}

void
ACE_TSS_Info::dump (void)
{
# if defined (ACE_HAS_DUMP)
  //  ACE_OS_TRACE ("ACE_TSS_Info::dump");

#   if 0
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("key_ = %u\n"), this->key_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("destructor_ = %u\n"), this->destructor_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("tss_obj_ = %u\n"), this->tss_obj_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#   endif /* 0 */
# endif /* ACE_HAS_DUMP */
}

// Moved class ACE_TSS_Keys declaration to OS.h so it can be visible
// to the single file of template instantiations.

ACE_TSS_Keys::ACE_TSS_Keys (void)
{
  for (u_int i = 0; i < ACE_WORDS; ++i)
    {
      key_bit_words_[i] = 0;
    }
}

ACE_SPECIAL_INLINE
void
ACE_TSS_Keys::find (const u_int key, u_int &word, u_int &bit)
{
  word = key / ACE_BITS_PER_WORD;
  bit = key % ACE_BITS_PER_WORD;
}

int
ACE_TSS_Keys::test_and_set (const ACE_thread_key_t key)
{
  ACE_KEY_INDEX (key_index, key);
  u_int word, bit;
  find (key_index, word, bit);

  if (ACE_BIT_ENABLED (key_bit_words_[word], 1 << bit))
    {
      return 1;
    }
  else
    {
      ACE_SET_BITS (key_bit_words_[word], 1 << bit);
      return 0;
    }
}

int
ACE_TSS_Keys::test_and_clear (const ACE_thread_key_t key)
{
  ACE_KEY_INDEX (key_index, key);
  u_int word, bit;
  find (key_index, word, bit);

  if (ACE_BIT_ENABLED (key_bit_words_[word], 1 << bit))
    {
      ACE_CLR_BITS (key_bit_words_[word], 1 << bit);
      return 0;
    }
  else
    {
      return 1;
    }
}

int
ACE_TSS_Keys::is_set (const ACE_thread_key_t key) const
{
  ACE_KEY_INDEX (key_index, key);
  u_int word, bit;
  find (key_index, word, bit);

  return ACE_BIT_ENABLED (key_bit_words_[word], 1 << bit);
}

/*****************************************************************************/

/**
 * @class ACE_TSS_Cleanup
 *
 * @brief Singleton that knows how to clean up all the thread-specific
 * resources for Win32.
 *
 * All this nonsense is required since Win32 doesn't
 * automatically cleanup thread-specific storage on thread exit,
 * unlike real operating systems... ;-)
 */
class ACE_TSS_Cleanup
{
public:
  static ACE_TSS_Cleanup *instance (void);

  ~ACE_TSS_Cleanup (void);

  /// Cleanup the thread-specific objects.  Does _NOT_ exit the thread.
  void exit (void *status);

  /// Insert a <key, destructor> tuple into the table.
  int insert (ACE_thread_key_t key, void (*destructor)(void *), void *inst);

  /// Remove a <key, destructor> tuple from the table.
  int remove (ACE_thread_key_t key);

  /// Detaches a tss_instance from its key.
  int detach (void *inst);

  /// Mark a key as being used by this thread.
  void key_used (ACE_thread_key_t key);

  /// Free all keys left in the table before destruction.
  int free_all_keys_left (void);

  /// Indication of whether the ACE_TSS_CLEANUP_LOCK is usable, and
  /// therefore whether we are in static constructor/destructor phase
  /// or not.
  static int lockable () { return instance_ != 0; }

protected:
  void dump (void);

  /// Ensure singleton.
  ACE_TSS_Cleanup (void);

private:
  // Array of <ACE_TSS_Info> objects.
  typedef ACE_TSS_Info ACE_TSS_TABLE[ACE_DEFAULT_THREAD_KEYS];
  typedef ACE_TSS_Info *ACE_TSS_TABLE_ITERATOR;

  /// Table of <ACE_TSS_Info>'s.
  ACE_TSS_TABLE table_;

  /// Key for the thread-specific array of whether each TSS key is in use.
  ACE_thread_key_t in_use_;

  /// Accessor for this threads ACE_TSS_Keys instance.
  ACE_TSS_Keys *tss_keys ();

# if defined (ACE_HAS_TSS_EMULATION)
  /// Key that is used by in_use_.  We save this key so that we know
  /// not to call its destructor in free_all_keys_left ().
  ACE_thread_key_t in_use_key_;
# endif /* ACE_HAS_TSS_EMULATION */

  // = Static data.
  /// Pointer to the singleton instance.
  static ACE_TSS_Cleanup *instance_;
};

// = Static object initialization.

// Pointer to the singleton instance.
ACE_TSS_Cleanup *ACE_TSS_Cleanup::instance_ = 0;

ACE_TSS_Cleanup::~ACE_TSS_Cleanup (void)
{
  // Zero out the instance pointer to support lockable () accessor.
  ACE_TSS_Cleanup::instance_ = 0;
}

void
ACE_TSS_Cleanup::exit (void * /* status */)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::exit");

  ACE_TSS_TABLE_ITERATOR key_info = table_;
  ACE_TSS_Info info_arr[ACE_DEFAULT_THREAD_KEYS];
  int info_ix = 0;

  // While holding the lock, we only collect the ACE_TSS_Info objects
  // in an array without invoking the according destructors.
  {
    ACE_TSS_CLEANUP_GUARD

    // Iterate through all the thread-specific items and free them all
    // up.

    for (unsigned int i = 0;
         i < ACE_DEFAULT_THREAD_KEYS;
         ++key_info, ++i)
      {
        if (key_info->key_ == ACE_OS::NULL_key  ||
            ! key_info->key_in_use ()) continue;

        // If the key's ACE_TSS_Info in-use bit for this thread was set,
        // unset it and decrement the key's thread_count_.
        if (! tss_keys ()->test_and_clear (key_info->key_))
          {
            --key_info->thread_count_;
          }

        void *tss_info = 0;

        if (key_info->destructor_
            && ACE_OS::thr_getspecific (key_info->key_, &tss_info) == 0
            && tss_info)
          {
            info_arr[info_ix].key_ = key_info->key_;
            info_arr[info_ix].destructor_ = key_info->destructor_;
            info_arr[info_ix++].tss_obj_ = key_info->tss_obj_;
          }
      }
  }

  // Now we have given up the ACE_TSS_Cleanup::lock_ and we start
  // invoking destructors, in the reverse order of creation.
  for (int i = info_ix - 1; i >= 0; --i)
    {
      void *tss_info = 0;

      ACE_OS::thr_getspecific (info_arr[i].key_, &tss_info);

      if (tss_info != 0)
        {
          // Only call the destructor if the value is non-zero for this
          // thread.
          (*info_arr[i].destructor_)(tss_info);
        }
    }

  // Acquire the ACE_TSS_CLEANUP_LOCK, then free TLS keys and remove
  // entries from ACE_TSS_Info table.
  {
    ACE_TSS_CLEANUP_GUARD

# if 0
    // We shouldn't free the key and remove it from the table here
    // because if we do and some thread ends before other threads
    // even get started (or their TSS object haven't been created yet,)
    // it's entry will be removed from the table and we are in big chaos.
    // For TSS object, these have been done in ACE_TSS_Cleanup::detach.
    // Two other use cases will be user managed TSS'es and system wide
    // TSS, ones are users responsibilities and the others should be
    // persistant system wide.
    for (int i = 0; i < index; i++)
      {
#   if defined (ACE_WIN32) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
        // Calling thr_keyfree here ensure the key
        // gets removed appropriately.  Notice that
        // a key should be removed before freeing it.
        ACE_OS::thr_keyfree (key_info->key_);
#   else
        // don't bother to free the key
        this->remove (key_info->key_);
#   endif /* ACE_WIN32 */
      }
# endif /* 0 */
  }
}

int
ACE_TSS_Cleanup::free_all_keys_left (void)
  // This is called from ACE_OS::cleanup_tss ().  When this gets
  // called, all threads should have exited except the main thread.
  // No key should be freed from this routine.  It there's any,
  // something might be wrong.
{
  ACE_thread_key_t key_arr[ACE_DEFAULT_THREAD_KEYS];
  ACE_TSS_TABLE_ITERATOR key_info = table_;
  unsigned int idx = 0;
  unsigned int i;

  for (i = 0;
       i < ACE_DEFAULT_THREAD_KEYS;
       ++key_info, ++i)
# if defined (ACE_HAS_TSS_EMULATION)
    if (key_info->key_ != in_use_key_)
# endif /* ACE_HAS_TSS_EMULATION */
      // Don't call ACE_OS::thr_keyfree () on ACE_TSS_Cleanup's own
      // key.  See the comments in ACE_OS::thr_key_detach ():  the key
      // doesn't get detached, so it will be in the table here.
      // However, there's no resource associated with it, so we don't
      // need to keyfree it.  The dynamic memory associated with it
      // was already deleted by ACE_TSS_Cleanup::exit (), so we don't
      // want to access it again.
      key_arr [idx++] = key_info->key_;

  for (i = 0; i < idx; i++)
    if (key_arr[i] != ACE_OS::NULL_key)
# if defined (ACE_HAS_TSS_EMULATION)
      ACE_OS::thr_keyfree (key_arr[i]);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
      // Don't call ACE_OS::thr_keyfree here.  It will try to use
      // <in_use_> which has already been cleaned up here.
      ::tsd_delete (key_arr[i]);
# else /* ACE_WIN32 */
      // Don't call ACE_OS::thr_keyfree here.  It will try to use
      // <in_use_> which has already been cleaned up here.
      TlsFree (key_arr[i]);
# endif /* ACE_HAS_TSS_EMULATION */

  return 0;
}

extern "C" void
ACE_TSS_Cleanup_keys_destroyer (void *tss_keys)
{
  delete ACE_reinterpret_cast (ACE_TSS_Keys *, tss_keys);
}

ACE_TSS_Cleanup::ACE_TSS_Cleanup (void)
  : in_use_ (ACE_OS::NULL_key)
# if defined (ACE_HAS_TSS_EMULATION)
    // ACE_TSS_Emulation::total_keys () provides the value of the next
    // key to be created.
  , in_use_key_ (ACE_TSS_Emulation::total_keys ())
# endif /* ACE_HAS_TSS_EMULATION */
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::ACE_TSS_Cleanup");
}

ACE_TSS_Cleanup *
ACE_TSS_Cleanup::instance (void)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::instance");

  // Create and initialize thread-specific key.
  if (ACE_TSS_Cleanup::instance_ == 0)
    {
      // Insure that we are serialized!
      ACE_TSS_CLEANUP_GUARD

      // Now, use the Double-Checked Locking pattern to make sure we
      // only create the ACE_TSS_Cleanup instance once.
      if (ACE_TSS_Cleanup::instance_ == 0)
        ACE_NEW_RETURN (ACE_TSS_Cleanup::instance_,
                        ACE_TSS_Cleanup,
                        0);
    }

  return ACE_TSS_Cleanup::instance_;
}

int
ACE_TSS_Cleanup::insert (ACE_thread_key_t key,
                         void (*destructor)(void *),
                         void *inst)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::insert");
  ACE_TSS_CLEANUP_GUARD

  ACE_KEY_INDEX (key_index, key);
  if (key_index < ACE_DEFAULT_THREAD_KEYS)
    {
      table_[key_index] = ACE_TSS_Info (key, destructor, inst);
      return 0;
    }
  else
    {
      return -1;
    }
}

int
ACE_TSS_Cleanup::remove (ACE_thread_key_t key)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::remove");
  ACE_TSS_CLEANUP_GUARD

  ACE_KEY_INDEX (key_index, key);
  if (key_index < ACE_DEFAULT_THREAD_KEYS)
    {
      // "Remove" the TSS_Info table entry by zeroing out its key_ and
      // destructor_ fields.  Also, keep track of the number threads
      // using the key.
      ACE_TSS_Info &info = this->table_ [key_index];

      // Don't bother to test/clear the in "use bit" if the program is
      // shutting down.  Doing so will cause a new ACE_TSS object to be
      // created again.
      if (!ACE_OS_Object_Manager::shutting_down ())
        tss_keys ()->test_and_clear (info.key_);
      info.key_in_use (0);
      info.key_ = ACE_OS::NULL_key;
      info.destructor_ = 0;
      return 0;
    }
  else
    return -1;
}

int
ACE_TSS_Cleanup::detach (void *inst)
{
  ACE_TSS_CLEANUP_GUARD

  ACE_TSS_TABLE_ITERATOR key_info = table_;
  int success = 0;
  int ref_cnt = 0;

  // Mark the key as detached in the TSS_Info table.
  // It only works for the first key that "inst" owns.
  // I don't know why.
  for (unsigned int i = 0;
       i < ACE_DEFAULT_THREAD_KEYS;
       ++key_info, ++i)
    {
      if (key_info->tss_obj_ == inst)
        {
          key_info->tss_obj_ = 0;
          ref_cnt = --key_info->thread_count_;
          success = 1;
          break;
        }
    }

  if (success == 0)
    return -1;
  else if (ref_cnt == 0)
    {
      // Mark the key as no longer being used.
      key_info->key_in_use (0);
# if defined (ACE_WIN32) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
      ACE_thread_key_t temp_key = key_info->key_;
# endif /* ACE_WIN32 */
      int retv = this->remove (key_info->key_);

# if defined (ACE_WIN32)
      ::TlsFree (temp_key);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
      ::tsd_delete (temp_key);
# endif /* ACE_WIN32 */
      return retv;
    }

  return 0;
}

void
ACE_TSS_Cleanup::key_used (ACE_thread_key_t key)
{
  // If the key's ACE_TSS_Info in-use bit for this thread is not set,
  // set it and increment the key's thread_count_.
  if (! tss_keys ()->test_and_set (key))
    {
      ACE_TSS_CLEANUP_GUARD

      // Retrieve the key's ACE_TSS_Info and increment its thread_count_.
      ACE_KEY_INDEX (key_index, key);
      ACE_TSS_Info &key_info = this->table_ [key_index];
      if (!key_info.key_in_use ())
        key_info.key_in_use (1);
      else
        ++key_info.thread_count_;
    }
}

void
ACE_TSS_Cleanup::dump (void)
{
# if defined (ACE_HAS_DUMP)
  // Iterate through all the thread-specific items and dump them all.

  ACE_TSS_TABLE_ITERATOR key_info = table_;
  for (unsigned int i = 0;
       i < ACE_DEFAULT_THREAD_KEYS;
       ++key_info, ++i)
    key_info->dump ();
# endif /* ACE_HAS_DUMP */
}

ACE_TSS_Keys *
ACE_TSS_Cleanup::tss_keys ()
{
  if (in_use_ == ACE_OS::NULL_key)
    {
      ACE_TSS_CLEANUP_GUARD
      // Double-check;
      if (in_use_ == ACE_OS::NULL_key)
        {
          // Initialize in_use_ with a new key.
          if (ACE_OS::thr_keycreate (&in_use_,
                                     &ACE_TSS_Cleanup_keys_destroyer))
            return 0; // Major problems, this should *never* happen!
        }
    }

  ACE_TSS_Keys *ts_keys = 0;
  if (ACE_OS::thr_getspecific (in_use_,
        ACE_reinterpret_cast (void **, &ts_keys)) == -1)
    return 0; // This should not happen!

  if (ts_keys == 0)
    {
      ACE_NEW_RETURN (ts_keys,
                      ACE_TSS_Keys,
                      0);
      // Store the dynamically allocated pointer in thread-specific
      // storage.
      if (ACE_OS::thr_setspecific (in_use_,
            ACE_reinterpret_cast (void *,
                                  ts_keys)) == -1)
        {
          delete ts_keys;
          return 0; // Major problems, this should *never* happen!
        }
    }

  return ts_keys;
}

#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION || (ACE_PSOS && ACE_PSOS_HAS_TSS) */

/*****************************************************************************/

// = Static initialization.

// This is necessary to deal with POSIX pthreads insanity.  This
// guarantees that we've got a "zero'd" thread id even when
// ACE_thread_t, ACE_hthread_t, and ACE_thread_key_t are implemented
// as structures...  Under no circumstances should these be given
// initial values.
// Note: these three objects require static construction.
ACE_thread_t ACE_OS::NULL_thread;
ACE_hthread_t ACE_OS::NULL_hthread;
#if defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  ACE_thread_key_t ACE_OS::NULL_key = ACE_static_cast (ACE_thread_key_t, -1);
#else  /* ! ACE_HAS_TSS_EMULATION */
  ACE_thread_key_t ACE_OS::NULL_key;
#endif /* ! ACE_HAS_TSS_EMULATION */

#if defined (CHORUS)
KnCap ACE_OS::actorcaps_[ACE_CHORUS_MAX_ACTORS];
// This is used to map an actor's id into a KnCap for killing and
// waiting actors.
#endif /* CHORUS */

/*****************************************************************************/

void
ACE_OS::cleanup_tss (const u_int main_thread)
{
#if defined (ACE_HAS_TSS_EMULATION) || defined (ACE_WIN32) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  // Call TSS destructors for current thread.
  ACE_TSS_Cleanup::instance ()->exit (0);
#endif /* ACE_HAS_TSS_EMULATION || ACE_WIN32 || ACE_PSOS_HAS_TSS */

  if (main_thread)
    {
#if !defined (ACE_HAS_TSS_EMULATION)  &&  !defined (ACE_HAS_MINIMAL_ACE_OS)
      // Just close the ACE_Log_Msg for the current (which should be
      // main) thread.  We don't have TSS emulation; if there's native
      // TSS, it should call its destructors when the main thread
      // exits.
      ACE_Base_Thread_Adapter::close_log_msg ();
#endif /* ! ACE_HAS_TSS_EMULATION  &&  ! ACE_HAS_MINIMAL_ACE_OS */

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
# if ! defined (ACE_HAS_TSS_EMULATION)
      // Don't do this with TSS_Emulation, because the the
      // ACE_TSS_Cleanup::instance () has already exited ().  We can't
      // safely access the TSS values that were created by the main
      // thread.

      // Remove all TSS_Info table entries.
      ACE_TSS_Cleanup::instance ()->free_all_keys_left ();
# endif /* ! ACE_HAS_TSS_EMULATION */

      // Finally, free up the ACE_TSS_Cleanup instance.  This method gets
      // called by the ACE_Object_Manager.
      delete ACE_TSS_Cleanup::instance ();
#endif /* WIN32 || ACE_HAS_TSS_EMULATION || ACE_PSOS_HAS_TSS */

#if defined (ACE_HAS_TSS_EMULATION)
      ACE_TSS_Emulation::tss_close ();
#endif /* ACE_HAS_TSS_EMULATION */
    }
}

#if defined (ACE_LACKS_COND_T) && ! defined (ACE_PSOS_DIAB_MIPS)
// NOTE: The ACE_OS::cond_* functions for some non-Unix platforms are
// defined here either because they're too big to be inlined, or
// to avoid use before definition if they were inline.

// @@ The following functions could be inlined if i could figure where
// to put it among the #ifdefs!
int
ACE_OS::condattr_init (ACE_condattr_t &attributes,
                       int type)
{
  attributes.type = type;
  return 0;
}

int
ACE_OS::condattr_destroy (ACE_condattr_t &)
{
  return 0;
}

int
ACE_OS::cond_broadcast (ACE_cond_t *cv)
{
  ACE_OS_TRACE ("ACE_OS::cond_broadcast");
# if defined (ACE_HAS_THREADS)
  // The <external_mutex> must be locked before this call is made.

  // This is needed to ensure that <waiters_> and <was_broadcast_> are
  // consistent relative to each other.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  int have_waiters = 0;

  if (cv->waiters_ > 0)
    {
      // We are broadcasting, even if there is just one waiter...
      // Record the fact that we are broadcasting.  This helps the
      // cond_wait() method know how to optimize itself.  Be sure to
      // set this with the <waiters_lock_> held.
      cv->was_broadcast_ = 1;
      have_waiters = 1;
    }
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);
  int result = 0;
  if (have_waiters)
    {
      // Wake up all the waiters.
      if (ACE_OS::sema_post (&cv->sema_, cv->waiters_) == -1)
        result = -1;
      // Wait for all the awakened threads to acquire their part of
      // the counting semaphore.
#   if defined (VXWORKS) || defined (ACE_PSOS)
      else if (ACE_OS::sema_wait (&cv->waiters_done_) == -1)
#   else
      else if (ACE_OS::event_wait (&cv->waiters_done_) == -1)
#   endif /* VXWORKS */
        result = -1;
      // This is okay, even without the <waiters_lock_> held because
      // no other waiter threads can wake up to access it.
      cv->was_broadcast_ = 0;
    }
  return result;
# else
  ACE_UNUSED_ARG (cv);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_destroy (ACE_cond_t *cv)
{
  ACE_OS_TRACE ("ACE_OS::cond_destroy");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_WTHREADS)
  ACE_OS::event_destroy (&cv->waiters_done_);
#   elif defined (VXWORKS) || defined (ACE_PSOS)
  ACE_OS::sema_destroy (&cv->waiters_done_);
#   endif /* VXWORKS */
  ACE_OS::thread_mutex_destroy (&cv->waiters_lock_);
  return ACE_OS::sema_destroy (&cv->sema_);
# else
  ACE_UNUSED_ARG (cv);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_init (ACE_cond_t *cv,
                   ACE_condattr_t &attributes,
                   const char *name, void *arg)
{
  return ACE_OS::cond_init (cv, attributes.type, name, arg);
}

# if defined (ACE_HAS_WCHAR)
int
ACE_OS::cond_init (ACE_cond_t *cv,
                   ACE_condattr_t &attributes,
                   const wchar_t *name, void *arg)
{
  return ACE_OS::cond_init (cv, attributes.type, name, arg);
}
# endif /* ACE_HAS_WCHAR */

int
ACE_OS::cond_init (ACE_cond_t *cv, short type, const char *name, void *arg)
{
  ACE_OS_TRACE ("ACE_OS::cond_init");
# if defined (ACE_HAS_THREADS)
  cv->waiters_ = 0;
  cv->was_broadcast_ = 0;

  int result = 0;
  if (ACE_OS::sema_init (&cv->sema_, 0, type, name, arg) == -1)
    result = -1;
  else if (ACE_OS::thread_mutex_init (&cv->waiters_lock_) == -1)
    result = -1;
#   if defined (VXWORKS) || defined (ACE_PSOS)
  else if (ACE_OS::sema_init (&cv->waiters_done_, 0, type) == -1)
#   else
  else if (ACE_OS::event_init (&cv->waiters_done_) == -1)
#   endif /* VXWORKS */
    result = -1;
  return result;
# else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

# if defined (ACE_HAS_WCHAR)
int
ACE_OS::cond_init (ACE_cond_t *cv, short type, const wchar_t *name, void *arg)
{
  ACE_OS_TRACE ("ACE_OS::cond_init");
#   if defined (ACE_HAS_THREADS)
  cv->waiters_ = 0;
  cv->was_broadcast_ = 0;

  int result = 0;
  if (ACE_OS::sema_init (&cv->sema_, 0, type, name, arg) == -1)
    result = -1;
  else if (ACE_OS::thread_mutex_init (&cv->waiters_lock_) == -1)
    result = -1;
#     if defined (VXWORKS) || defined (ACE_PSOS)
  else if (ACE_OS::sema_init (&cv->waiters_done_, 0, type) == -1)
#     else
  else if (ACE_OS::event_init (&cv->waiters_done_) == -1)
#     endif /* VXWORKS */
    result = -1;
  return result;
#   else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}
# endif /* ACE_HAS_WCHAR */

int
ACE_OS::cond_signal (ACE_cond_t *cv)
{
  ACE_OS_TRACE ("ACE_OS::cond_signal");
# if defined (ACE_HAS_THREADS)
  // If there aren't any waiters, then this is a no-op.  Note that
  // this function *must* be called with the <external_mutex> held
  // since other wise there is a race condition that can lead to the
  // lost wakeup bug...  This is needed to ensure that the <waiters_>
  // value is not in an inconsistent internal state while being
  // updated by another thread.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  int have_waiters = cv->waiters_ > 0;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (have_waiters != 0)
    return ACE_OS::sema_post (&cv->sema_);
  else
    return 0; // No-op
# else
  ACE_UNUSED_ARG (cv);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_wait (ACE_cond_t *cv,
                   ACE_mutex_t *external_mutex)
{
  ACE_OS_TRACE ("ACE_OS::cond_wait");
# if defined (ACE_HAS_THREADS)
  // Prevent race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;

#   if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  if (external_mutex->type_ == USYNC_PROCESS)
    // This call will automatically release the mutex and wait on the semaphore.
    ACE_WIN32CALL (ACE_ADAPT_RETVAL (::SignalObjectAndWait (external_mutex->proc_mutex_,
                                                            cv->sema_, INFINITE, FALSE),
                                     result),
                   int, -1, result);
  else
#   endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
    {
      // We keep the lock held just long enough to increment the count of
      // waiters by one.  Note that we can't keep it held across the call
      // to ACE_OS::sema_wait() since that will deadlock other calls to
      // ACE_OS::cond_signal().
      if (ACE_OS::mutex_unlock (external_mutex) != 0)
        return -1;

      // Wait to be awakened by a ACE_OS::cond_signal() or
      // ACE_OS::cond_broadcast().
      result = ACE_OS::sema_wait (&cv->sema_);
    }

  // Reacquire lock to avoid race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);

  // We're ready to return, so there's one less waiter.
  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  // Release the lock so that other collaborating threads can make
  // progress.
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (result == -1)
    // Bad things happened, so let's just return below.
    /* NOOP */;
#   if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  else if (external_mutex->type_ == USYNC_PROCESS)
    {
      if (last_waiter)

        // This call atomically signals the <waiters_done_> event and
        // waits until it can acquire the mutex.  This is important to
        // prevent unfairness.
        ACE_WIN32CALL (ACE_ADAPT_RETVAL (::SignalObjectAndWait (cv->waiters_done_,
                                                                external_mutex->proc_mutex_,
                                                                INFINITE, FALSE),
                                         result),
                       int, -1, result);
      else
        // We must always regain the <external_mutex>, even when
        // errors occur because that's the guarantee that we give to
        // our callers.
        ACE_OS::mutex_lock (external_mutex);

      return result;
      /* NOTREACHED */
    }
#   endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
  // If we're the last waiter thread during this particular broadcast
  // then let all the other threads proceed.
  else if (last_waiter)
#   if defined (VXWORKS) || defined (ACE_PSOS)
    ACE_OS::sema_post (&cv->waiters_done_);
#   else
    ACE_OS::event_signal (&cv->waiters_done_);
#   endif /* VXWORKS */

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::mutex_lock (external_mutex);

  return result;
# else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (external_mutex);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_timedwait (ACE_cond_t *cv,
                        ACE_mutex_t *external_mutex,
                        ACE_Time_Value *timeout)
{
  ACE_OS_TRACE ("ACE_OS::cond_timedwait");
# if defined (ACE_HAS_THREADS)
  // Handle the easy case first.
  if (timeout == 0)
    return ACE_OS::cond_wait (cv, external_mutex);
#   if defined (ACE_HAS_WTHREADS) || defined (VXWORKS) || defined (ACE_PSOS)

  // Prevent race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;
  ACE_Errno_Guard error (errno, 0);
  int msec_timeout;

  if (timeout->sec () == 0 && timeout->usec () == 0)
    msec_timeout = 0; // Do a "poll."
  else
    {
      // Note that we must convert between absolute time (which is
      // passed as a parameter) and relative time (which is what
      // WaitForSingleObjects() expects).
      ACE_Time_Value relative_time (*timeout - ACE_OS::gettimeofday ());

      // Watchout for situations where a context switch has caused the
      // current time to be > the timeout.
      if (relative_time < ACE_Time_Value::zero)
        msec_timeout = 0;
      else
        msec_timeout = relative_time.msec ();
    }

#     if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  if (external_mutex->type_ == USYNC_PROCESS)
    // This call will automatically release the mutex and wait on the
    // semaphore.
    result = ::SignalObjectAndWait (external_mutex->proc_mutex_,
                                    cv->sema_,
                                    msec_timeout,
                                    FALSE);
  else
#     endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
    {
      // We keep the lock held just long enough to increment the count
      // of waiters by one.  Note that we can't keep it held across
      // the call to WaitForSingleObject since that will deadlock
      // other calls to ACE_OS::cond_signal().
      if (ACE_OS::mutex_unlock (external_mutex) != 0)
        return -1;

      // Wait to be awakened by a ACE_OS::signal() or
      // ACE_OS::broadcast().
#     if defined (ACE_WIN32)
#       if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
      result = ::WaitForSingleObject (cv->sema_, msec_timeout);
#       else /* ACE_USES_WINCE_SEMA_SIMULATION */
      // Can't use Win32 API on our simulated semaphores.
      result = ACE_OS::sema_wait (&cv->sema_,
                                  timeout);
#       endif /* ACE_USES_WINCE_SEMA_SIMULATION */
#     elif defined (ACE_PSOS)
      // Inline the call to ACE_OS::sema_wait () because it takes an
      // ACE_Time_Value argument.  Avoid the cost of that conversion . . .
      u_long ticks = (KC_TICKS2SEC * msec_timeout) / ACE_ONE_SECOND_IN_MSECS;
      //Tick set to 0 tells pSOS to wait forever is SM_WAIT is set.
      if(ticks == 0)
        result = ::sm_p (cv->sema_.sema_, SM_NOWAIT, ticks); //no timeout
      else
        result = ::sm_p (cv->sema_.sema_, SM_WAIT, ticks);
#     elif defined (VXWORKS)
      // Inline the call to ACE_OS::sema_wait () because it takes an
      // ACE_Time_Value argument.  Avoid the cost of that conversion . . .
      int ticks_per_sec = ::sysClkRateGet ();
      int ticks = msec_timeout * ticks_per_sec / ACE_ONE_SECOND_IN_MSECS;
      result = ::semTake (cv->sema_.sema_, ticks);
#     endif /* ACE_WIN32 || VXWORKS */
    }

  // Reacquire lock to avoid race conditions.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

#     if defined (ACE_WIN32)
  if (result != WAIT_OBJECT_0)
    {
      switch (result)
        {
        case WAIT_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = ::GetLastError ();
          break;
        }
      result = -1;
    }
#     elif defined (ACE_PSOS)
  if (result != 0)
    {
      switch (result)
        {
        case ERR_TIMEOUT:  // Timeout occured with SM_WAIT
        case ERR_NOMSG:    // Didn't acquire semaphore w/ SM_NOWAIT (ticks=0)
          error = ETIME;
          break;
        default:
          error = errno;
          break;
        }
      result = -1;
    }
#     elif defined (VXWORKS)
  if (result == ERROR)
    {
      switch (errno)
        {
        case S_objLib_OBJ_TIMEOUT:
          error = ETIME;
          break;
        case S_objLib_OBJ_UNAVAILABLE:
          if (msec_timeout == 0)
            error = ETIME;
          break;
        default:
          error = errno;
          break;
        }
      result = -1;
    }
#     endif /* ACE_WIN32 || VXWORKS */
#     if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  if (external_mutex->type_ == USYNC_PROCESS)
    {
      if (last_waiter)
        // This call atomically signals the <waiters_done_> event and
        // waits until it can acquire the mutex.  This is important to
        // prevent unfairness.
        ACE_WIN32CALL (ACE_ADAPT_RETVAL (::SignalObjectAndWait (cv->waiters_done_,
                                                                external_mutex->proc_mutex_,
                                                                INFINITE, FALSE),
                                         result),
                       int, -1, result);
      else
        // We must always regain the <external_Mutex>, even when
        // errors occur because that's the guarantee that we give to
        // our callers.
        ACE_OS::mutex_lock (external_mutex);

      return result;
      /* NOTREACHED */
    }
#     endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
  // Note that this *must* be an "if" statement rather than an "else
  // if" statement since the caller may have timed out and hence the
  // result would have been -1 above.
  if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
#     if defined (ACE_WIN32)
    ACE_OS::event_signal (&cv->waiters_done_);
#     else
    ACE_OS::sema_post (&cv->waiters_done_);
#     endif /* ACE_WIN32 */

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::mutex_lock (external_mutex);

  return result;
#   endif /* ACE_HAS_WTHREADS || ACE_HAS_VXWORKS || ACE_PSOS */
# else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (external_mutex);
  ACE_UNUSED_ARG (timeout);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

# if defined (ACE_HAS_WTHREADS)
int
ACE_OS::cond_timedwait (ACE_cond_t *cv,
                        ACE_thread_mutex_t *external_mutex,
                        ACE_Time_Value *timeout)
{
  ACE_OS_TRACE ("ACE_OS::cond_timedwait");
#   if defined (ACE_HAS_THREADS)
  // Handle the easy case first.
  if (timeout == 0)
    return ACE_OS::cond_wait (cv, external_mutex);

  // Prevent race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;
  int error = 0;
  int msec_timeout;

  if (timeout->sec () == 0 && timeout->usec () == 0)
    msec_timeout = 0; // Do a "poll."
  else
    {
      // Note that we must convert between absolute time (which is
      // passed as a parameter) and relative time (which is what
      // WaitForSingleObjects() expects).
      ACE_Time_Value relative_time (*timeout - ACE_OS::gettimeofday ());

      // Watchout for situations where a context switch has caused the
      // current time to be > the timeout.
      if (relative_time < ACE_Time_Value::zero)
        msec_timeout = 0;
      else
        msec_timeout = relative_time.msec ();
    }

  // We keep the lock held just long enough to increment the count of
  // waiters by one.  Note that we can't keep it held across the call
  // to WaitForSingleObject since that will deadlock other calls to
  // ACE_OS::cond_signal().
  if (ACE_OS::thread_mutex_unlock (external_mutex) != 0)
    return -1;

  // Wait to be awakened by a ACE_OS::signal() or ACE_OS::broadcast().
#     if defined (ACE_USES_WINCE_SEMA_SIMULATION)
  // Can't use Win32 API on simulated semaphores.
  result = ACE_OS::sema_wait (&cv->sema_,
                              timeout);

  if (result == -1 && errno == ETIME)
    result = WAIT_TIMEOUT;
#     else
  result = ::WaitForSingleObject (cv->sema_, msec_timeout);
#     endif /* ACE_USES_WINCE_SEMA_SIMULATION */

  // Reacquire lock to avoid race conditions.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);

  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (result != WAIT_OBJECT_0)
    {
      switch (result)
        {
        case WAIT_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = ::GetLastError ();
          break;
        }
      result = -1;
    }

  if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
    ACE_OS::event_signal (&cv->waiters_done_);

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::thread_mutex_lock (external_mutex);
  errno = error;
  return result;
#   else
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_wait (ACE_cond_t *cv,
                   ACE_thread_mutex_t *external_mutex)
{
  ACE_OS_TRACE ("ACE_OS::cond_wait");
#   if defined (ACE_HAS_THREADS)
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;
  int error = 0;

  // We keep the lock held just long enough to increment the count of
  // waiters by one.  Note that we can't keep it held across the call
  // to ACE_OS::sema_wait() since that will deadlock other calls to
  // ACE_OS::cond_signal().
  if (ACE_OS::thread_mutex_unlock (external_mutex) != 0)
    return -1;

  // Wait to be awakened by a ACE_OS::cond_signal() or
  // ACE_OS::cond_broadcast().
#     if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
  result = ::WaitForSingleObject (cv->sema_, INFINITE);
#     else
  // Can't use Win32 API on simulated semaphores.
  result = ACE_OS::sema_wait (&cv->sema_);

  if (result != WAIT_OBJECT_0 && errno == ETIME)
    result = WAIT_TIMEOUT;

#     endif /* ACE_USES_WINCE_SEMA_SIMULATION */

  // Reacquire lock to avoid race conditions.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);

  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (result != WAIT_OBJECT_0)
    {
      switch (result)
        {
        case WAIT_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = ::GetLastError ();
          break;
        }
    }
  else if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
    ACE_OS::event_signal (&cv->waiters_done_);

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::thread_mutex_lock (external_mutex);

  // Reset errno in case mutex_lock() also fails...
  errno = error;
  return result;
#   else
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}
# endif /* ACE_HAS_WTHREADS */
#endif /* ACE_LACKS_COND_T */

int
ACE_OS::lwp_getparams (ACE_Sched_Params &sched_params)
{
#if defined (ACE_HAS_STHREADS) || defined (sun)
  // Get the class TS and RT class IDs.
  ACE_id_t rt_id;
  ACE_id_t ts_id;
  if (ACE_OS::scheduling_class ("RT", rt_id) == -1
      || ACE_OS::scheduling_class ("TS", ts_id) == -1)
    return -1;

  // Get this LWP's scheduling parameters.
  pcparms_t pcparms;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcparms, 0, sizeof pcparms);
  pcparms.pc_cid = PC_CLNULL;

  if (ACE_OS::priority_control (P_LWPID,
                                P_MYID,
                                PC_GETPARMS,
                                (char *) &pcparms) == -1)
    return -1;
  else if (pcparms.pc_cid == rt_id)
    {
      // RT class.
      rtparms_t rtparms;
      ACE_OS::memcpy (&rtparms, pcparms.pc_clparms, sizeof rtparms);

      sched_params.policy (ACE_SCHED_FIFO);
      sched_params.priority (rtparms.rt_pri);
      sched_params.scope (ACE_SCOPE_THREAD);
      ACE_Time_Value quantum (rtparms.rt_tqsecs,
                              rtparms.rt_tqnsecs == RT_TQINF
                              ? 0 : rtparms.rt_tqnsecs * 1000);
      sched_params.quantum (quantum);
      return 0;
    }
  else if (pcparms.pc_cid == ts_id)
    {
      /* TS class */
      tsparms_t tsparms;
      ACE_OS::memcpy (&tsparms, pcparms.pc_clparms, sizeof tsparms);

      sched_params.policy (ACE_SCHED_OTHER);
      sched_params.priority (tsparms.ts_upri);
      sched_params.scope (ACE_SCOPE_THREAD);
      return 0;
    }
  else
    return -1;

#else  /* ! ACE_HAS_STHREADS && ! sun */
  ACE_UNUSED_ARG (sched_params);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_STHREADS && ! sun */
}

int
ACE_OS::lwp_setparams (const ACE_Sched_Params &sched_params)
{
#if defined (ACE_HAS_STHREADS) || defined (sun)
  ACE_Sched_Params lwp_params (sched_params);
  lwp_params.scope (ACE_SCOPE_LWP);
  return ACE_OS::sched_params (lwp_params);
#else  /* ! ACE_HAS_STHREADS && ! sun */
  ACE_UNUSED_ARG (sched_params);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_STHREADS && ! sun */
}

void
ACE_OS::mutex_lock_cleanup (void *mutex)
{
  ACE_OS_TRACE ("ACE_OS::mutex_lock_cleanup");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_PTHREADS)
  ACE_mutex_t *p_lock = (ACE_mutex_t *) mutex;
  ACE_OS::mutex_unlock (p_lock);
# else
  ACE_UNUSED_ARG (mutex);
# endif /* ACE_HAS_PTHREADS */
#else
  ACE_UNUSED_ARG (mutex);
#endif /* ACE_HAS_THREADS */
}

#if !defined (ACE_HAS_THREADS) || defined (ACE_LACKS_RWLOCK_T)
int
ACE_OS::rwlock_init (ACE_rwlock_t *rw,
                     int type,
                     const ACE_TCHAR *name,
                     void *arg)
{
  // ACE_OS_TRACE ("ACE_OS::rwlock_init");
# if defined (ACE_HAS_THREADS) && defined (ACE_LACKS_RWLOCK_T)
  // NT, POSIX, and VxWorks don't support this natively.
  ACE_UNUSED_ARG (name);
  int result = -1;

  // Since we cannot use the user specified name for all three
  // objects, we will create three completely new names.
  ACE_TCHAR name1[ACE_UNIQUE_NAME_LEN];
  ACE_TCHAR name2[ACE_UNIQUE_NAME_LEN];
  ACE_TCHAR name3[ACE_UNIQUE_NAME_LEN];
  ACE_TCHAR name4[ACE_UNIQUE_NAME_LEN];

  ACE_OS::unique_name ((const void *) &rw->lock_,
                       name1,
                       ACE_UNIQUE_NAME_LEN);
  ACE_OS::unique_name ((const void *) &rw->waiting_readers_,
                       name2,
                       ACE_UNIQUE_NAME_LEN);
  ACE_OS::unique_name ((const void *) &rw->waiting_writers_,
                       name3,
                       ACE_UNIQUE_NAME_LEN);
  ACE_OS::unique_name ((const void *) &rw->waiting_important_writer_,
                       name4,
                       ACE_UNIQUE_NAME_LEN);

  ACE_condattr_t attributes;
  if (ACE_OS::condattr_init (attributes, type) == 0)
    {
      if (ACE_OS::mutex_init (&rw->lock_, type, name1,
                              (ACE_mutexattr_t *) arg) == 0
          && ACE_OS::cond_init (&rw->waiting_readers_,
                                attributes, name2, arg) == 0
          && ACE_OS::cond_init (&rw->waiting_writers_,
                                attributes, name3, arg) == 0
          && ACE_OS::cond_init (&rw->waiting_important_writer_,
                                attributes, name4, arg) == 0)
        {
          // Success!
          rw->ref_count_ = 0;
          rw->num_waiting_writers_ = 0;
          rw->num_waiting_readers_ = 0;
          rw->important_writer_ = 0;
          result = 0;
        }
      ACE_OS::condattr_destroy (attributes);
    }

  if (result == -1)
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      ACE_OS::mutex_destroy (&rw->lock_);
      ACE_OS::cond_destroy (&rw->waiting_readers_);
      ACE_OS::cond_destroy (&rw->waiting_writers_);
      ACE_OS::cond_destroy (&rw->waiting_important_writer_);
    }
  return result;
# else
  ACE_UNUSED_ARG (rw);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}
#endif /* ! ACE_HAS_THREADS || ACE_LACKS_RWLOCK_T */

int
ACE_OS::sched_params (const ACE_Sched_Params &sched_params,
                      ACE_id_t id)
{
  ACE_OS_TRACE ("ACE_OS::sched_params");
#if defined (CHORUS)
  ACE_UNUSED_ARG (id);
  int result;
  struct sched_param param;
  ACE_thread_t thr_id = ACE_OS::thr_self ();

  param.sched_priority = sched_params.priority ();

  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (thr_id,
                                                                sched_params.policy (),
                                                                &param),
                                       result),
                     int, -1);
#elif defined (ACE_HAS_STHREADS)
  return ACE_OS::set_scheduling_params (sched_params, id);
#elif defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_SETSCHED)
  ACE_UNUSED_ARG (id);
  if (sched_params.quantum () != ACE_Time_Value::zero)
    {
      // quantums not supported
      errno = EINVAL;
      return -1;
    }

  // Thanks to Thilo Kielmann <kielmann@informatik.uni-siegen.de> for
  // providing this code for 1003.1c PThreads.  Please note that this
  // has only been tested for POSIX 1003.1c threads, and may cause
  // problems with other PThreads flavors!

  struct sched_param param;
  param.sched_priority = sched_params.priority ();

  if (sched_params.scope () == ACE_SCOPE_PROCESS)
    {
      int result = ::sched_setscheduler (0, // this process
                                         sched_params.policy (),
                                         &param) == -1 ? -1 : 0;
# if defined (DIGITAL_UNIX)
      return result == 0
        ? // Use priocntl (2) to set the process in the RT class,
        // if using an RT policy.
        ACE_OS::set_scheduling_params (sched_params)
        : result;
# else  /* ! DIGITAL_UNIX */
      return result;
# endif /* ! DIGITAL_UNIX */
    }
  else if (sched_params.scope () == ACE_SCOPE_THREAD)
    {
      ACE_thread_t thr_id = ACE_OS::thr_self ();

# if defined (ACE_HAS_PTHREADS_DRAFT4)
      return (::pthread_setscheduler (thr_id,
                                      sched_params.policy (),
                                      sched_params.priority()) == -1 ? -1 : 0);
# else
      int result;
      ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (thr_id,
                                                                    sched_params.policy (),
                                                                    &param),
                                           result),
                         int, -1);
# endif  /* ACE_HAS_PTHREADS_DRAFT4 */
    }
# if defined (sun)
  // We need to be able to set LWP priorities on Suns, even without
  // ACE_HAS_STHREADS, to obtain preemption.
  else if (sched_params.scope () == ACE_SCOPE_LWP)
    return ACE_OS::set_scheduling_params (sched_params, id);
# endif /* sun */
  else // sched_params.scope () == ACE_SCOPE_LWP, which isn't POSIX
    {
      errno = EINVAL;
      return -1;
    }

#elif defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  // PharLap ETS can act on the current thread - it can set the
  // quantum also, unlike Win32. All this only works on the RT
  // version.
# if defined (ACE_HAS_PHARLAP_RT)
  if (id != ACE_SELF)
    ACE_NOTSUP_RETURN (-1);

  if (sched_params.quantum() != ACE_Time_Value::zero)
    EtsSetTimeSlice (sched_params.quantum().msec());

# else
  ACE_UNUSED_ARG (id);

  if (sched_params.scope () != ACE_SCOPE_PROCESS
      || sched_params.quantum () != ACE_Time_Value::zero)
    {
      // Win32 only allows setting priority class (therefore, policy)
      // at the process level.  I don't know of a way to set the
      // quantum.
      errno = EINVAL;
      return -1;
    }

  // Set the priority class of this process to the REALTIME process class
  // _if_ the policy is ACE_SCHED_FIFO.  Otherwise, set to NORMAL.
  if (!::SetPriorityClass (::GetCurrentProcess (),
                           (sched_params.policy () == ACE_SCHED_FIFO ||
			   sched_params.policy () == ACE_SCHED_RR)
                           ? REALTIME_PRIORITY_CLASS
                           : NORMAL_PRIORITY_CLASS))
    {
      ACE_OS::set_errno_to_last_error ();
      return -1;
    }
# endif /* ACE_HAS_PHARLAP_RT */

  // Set the thread priority on the current thread.
  return ACE_OS::thr_setprio (sched_params.priority ());

#elif defined (VXWORKS) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (id);

  // There is only one class of priorities on VxWorks, and no time
  // quanta.  So, just set the current thread's priority.

  if (sched_params.policy () != ACE_SCHED_FIFO
      || sched_params.scope () != ACE_SCOPE_PROCESS
      || sched_params.quantum () != ACE_Time_Value::zero)
    {
      errno = EINVAL;
      return -1;
    }

  // Set the thread priority on the current thread.
  return ACE_OS::thr_setprio (sched_params.priority ());
#else
  ACE_UNUSED_ARG (sched_params);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
#endif /* CHORUS */
}

int
ACE_OS::scheduling_class (const char *class_name, ACE_id_t &id)
{
#if defined (ACE_HAS_PRIOCNTL)
  // Get the priority class ID.
  pcinfo_t pcinfo;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcinfo, 0, sizeof pcinfo);

  ACE_OS::strcpy (pcinfo.pc_clname, class_name);
  if (ACE_OS::priority_control (P_ALL /* ignored */,
                                P_MYID /* ignored */,
                                PC_GETCID,
                                (char *) &pcinfo) == -1)
    {
      return -1;
    }
  else
    {
      id = pcinfo.pc_cid;
      return 0;
    }
#else  /* ! ACE_HAS_PRIOCNTL */
  ACE_UNUSED_ARG (class_name);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_PRIOCNTL */
}

int
ACE_OS::set_scheduling_params (const ACE_Sched_Params &sched_params,
                               ACE_id_t id)
{
#if defined (ACE_HAS_PRIOCNTL)
  // Set priority class, priority, and quantum of this LWP or process as
  // specified in sched_params.

  // Get the priority class ID.
  ACE_id_t class_id;
  if (ACE_OS::scheduling_class (sched_params.policy() == ACE_SCHED_OTHER  ?
                                  "TS"  :
                                  "RT", class_id) == -1)
    {
      return -1;
    }

  pcparms_t pcparms;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcparms, 0, sizeof pcparms);

  pcparms.pc_cid = class_id;

  if (sched_params.policy () == ACE_SCHED_OTHER  &&
      sched_params.quantum () == ACE_Time_Value::zero)
      // SunOS doesn't support non-zero quantums in time-sharing class:  use
      // real-time class instead.
    {
      tsparms_t tsparms;
      // The following is just to avoid Purify warnings about unitialized
      // memory reads.
      ACE_OS::memset (&tsparms, 0, sizeof tsparms);

      // Don't change ts_uprilim (user priority limit)
      tsparms.ts_uprilim = TS_NOCHANGE;
      tsparms.ts_upri = sched_params.priority ();

      // Package up the TS class ID and parameters for the
      // priority_control () call.
      ACE_OS::memcpy (pcparms.pc_clparms, &tsparms, sizeof tsparms);
    }
  else if (sched_params.policy () == ACE_SCHED_FIFO  ||
           (sched_params.policy () == ACE_SCHED_RR &&
            sched_params.quantum () != ACE_Time_Value::zero))
           // must have non-zero quantum for RR, to make it meaningful
           // A zero quantum with FIFO has special significance:  it actually
           // means infinite time quantum, i.e., run-to-completion.
    {
      rtparms_t rtparms;
      // The following is just to avoid Purify warnings about unitialized
      // memory reads.
      ACE_OS::memset (&rtparms, 0, sizeof rtparms);

      rtparms.rt_pri = sched_params.priority ();

      if (sched_params.quantum () == ACE_Time_Value::zero)
        {
          // rtparms.rt_tqsecs is ignored with RT_TQINF
          rtparms.rt_tqnsecs = RT_TQINF;
        }
      else
        {
          rtparms.rt_tqsecs = (ulong) sched_params.quantum ().sec ();
          rtparms.rt_tqnsecs = sched_params.quantum ().usec () * 1000;
        }

      // Package up the RT class ID and parameters for the
      // priority_control () call.
      ACE_OS::memcpy (pcparms.pc_clparms, &rtparms, sizeof rtparms);
    }
  else
    {
      errno = EINVAL;
      return -1;
    }

  if (ACE_OS::priority_control ((idtype_t) (sched_params.scope () == ACE_SCOPE_THREAD
                                            ? ACE_SCOPE_PROCESS
                                            : sched_params.scope ()),
                                id,
                                PC_SETPARMS,
                                (char *) &pcparms) < 0)
    {
      return ACE_OS::last_error ();
    }

  return 0;
#else  /* ! ACE_HAS_PRIOCNTL */
  ACE_UNUSED_ARG (sched_params);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_PRIOCNTL */
}

int
ACE_OS::thr_create (ACE_THR_FUNC func,
                    void *args,
                    long flags,
                    ACE_thread_t *thr_id,
                    ACE_hthread_t *thr_handle,
                    long priority,
                    void *stack,
                    size_t stacksize,
                    ACE_Base_Thread_Adapter *thread_adapter)
{
  ACE_OS_TRACE ("ACE_OS::thr_create");

  if (ACE_BIT_DISABLED (flags, THR_DETACHED) &&
      ACE_BIT_DISABLED (flags, THR_JOINABLE))
    ACE_SET_BITS (flags, THR_JOINABLE);

#if defined (ACE_NO_THREAD_ADAPTER)
# define  ACE_THREAD_FUNCTION  func
# define  ACE_THREAD_ARGUMENT  args
#else /* ! defined (ACE_NO_THREAD_ADAPTER) */
# if defined (ACE_PSOS)
#   define  ACE_THREAD_FUNCTION (PSOS_TASK_ENTRY_POINT) thread_args->entry_point ()
# else
#   define  ACE_THREAD_FUNCTION  thread_args->entry_point ()
# endif /* defined (ACE_PSOS) */
# define  ACE_THREAD_ARGUMENT  thread_args
#endif /* ! defined (ACE_NO_THREAD_ADAPTER) */


  ACE_Base_Thread_Adapter *thread_args;
  if (thread_adapter == 0)

#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
    ACE_NEW_RETURN (thread_args,
                    ACE_OS_Thread_Adapter (func, args,
                                           (ACE_THR_C_FUNC) ace_thread_adapter,
                                           ACE_OS_Object_Manager::seh_except_selector(),
                                           ACE_OS_Object_Manager::seh_except_handler()),
                    -1);
#else
  ACE_NEW_RETURN (thread_args,
                  ACE_OS_Thread_Adapter (func, args,
                                         (ACE_THR_C_FUNC) ace_thread_adapter),
                  -1);

#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
  else
    thread_args = thread_adapter;

#if defined (ACE_HAS_THREADS)

  // *** Set Stack Size
# if defined (ACE_NEEDS_HUGE_THREAD_STACKSIZE)
  if (stacksize < ACE_NEEDS_HUGE_THREAD_STACKSIZE)
    stacksize = ACE_NEEDS_HUGE_THREAD_STACKSIZE;
# endif /* ACE_NEEDS_HUGE_THREAD_STACKSIZE */

# if !defined (VXWORKS)
  // On VxWorks, the OS will provide a task name if the user doesn't.
  // So, we don't need to create a tmp_thr.  If the caller of this
  // member function is the Thread_Manager, than thr_id will be non-zero
  // anyways.
  ACE_thread_t tmp_thr;

  if (thr_id == 0)
    thr_id = &tmp_thr;
# endif /* ! VXWORKS */

  ACE_hthread_t tmp_handle;
  if (thr_handle == 0)
    thr_handle = &tmp_handle;

# if defined (ACE_HAS_PTHREADS)

  int result;
  pthread_attr_t attr;
#   if defined (ACE_HAS_PTHREADS_DRAFT4)
  if (::pthread_attr_create (&attr) != 0)
#   else /* ACE_HAS_PTHREADS_DRAFT4 */
    if (::pthread_attr_init (&attr) != 0)
#   endif /* ACE_HAS_PTHREADS_DRAFT4 */
      return -1;

#   if defined (CHORUS)
  // If it is a super actor, we can't set stacksize.  But for the time
  // being we are all non-super actors.  Should be fixed to take care
  // of super actors!!!
  if (stacksize == 0)
    stacksize = ACE_CHORUS_DEFAULT_MIN_STACK_SIZE;
  else if (stacksize < ACE_CHORUS_DEFAULT_MIN_STACK_SIZE)
    stacksize = ACE_CHORUS_DEFAULT_MIN_STACK_SIZE;
#   endif /*CHORUS */

#   if defined (ACE_HAS_PTHREAD_SETSTACK)
  if ((stacksize != 0) && (stack != 0))
#   else
  if (stacksize != 0)
#   endif /* ACE_HAS_PTHREAD_SETSTACK */
    {
      size_t size = stacksize;

#   if defined (PTHREAD_STACK_MIN)
      if (size < ACE_static_cast (size_t, PTHREAD_STACK_MIN))
        size = PTHREAD_STACK_MIN;
#   endif /* PTHREAD_STACK_MIN */

#   if !defined (ACE_LACKS_THREAD_STACK_SIZE)      // JCEJ 12/17/96
#     if defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
      if (::pthread_attr_setstacksize (&attr, size) != 0)
#     else
#       if defined (ACE_HAS_PTHREAD_SETSTACK)
        if (ACE_ADAPT_RETVAL(pthread_attr_setstack (&attr, stack, size), result) == -1)
#       else
        if (ACE_ADAPT_RETVAL(pthread_attr_setstacksize (&attr, size), result) == -1)
#       endif /* ACE_HAS_PTHREAD_SETSTACK */
#     endif /* ACE_HAS_PTHREADS_DRAFT4, 6 */
          {
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
            ::pthread_attr_delete (&attr);
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
            ::pthread_attr_destroy (&attr);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
            return -1;
          }
#   else
      ACE_UNUSED_ARG (size);
#   endif /* !ACE_LACKS_THREAD_STACK_SIZE */
    }

  // *** Set Stack Address
#   if !defined (ACE_HAS_PTHREAD_SETSTACK)
#     if !defined (ACE_LACKS_THREAD_STACK_ADDR)
  if (stack != 0)
    {
      if (::pthread_attr_setstackaddr (&attr, stack) != 0)
        {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
          ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
          ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
          return -1;
        }
    }
#     else
  ACE_UNUSED_ARG (stack);
#     endif /* !ACE_LACKS_THREAD_STACK_ADDR */
#   endif /* ACE_HAS_PTHREAD_SETSTACK */

  // *** Deal with various attributes
  if (flags != 0)
    {
      // *** Set Detach state
#   if !defined (ACE_LACKS_SETDETACH)
      if (ACE_BIT_ENABLED (flags, THR_DETACHED)
          || ACE_BIT_ENABLED (flags, THR_JOINABLE))
        {
          int dstate = PTHREAD_CREATE_JOINABLE;

          if (ACE_BIT_ENABLED (flags, THR_DETACHED))
            dstate = PTHREAD_CREATE_DETACHED;

#     if defined (ACE_HAS_PTHREADS_DRAFT4)
          if (::pthread_attr_setdetach_np (&attr, dstate) != 0)
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
#       if defined (ACE_HAS_PTHREADS_DRAFT6)
            if (::pthread_attr_setdetachstate (&attr, &dstate) != 0)
#       else
              if (ACE_ADAPT_RETVAL(::pthread_attr_setdetachstate (&attr, dstate),
                                   result) != 0)
#       endif /* ACE_HAS_PTHREADS_DRAFT6 */
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
                {
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
                  ::pthread_attr_delete (&attr);
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
                  ::pthread_attr_destroy (&attr);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
                  return -1;
                }
        }

      // Note: if ACE_LACKS_SETDETACH and THR_DETACHED is enabled, we
      // call ::pthread_detach () below.  If THR_DETACHED is not
      // enabled, we call ::pthread_detach () in the Thread_Manager,
      // after joining with the thread.
#   endif /* ACE_LACKS_SETDETACH */

      // *** Set Policy
#   if !defined (ACE_LACKS_SETSCHED)
      // If we wish to set the priority explicitly, we have to enable
      // explicit scheduling, and a policy, too.
      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          ACE_SET_BITS (flags, THR_EXPLICIT_SCHED);
          if (ACE_BIT_DISABLED (flags, THR_SCHED_FIFO)
              && ACE_BIT_DISABLED (flags, THR_SCHED_RR)
              && ACE_BIT_DISABLED (flags, THR_SCHED_DEFAULT))
            ACE_SET_BITS (flags, THR_SCHED_DEFAULT);
        }

      if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO)
          || ACE_BIT_ENABLED (flags, THR_SCHED_RR)
          || ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
        {
          int spolicy;

#     if defined (ACE_HAS_ONLY_SCHED_OTHER)
          // SunOS, thru version 5.6, only supports SCHED_OTHER.
          spolicy = SCHED_OTHER;
#     else
          // Make sure to enable explicit scheduling, in case we didn't
          // enable it above (for non-default priority).
          ACE_SET_BITS (flags, THR_EXPLICIT_SCHED);

          if (ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
            spolicy = SCHED_OTHER;
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            spolicy = SCHED_FIFO;
#       if defined (SCHED_IO)
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_IO))
            spolicy = SCHED_IO;
#       else
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_IO))
            {
              errno = ENOSYS;
              return -1;
            }
#       endif /* SCHED_IO */
          else
            spolicy = SCHED_RR;

#       if defined (ACE_HAS_FSU_PTHREADS)
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
              ::pthread_attr_destroy (&attr);
              return -1;
            }
#       endif    /*  ACE_HAS_FSU_PTHREADS */

#     endif /* ACE_HAS_ONLY_SCHED_OTHER */

#     if defined (ACE_HAS_PTHREADS_DRAFT4)
          result = ::pthread_attr_setsched (&attr, spolicy);
#     elif defined (ACE_HAS_PTHREADS_DRAFT6)
          result = ::pthread_attr_setschedpolicy (&attr, spolicy);
#     else  /* draft 7 or std */
          ACE_ADAPT_RETVAL(::pthread_attr_setschedpolicy (&attr, spolicy),
                           result);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
          if (result != 0)
            {
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
              ::pthread_attr_delete (&attr);
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
              ::pthread_attr_destroy (&attr);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
              return -1;
            }
        }

      // *** Set Priority (use reasonable default priorities)
#     if defined(ACE_HAS_PTHREADS_STD)
      // If we wish to explicitly set a scheduling policy, we also
      // have to specify a priority.  We choose a "middle" priority as
      // default.  Maybe this is also necessary on other POSIX'ish
      // implementations?
      if ((ACE_BIT_ENABLED (flags, THR_SCHED_FIFO)
           || ACE_BIT_ENABLED (flags, THR_SCHED_RR)
           || ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
          && priority == ACE_DEFAULT_THREAD_PRIORITY)
        {
          if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            priority = ACE_THR_PRI_FIFO_DEF;
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_RR))
            priority = ACE_THR_PRI_RR_DEF;
          else // THR_SCHED_DEFAULT
            priority = ACE_THR_PRI_OTHER_DEF;
        }
#     endif /* ACE_HAS_PTHREADS_STD */
      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          struct sched_param sparam;
          ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);

#     if defined (ACE_HAS_IRIX62_THREADS)
          sparam.sched_priority = ACE_MIN (priority,
                                           (long) PTHREAD_MAX_PRIORITY);
#     elif defined (PTHREAD_MAX_PRIORITY) && !defined(ACE_HAS_PTHREADS_STD)
          /* For MIT pthreads... */
          sparam.prio = ACE_MIN (priority, PTHREAD_MAX_PRIORITY);
#     elif defined(ACE_HAS_PTHREADS_STD) && !defined (ACE_HAS_STHREADS)
          // The following code forces priority into range.
          if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_FIFO_MAX,
                       ACE_MAX (ACE_THR_PRI_FIFO_MIN, priority));
          else if (ACE_BIT_ENABLED(flags, THR_SCHED_RR))
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_RR_MAX,
                       ACE_MAX (ACE_THR_PRI_RR_MIN, priority));
          else // Default policy, whether set or not
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_OTHER_MAX,
                       ACE_MAX (ACE_THR_PRI_OTHER_MIN, priority));
#     elif defined (PRIORITY_MAX)
          sparam.sched_priority = ACE_MIN (priority,
                                           (long) PRIORITY_MAX);
#     else
          sparam.sched_priority = priority;
#     endif /* ACE_HAS_IRIX62_THREADS */

#     if defined (ACE_HAS_FSU_PTHREADS)
          if (sparam.sched_priority >= PTHREAD_MIN_PRIORITY
              && sparam.sched_priority <= PTHREAD_MAX_PRIORITY)
            attr.prio = sparam.sched_priority;
          else
            {
              pthread_attr_destroy (&attr);
              errno = EINVAL;
              return -1;
            }
#     else
          {
#       if defined (sun)  &&  defined (ACE_HAS_ONLY_SCHED_OTHER)
            // SunOS, through 5.6, POSIX only allows priorities > 0 to
            // ::pthread_attr_setschedparam.  If a priority of 0 was
            // requested, set the thread priority after creating it, below.
            if (priority > 0)
#       endif /* sun && ACE_HAS_ONLY_SCHED_OTHER */
              {
#       if defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
                result = ::pthread_attr_setprio (&attr,
                                                 sparam.sched_priority);
#       else /* this is draft 7 or std */
                ACE_ADAPT_RETVAL(::pthread_attr_setschedparam (&attr, &sparam),
                                 result);
#       endif /* ACE_HAS_PTHREADS_DRAFT4, 6 */
                if (result != 0)
                  {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
                    ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
                    ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
                    return -1;
                  }
              }
          }
#     endif    /* ACE_HAS_FSU_PTHREADS */
        }

      // *** Set scheduling explicit or inherited
      if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED)
          || ACE_BIT_ENABLED (flags, THR_EXPLICIT_SCHED))
        {
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
          int sched = PTHREAD_DEFAULT_SCHED;
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
          int sched = PTHREAD_EXPLICIT_SCHED;
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
          if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED))
            sched = PTHREAD_INHERIT_SCHED;
          if (::pthread_attr_setinheritsched (&attr, sched) != 0)
            {
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
              ::pthread_attr_delete (&attr);
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
              ::pthread_attr_destroy (&attr);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
              return -1;
            }
        }
#   else /* ACE_LACKS_SETSCHED */
      ACE_UNUSED_ARG (priority);
#   endif /* ACE_LACKS_SETSCHED */

      // *** Set Scope
#   if !defined (ACE_LACKS_THREAD_PROCESS_SCOPING)
      if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM)
          || ACE_BIT_ENABLED (flags, THR_SCOPE_PROCESS))
        {
#     if defined (ACE_CONFIG_LINUX_H) || defined (HPUX)
          // LinuxThreads do not have support for PTHREAD_SCOPE_PROCESS.
          // Neither does HPUX (up to HP-UX 11.00, as far as I know).
          int scope = PTHREAD_SCOPE_SYSTEM;
#     else /* ACE_CONFIG_LINUX_H */
          int scope = PTHREAD_SCOPE_PROCESS;
#     endif /* ACE_CONFIG_LINUX_H */
          if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM))
            scope = PTHREAD_SCOPE_SYSTEM;

          if (::pthread_attr_setscope (&attr, scope) != 0)
            {
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
              ::pthread_attr_delete (&attr);
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
              ::pthread_attr_destroy (&attr);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
              return -1;
            }
        }
#   endif /* !ACE_LACKS_THREAD_PROCESS_SCOPING */

      if (ACE_BIT_ENABLED (flags, THR_NEW_LWP))
        {
          // Increment the number of LWPs by one to emulate the
          // SunOS semantics.
          int lwps = ACE_OS::thr_getconcurrency ();
          if (lwps == -1)
            {
              if (errno == ENOTSUP)
                // Suppress the ENOTSUP because it's harmless.
                errno = 0;
              else
                // This should never happen on SunOS:
                // ::thr_getconcurrency () should always succeed.
                return -1;
            }
          else if (ACE_OS::thr_setconcurrency (lwps + 1) == -1)
            {
              if (errno == ENOTSUP)
                {
                  // Unlikely:  ::thr_getconcurrency () is supported but
                  // ::thr_setconcurrency () is not?
                }
              else
                return -1;
            }
        }
    }

#   if defined (ACE_HAS_PTHREADS_DRAFT4)
  ACE_OSCALL (::pthread_create (thr_id, attr,
                                thread_args->entry_point (),
                                thread_args),
              int, -1, result);

#     if defined (ACE_LACKS_SETDETACH)
  if (ACE_BIT_ENABLED (flags, THR_DETACHED))
    {
#       if defined (HPUX_10)
      // HP-UX DCE threads' pthread_detach will smash thr_id if it's
      // just given as an argument.  This will cause
      // ACE_Thread_Manager (if it's doing this create) to lose track
      // of the new thread since the ID will be passed back equal to
      // 0.  So give pthread_detach a junker to scribble on.
      ACE_thread_t  junker;
      cma_handle_assign(thr_id, &junker);
      ::pthread_detach (&junker);
#       else
      ::pthread_detach (thr_id);
#       endif /* HPUX_10 */
    }
#     endif /* ACE_LACKS_SETDETACH */

  ::pthread_attr_delete (&attr);

#   elif defined (ACE_HAS_PTHREADS_DRAFT6)
  ACE_OSCALL (::pthread_create (thr_id, &attr,
                                thread_args->entry_point (),
                                thread_args),
              int, -1, result);
  ::pthread_attr_destroy (&attr);

#   else /* this is draft 7 or std */
  ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (thr_id,
                                                  &attr,
                                                  thread_args->entry_point (),
                                                  thread_args),
                                result),
              int, -1, result);
  ::pthread_attr_destroy (&attr);
#   endif /* ACE_HAS_PTHREADS_DRAFT4 */

  // This is a SunOS or POSIX implementation of pthreads, where we
  // assume that ACE_thread_t and ACE_hthread_t are the same.  If this
  // *isn't* correct on some platform, please let us know.
  if (result != -1)
    *thr_handle = *thr_id;

#   if defined (sun)  &&  defined (ACE_HAS_ONLY_SCHED_OTHER)
  // SunOS prior to 5.7:

  // If the priority is 0, then we might have to set it now because we
  // couldn't set it with ::pthread_attr_setschedparam, as noted
  // above.  This doesn't provide strictly correct behavior, because
  // the thread was created (above) with the priority of its parent.
  // (That applies regardless of the inherit_sched attribute: if it
  // was PTHREAD_INHERIT_SCHED, then it certainly inherited its
  // parent's priority.  If it was PTHREAD_EXPLICIT_SCHED, then "attr"
  // was initialized by the SunOS ::pthread_attr_init () to contain
  // NULL for the priority, which indicated to SunOS ::pthread_create
  // () to inherit the parent priority.)
  if (priority == 0)
    {
      // Check the priority of this thread, which is the parent
      // of the newly created thread.  If it is 0, then the
      // newly created thread will have inherited the priority
      // of 0, so there's no need to explicitly set it.
      struct sched_param sparam;
      int policy = 0;
      ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_getschedparam (thr_self (),
                                                             &policy,
                                                             &sparam),
                                    result), int,
                  -1, result);

      // The only policy supported by by SunOS, thru version 5.6,
      // is SCHED_OTHER, so that's hard-coded here.
      policy = ACE_SCHED_OTHER;

      if (sparam.sched_priority != 0)
        {
          ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);
          // The memset to 0 sets the priority to 0, so we don't need
          // to explicitly set sparam.sched_priority.

          ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (*thr_id,
                                                                        policy,
                                                                        &sparam),
                                               result),
                             int, -1);
        }
    }

#     if defined (ACE_NEEDS_LWP_PRIO_SET)
#       if 0
  // It would be useful if we could make this work.  But, it requires
  // a mechanism for determining the ID of an LWP to which another
  // thread is bound.  Is there a way to do that?  Instead, just rely
  // on the code in ACE_Thread_Adapter::invoke () to set the LWP
  // priority.

  // If the thread is bound, then set the priority on its LWP.
  if (ACE_BIT_ENABLED (flags, THR_BOUND))
    {
      ACE_Sched_Params sched_params (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO) ||
                                     ACE_BIT_ENABLED (flags, THR_SCHED_RR)  ?
                                     ACE_SCHED_FIFO  :
                                     ACE_SCHED_OTHER,
                                     priority);
      result = ACE_OS::lwp_setparams (sched_params,
                                      /* ? How do we find the ID of the LWP
                                         to which *thr_id is bound? */);
    }
#       endif /* 0 */
#     endif /* ACE_NEEDS_LWP_PRIO_SET */

#   endif /* sun && ACE_HAS_ONLY_SCHED_OTHER */
  return result;
# elif defined (ACE_HAS_STHREADS)
  int result;
  int start_suspended = ACE_BIT_ENABLED (flags, THR_SUSPENDED);

  if (priority != ACE_DEFAULT_THREAD_PRIORITY)
    // If we need to set the priority, then we need to start the
    // thread in a suspended mode.
    ACE_SET_BITS (flags, THR_SUSPENDED);

  ACE_OSCALL (ACE_ADAPT_RETVAL (::thr_create (stack, stacksize,
                                              thread_args->entry_point (),
                                              thread_args,
                                              flags, thr_id), result),
              int, -1, result);

  if (result != -1)
    {
      // With SunOS threads, ACE_thread_t and ACE_hthread_t are the same.
      *thr_handle = *thr_id;

      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          // Set the priority of the new thread and then let it
          // continue, but only if the user didn't start it suspended
          // in the first place!
          if ((result = ACE_OS::thr_setprio (*thr_id, priority)) != 0)
            {
              errno = result;
              return -1;
            }

          if (start_suspended == 0)
            {
              if ((result = ACE_OS::thr_continue (*thr_id)) != 0)
                {
                  errno = result;
                  return -1;
                }
            }
        }
    }
  return result;
# elif defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (stack);
#   if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
  if (ACE_BIT_ENABLED (flags, THR_USE_AFX))
    {
      CWinThread *cwin_thread =
        ::AfxBeginThread ((AFX_THREADPROC) thread_args->entry_point (),
                          thread_args,
                          priority,
                          0,
                          flags | THR_SUSPENDED);
      // Have to duplicate the handle because
      // CWinThread::~CWinThread() closes the original handle.
#     if !defined (ACE_HAS_WINCE)
      (void) ::DuplicateHandle (::GetCurrentProcess (),
                                cwin_thread->m_hThread,
                                ::GetCurrentProcess (),
                                thr_handle,
                                0,
                                TRUE,
                                DUPLICATE_SAME_ACCESS);
#     endif /* ! ACE_HAS_WINCE */
      *thr_id = cwin_thread->m_nThreadID;

      if (ACE_BIT_ENABLED (flags, THR_SUSPENDED) == 0)
        cwin_thread->ResumeThread ();
      // cwin_thread will be deleted in AfxThreadExit()
      // Warning: If AfxThreadExit() is called from within the
      // thread, ACE_TSS_Cleanup->exit() never gets called !
    }
  else
#   endif /* ACE_HAS_MFC */
    {
      int start_suspended = ACE_BIT_ENABLED (flags, THR_SUSPENDED);

      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        // If we need to set the priority, then we need to start the
        // thread in a suspended mode.
        ACE_SET_BITS (flags, THR_SUSPENDED);

      *thr_handle = (void *) ACE_BEGINTHREADEX (0,
                                                ACE_static_cast
                                                   (u_int, stacksize),
                                                thread_args->entry_point (),
                                                thread_args,
                                                flags,
                                                thr_id);

      if (priority != ACE_DEFAULT_THREAD_PRIORITY && *thr_handle != 0)
        {
          // Set the priority of the new thread and then let it
          // continue, but only if the user didn't start it suspended
          // in the first place!
          ACE_OS::thr_setprio (*thr_handle, priority);

          if (start_suspended == 0)
            ACE_OS::thr_continue (*thr_handle);
        }
    }
#   if 0
  *thr_handle = ::CreateThread
    (0,
     stacksize,
     LPTHREAD_START_ROUTINE (thread_args->entry_point ()),
     thread_args,
     flags,
     thr_id);
#   endif /* 0 */

  // Close down the handle if no one wants to use it.
  if (thr_handle == &tmp_handle)
    ::CloseHandle (tmp_handle);

  if (*thr_handle != 0)
    return 0;
  else
    ACE_FAIL_RETURN (-1);
  /* NOTREACHED */

# elif defined (ACE_PSOS)

  // stack is created in the task's memory region 0
  ACE_UNUSED_ARG (stack);

  // task creation and start flags are fixed
  ACE_UNUSED_ARG (flags);

  // lowest priority is reserved for the IDLE pSOS+ system daemon,
  // highest are reserved for high priority pSOS+ system daemons
  if (priority < PSOS_TASK_MIN_PRIORITY)
    {
      priority = PSOS_TASK_MIN_PRIORITY;
    }
  else if (priority > PSOS_TASK_MAX_PRIORITY)
    {
      priority = PSOS_TASK_MAX_PRIORITY;
    }

  // set the stacksize to a default value if no size is specified
  if (stacksize == 0)
    stacksize = ACE_PSOS_DEFAULT_STACK_SIZE;

  ACE_hthread_t tid;
  *thr_handle = 0;

  // create the thread
  if (t_create ((char *) thr_id, // task name
                priority,        // (possibly adjusted) task priority
                stacksize,       // passed stack size is used for supervisor stack
                0,               // no user stack: tasks run strictly in supervisor mode
                T_LOCAL,         // local to the pSOS+ node (does not support pSOS+m)
                &tid)            // receives task id
      != 0)
    {
      return -1;
    }

  // pSOS tasks are passed an array of 4 u_longs
  u_long targs[4];
  targs[0] = (u_long) ACE_THREAD_ARGUMENT;
  targs[1] = 0;
  targs[2] = 0;
  targs[3] = 0;

  // start the thread
  if (t_start (tid,
               T_PREEMPT |            // Task can be preempted
               //             T_NOTSLICE |           // Task is not timesliced with other tasks at same priority
               T_TSLICE |             // Task is timesliced with other tasks at same priority
               T_NOASR |              // Task level signals disabled
               T_SUPV |               // Task runs strictly in supervisor mode
               T_ISR,                 // Hardware interrupts are enabled
               ACE_THREAD_FUNCTION,   // Task entry point
               targs)                 // Task argument(s)
      != 0)
    {
      return -1;
    }

  // store the task id in the handle and return success
  *thr_handle = tid;
  return 0;

# elif defined (VXWORKS)
  // The hard-coded values below are what ::sp () would use.  (::sp ()
  // hardcodes priority to 100, flags to VX_FP_TASK, and stacksize to
  // 20,000.)  stacksize should be an even integer.  If a stack is not
  // specified, ::taskSpawn () is used so that we can set the
  // priority, flags, and stacksize.  If a stack is specified,
  // ::taskInit ()/::taskActivate() are used.

  // If called with thr_create() defaults, use same default values as ::sp ():
  if (priority == ACE_DEFAULT_THREAD_PRIORITY) priority = 100;
  // Assumes that there is a floating point coprocessor.  As noted
  // above, ::sp () hardcodes this, so we should be safe with it.
  if (flags == 0) flags = VX_FP_TASK;
  if (stacksize == 0) stacksize = 20000;

  const u_int thr_id_provided =
    thr_id  &&  *thr_id  &&  (*thr_id)[0] != ACE_THR_ID_ALLOCATED;

  ACE_hthread_t tid;
#   if 0 /* Don't support setting of stack, because it doesn't seem to work. */
  if (stack == 0)
    {
#   else
      ACE_UNUSED_ARG (stack);
#   endif /* 0 */
      // The call below to ::taskSpawn () causes VxWorks to assign a
      // unique task name of the form: "t" + an integer, because the
      // first argument is 0.
      tid = ::taskSpawn (thr_id_provided  ?  *thr_id  :  0,
                         priority,
                         (int) flags,
                         (int) stacksize,
                         thread_args->entry_point (),
                         (int) thread_args,
                         0, 0, 0, 0, 0, 0, 0, 0, 0);
#   if 0 /* Don't support setting of stack, because it doesn't seem to work. */
    }
  else
    {
      // If a task name (thr_id) was not supplied, then the task will
      // not have a unique name.  That's VxWorks' behavior.

      // Carve out a TCB at the beginning of the stack space.  The TCB
      // occupies 400 bytes with VxWorks 5.3.1/I386.
      WIND_TCB *tcb = (WIND_TCB *) stack;

      // The TID is defined to be the address of the TCB.
      int status = ::taskInit (tcb,
                               thr_id_provided  ?  *thr_id  :  0,
                               priority,
                               (int) flags,
                               (char *) stack + sizeof (WIND_TCB),
                               (int) (stacksize - sizeof (WIND_TCB)),
                               thread_args->entry_point (),
                               (int) thread_args,
                               0, 0, 0, 0, 0, 0, 0, 0, 0);

      if (status == OK)
        {
          // The task was successfully initialized, now activate it.
          status = ::taskActivate ((ACE_hthread_t) tcb);
        }

      tid = status == OK  ?  (ACE_hthread_t) tcb  :  ERROR;
    }
#   endif /* 0 */

  if (tid == ERROR)
    return -1;
  else
    {
      if (! thr_id_provided  &&  thr_id)
        {
          if (*thr_id  &&  (*thr_id)[0] == ACE_THR_ID_ALLOCATED)
            // *thr_id was allocated by the Thread_Manager.  ::taskTcb
            // (int tid) returns the address of the WIND_TCB (task
            // control block).  According to the ::taskSpawn()
            // documentation, the name of the new task is stored at
            // pStackBase, but is that of the current task?  If so, it
            // might be a bit quicker than this extraction of the tcb
            // . . .
            ACE_OS::strsncpy (*thr_id + 1, ::taskTcb (tid)->name, 10);
          else
            // *thr_id was not allocated by the Thread_Manager.
            // Pass back the task name in the location pointed to
            // by thr_id.
            *thr_id = ::taskTcb (tid)->name;
        }
      // else if the thr_id was provided, there's no need to overwrite
      // it with the same value (string).  If thr_id is 0, then we can't
      // pass the task name back.

      if (thr_handle)
        *thr_handle = tid;

      return 0;
    }

# endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (func);
  ACE_UNUSED_ARG (args);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (thr_id);
  ACE_UNUSED_ARG (thr_handle);
  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (stack);
  ACE_UNUSED_ARG (stacksize);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

void
ACE_OS::thr_exit (ACE_THR_FUNC_RETURN status)
{
  ACE_OS_TRACE ("ACE_OS::thr_exit");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_PTHREADS)
    ::pthread_exit (status);
# elif defined (ACE_HAS_STHREADS)
    ::thr_exit (status);
# elif defined (ACE_HAS_WTHREADS)
    // Can't call it here because on NT, the thread is exited
    // directly by ACE_Thread_Adapter::invoke ().
    //   ACE_TSS_Cleanup::instance ()->exit (status);

#   if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
    int using_afx = -1;
    // An ACE_Thread_Descriptor really is an ACE_OS_Thread_Descriptor.
    // But without #including ace/Thread_Manager.h, we don't know that.
    ACE_OS_Thread_Descriptor *td =
      ACE_Base_Thread_Adapter::thr_desc_log_msg ();
    if (td)
      using_afx = ACE_BIT_ENABLED (td->flags (), THR_USE_AFX);
#   endif /* ACE_HAS_MFC && (ACE_HAS_MFC != 0) */

    // Call TSS destructors.
    ACE_OS::cleanup_tss (0 /* not main thread */);

    // Exit the thread.
    // Allow CWinThread-destructor to be invoked from AfxEndThread.
    // _endthreadex will be called from AfxEndThread so don't exit the
    // thread now if we are running an MFC thread.
#   if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
    if (using_afx != -1)
      {
        if (using_afx)
          ::AfxEndThread (status);
        else
          ACE_ENDTHREADEX (status);
      }
    else
      {
        // Not spawned by ACE_Thread_Manager, use the old buggy
        // version.  You should seriously consider using
        // ACE_Thread_Manager to spawn threads.  The following code is
        // know to cause some problem.
        CWinThread *pThread = ::AfxGetThread ();
        if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
          ACE_ENDTHREADEX (status);
        else
          ::AfxEndThread (status);
      }
#   else
    ACE_ENDTHREADEX (status);
#   endif /* ACE_HAS_MFC && ACE_HAS_MFS != 0*/

# elif defined (VXWORKS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);
    *((int *) status) = ::taskDelete (tid);
# elif defined (ACE_PSOS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);

#   if defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
    // Call TSS destructors.
    ACE_OS::cleanup_tss (0 /* not main thread */);
#   endif /* ACE_PSOS && ACE_PSOS_HAS_TSS */

    *((u_long *) status) = ::t_delete (tid);
# endif /* ACE_HAS_PTHREADS */
#else
  ACE_UNUSED_ARG (status);
#endif /* ACE_HAS_THREADS */
}

#if defined (VXWORKS)
// Leave this in the global scope to allow
// users to adjust the delay value.
int ACE_THR_JOIN_DELAY = 5;

int
ACE_OS::thr_join (ACE_hthread_t thr_handle,
                  ACE_THR_FUNC_RETURN *status)
{
  // We can't get the status of the thread
  if (status != 0)
    {
      *status = 0;
    }

  // This method can not support joining all threads
  if (ACE_OS::thr_cmp (thr_handle, ACE_OS::NULL_hthread))
    {
      ACE_NOTSUP_RETURN (-1);
    }

  int retval = ESRCH;
  ACE_hthread_t current;
  ACE_OS::thr_self (current);

  // Make sure we are not joining ourself
  if (ACE_OS::thr_cmp (thr_handle, current))
    {
      retval = EDEADLK;
    }
  else
    {
      // Whether the task exists or not
      // we will return a successful value
      retval = 0;

      // Verify that the task id still exists
      while (taskIdVerify (thr_handle) == OK)
        {
          // Wait a bit to see if the task is still active.
          ACE_OS::sleep (ACE_THR_JOIN_DELAY);
        }
    }

  // Adapt the return value into errno and return value.
  // The ACE_ADAPT_RETVAL macro doesn't exactly do what
  // we need to do here, so we do it manually.
  if (retval != 0)
    {
      errno = retval;
      retval = -1;
    }

  return retval;
}

int
ACE_OS::thr_join (ACE_thread_t waiter_id,
                  ACE_thread_t *thr_id,
                  ACE_THR_FUNC_RETURN *status)
{
  thr_id = 0;
  return ACE_OS::thr_join (taskNameToId (waiter_id), status);
}
#endif /* VXWORKS */

int
ACE_OS::thr_key_detach (void *inst)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  if (ACE_TSS_Cleanup::lockable ())
    return ACE_TSS_Cleanup::instance()->detach (inst);
  else
    // We're in static constructor/destructor phase.  Don't
    // try to use the ACE_TSS_Cleanup instance because its lock
    // might not have been constructed yet, or might have been
    // destroyed already.  Just leak the key . . .
    return -1;
#else
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */
}

int
ACE_OS::thr_key_used (ACE_thread_key_t key)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  ACE_TSS_Cleanup::instance ()->key_used (key);
  return 0;
#else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION || ACE_PSOS_HAS_TSS */
}

#if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
int
ACE_OS::thr_keycreate (ACE_OS_thread_key_t *key,
# if defined (ACE_HAS_THR_C_DEST)
                       ACE_THR_C_DEST dest,
# else
                       ACE_THR_DEST dest,
# endif /* ACE_HAS_THR_C_DEST */
                       void *inst)
{
  // ACE_OS_TRACE ("ACE_OS::thr_keycreate");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_PTHREADS)
    ACE_UNUSED_ARG (inst);


#     if defined (ACE_HAS_PTHREADS_DRAFT4)
#       if defined (ACE_HAS_STDARG_THR_DEST)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#       else  /* ! ACE_HAS_STDARG_THR_DEST */
    ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#       endif /* ! ACE_HAS_STDARG_THR_DEST */
#     elif defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_key_create (key, dest), int, -1);
#     else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
#   elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest),
                                         ace_result_),
                       int, -1);
#   elif defined (ACE_HAS_WTHREADS)
    *key = ::TlsAlloc ();

    if (*key != ACE_SYSCALL_FAILED)
      {
        // Extract out the thread-specific table instance and stash away
        // the key and destructor so that we can free it up later on...
        return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
      }
    else
      ACE_FAIL_RETURN (-1);
      /* NOTREACHED */
#   endif /* ACE_HAS_STHREADS */
# else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (dest);
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}
#endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */

int
ACE_OS::thr_keycreate (ACE_thread_key_t *key,
#if defined (ACE_HAS_THR_C_DEST)
                       ACE_THR_C_DEST dest,
#else
                       ACE_THR_DEST dest,
#endif /* ACE_HAS_THR_C_DEST */
                       void *inst)
{
  // ACE_OS_TRACE ("ACE_OS::thr_keycreate");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_TSS_EMULATION)
    if (ACE_TSS_Emulation::next_key (*key) == 0)
      {
        ACE_TSS_Emulation::tss_destructor (*key, dest);

        // Extract out the thread-specific table instance and stash away
        // the key and destructor so that we can free it up later on...
        return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
      }
    else
      {
        errno = EAGAIN;
        return -1;
      }
# elif defined (ACE_HAS_PTHREADS)
    ACE_UNUSED_ARG (inst);

#   if defined (ACE_HAS_PTHREADS_DRAFT4)
#     if defined (ACE_HAS_STDARG_THR_DEST)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#     else  /* ! ACE_HAS_STDARG_THR_DEST */
    ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#     endif /* ! ACE_HAS_STDARG_THR_DEST */
#   elif defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_key_create (key, dest), int, -1);
#   else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
#   endif /* ACE_HAS_PTHREADS_DRAFT4 */

# elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest),
                                         ace_result_),
                       int, -1);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)

    static u_long unique_name = 0;
    void *tsdanchor;

    ++unique_name;
    if (::tsd_create (ACE_reinterpret_cast (char *, &unique_name),
                      0,
                      TSD_NOALLOC,
                      (void ****) &tsdanchor,
                      key) != 0)
      {
        return -1;
      }

    return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
# elif defined (ACE_HAS_WTHREADS)
    *key = ::TlsAlloc ();

    if (*key != ACE_SYSCALL_FAILED)
      {
        // Extract out the thread-specific table instance and stash away
        // the key and destructor so that we can free it up later on...
        return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
      }
    else
      ACE_FAIL_RETURN (-1);
      /* NOTREACHED */
# else
    ACE_UNUSED_ARG (key);
    ACE_UNUSED_ARG (dest);
    ACE_UNUSED_ARG (inst);
    ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_TSS_EMULATION */
#else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (dest);
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_keyfree (ACE_thread_key_t key)
{
  ACE_OS_TRACE ("ACE_OS::thr_keyfree");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_TSS_EMULATION)
    // Release the key in the TSS_Emulation administration
    ACE_TSS_Emulation::release_key (key);
    return ACE_TSS_Cleanup::instance ()->remove (key);
# elif defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_PTHREADS)
    return ::pthread_key_delete (key);
# elif defined (ACE_HAS_THR_KEYDELETE)
    return ::thr_keydelete (key);
# elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_WTHREADS)
    // Extract out the thread-specific table instance and free up
    // the key and destructor.
    ACE_TSS_Cleanup::instance ()->remove (key);
    ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::TlsFree (key), ace_result_), int, -1);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
    // Extract out the thread-specific table instance and free up
    // the key and destructor.
    ACE_TSS_Cleanup::instance ()->remove (key);
    return (::tsd_delete (key) == 0) ? 0 : -1;
# else
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_TSS_EMULATION */
#else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_setprio (const ACE_Sched_Priority prio)
{
  // Set the thread priority on the current thread.
  ACE_hthread_t my_thread_id;
  ACE_OS::thr_self (my_thread_id);

  int status = ACE_OS::thr_setprio (my_thread_id, prio);

#if defined (ACE_NEEDS_LWP_PRIO_SET)
  // If the thread is in the RT class, then set the priority on its
  // LWP.  (Instead of doing this if the thread is in the RT class, it
  // should be done for all bound threads.  But, there doesn't appear
  // to be an easy way to determine if the thread is bound.)

  if (status == 0)
    {
      // Find what scheduling class the thread's LWP is in.
      ACE_Sched_Params sched_params (ACE_SCHED_OTHER, 0);
      if (ACE_OS::lwp_getparams (sched_params) == -1)
        {
          return -1;
        }
      else if (sched_params.policy () == ACE_SCHED_FIFO  ||
               sched_params.policy () == ACE_SCHED_RR)
        {
          // This thread's LWP is in the RT class, so we need to set
          // its priority.
          sched_params.priority (prio);
          return ACE_OS::lwp_setparams (sched_params);
        }
      // else this is not an RT thread.  Nothing more needs to be
      // done.
    }
#endif /* ACE_NEEDS_LWP_PRIO_SET */

  return status;
}

#if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
int
ACE_OS::thr_setspecific (ACE_OS_thread_key_t key, void *data)
{
  // ACE_OS_TRACE ("ACE_OS::thr_setspecific");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_PTHREADS)
#     if defined (ACE_HAS_FSU_PTHREADS)
      // Call pthread_init() here to initialize threads package.  FSU
      // threads need an initialization before the first thread constructor.
      // This seems to be the one; however, a segmentation fault may
      // indicate that another pthread_init() is necessary, perhaps in
      // Synch.cpp or Synch_T.cpp.  FSU threads will not reinit if called
      // more than once, so another call to pthread_init will not adversely
      // affect existing threads.
      pthread_init ();
#     endif /*  ACE_HAS_FSU_PTHREADS */
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setspecific (key, data), ace_result_), int, -1);
#   elif defined (ACE_HAS_STHREADS)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_setspecific (key, data), ace_result_), int, -1);
#   elif defined (ACE_HAS_WTHREADS)
    ::TlsSetValue (key, data);
    return 0;
#   endif /* ACE_HAS_STHREADS */
# else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (data);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}
#endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */

int
ACE_OS::thr_setspecific (ACE_thread_key_t key, void *data)
{
  // ACE_OS_TRACE ("ACE_OS::thr_setspecific");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_TSS_EMULATION)
    ACE_KEY_INDEX (key_index, key);

    if (key_index >= ACE_TSS_Emulation::total_keys ())
      {
        errno = EINVAL;
        data = 0;
        return -1;
      }
    else
      {
        ACE_TSS_Emulation::ts_object (key) = data;
        ACE_TSS_Cleanup::instance ()->key_used (key);

        return 0;
      }
# elif defined (ACE_HAS_PTHREADS)
#   if defined (ACE_HAS_FSU_PTHREADS)
      // Call pthread_init() here to initialize threads package.  FSU
      // threads need an initialization before the first thread constructor.
      // This seems to be the one; however, a segmentation fault may
      // indicate that another pthread_init() is necessary, perhaps in
      // Synch.cpp or Synch_T.cpp.  FSU threads will not reinit if called
      // more than once, so another call to pthread_init will not adversely
      // affect existing threads.
      pthread_init ();
#   endif /*  ACE_HAS_FSU_PTHREADS */

#   if defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_setspecific (key, data), int, -1);
#   else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setspecific (key, data),
                                         ace_result_),
                       int, -1);
#   endif /* ACE_HAS_PTHREADS_DRAFT4, 6 */

# elif defined (ACE_HAS_STHREADS)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_setspecific (key, data), ace_result_), int, -1);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);
    if (::tsd_setval (key, tid, data) != 0)
     return -1;
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
# elif defined (ACE_HAS_WTHREADS)
    ::TlsSetValue (key, data);
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
# else
    ACE_UNUSED_ARG (key);
    ACE_UNUSED_ARG (data);
    ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (data);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

void
ACE_OS::unique_name (const void *object,
                     ACE_TCHAR *name,
                     size_t length)
{
  // The process ID will provide uniqueness between processes on the
  // same machine. The "this" pointer of the <object> will provide
  // uniqueness between other "live" objects in the same process. The
  // uniqueness of this name is therefore only valid for the life of
  // <object>.
  ACE_TCHAR temp_name[ACE_UNIQUE_NAME_LEN];
  ACE_OS::sprintf (temp_name,
                   ACE_LIB_TEXT ("%p%d"),
                   object,
                   ACE_static_cast (int, ACE_OS::getpid ()));
  ACE_OS::strsncpy (name,
                    temp_name,
                    length);
}

