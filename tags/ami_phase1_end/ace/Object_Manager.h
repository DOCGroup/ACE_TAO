/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Object_Manager.h
//
// = AUTHORS
//    David L. Levine, Matthias Kerkhoff, and Per Andersson
//
// ============================================================================

#ifndef ACE_OBJECT_MANAGER_H
#define ACE_OBJECT_MANAGER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class ACE_Object_Manager_Preallocations;
class ACE_Sig_Adapter;
class ACE_Sig_Set;
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  class ACE_Mutex;
  class ACE_Null_Mutex;
  class ACE_Thread_Mutex;
  class ACE_Recursive_Thread_Mutex;
  class ACE_RW_Thread_Mutex;
#endif /* ACE_MT_SAFE */

template <class T> class ACE_Unbounded_Queue;
template <class T> class ACE_Cleanup_Adapter;


// Configuration parameters.
#if !defined (ACE_MAX_MANAGED_OBJECTS)
# define ACE_MAX_MANAGED_OBJECTS 128
#endif /* ! ACE_MAX_MANAGED_OBJECTS */

#if !defined (ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS)
# define ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS
#endif /* ! ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS */

#if !defined (ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS)
# define ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS
#endif /* ! ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS */


class ACE_Export ACE_Object_Manager : public ACE_Object_Manager_Base
{
  // = TITLE
  //     Manager for ACE library services and singleton cleanup.
  //
  // = DESCRIPTION
  //     The <ACE_Object_Manager> manages cleanup of objects, typically
  //     singletons, at program termination.  In addition to managing
  //     the cleanup of the ACE library, it provides an interface for
  //     application to register objects to be cleaned up.
  //
  //     This class also shuts down ACE library services, so that they
  //     can reclaim their storage, at program termination.  It works
  //     by creating a static instance whose destructor gets called
  //     along with those of all other static objects.  Hooks are
  //     provided for application code to register objects and arrays
  //     for cleanup, e.g., destruction.  The order of such cleanup
  //     calls is in the reverse order of registration, i.e., that
  //     last object/array to register gets cleaned up first.
  //
  //     The <ACE_Object_Manager> API includes <ACE_Managed_Object>.  That
  //     class is contained in a separate file because it is a
  //     template class, and some compilers require that template and
  //     non-template class definitions appear in separate files.
  //     Please see ace/Managed_Object.h for a description of that
  //     part of the API.  In summary, <ACE_Managed_Object> provides two
  //     adapters, the <ACE_Cleanup_Adapter> and <ACE_Managed_Object>
  //     template classes for adapting objects of any type to be
  //     easily managed by the <ACE_Object_Manager>.  There are several
  //     mechanisms for adapting objects and arrays for cleanup at
  //     program termination, in roughly increasing order of ease-of-use:
  //
  //     1) Derive the object's class from <ACE_Cleanup>.
  //     2) Allow the <ACE_Object_Manager> to both dynamically allocate
  //        and deallocate the object.
  //     3) Provide an <ACE_CLEANUP_FUNC> cleanup hook for the object or
  //        array.
  //     4) Allow the <ACE_Object_Manager> to both preallocate the object
  //        or array, either statically in global data or dynamically on
  //        the heap, when its singleton instance is construction.
  //
  //     There are also several mechanisms for registering objects and
  //     arrays for cleanup.  In decreasing order of flexibility and
  //     complexity (with the exception of the last mechanism):
  //
  //      1) ACE_Object_Manager::at_exit (void *object,
  //                                      ACE_CLEANUP_FUNC cleanup_hook,
  //                                      void *param);
  //         can be used to register any object or array for any
  //         cleanup activity at program termination.
  //
  //      2) ACE_Object_Manager::at_exit (ACE_Cleanup *object,
  //                                      void *param = 0);
  //         can be used to register an <ACE_Cleanup> object
  //         for any cleanup activity at program termination.
  //
  //      The final mechanism is not general purpose, but can only
  //      be used to allocate objects and arrays at program startup:
  //
  //      3) ACE_Managed_Object::get_preallocated_object
  //           (ACE_Object_Manager::Preallocated_Object id);
  //         and
  //         ACE_Managed_Object::get_preallocated_array
  //           (ACE_Object_Manager::Preallocated_Array id);
  //         can only be used to allocate objects at program startup,
  //         either in global data or on the heap (selected at compile
  //         time).  These are intended to replace static locks, etc.
  //
  //     Instead of creating a static <ACE_Object_Manager> instance, one
  //     can alternatively be created on the stack of the main program
  //     thread.  It is created just after entry to ::main (int, char
  //     *[]), and before any existing code in that function is
  //     executed.  To enable this alternative, add #define
  //     ACE_HAS_NONSTATIC_OBJECT_MANAGER to ace/config.h prior to
  //     building the ACE library and your applications.  This #define
  //     is enabled in some config files that are supplied with ACE.
  //
  //     Note that the ACE_Object_Manager _must_ be created before
  //     any threads are spawned by the program.
  //
  //     If ACE_HAS_NONSTATIC_OBJECT_MANAGER is not #defined, the ACE
  //     library creates a static, singleton <ACE_Object_Manager> instance.
  //     The instance is placed in global program data, and constructed
  //     via a static object constructor.  If ACE_HAS_NONSTATIC_OBJECT_MANAGER
  //     is #defined, the <ACE_Object_Manager> instance is created on the stack
  //     of the main program thread, as noted above.
  //
  //     With ACE_HAS_NONSTATIC_OBJECT_MANAGER enabled, the ACE
  //     library has no static objects that require destruction.
  //     However, there are two drawbacks to using it:
  //
  //     1) main (int, char *[]) must be declared with arguments, even
  //        if they're not used.  All of ACE is converted to this, so
  //        just applications have to be concerned with it.
  //
  //     2) If there any static objects that depend on those that are
  //        cleaned up by the Object_Manager, they'll get cleaned up too
  //        late.  The ACE tests do not violate this requirement.
  //        However, applications may have trouble with it.
  //
  //     NOTE on the use of ::exit ():  ::exit () does not destroy
  //     static objects.  Therefore, if
  //     ACE_HAS_NONSTATIC_OBJECT_MANAGER is enabled, the
  //     ACE_Object_Manager instance will *not* be destroyed if ::exit
  //     () is called!  However, ACE_OS::exit () will properly destroy
  //     the ACE_Object_Manager.  It is highly recommended that
  //     ACE_OS::exit () be used instead of ::exit ().
  //
  //     However, ::exit () and ACE_OS::exit () are tricky to use
  //     properly, especially in multithread programs.  It is much
  //     safer to throw an exception (or simulate that effect) that
  //     will be caught by main () instead of calling exit.  Then,
  //     main () can perform any necessary application-specific
  //     cleanup and return the status value.  In addition, it's
  //     usually best to avoid calling ::exit () and ACE_OS::exit ()
  //     from threads other than the main thread.  Thanks to Jeff
  //     Greif <jmg@trivida.com> for pointing out that ::exit ()
  //     doesn't destroy automatic objects, and for developing the
  //     recommendations in this paragraph.
  //
  //     Instead of creating a static ACE_Object_Manager, or creating
  //     it on the stack of main (), another alternative is to #define
  //     ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER.  With that
  //     #define, the application _must_ call ACE::init () at the
  //     start of the program, and call ACE::fini () at the end.

public:
  virtual int init (void);
  // Explicitly initialize (construct the singleton instance of) the
  // ACE_Object_Manager.

