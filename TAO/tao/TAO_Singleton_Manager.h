// -*- C++ -*-


//=============================================================================
/**
 *  @file    TAO_Singleton_Manager.h
 *
 *  $Id$
 *
 *   Header file for the TAO-specific Singleton Manager.  Based
 *   entirely on ace/Object_Manager.{h,i,cpp}.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SINGLETON_MANAGER_H
#define TAO_SINGLETON_MANAGER_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS.h"


#if defined (ACE_HAS_EXCEPTIONS)
typedef void (*TAO_unexpected_handler)(void);
#endif  /* ACE_HAS_EXCEPTIONS */


/// Adapter for cleanup, used to register cleanup function with the
/// ACE_Object_Manager.
extern "C"
void
TAO_Singleton_Manager_cleanup_destroyer (void *, void *);


/**
 * @class TAO_Singleton_Manager
 *
 * @brief Manager for TAO library services and singleton cleanup.
 *
 * The TAO_Singleton_Manager is basically simplified version of the
 * ACE_Object_Manager.  It is designed specifically to manage
 * singletons created by TAO.  For example, singleton instances
 * created by TAO will be automatically registered with the singleton
 * instance of this Singleton Manager.
 * @par
 * This class is necessary to ensure that TAO-specific
 * singletons are isolated to TAO itself, not ACE, for example.  The
 * idea is that destruction of the instance of the
 * TAO_Singleton_Manager triggers destruction of all objects/services
 * registered with it.
 */
class TAO_Export TAO_Singleton_Manager : public ACE_Object_Manager_Base
{

  friend void TAO_Singleton_Manager_cleanup_destroyer (void *, void *);

public:

  /// Explicitly initialize.
  virtual int init (void);

  /**
   * Explicitly initialize the TAO_Singleton_Manager, in addition to
   * explicitly registering (or not registering) with the
   * ACE_Object_Manager.
   */
  int init (int register_with_object_manager);

  /// Explicitly destroy.
  virtual int fini (void);

  /**
   * Returns 1 before the TAO_Singleton_Manager has been constructed.
   * See ACE_Object_Manager::starting_up for more information.
   */
  static int starting_up (void);

  /// Returns 1 after the TAO_Singleton_Manager has been destroyed.
  /// See ACE_Object_Manager::shutting_down for more information.
  static int shutting_down (void);

  /// Unique identifiers for preallocated Objects.
  enum Preallocated_Object
    {
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      /// @@ No MT-specific preallocated objects (yet).  Remove the
      ///    below dummy enum once a preallocated object is added.
      TAO_EMPTY_PREALLOCATED_OBJECT,
# else
      /// Without ACE_MT_SAFE, There are no preallocated objects.
      /// Make sure that the preallocated_array size is at least one
      /// by declaring this dummy ...
      TAO_EMPTY_PREALLOCATED_OBJECT,
# endif /* ACE_MT_SAFE */

      /// This enum value must be last!
      TAO_PREALLOCATED_OBJECTS
    };


  /// Accesses a default signal set used, for example, in
  /// ACE_Sig_Guard methods.
  static sigset_t *default_mask (void);

  /// Returns the current thread hook for the process.
  static ACE_Thread_Hook *thread_hook (void);

  /// Returns the existing thread hook and assign a new_thread_hook.
  static ACE_Thread_Hook *thread_hook (ACE_Thread_Hook *new_thread_hook);

  /// Accessor to singleton instance.
  static TAO_Singleton_Manager *instance (void);

  /// Register an ACE_Cleanup object for cleanup at process
  /// termination.
  /**
   * The object is deleted via the ace_cleanup_destroyer.  If you need
   * more flexiblity, see the other at_exit method below.  For OS's
   * that do not have processes, cleanup takes place  at the end of
   * main.  Returns 0 on success.  On failure, returns -1 and sets
   * errno to: EAGAIN if shutting down, ENOMEM if insufficient virtual
   * memory, or EEXIST if the object (or array) had already been
   * registered.
   */
  static int at_exit (ACE_Cleanup *object, void *param = 0);

  /// Register an object (or array) for cleanup at process
  /// termination.
  /**
   * cleanup_hook points to a (global, or static member) function that
   * is called for the object or array when it to be destroyed.  It
   * may perform any necessary cleanup specific for that object or its
   * class.  param is passed as the second parameter to the
   * cleanup_hook function; the first parameter is the object (or
   * array) to be destroyed.  cleanup_hook, for example, may delete
   * the object (or array).  For OS's that do not have processes, this
   * function is the same as <at_thread_exit>.  Returns 0 on success.
   * On failure, returns -1 and sets errno to: EAGAIN if shutting
   * down, ENOMEM if insufficient virtual memory, or EEXIST if the
   * object (or array) had already been registered.
   */
  static int at_exit (void *object,
                      ACE_CLEANUP_FUNC cleanup_hook,
                      void *param);

#if defined (ACE_HAS_EXCEPTIONS)
  /// Set a new unexpected exception handler.
  /**
   * The old one will be stored for restoration later on.
   *
   * @note Calling this method multiple times will cause the stored
   *       old unexpected exception handler pointer to be lost.
   */
  void _set_unexpected (TAO_unexpected_handler u);
#endif /* ACE_HAS_EXCEPTIONS */

private:

  /// Force allocation on the heap.
  //@{
  TAO_Singleton_Manager (void);
  ~TAO_Singleton_Manager (void);
  //@}

  /// Disallow copying by not implementing the following ...
  //@{
  TAO_Singleton_Manager (const TAO_Singleton_Manager &);
  TAO_Singleton_Manager &operator= (const TAO_Singleton_Manager &);
  //@}

  /// Register an object or array for deletion at program termination.
  /// See description of static version above for return values.
  int at_exit_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param);

private:

  /// Singleton instance pointer.
  static TAO_Singleton_Manager *instance_;

  /// Table of preallocated objects.
  static void *preallocated_object[TAO_PREALLOCATED_OBJECTS];

  /// Default signal set used, for example, in ACE_Sig_Guard.
  sigset_t *default_mask_;

  /// Thread hook that's used by this process.
  ACE_Thread_Hook *thread_hook_;

  /// For at_exit support.
  ACE_OS_Exit_Info exit_info_;

  /// Indicates if TAO_Singleton_Manager is registered with the
  /// ACE_Object_Manager.
  int registered_with_object_manager_;

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  /// Lock that is used to guard internal structures.
  TAO_SYNCH_RECURSIVE_MUTEX *internal_lock_;
#endif /* ACE_MT_SAFE */

#if defined (ACE_HAS_EXCEPTIONS)
  /// The old unexpected exception handler.
  /**
   * A pointer to the old unexpected exception handler is stored so
   * that it can be restored when TAO is unloaded, for example.
   * Otherwise, any unexpected exceptions will result in a call to
   * TAO's unexpected exception handler which may no longer exist if
   * TAO was unloaded.
   */
  TAO_unexpected_handler old_unexpected_;
#endif  /* ACE_HAS_EXCEPTIONS */

};

#if defined (__ACE_INLINE__)
# include "tao/TAO_Singleton_Manager.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_SINGLETON_MANAGER_H */
