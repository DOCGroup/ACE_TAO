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

#include "ace/Containers.h"

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
  typedef void (*ACE_CLEANUP_FUNC)(void *object, void *param);

  static int cleanup (void *object,
                      ACE_CLEANUP_FUNC cleanup_hook,
                      void *param,
                      ACE_hthread_t thread_handle = (ACE_hthread_t) -1);
  // Register an object (or array) for cleanup at program or thread
  // termination.
  // "cleanup_hook" points to a (global, or static member) function that
  // is called for the object or array when it to be deleted.  It may
  // perform any necessary cleanup specific for that object or its class.
  // "param" is passed as the second parameter to the "cleanup_hook"
  // function; the first parameter is the object (or array) to be deleted.
  // "cleanup_hook", for example, may delete the object (or array), and then
  // it may set the object (or array) address to 0.
  // If "thread_handle" is not -1, then cleanup is registered for termination
  // of the thread identified by the handle.  If "thread_handle" is -1,
  // then the registration is for termination of the process.
  // For OS's that do not have processes, -1 indicates indicates the current
  // thread.
  // Returns 0 on success, non-zero on failure: -1 if virtual memory is
  // exhausted or 1 if the object (or arrayt) had already been registered.

private:
  // not currently used:
  // typedef void (*ACE_CONSTRUCT_FUNC)(void *object, void *param);

  static ACE_Object_Manager *instance_;
  // Singleton pointer.

public:  // For template instantiation, with some compilers that need the
         // struct definition to be public.
  typedef struct object_info
  {
    void *object_;
    ACE_CLEANUP_FUNC cleanup_hook_;
    void *param_;
  } object_info_t;
private:

  ACE_Unbounded_Queue<object_info_t> registered_objects_;
  // Keeps track of all the register objects.

  int cleanup_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param,
                 ACE_hthread_t thread_handle = (ACE_hthread_t) -1);
  // Register an object or array for deletion at program termination.
  // See description of static version above for return values.

  ACE_Object_Manager (void);
  ~ACE_Object_Manager (void);

  static ACE_Object_Manager *instance (void);
  // Accessor to singleton instance.  Because static member functions are
  // provided in the interface, this does not need to be public.

  friend class ACE_Object_Manager_Destroyer;

  // Disallow copying by not implementing the following . . .
  ACE_Object_Manager (const ACE_Object_Manager &);
  ACE_Object_Manager &operator= (const ACE_Object_Manager &);
};

#if defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_OBJECT_MANAGER_H */
