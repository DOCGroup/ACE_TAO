// $Id$

#define ACE_BUILD_DLL

#include "ace/Object_Manager.h"
#include "ace/Token_Manager.h"
#if !defined (ACE_HAS_WINCE)
# include "ace/Naming_Context.h"
#endif /* !ACE_HAS_WINCE */
#include "ace/Service_Manager.h"
#include "ace/Service_Config.h"
#include "ace/Signal.h"
#include "ace/Log_Msg.h"
#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/Malloc.h"
#include "ace/Signal.h"

#if !defined (__ACE_INLINE__)
# include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Object_Manager, "$Id$")

#if ! defined (ACE_APPLICATION_PREALLOCATED_OBJECT_DEFINITIONS)
# define ACE_APPLICATION_PREALLOCATED_OBJECT_DEFINITIONS
#endif /* ACE_APPLICATION_PREALLOCATED_OBJECT_DEFINITIONS */

#if ! defined (ACE_APPLICATION_PREALLOCATED_ARRAY_DEFINITIONS)
# define ACE_APPLICATION_PREALLOCATED_ARRAY_DEFINITIONS
#endif /* ACE_APPLICATION_PREALLOCATED_ARRAY_DEFINITIONS */

#if ! defined (ACE_APPLICATION_PREALLOCATED_OBJECT_DELETIONS)
# define ACE_APPLICATION_PREALLOCATED_OBJECT_DELETIONS
#endif /* ACE_APPLICATION_PREALLOCATED_OBJECT_DELETIONS */

#if ! defined (ACE_APPLICATION_PREALLOCATED_ARRAY_DELETIONS)
# define ACE_APPLICATION_PREALLOCATED_ARRAY_DELETIONS
#endif /* ACE_APPLICATION_PREALLOCATED_ARRAY_DELETIONS */

// Static data.
ACE_Object_Manager *ACE_Object_Manager::instance_ = 0;

int ACE_Object_Manager::starting_up_ = 1;

int ACE_Object_Manager::shutting_down_ = 0;

u_int ACE_Object_Manager::initialized_ = 0;
// Flag to allow handling of multiple calls to init ().

void *ACE_Object_Manager::preallocated_object[
  ACE_Object_Manager::ACE_PREALLOCATED_OBJECTS] = { 0 };

void *ACE_Object_Manager::preallocated_array[
  ACE_Object_Manager::ACE_PREALLOCATED_ARRAYS] = { 0 };

// Handy macros for use by ACE_Object_Manager constructor to
// preallocate or delete an object or array, either statically (in
// global data) or dynamically (on the heap).
#if defined (ACE_HAS_STATIC_PREALLOCATION)
# define ACE_PREALLOCATE_OBJECT(TYPE, ID)\
    {\
      static ACE_Cleanup_Adapter<TYPE> obj;\
      preallocated_object[ID] = &obj;\
    }
# define ACE_PREALLOCATE_ARRAY(TYPE, ID, COUNT)\
    {\
      static ACE_Cleanup_Adapter<TYPE> obj[COUNT];\
      preallocated_array[ID] = &obj;\
    }
#else
# define ACE_PREALLOCATE_OBJECT(TYPE, ID)\
    {\
      ACE_Cleanup_Adapter<TYPE> *obj_p;\
      ACE_NEW_RETURN (obj_p, ACE_Cleanup_Adapter<TYPE>, -1);\
      preallocated_object[ID] = obj_p;\
    }
# define ACE_PREALLOCATE_ARRAY(TYPE, ID, COUNT)\
    {\
      ACE_Cleanup_Adapter<TYPE[COUNT]> *array_p;\
      ACE_NEW_RETURN (array_p, ACE_Cleanup_Adapter<TYPE[COUNT]>, -1);\
      preallocated_array[ID] = array_p;\
    }
# define ACE_DELETE_PREALLOCATED_OBJECT(TYPE, ID)\
    ace_cleanup_destroyer (\
      (ACE_Cleanup_Adapter<TYPE> *) preallocated_object[ID], 0);\
    preallocated_object[ID] = 0;
# define ACE_DELETE_PREALLOCATED_ARRAY(TYPE, ID, COUNT)\
    delete (ACE_Cleanup_Adapter<TYPE[COUNT]> *) preallocated_array[ID];\
    preallocated_array[ID] = 0;
#endif /* ACE_HAS_STATIC_PREALLOCATION */

