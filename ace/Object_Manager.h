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

#if !defined (ACE_OBJECT_MANAGER_H)
#define ACE_OBJECT_MANAGER_H

#include "ace/OS.h"

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  class ACE_Thread_Mutex;
#endif /* ACE_MT_SAFE */

// Forward declaration.
template <class T> class ACE_Unbounded_Queue;

#if !defined (ACE_MAX_MANAGED_OBJECTS)
# define ACE_MAX_MANAGED_OBJECTS 128
#endif /* ! ACE_MAX_MANAGED_OBJECTS */

#if !defined (ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS)
# define ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS
#endif /* ! ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS */

#if !defined (ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS)
# define ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS
#endif /* ! ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS */

class ACE_Export ACE_Object_Manager
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
  //     is enabled in the VxWorks config files that are supplied with
  //     ACE.
{
public:
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
      ACE_TOKEN_MANAGER_CREATION_LOCK,
      ACE_TSS_CLEANUP_LOCK,
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

private:
  ACE_Unbounded_Queue<ACE_Cleanup_Info> *registered_objects_;
  // Keeps track of all registered objects.

  int shutting_down_;
  // Non-zero if this being destroyed

  int at_exit_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param);
  // Register an object or array for deletion at program termination.
  // See description of static version above for return values.

  static ACE_Object_Manager *instance_;
  // Singleton pointer.

public:
  // For internal use only by ACE_Managed_Objects.

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_Thread_Mutex *lock_;
  // Lock that is used to guard internal structures.  Just a pointer
  // is declared here in order to minimize the headers that this one
  // includes.
#endif /* ACE_MT_SAFE */

  static ACE_Object_Manager *instance (void);
  // Accessor to singleton instance.  Because static member functions
  // are provided in the interface, this should not be public.  However,
  // it is public so that ACE_Managed_Object<TYPE> can access it.

  static void *managed_object[ACE_MAX_MANAGED_OBJECTS];
  // Table of managed objects.

  static u_int next_managed_object;
  // Index of the next available managed object table slot.

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
#if defined (ACE_HAS_TSS_EMULATION)
  // Main thread's thread-specific storage array.
  void *ts_storage_[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
#endif /* ACE_HAS_TSS_EMULATION */

#if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
  friend class ACE_Object_Manager_Destroyer;
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

  // Disallow copying by not implementing the following . . .
  ACE_Object_Manager (const ACE_Object_Manager &);
  ACE_Object_Manager &operator= (const ACE_Object_Manager &);
};

#if defined (ACE_HAS_THREADS)

class ACE_Recursive_Thread_Mutex;

class ACE_Export ACE_Static_Object_Lock
  // = TITLE
  //     Provide an interface to access a global lock.
  //
  // = DESCRIPTION
  //     This class is used to serialize the creation of static
  //     singleton objects.  It really isn't needed any more, because
  //     anyone can access ACE_STATIC_OBJECT_LOCK directly.  But, it
  //     is retained for backward compatibility.
{
public:
  static ACE_Recursive_Thread_Mutex *instance (void);
  // static lock access point
};

#endif /* ACE_HAS_THREADS */

#if defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/Managed_Object.h"

#endif /* ACE_OBJECT_MANAGER_H */
