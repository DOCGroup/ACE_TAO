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
# define ACE_MAX_MANAGED_OBJECTS 256
#endif /* ACE_MAX_MANAGED_OBJECTS */

class ACE_Export ACE_Object_Manager
  // = TITLE
  //     Manager for ACE library services and singleton cleanup.
  //
  // = DESCRIPTION
  //     This class shuts down ACE library services, so that they can
  //     reclaim their storage, at program termination.  It does that by
  //     creating a static instance, whose destructor gets called along
  //     with those of all other static objects.  Hooks are provided for
  //     application code to register objects and arrays for
  //     cleanup, e.g., destruction.  Please note that the order of such
  //     cleanup calls is not specified.  Therefore, these cleanup hooks
  //     should not depend on any of the static instances.  Also note that
  //     ACE_Log_Msg currently takes care of its own cleanup.
  //
  //     It  would be worth adding a capability to do the shutdown prior
  //     to static object destruction, e.g., via an ::at_exit () call.
  //     Without that capability, on VxWorks, for example, the program
  //     must be unloaded for this to work.  (On VxWorks,
  //     alternatively, the explicity OS calls  to call all static
  //     destructors and constructors could be used.)
  //
  //     WARNING:  this class is under development.  Its interface
  //     is likely to change, without providing backward capability.
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

  enum Preallocated_Objects
    {
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      ACE_MT_CORBA_HANDLER_LOCK,
      ACE_DUMP_LOCK,
      ACE_TSS_CLEANUP_LOCK,
#endif /* ACE_MT_SAFE */

      ACE_END_OF_PREALLOCATED_OBJECTS  // This enum value must be last!
    };

private:
  static ACE_Object_Manager *instance_;
  // Singleton pointer.

  ACE_Unbounded_Queue<ACE_Cleanup_Info> *registered_objects_;
  // Keeps track of all registered objects.

  int shutting_down_;
  // Non-zero if this being destroyed

public:
  ACE_MT (ACE_Thread_Mutex *lock_);
  // Lock that is used to guard internal structures.  Just a pointer
  // is declared here in order to minimize the headers that this one
  // includes.

  static ACE_Object_Manager *instance (void);
  // Accessor to singleton instance.  Because static member functions
  // are provided in the interface, this should not be public.  However,
  // it is public so that ACE_Managed_Object<TYPE> can access it.

  static void *managed_object[ACE_MAX_MANAGED_OBJECTS];
  // Table of managed objects.

  static u_int next_managed_object;
  // Index of the next available managed object table slot.

private:
  int at_exit_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param);
  // Register an object or array for deletion at program termination.
  // See description of static version above for return values.

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

#if defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/Managed_Object.h"

#endif /* ACE_OBJECT_MANAGER_H */