class ACE_Object_Manager_Preallocations
{
  // = TITLE
  //   Performs preallocations of certain statically allocated
  //   services needed by ACE.
public:
  ACE_Object_Manager_Preallocations (void);
  ~ACE_Object_Manager_Preallocations (void);

private:
#if !defined (ACE_HAS_WINCE)
  ACE_Static_Svc_Descriptor ace_svc_desc_ACE_Naming_Context;
#endif /* !ACE_HAS_WINCE */
  ACE_Static_Svc_Descriptor ace_svc_desc_ACE_Service_Manager;
};

// We can't use the ACE_SVC_FACTORY_DECLARE macro here because this
// needs to be in the ACE_Export context rather than the
// ACE_Svc_Export context.
extern "C" ACE_Export
ACE_Service_Object *
_make_ACE_Service_Manager (ACE_Service_Object_Exterminator *);

ACE_Object_Manager_Preallocations::ACE_Object_Manager_Preallocations (void)
{
  // Define the static services.  This macro call creates static
  // service descriptors that are used for initialization below.
#if !defined (ACE_HAS_WINCE)
  ACE_STATIC_SVC_DEFINE (ACE_Naming_Context_initializer,
                         ASYS_TEXT ("ACE_Naming_Context"),
                         ACE_SVC_OBJ_T,
                         &ACE_SVC_NAME (ACE_Naming_Context),
                         ACE_Service_Type::DELETE_THIS |
                           ACE_Service_Type::DELETE_OBJ,
                         0)
#endif /* !ACE_HAS_WINCE */

  ACE_STATIC_SVC_DEFINE (ACE_Service_Manager_initializer,
                         ASYS_TEXT ("ACE_Service_Manager"),
                         ACE_SVC_OBJ_T,
                         &ACE_SVC_NAME (ACE_Service_Manager),
                         ACE_Service_Type::DELETE_THIS |
                           ACE_Service_Type::DELETE_OBJ,
                         0)

  // Initialize the static service objects using the descriptors created
  // above.
#if !defined (ACE_HAS_WINCE)
  ace_svc_desc_ACE_Naming_Context =
    ace_svc_desc_ACE_Naming_Context_initializer;
#endif /* !ACE_HAS_WINCE */

  ace_svc_desc_ACE_Service_Manager =
    ace_svc_desc_ACE_Service_Manager_initializer;

  // Add to the list of static configured services.
#if !defined (ACE_HAS_WINCE)
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_ACE_Naming_Context);
#endif /* !ACE_HAS_WINCE */

  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_ACE_Service_Manager);
}

ACE_Object_Manager_Preallocations::~ACE_Object_Manager_Preallocations (void)
{
}

ACE_Object_Manager_Base::ACE_Object_Manager_Base (void)
  : registered_objects_ (0)
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  , internal_lock_ (new ACE_Recursive_Thread_Mutex)
  , singleton_null_lock_ (0)
  , singleton_thread_locks_ (0)
  , singleton_mutex_locks_ (0)
  , singleton_recursive_lock_ (0)
  , singleton_rw_locks_ (0)
# endif /* ACE_MT_SAFE */
{
}

