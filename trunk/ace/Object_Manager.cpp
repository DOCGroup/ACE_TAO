// $Id$

#define ACE_BUILD_DLL

#include "ace/Object_Manager.h"
#include "ace/Containers.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"
#include "ace/Filecache.h"
#include "ace/Token_Manager.h"
#include "ace/Synch.h"
#include "ace/Malloc.h"

#if !defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

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

void *ACE_Object_Manager::managed_object[ACE_MAX_MANAGED_OBJECTS] = { 0 };

u_int ACE_Object_Manager::next_managed_object = 0;

void *ACE_Object_Manager::preallocated_object[
  ACE_Object_Manager::ACE_PREALLOCATED_OBJECTS] = { 0 };

void *ACE_Object_Manager::preallocated_array[
  ACE_Object_Manager::ACE_PREALLOCATED_ARRAYS] = { 0 };

// Handy macros for use by ACE_Object_Manager constructor to preallocate or
// delete an object or array, either statically (in global data) or
// dynamically (on the heap).
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
      ACE_NEW (obj_p, ACE_Cleanup_Adapter<TYPE>);\
      preallocated_object[ID] = obj_p;\
    }
# define ACE_PREALLOCATE_ARRAY(TYPE, ID, COUNT)\
    {\
      ACE_Cleanup_Adapter<TYPE[COUNT]> *array_p;\
      ACE_NEW (array_p, ACE_Cleanup_Adapter<TYPE[COUNT]>);\
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


ACE_Object_Manager::ACE_Object_Manager (void)
  : shutting_down_(0)
  // , lock_ is initialized in the function body.
  // With ACE_HAS_TSS_EMULATION, ts_storage_ is initialized by the call
  // to ACE_OS::tss_open () in the function body.
{
  ACE_NEW (registered_objects_, ACE_Unbounded_Queue<ACE_Cleanup_Info>);

  ACE_MT (ACE_NEW (lock_, ACE_Thread_Mutex));

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
  // Store the address of this static instance so that instance ()
  // doesn't allocate a new one when called.
  instance_ = this;
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

  // Allocate the preallocated (hard-coded) object instances.
  ACE_PREALLOCATE_OBJECT (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_LOCK)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_LOG_MSG_INSTANCE_LOCK)
  ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_MT_CORBA_HANDLER_LOCK)
  ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_DUMP_LOCK)
  ACE_PREALLOCATE_OBJECT (ACE_Recursive_Thread_Mutex, ACE_SIG_HANDLER_LOCK)
  ACE_PREALLOCATE_OBJECT (ACE_TOKEN_CONST::MUTEX,
                          ACE_TOKEN_MANAGER_CREATION_LOCK)
  ACE_PREALLOCATE_OBJECT (ACE_Thread_Mutex, ACE_TSS_CLEANUP_LOCK)
# endif /* ACE_MT_SAFE */

  ACE_PREALLOCATE_ARRAY (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_FILE_LOCK,
                         ACE_Filecache::DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE)
  ACE_PREALLOCATE_ARRAY (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_HASH_LOCK,
                         ACE_Filecache::DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE)

  // Hooks for preallocated objects and arrays provided by application.
  ACE_APPLICATION_PREALLOCATED_OBJECT_DEFINITIONS
  ACE_APPLICATION_PREALLOCATED_ARRAY_DEFINITIONS

#if defined (ACE_HAS_TSS_EMULATION)
  // Initialize the main thread's TS storage.
  ACE_TSS_Emulation::tss_open (ts_storage_);
#endif /* ACE_HAS_TSS_EMULATION */
}

ACE_Object_Manager::~ACE_Object_Manager (void)
{
  // No mutex here.  Only the main thread should destroy the
  // singleton ACE_Object_Manager instance.

  ACE_Cleanup_Info info;

  // Call all registered cleanup hooks, in reverse order of
  // registration.  Before starting, mark this object as being
  // destroyed - then if during the course of shutting things down,
  // some object tries to register, it won't be.
  shutting_down_ = 1;
  ACE_Trace::stop_tracing ();
  while (registered_objects_ &&
         registered_objects_->dequeue_head (info) != -1)
    {
      if (info.cleanup_hook_ == (ACE_CLEANUP_FUNC) ace_cleanup_destroyer)
        {
          // The object is an ACE_Cleanup.
          ace_cleanup_destroyer ((ACE_Cleanup *) info.object_, info.param_);
        }
      else
        {
          (*info.cleanup_hook_) (info.object_, info.param_);
        }
    }

  // This call closes and deletes all ACE library services and
  // singletons.  This closes the ACE_Thread_Manager, which cleans
  // up all running threads.
  ACE_Service_Config::close ();

  // Close the main thread's TSS, including its Log_Msg instance.
  ACE_OS::cleanup_tss ();

  ACE_MT (delete lock_;  lock_ = 0);

  delete registered_objects_;
  registered_objects_ = 0;

  // Close the ACE_Allocator.
  ACE_Allocator::close_singleton ();

# if defined (ACE_HAS_THREADS)
  // Close the ACE_Allocator and ACE_Static_Object_Lock.
  ACE_Static_Object_Lock::close_singleton ();
# endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_TSS_EMULATION)
  // Close the thread's local TS storage.
  ACE_TSS_Emulation::tss_close (ts_storage_);
#endif /* ACE_HAS_TSS_EMULATION */

#if ! defined (ACE_HAS_STATIC_PREALLOCATION)
  // Hooks for deletion of preallocated objects and arrays provided by
  // application.
  ACE_APPLICATION_PREALLOCATED_ARRAY_DELETIONS
  ACE_APPLICATION_PREALLOCATED_OBJECT_DELETIONS

  // Cleanup the dynamically preallocated arrays.
  ACE_DELETE_PREALLOCATED_ARRAY (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_FILE_LOCK,
    ACE_Filecache::DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE)
  ACE_DELETE_PREALLOCATED_ARRAY (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_HASH_LOCK,
    ACE_Filecache::DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE)

  // Cleanup the dynamically preallocated objects.
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_SYNCH_RW_MUTEX, ACE_FILECACHE_LOCK)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_LOG_MSG_INSTANCE_LOCK)
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_MT_CORBA_HANDLER_LOCK)
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_DUMP_LOCK)
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_Recursive_Thread_Mutex,
                                  ACE_SIG_HANDLER_LOCK)
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_TOKEN_CONST::MUTEX,
                                  ACE_TOKEN_MANAGER_CREATION_LOCK)
  ACE_DELETE_PREALLOCATED_OBJECT (ACE_Thread_Mutex, ACE_TSS_CLEANUP_LOCK)
