// -*- C++ -*-
//
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    TAO_Singleton_Manager.h
//
// = DESCRIPTION
//     Header file for the TAO-specific Singleton Manager.  Based
//     entirely on ace/Object_Manager.{h,i,cpp}.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_OBJECT_MANAGER_H
#define TAO_OBJECT_MANAGER_H

#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS.h"

// Forward declarations
class ACE_Recursive_Thread_Mutex;

// Adapter for cleanup, used to register cleanup function with the
// ACE_Object_Manager.
extern "C"
void
TAO_Singleton_Manager_cleanup_destroyer (void *, void *);

class TAO_Export TAO_Singleton_Manager : public ACE_Object_Manager_Base
{
  // = TITLE
  //     Manager for TAO library services and singleton cleanup.
  //
  // = DESCRIPTION
  //     The <TAO_Singleton_Manager> is basically simplified version of
  //     the ACE_Object_Manager.  It is designed specifically to
  //     manage singletons created by TAO.  For example, Singleton
  //     instances created by TAO will be automatically registered
  //     with the Singleton instance of this Singleton Manager.
  //
  //     This class is necessary to ensure that TAO-specific
  //     Singletons are centrally isolated.  The idea is that
  //     destruction of the instance of the <TAO_Singleton_Manager>
  //     triggers destruction of all objects/services registered with
  //     it.

  friend void TAO_Singleton_Manager_cleanup_destroyer (void *, void *);

public:
  virtual int init (void);
  // Explicitly initialize.

  int init (int register_with_object_manager);
  // Explicitly initialize the TAO_Singleton_Manager, in addition to
  // explicitly registering (or not registering) with the
  // ACE_Object_Manager.

  virtual int fini (void);
  // Explicitly destroy.

  static int starting_up (void);
  // Returns 1 before the <TAO_Singleton_Manager> has been
  // constructed.  See <ACE_Object_Manager::starting_up> for more
  // information.

  static int shutting_down (void);
  // Returns 1 after the <TAO_Singleton_Manager> has been destroyed.
  // See <ACE_Object_Manager::shutting_down> for more information.

  enum Preallocated_Object
    {
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      // @@ No MT-specific preallocated objects (yet).  Remove the
      //    below dummy enum once a preallocated object is added.
      TAO_EMPTY_PREALLOCATED_OBJECT,
# else
      // Without ACE_MT_SAFE, There are no preallocated objects.  Make
      // sure that the preallocated_array size is at least one by
      // declaring this dummy . . .
      TAO_EMPTY_PREALLOCATED_OBJECT,
# endif /* ACE_MT_SAFE */

      TAO_PREALLOCATED_OBJECTS  // This enum value must be last!
    };
  // Unique identifiers for preallocated objects.

  static sigset_t *default_mask (void);
  // Accesses a default signal set used, for example, in
  // <ACE_Sig_Guard> methods.

  static ACE_Thread_Hook *thread_hook (void);
  // Returns the current thread hook for the process.

  static ACE_Thread_Hook *thread_hook (ACE_Thread_Hook *new_thread_hook);
  // Returns the existing thread hook and assign a <new_thread_hook>.

  static TAO_Singleton_Manager *instance (void);
  // Accessor to singleton instance.

  static int at_exit (ACE_Cleanup *object, void *param = 0);
  // Register an ACE_Cleanup object for cleanup at process
  // termination.  The object is deleted via the
  // <ace_cleanup_destroyer>.  If you need more flexiblity, see the
  // <other at_exit> method below.  For OS's that do not have
  // processes, cleanup takes place at the end of <main>.  Returns 0
  // on success.  On failure, returns -1 and sets errno to: EAGAIN if
  // shutting down, ENOMEM if insufficient virtual memory, or EEXIST
  // if the object (or array) had already been registered.

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
  // function is the same as <at_thread_exit>.  Returns 0 on success.
  // On failure, returns -1 and sets errno to: EAGAIN if shutting
  // down, ENOMEM if insufficient virtual memory, or EEXIST if the
  // object (or array) had already been registered.

private:
  // Force allocation on the heap.
  TAO_Singleton_Manager (void);
  ~TAO_Singleton_Manager (void);

  // Disallow copying by not implementing the following . . .
  TAO_Singleton_Manager (const TAO_Singleton_Manager &);
  TAO_Singleton_Manager &operator= (const TAO_Singleton_Manager &);

  int at_exit_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param);
  // Register an object or array for deletion at program termination.
  // See description of static version above for return values.

private:

  static TAO_Singleton_Manager *instance_;
  // Singleton instance pointer.

  static void *preallocated_object[TAO_PREALLOCATED_OBJECTS];
  // Table of preallocated objects.

  sigset_t *default_mask_;
  // Default signal set used, for example, in ACE_Sig_Guard.

  ACE_Thread_Hook *thread_hook_;
  // Thread hook that's used by this process.

  ACE_OS_Exit_Info exit_info_;
  // For at_exit support.

  int registered_with_object_manager_;
  // Indicates if TAO_Singleton_Manager is registered with the
  // ACE_Object_Manager.

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_Recursive_Thread_Mutex *internal_lock_;
  // Lock that is used to guard internal structures.
#endif /* ACE_MT_SAFE */
};

#if defined (__ACE_INLINE__)
# include "tao/TAO_Singleton_Manager.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_OBJECT_MANAGER_H */