int
ACE_Object_Manager::init (void)
{
  if (initialized_ == 0)
    {
      initialized_ = 1;

      if (instance_ == 0)
        {
          // Allocate the ACE_Object_Manager instance on the heap.  Assume
          // that the application will call fini () to destroy it.
          ACE_Object_Manager::instance ();
        }

      // Allocate the preallocated (hard-coded) object instances.
      ACE_PREALLOCATE_OBJECT (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_LOCK)
#     if defined (ACE_HAS_THREADS)
        ACE_PREALLOCATE_OBJECT (ACE_Recursive_Thread_Mutex,
                                ACE_STATIC_OBJECT_LOCK)
#     endif /* ACE_HAS_THREADS */
#     if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
        ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_LOG_MSG_INSTANCE_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_MT_CORBA_HANDLER_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_DUMP_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_OS_MONITOR_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Recursive_Thread_Mutex,
                                ACE_SIG_HANDLER_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Null_Mutex, ACE_SINGLETON_NULL_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Recursive_Thread_Mutex,
                                ACE_SINGLETON_RECURSIVE_THREAD_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_THREAD_EXIT_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_TOKEN_CONST::MUTEX,
                                ACE_TOKEN_MANAGER_CREATION_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_TOKEN_CONST::MUTEX,
                                ACE_TOKEN_INVARIANTS_CREATION_LOCK)
        ACE_PREALLOCATE_OBJECT (ACE_Recursive_Thread_Mutex,
                                ACE_TSS_CLEANUP_LOCK)
#       if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
          ACE_PREALLOCATE_OBJECT (ACE_Recursive_Thread_Mutex,
                                  ACE_TSS_BASE_LOCK)
#       endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
#     endif /* ACE_MT_SAFE */

      // Do this after the allocation of ACE_STATIC_OBJECT_LOCK.  It
      // shouldn't matter, but just in case
      // ACE_Static_Object_Lock::instance () gets changed . . .
      ACE_NEW_RETURN (instance_->registered_objects_,
                      ACE_Unbounded_Queue<ACE_Cleanup_Info>, -1);

      // Hooks for preallocated objects and arrays provided by application.
      ACE_APPLICATION_PREALLOCATED_OBJECT_DEFINITIONS
      ACE_APPLICATION_PREALLOCATED_ARRAY_DEFINITIONS

#     if defined (ACE_HAS_TSS_EMULATION)
        // Initialize the main thread's TS storage.
        ACE_TSS_Emulation::tss_open (instance_->ts_storage_);
#     endif /* ACE_HAS_TSS_EMULATION */

      // Open Winsock (no-op on other platforms).
      ACE_OS::socket_init (ACE_WSOCK_VERSION);

      ACE_NEW_RETURN (instance_->preallocations_,
        ACE_Object_Manager_Preallocations, -1);

      // Open the main thread's ACE_Log_Msg.
      (void) ACE_LOG_MSG;

      ACE_NEW_RETURN (instance_->default_mask_,
        ACE_Sig_Set (1), -1);

      // Finally, indicate that the ACE_Object_Manager instance has
      // been constructed.
      instance_->starting_up_ = 0;

      return 0;
    } else {
      // Had already initialized.
      return -1;
    }
}

int
ACE_Object_Manager::fini (void)
{
  if (instance_ == 0  ||  instance_->shutting_down_ == 1)
    // Too late.  Or, maybe too early.  Either fini () has already
    // been called, or init () was never called.
    return -1;

  // No mutex here.  Only the main thread should destroy the singleton
  // ACE_Object_Manager instance.

  // First, indicate that the ACE_Object_Manager instance is (being)
  // destroyed.  If an object tries to register after this, it will be
  // refused.
  instance_->shutting_down_ = 1;

  ACE_Trace::stop_tracing ();

  ACE_Cleanup_Info info;

  // Close and possibly delete all service instances in the Service
  // Repository.
  ACE_Service_Config::fini_svcs ();

  // Call all registered cleanup hooks, in reverse order of
  // registration.
  while (instance_->registered_objects_ &&
         instance_->registered_objects_->dequeue_head (info) != -1)
    {
      if (info.cleanup_hook_ == (ACE_CLEANUP_FUNC) ace_cleanup_destroyer)
        // The object is an ACE_Cleanup.
        ace_cleanup_destroyer ((ACE_Cleanup *) info.object_, info.param_);
      else
        (*info.cleanup_hook_) (info.object_, info.param_);
    }

  // Close the main thread's TSS, including its Log_Msg instance.
  ACE_OS::cleanup_tss (1 /* main thread */);

  //
  // Note:  Do not access Log Msg after this since it is gone
  //

  // Unlink all services in the Service Repository and close/delete
  // all ACE library services and singletons.
  ACE_Service_Config::close ();

  // Close down Winsock (no-op on other platforms).
  ACE_OS::socket_fini ();

  delete instance_->preallocations_;
  instance_->preallocations_ = 0;

  // Close the ACE_Allocator.
  ACE_Allocator::close_singleton ();

#if ! defined (ACE_HAS_STATIC_PREALLOCATION)
  // Hooks for deletion of preallocated objects and arrays provided by
  // application.
  ACE_APPLICATION_PREALLOCATED_ARRAY_DELETIONS
    ACE_APPLICATION_PREALLOCATED_OBJECT_DELETIONS

    // Cleanup the dynamically preallocated arrays.
    // (none)

    // Cleanup the dynamically preallocated objects.
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_LOCK)
#if defined (ACE_HAS_THREADS)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Recursive_Thread_Mutex,
                                    ACE_STATIC_OBJECT_LOCK)