# endif /* ACE_MT_SAFE */
#endif /* ! ACE_HAS_STATIC_PREALLOCATION */
}

ACE_Object_Manager *
ACE_Object_Manager::instance (void)
{
  // This function should be call during construction of static
  // instances, so it's not thread safe.

  if (instance_ == 0)
    ACE_NEW_RETURN (instance_, ACE_Object_Manager, 0);

  return instance_;
}

int
ACE_Object_Manager::at_exit_i (void *object,
                               ACE_CLEANUP_FUNC cleanup_hook,
                               void *param)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock_, -1));

  if (shutting_down_)
    {
      errno = EAGAIN;
      return -1;
    }

  // Check for already in queue, and return 1 if so.
  ACE_Cleanup_Info *info = 0;
  for (ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info> iter (*registered_objects_);
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

#if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
class ACE_Export ACE_Object_Manager_Destroyer
  // = TITLE
  //    Ensure that the <ACE_Object_Manager> gets initialized before any
  //    application threads have been spawned.  
  //
  // = DESCRIPTION
  //    The <ACE_Object_Manager_Destroyer> class is placed in this
  //    file, rather than Object_Manager.cpp, to be sure that the
  //    static Object_Manager gets linked into applications that
  //    statically link libACE.a.
{
public:
  ACE_Object_Manager_Destroyer (void);
  ~ACE_Object_Manager_Destroyer (void);
};

ACE_Object_Manager_Destroyer::ACE_Object_Manager_Destroyer (void)
{
  // Ensure that the Object_Manager gets initialized before any
  // application threads have been spawned.  Because this will be called
  // during construction of static objects, that should always be the
  // case.
  ACE_Object_Manager &object_manager = *ACE_Object_Manager::instance ();
  ACE_UNUSED_ARG (object_manager);
}

ACE_Object_Manager_Destroyer::~ACE_Object_Manager_Destroyer (void)
{
  delete ACE_Object_Manager::instance_;
  ACE_Object_Manager::instance_ = 0;
}

static ACE_Object_Manager_Destroyer ACE_Object_Manager_Destroyer_internal;
#endif /* ! ACE_HAS_NONSTATIC_OBJECT_MANAGER */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
    template class ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>;
    template class ACE_Cleanup_Adapter<ACE_Thread_Mutex>;
    template class ACE_Managed_Object<ACE_Recursive_Thread_Mutex>;
    template class ACE_Managed_Object<ACE_Thread_Mutex>;
# endif /* ACE_MT_SAFE */
template class ACE_Cleanup_Adapter<ACE_SYNCH_RW_MUTEX>;
template class ACE_Cleanup_Adapter<
  ACE_SYNCH_RW_MUTEX[ACE_Filecache::DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE]>;
template class ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>;
template class ACE_Unbounded_Queue<ACE_Cleanup_Info>;
template class ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info>;
template class ACE_Node<ACE_Cleanup_Info>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   pragma instantiate ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>
#   pragma instantiate ACE_Cleanup_Adapter<ACE_Thread_Mutex>
#   pragma instantiate ACE_Managed_Object<ACE_Recursive_Thread_Mutex>
#   pragma instantiate ACE_Managed_Object<ACE_Thread_Mutex>
# endif /* ACE_MT_SAFE */
#pragma instantiate ACE_Cleanup_Adapter<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Cleanup_Adapter<
  ACE_SYNCH_RW_MUTEX[ACE_Filecache::DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE]>
#pragma instantiate ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Unbounded_Queue<ACE_Cleanup_Info>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info>
#pragma instantiate ACE_Node<ACE_Cleanup_Info>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