  virtual int fini (void);
  // Explicitly destroy the singleton instance of the
  // ACE_Object_Manager.

  static int starting_up (void);
  // Returns 1 before the ACE_Object_Manager has been constructed.
  // This flag can be used to determine if the program is constructing
  // static objects.  If no static object spawns any threads, the
  // program will be single-threaded when this flag returns 1.  (Note
  // that the program still might construct some static objects when
  // this flag returns 0, if ACE_HAS_NONSTATIC_OBJECT_MANAGER is not
  // defined.)

  static int shutting_down (void);
  // Returns 1 after the ACE_Object_Manager has been destroyed.  This
  // flag can be used to determine if the program is in the midst of
  // destroying static objects.  (Note that the program might destroy
  // some static objects before this flag can return 1, if
  // ACE_HAS_NONSTATIC_OBJECT_MANAGER is not defined.)

  static int at_exit (ACE_Cleanup *object, void *param = 0);
  // Register an ACE_Cleanup object for cleanup at process termination.
  // The object is deleted via the ace_cleanup_destroyer ().  If you
  // need more flexiblity, see the other at_exit () method below.  For
  // OS's that do not have processes, cleanup takes place at the end of
  // main ().  Returns 0 on success.  On failure, returns -1 and sets
  // errno to:
  //   EAGAIN if shutting down,
  //   ENOMEM if insufficient virtual memory, or
  //   EEXIST if the object (or array) had already been registered.