#endif /* ACE_HAS_THREADS */
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex,
                                    ACE_LOG_MSG_INSTANCE_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex,
                                    ACE_MT_CORBA_HANDLER_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_DUMP_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_OS_MONITOR_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Recursive_Thread_Mutex,
                                    ACE_SIG_HANDLER_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Null_Mutex, ACE_SINGLETON_NULL_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Recursive_Thread_Mutex,
                                    ACE_SINGLETON_RECURSIVE_THREAD_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_THREAD_EXIT_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_TOKEN_CONST::MUTEX,
                                    ACE_TOKEN_MANAGER_CREATION_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_TOKEN_CONST::MUTEX,
                                    ACE_TOKEN_INVARIANTS_CREATION_LOCK)
    ACE_DELETE_PREALLOCATED_OBJECT (ACE_Recursive_Thread_Mutex,
                                    ACE_TSS_CLEANUP_LOCK)
#     if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
      ACE_DELETE_PREALLOCATED_OBJECT (ACE_Recursive_Thread_Mutex,
                                      ACE_TSS_BASE_LOCK)
#     endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
# endif /* ACE_MT_SAFE */
#endif /* ! ACE_HAS_STATIC_PREALLOCATION */

#if defined (ACE_HAS_THREADS)
  ACE_Static_Object_Lock::cleanup_lock ();
#endif /* ACE_HAS_THREADS */

  delete instance_->default_mask_;
  instance_->default_mask_ = 0;

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
  if (instance_->dynamically_allocated_)
    {
      delete instance_;
      instance_ = 0;
    }
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

  return 0;
}

ACE_Object_Manager::ACE_Object_Manager (void)
  // With ACE_HAS_TSS_EMULATION, ts_storage_ is initialized by the call to
  // ACE_OS::tss_open () in the function body.
  : dynamically_allocated_ (0)
  , preallocations_ (0)
  , default_mask_ (0)
  , ace_service_config_sig_handler_ (0)
{
  if (instance_ == 0)
    {
      // Store the address of the instance so that instance () doesn't
      // allocate a new one when called.
      instance_ = this;

      // Construct the ACE_Service_Config's signal handler.
      ACE_NEW (ace_service_config_sig_handler_,
               ACE_Sig_Adapter (&ACE_Service_Config::handle_signal));
      ACE_Service_Config::signal_handler (ace_service_config_sig_handler_);

      init ();
    }
  // else if instance_ was not 0, then then another
  // ACE_Object_Manager has already been instantiated.
  // Don't do anything, so that it will own all
  // ACE_Object_Manager resources.
}

ACE_Object_Manager *
ACE_Object_Manager::instance (void)
{
  // This function should be called during construction of static
  // instances, or before any other threads have been created in
  // the process.  So, it's not thread safe.

  if (instance_ == 0)
    {
      ACE_Object_Manager *instance_pointer;

      ACE_NEW_RETURN (instance_pointer, ACE_Object_Manager, 0);
      ACE_ASSERT (instance_pointer == instance_);

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
      instance_pointer->dynamically_allocated_ = 1;
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

      return instance_pointer;
    }
  else
    {
      return instance_;
    }
}

int
ACE_Object_Manager::starting_up ()
{
  return starting_up_;
}

int
ACE_Object_Manager::shutting_down ()
{
  return shutting_down_;
}

ACE_Sig_Set &
ACE_Object_Manager::default_mask (void)
{
  return *ACE_Object_Manager::instance ()->default_mask_;
}

int
ACE_Object_Manager::at_exit_i (void *object,
                               ACE_CLEANUP_FUNC cleanup_hook,
                               void *param)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
    *instance_->internal_lock_, -1));

  if (shutting_down ())
    {
      errno = EAGAIN;
      return -1;
    }

  // Check for already in queue, and return 1 if so.
  ACE_Cleanup_Info *info = 0;
  for (ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info>
         iter (*registered_objects_);
       iter.next (info) != 0;
       iter.advance ())
    {
      if (info->object_ == object)
        {
          // The object has already been registered.
          errno = EEXIST;
          return -1;
        }
    }

  ACE_Cleanup_Info new_info;
  new_info.object_ = object;
  new_info.cleanup_hook_ = cleanup_hook;
  new_info.param_ = param;

  // Returns -1 and sets errno if unable to allocate storage.  Enqueue
  // at the head and dequeue from the head to get LIFO ordering.
  return registered_objects_->enqueue_head (new_info);
}

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)

int
ACE_Object_Manager_Base::get_singleton_lock (ACE_Null_Mutex *&lock)
{
  if (ACE_Object_Manager::starting_up ()  ||
      ACE_Object_Manager::shutting_down ())
    {
      // The preallocated lock has not been constructed yet.
      // Therefore, the program is single-threaded at this point.  Or,
      // the ACE_Object_Manager instance has been destroyed, so the
      // preallocated lock is not available.  Allocate a lock to use,
      // for interface compatibility, though there should be no
      // contention on it.
      if (ACE_Object_Manager::instance ()->singleton_null_lock_ == 0)
        {
          ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                            singleton_null_lock_,
                          ACE_Cleanup_Adapter<ACE_Null_Mutex>,
                          -1);

          // Can't register with the ACE_Object_Manager here!  The
          // lock's declaration is visible to the ACE_Object_Manager
          // destructor, so it will clean it up as a special case.
        }

      if (ACE_Object_Manager::instance ()->singleton_null_lock_ != 0)
        lock = &ACE_Object_Manager::instance ()->singleton_null_lock_->
          object ();
    }
  else
    // Use the Object_Manager's preallocated lock.
    lock = ACE_Managed_Object<ACE_Null_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_SINGLETON_NULL_LOCK);

  return 0;
}

int
ACE_Object_Manager_Base::get_singleton_lock (ACE_Thread_Mutex *&lock)
{
  if (lock == 0)
    {
      if (ACE_Object_Manager::starting_up ()  ||
          ACE_Object_Manager::shutting_down ())
        {
          // The Object_Manager and its internal lock have not been
          // constructed yet.  Therefore, the program is single-
          // threaded at this point.  Or, the ACE_Object_Manager
          // instance has been destroyed, so the internal lock is not
          // available.  Either way, we can not use double-checked
          // locking.

          ACE_NEW_RETURN (lock, ACE_Thread_Mutex, -1);

          // Add the new lock to the array of locks to be deleted
          // at program termination.
          if (ACE_Object_Manager::instance ()->singleton_thread_locks_ == 0)
            {
              // Create the array, then insert the new lock.
              ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                                singleton_thread_locks_,
                              ACE_Array<ACE_Thread_Mutex *> (
                                (size_t) 1,
                                (ACE_Thread_Mutex *) 0),
                              -1);
              (*ACE_Object_Manager::instance ()->singleton_thread_locks_)[0] =
                 lock;
            }
          else
            {
              // Grow the array, then insert the new lock.

              // Copy the array pointer.
              ACE_Array<ACE_Thread_Mutex *> *tmp =
                ACE_Object_Manager::instance ()->singleton_thread_locks_;

              // Create a new array with one more slot than the current one.
              ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                                singleton_thread_locks_,
                              ACE_Array<ACE_Thread_Mutex *> (
                                tmp->size () + (size_t) 1,
                                (ACE_Thread_Mutex *) 0),
                              -1);

              // Copy the old array to the new array.
              for (u_int i = 0; i < tmp->size (); ++i)
                (*ACE_Object_Manager::instance ()->
                   singleton_thread_locks_)[i] = (*tmp) [i];

              // Insert the new lock at the end of the array.
              (*ACE_Object_Manager::instance ()->singleton_thread_locks_)
                 [tmp->size ()] = lock;

              delete tmp;
            }
        }
      else
        {
          // Allocate a new lock, but use double-checked locking to
          // ensure that only one thread allocates it.
          ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex,
                                    ace_mon,
                                    *ACE_Object_Manager::instance ()->
                                      internal_lock_,
                                    -1));

          if (lock == 0)
            {
              ACE_Cleanup_Adapter<ACE_Thread_Mutex> *lock_adapter;
              ACE_NEW_RETURN (lock_adapter,
                              ACE_Cleanup_Adapter<ACE_Thread_Mutex>,
                              -1);
              lock = &lock_adapter->object ();

              // Register the lock for destruction at program
              // termination.  This call will cause us to grab the
              // ACE_Object_Manager::instance ()->internal_lock_
              // again; that's why it is a recursive lock.
              ACE_Object_Manager::at_exit (lock_adapter);
            }
        }
    }

  return 0;
}