  static int at_exit (void *object,
                      ACE_CLEANUP_FUNC cleanup_hook,
                      void *param);
  // Register an object (or array) for cleanup at process termination.
  // "cleanup_hook" points to a (global, or static member) function
  // that is called for the object or array when it to be destroyed.
  // It may perform any necessary cleanup specific for that object or
  // its class.  "param" is passed as the second parameter to the
  // "cleanup_hook" function; the first parameter is the object (or
  // array) to be destroyed.  "cleanup_hook", for example, may delete
  // the object (or array).  For OS's that do not have processes, this
  // function is the same as at_thread_exit ().  Returns 0 on success.
  // On failure, returns -1 and sets errno to:
  //   EAGAIN if shutting down,
  //   ENOMEM if insufficient virtual memory, or
  //   EEXIST if the object (or array) had already been registered.

#if 0 /* not implemented yet */
  static int at_thread_exit (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param);
  // Similar to at_exit (), except that the cleanup_hook is called
  // when the current thread exits instead of when the program terminates.
#endif /* 0 */

  enum Preallocated_Object
    {
      ACE_FILECACHE_LOCK,
#if defined (ACE_HAS_THREADS)
      ACE_STATIC_OBJECT_LOCK,
#endif /* ACE_HAS_THREADS */
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      ACE_LOG_MSG_INSTANCE_LOCK,
      ACE_MT_CORBA_HANDLER_LOCK,
      ACE_DUMP_LOCK,
      ACE_SIG_HANDLER_LOCK,
      ACE_SINGLETON_NULL_LOCK,
      ACE_SINGLETON_RECURSIVE_THREAD_LOCK,
      ACE_THREAD_EXIT_LOCK,
      ACE_TOKEN_MANAGER_CREATION_LOCK,
      ACE_TOKEN_INVARIANTS_CREATION_LOCK,
#endif /* ACE_MT_SAFE */

      // Hook for preallocated objects provided by application.
      ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS

      ACE_PREALLOCATED_OBJECTS  // This enum value must be last!
    };
  // Unique identifiers for preallocated objects.  Please see
  // ace/Managed_Object.h for information on accessing preallocated
  // objects.

  enum Preallocated_Array
    {
      // There currently are no preallocated arrays in the ACE
      // library.  If the application doesn't have any, make sure
      // the the preallocated_array size is at least one by declaring
      // this dummy . . .
      ACE_EMPTY_PREALLOCATED_ARRAY,

      // Hook for preallocated arrays provided by application.
      ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS

      ACE_PREALLOCATED_ARRAYS  // This enum value must be last!
    };
  // Unique identifiers for preallocated arrays.  Please see
  // ace/Managed_Object.h for information on accessing preallocated
  // arrays.

  static ACE_Sig_Set &default_mask (void);
  // Accesses a default signal set used in ACE_Sig_Guard methods.

private:
  ACE_Unbounded_Queue<ACE_Cleanup_Info> *registered_objects_;
  // Keeps track of all registered objects.

  ACE_Object_Manager_Preallocations *preallocations_;
  // Preallocated objects collection.

  ACE_Sig_Set *default_mask_;
  // Default signal set used in ACE_Sig_Guard.

  ACE_Sig_Adapter *ace_service_config_sig_handler_;
  // ACE_Service_Config signal handler.