int
ACE_Object_Manager_Base::get_singleton_lock (ACE_Mutex *&lock)
{
  if (lock == 0)
    {
      if (ACE_Object_Manager::starting_up ()  ||
          ACE_Object_Manager::shutting_down ())
        {
          // The Object_Manager and its internal lock have not been
          // constructed yet.  Therefore, the program is single-
          // threaded at this point.  Or, the ACE_Object_Manager
          // instance has been destroyed, so the internal lock is not
          // available.  Either way, we can not use double-checked
          // locking.

          ACE_NEW_RETURN (lock, ACE_Mutex, -1);

          // Add the new lock to the array of locks to be deleted
          // at program termination.
          if (ACE_Object_Manager::instance ()->singleton_mutex_locks_ == 0)
            {
              // Create the array, then insert the new lock.
              ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                                singleton_mutex_locks_,
                              ACE_Array<ACE_Mutex *> (
                                (size_t) 1,
                                (ACE_Mutex *) 0),
                              -1);
              (*ACE_Object_Manager::instance ()->singleton_mutex_locks_)[0] =
                 lock;
            }
          else
            {
              // Grow the array, then insert the new lock.

              // Copy the array pointer.
              ACE_Array<ACE_Mutex *> *tmp =
                ACE_Object_Manager::instance ()->singleton_mutex_locks_;

              // Create a new array with one more slot than the current one.
              ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                                singleton_mutex_locks_,
                              ACE_Array<ACE_Mutex *> (
                                tmp->size () + (size_t) 1,
                                (ACE_Mutex *) 0),
                              -1);

              // Copy the old array to the new array.
              for (u_int i = 0; i < tmp->size (); ++i)
                (*ACE_Object_Manager::instance ()->singleton_mutex_locks_)[i] =
                   (*tmp) [i];

              // Insert the new lock at the end of the array.
              (*ACE_Object_Manager::instance ()->singleton_mutex_locks_)
                 [tmp->size ()] = lock;

              delete tmp;
            }
        }
      else
        {
          // Allocate a new lock, but use double-checked locking to
          // ensure that only one thread allocates it.
          ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex,
                                    ace_mon,
                                    *ACE_Object_Manager::instance ()->
                                      internal_lock_,
                                    -1));

          if (lock == 0)
            {
              ACE_Cleanup_Adapter<ACE_Mutex> *lock_adapter;
              ACE_NEW_RETURN (lock_adapter,
                              ACE_Cleanup_Adapter<ACE_Mutex>,
                              -1);
              lock = &lock_adapter->object ();

              // Register the lock for destruction at program
              // termination.  This call will cause us to grab the
              // ACE_Object_Manager::instance ()->internal_lock_
              // again; that's why it is a recursive lock.
              ACE_Object_Manager::at_exit (lock_adapter);
            }
        }
    }

  return 0;
}

int
ACE_Object_Manager_Base::get_singleton_lock (ACE_Recursive_Thread_Mutex *&lock)
{
  if (ACE_Object_Manager::starting_up ()  ||
      ACE_Object_Manager::shutting_down ())
    {
      // The preallocated lock has not been constructed yet.
      // Therefore, the program is single-threaded at this point.  Or,
      // the ACE_Object_Manager instance has been destroyed, so the
      // preallocated lock is not available.  Allocate a lock to use,
      // for interface compatibility, though there should be no
      // contention on it.
      if (ACE_Object_Manager::instance ()->singleton_recursive_lock_ == 0)
        ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                          singleton_recursive_lock_,
                        ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>,
                        -1);

      // Can't register with the ACE_Object_Manager here!  The lock's
      // declaration is visible to the ACE_Object_Manager destructor,
      // so it will clean it up as a special case.

      if (ACE_Object_Manager::instance ()->singleton_recursive_lock_ != 0)
        lock = &ACE_Object_Manager::instance ()->singleton_recursive_lock_->
          object ();
    }
  else
    // Use the Object_Manager's preallocated lock.
    lock = ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::
    get_preallocated_object (ACE_Object_Manager::
                             ACE_SINGLETON_RECURSIVE_THREAD_LOCK);

  return 0;
}

int
ACE_Object_Manager_Base::get_singleton_lock (ACE_RW_Thread_Mutex *&lock)
{
  if (lock == 0)
    {
      if (ACE_Object_Manager::starting_up ()  ||
          ACE_Object_Manager::shutting_down ())
        {
          // The Object_Manager and its internal lock have not been
          // constructed yet.  Therefore, the program is single-
          // threaded at this point.  Or, the ACE_Object_Manager
          // instance has been destroyed, so the internal lock is not
          // available.  Either way, we can not use double-checked
          // locking.

          ACE_NEW_RETURN (lock, ACE_RW_Thread_Mutex, -1);

          // Add the new lock to the array of locks to be deleted
          // at program termination.
          if (ACE_Object_Manager::instance ()->singleton_rw_locks_ == 0)
            {
              // Create the array, then insert the new lock.
              ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                                singleton_rw_locks_,
                              ACE_Array<ACE_RW_Thread_Mutex *> (
                                (size_t) 1,
                                (ACE_RW_Thread_Mutex *) 0),
                              -1);
              (*ACE_Object_Manager::instance ()->singleton_rw_locks_)[0] =
                 lock;
            }
          else
            {
              // Grow the array, then insert the new lock.

              // Copy the array pointer.
              ACE_Array<ACE_RW_Thread_Mutex *> *tmp =
                ACE_Object_Manager::instance ()->singleton_rw_locks_;

              // Create a new array with one more slot than the current one.
              ACE_NEW_RETURN (ACE_Object_Manager::instance ()->
                                singleton_rw_locks_,
                              ACE_Array<ACE_RW_Thread_Mutex *> (
                                tmp->size () + (size_t) 1,
                                (ACE_RW_Thread_Mutex *) 0),
                              -1);

              // Copy the old array to the new array.
              for (u_int i = 0; i < tmp->size (); ++i)
                (*ACE_Object_Manager::instance ()->singleton_rw_locks_)[i] =
                   (*tmp) [i];

              // Insert the new lock at the end of the array.
              (*ACE_Object_Manager::instance ()->singleton_rw_locks_)
                 [tmp->size ()] = lock;

              delete tmp;
            }
        }
      else
        {
          // Allocate a new lock, but use double-checked locking to
          // ensure that only one thread allocates it.
          ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex,
                                    ace_mon,
                                    *ACE_Object_Manager::instance ()->
                                      internal_lock_,
                                    -1));

          if (lock == 0)
            {
              ACE_Cleanup_Adapter<ACE_RW_Thread_Mutex> *lock_adapter;
              ACE_NEW_RETURN (lock_adapter,
                              ACE_Cleanup_Adapter<ACE_RW_Thread_Mutex>,
                              -1);
              lock = &lock_adapter->object ();


              // Register the lock for destruction at program
              // termination.  This call will cause us to grab the
              // ACE_Object_Manager::instance ()->internal_lock_
              // again; that's why it is a recursive lock.
              ACE_Object_Manager::at_exit (lock_adapter);
            }
        }
    }

  return 0;
}
#endif /* ACE_MT_SAFE */

// NOTE:  this function needs to appear _after_ the
// get_singleton_lock () functions in order to compile with
// g++ 2.7.2.3.
ACE_Object_Manager_Base::~ACE_Object_Manager_Base (void)
{
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  delete internal_lock_;
  internal_lock_ = 0;

  delete singleton_null_lock_;
  singleton_null_lock_ = 0;

  delete singleton_thread_locks_;
  singleton_thread_locks_ = 0;

  delete singleton_mutex_locks_;
  singleton_mutex_locks_ = 0;

  delete singleton_recursive_lock_;
  singleton_recursive_lock_ = 0;

  delete singleton_rw_locks_;
  singleton_rw_locks_ = 0;
#endif /* ACE_MT_SAFE */

  delete registered_objects_;
  registered_objects_ = 0;
}

ACE_Object_Manager::~ACE_Object_Manager (void)
{
#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
  // Clear the flag so that fini () doesn't delete again.
  dynamically_allocated_ = 0;
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

  if (fini () != -1)
    {
      delete ace_service_config_sig_handler_;
      ace_service_config_sig_handler_ = 0;
    }
  // else fini () had already been called.  It would be safe
  // to delete the 0 pointers again, but this allows us to
  // add other code to the block above.
}

#if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
class ACE_Export ACE_Object_Manager_Destroyer
  // = TITLE
  //    Ensure that the <ACE_Object_Manager> gets initialized before any
  //    application threads have been spawned, and destroyed at program
  //    termination.
  //
  // = DESCRIPTION
  //    Without ACE_HAS_NONSTATIC_OBJECT_MANAGER, a static instance of this
  //    class is created.  Therefore, it gets created before main ()
  //    is called.  And it gets destroyed after main () returns.
{
public:
  ACE_Object_Manager_Destroyer (void);
  ~ACE_Object_Manager_Destroyer (void);

private:
  ACE_thread_t saved_main_thread_id_;
  // Save the main thread ID, so that destruction can be suppressed.
};

ACE_Object_Manager_Destroyer::ACE_Object_Manager_Destroyer (void)
  : saved_main_thread_id_ (ACE_OS::thr_self ())
{
  // Ensure that the Object_Manager gets initialized before any
  // application threads have been spawned.  Because this will be called
  // during construction of static objects, that should always be the
  // case.
  (void) ACE_Object_Manager::instance ();
}

ACE_Object_Manager_Destroyer::~ACE_Object_Manager_Destroyer (void)
{
  if (ACE_OS::thr_equal (ACE_OS::thr_self (),
                         saved_main_thread_id_))
    {
      delete ACE_Object_Manager::instance_;
      ACE_Object_Manager::instance_ = 0;
    }
  // else if this destructor is not called by the main thread, then do
  // not delete the ACE_Object_Manager.  That causes problems, on
  // WIN32 at least.
}