  int at_exit_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param);
  // Register an object or array for deletion at program termination.
  // See description of static version above for return values.

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
public:
  // = The <get_singleton_lock> accessors are for internal
  // use by ACE_Singleton _only_.

  static int get_singleton_lock (ACE_Null_Mutex *&);
  // Accesses an <ACE_Null_Mutex> to be used for construction of
  // <ACE_Singletons>.  Returns 0, and the lock in the argument, on
  // success; returns -1 on failure.

  static int get_singleton_lock (ACE_Thread_Mutex *&);
  // Accesses a non-recursive <ACE_Thread_Mutex> to be used for
  // construction of <ACE_Singletons>.  Returns 0, and the lock in the
  // argument, on success; returns -1 on failure.

  static int get_singleton_lock (ACE_Mutex *&);
  // Accesses a non-recursive <ACE_Mutex> to be used for construction
  // of <ACE_Singletons>.  Returns 0, and the lock in the argument, on
  // success; returns -1 on failure.

  static int get_singleton_lock (ACE_Recursive_Thread_Mutex *&);
  // Accesses a recursive <ACE_Recursive_Thread_Mutex> to be used for
  // construction of <ACE_Singletons>.  Returns 0, and the lock in the
  // argument, on success; returns -1 on failure.

  static int get_singleton_lock (ACE_RW_Thread_Mutex *&);
  // Accesses a readers/writer <ACE_RW_Thread_Mutex> to be used for
  // construction of <ACE_Singletons>.  Returns 0, and the lock in the
  // argument, on success; returns -1 on failure.
#endif /* ACE_MT_SAFE */

public:
  // For internal use only by ACE_Managed_Objects.

  static ACE_Object_Manager *instance (void);
  // Accessor to singleton instance.  Because static member functions
  // are provided in the interface, this should not be public.  However,
  // it is public so that ACE_Managed_Object<TYPE> can access it.

  static void *preallocated_object[ACE_PREALLOCATED_OBJECTS];
  // Table of preallocated objects.

  static void *preallocated_array[ACE_PREALLOCATED_ARRAYS];
  // Table of preallocated arrays.

public:
  // Application code should not use these explicitly, so they're
  // hidden here.  They're public so that the ACE_Object_Manager can
  // be constructed/destructed in main () with
  // ACE_HAS_NONSTATIC_OBJECT_MANAGER.
  ACE_Object_Manager (void);
  ~ACE_Object_Manager (void);

private:
  static ACE_Object_Manager *instance_;
  // Singleton pointer.

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_Recursive_Thread_Mutex *internal_lock_;
  // Lock that is used to guard internal structures.

  ACE_Cleanup_Adapter<ACE_Null_Mutex> *singleton_null_lock_;
  // Null lock for guarding singleton creation.

  ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex> *singleton_recursive_lock_;
  // Lock for guarding singleton creation, when Object_Manager
  // hasn't been started up, or has already been shut down.
#endif /* ACE_MT_SAFE */

#if defined (ACE_HAS_TSS_EMULATION)
  // Main thread's thread-specific storage array.
  void *ts_storage_[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
#endif /* ACE_HAS_TSS_EMULATION */

#if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
  friend class ACE_Object_Manager_Manager;
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

  // Disallow copying by not implementing the following . . .
  ACE_Object_Manager (const ACE_Object_Manager &);
  ACE_Object_Manager &operator= (const ACE_Object_Manager &);
};


#if defined (ACE_HAS_THREADS)

class ACE_Recursive_Thread_Mutex;

class ACE_Export ACE_Static_Object_Lock
{
  // = TITLE
  //     Provide an interface to access a global lock.
  //
  // = DESCRIPTION
  //     This class is used to serialize the creation of static
  //     singleton objects.  It really isn't needed any more, because
  //     anyone can access ACE_STATIC_OBJECT_LOCK directly.  But, it
  //     is retained for backward compatibility.
public:
  static ACE_Recursive_Thread_Mutex *instance (void);
  // Static lock access point.

  static void cleanup_lock (void);
  // For use only by ACE_Object_Manager to clean up lock if it
  // what dynamically allocated.
};

#endif /* ACE_HAS_THREADS */


#if defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/Managed_Object.h"

#endif /* ACE_OBJECT_MANAGER_H */