static ACE_Object_Manager_Destroyer ACE_Object_Manager_Destroyer_internal;
#endif /* ! ACE_HAS_NONSTATIC_OBJECT_MANAGER */

#if defined (ACE_HAS_THREADS)

// This global so that it doesn't have to be declared in the
// header file.  That would cause nasty circular include problems.
static ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex> *
ACE_Static_Object_Lock_lock = 0;

ACE_Recursive_Thread_Mutex *
ACE_Static_Object_Lock::instance (void)
{
  if (ACE_Object_Manager::starting_up ()  ||
      ACE_Object_Manager::shutting_down ())
    {
      // The preallocated ACE_STATIC_OBJECT_LOCK has not been
      // constructed yet.  Therefore, the program is single-threaded
      // at this point.  Or, the ACE_Object_Manager instance has been
      // destroyed, so the preallocated lock is not available.
      // Allocate a lock to use, for interface compatibility, though
      // there should be no contention on it.
      if (ACE_Static_Object_Lock_lock == 0)
        ACE_NEW_RETURN (ACE_Static_Object_Lock_lock,
                        ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>,
                        0);

      // Can't register with the ACE_Object_Manager here!  The lock's
      // declaration is visible to the ACE_Object_Manager destructor,
      // so it will clean it up as a special case.

      return &ACE_Static_Object_Lock_lock->object ();
    }
  else
    // Return the preallocated ACE_STATIC_OBJECT_LOCK.
    return
      ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
        (ACE_Object_Manager::ACE_STATIC_OBJECT_LOCK);
}

void
ACE_Static_Object_Lock::cleanup_lock (void)
{
  delete ACE_Static_Object_Lock_lock;
  ACE_Static_Object_Lock_lock = 0;
}
#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
    template class ACE_Array<ACE_Thread_Mutex *>;
    template class ACE_Array_Base<ACE_Thread_Mutex *>;
    template class ACE_Array<ACE_Mutex *>;
    template class ACE_Array_Base<ACE_Mutex *>;
    template class ACE_Array<ACE_RW_Thread_Mutex *>;
    template class ACE_Array_Base<ACE_RW_Thread_Mutex *>;
    template class ACE_Cleanup_Adapter<ACE_Null_Mutex>;
    template class ACE_Cleanup_Adapter<ACE_Mutex>;
    template class ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>;
    template class ACE_Cleanup_Adapter<ACE_Thread_Mutex>;
    template class ACE_Managed_Object<ACE_Null_Mutex>;
    template class ACE_Managed_Object<ACE_Mutex>;
    template class ACE_Managed_Object<ACE_Recursive_Thread_Mutex>;
    template class ACE_Managed_Object<ACE_Thread_Mutex>;
# endif /* ACE_MT_SAFE */
  template class ACE_Cleanup_Adapter<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Unbounded_Queue<ACE_Cleanup_Info>;
  template class ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info>;
  template class ACE_Node<ACE_Cleanup_Info>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   pragma instantiate ACE_Array<ACE_Thread_Mutex *>
#   pragma instantiate ACE_Array_Base<ACE_Thread_Mutex *>
#   pragma instantiate ACE_Array<ACE_Mutex *>
#   pragma instantiate ACE_Array_Base<ACE_Mutex *>
#   pragma instantiate ACE_Array<ACE_RW_Thread_Mutex *>
#   pragma instantiate ACE_Array_Base<ACE_RW_Thread_Mutex *>
#   pragma instantiate ACE_Cleanup_Adapter<ACE_Null_Mutex>
#   pragma instantiate ACE_Cleanup_Adapter<ACE_Mutex>
#   pragma instantiate ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>
#   pragma instantiate ACE_Cleanup_Adapter<ACE_Thread_Mutex>
#   pragma instantiate ACE_Managed_Object<ACE_Null_Mutex>
#   pragma instantiate ACE_Managed_Object<ACE_Mutex>
#   pragma instantiate ACE_Managed_Object<ACE_Recursive_Thread_Mutex>
#   pragma instantiate ACE_Managed_Object<ACE_Thread_Mutex>
# endif /* ACE_MT_SAFE */
# pragma instantiate ACE_Cleanup_Adapter<ACE_SYNCH_RW_MUTEX>
# pragma instantiate ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>
# pragma instantiate ACE_Unbounded_Queue<ACE_Cleanup_Info>
# pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info>
# pragma instantiate ACE_Node<ACE_Cleanup_Info>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
