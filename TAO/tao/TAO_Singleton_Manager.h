// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_Singleton_Manager.h
 *
 *  $Id$
 *
 *   Header file for the TAO-specific Singleton Manager.  Based
 *   entirely on ace/Object_Manager.{h,inl,cpp}.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SINGLETON_MANAGER_H
#define TAO_SINGLETON_MANAGER_H

#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/orbconf.h"
#include "ace/Object_Manager_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Singleton_Manager
 *
 * @brief Manager for TAO library services and singleton cleanup.
 *
 * The TAO_Singleton_Manager is basically simplified version of the
 * ACE_Object_Manager.  It is designed specifically to manage
 * singletons created by TAO. For example, singleton instances
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
   * that do not have processes, cleanup takes place at the end of
   * main.  Returns 0 on success.  On failure, returns -1 and sets
   * errno to: EAGAIN if shutting down, ENOMEM if insufficient virtual
   * memory, or EEXIST if the object (or array) had already been
   * registered.
   */
  static int at_exit (ACE_Cleanup *object, void *param = 0, const char* name = 0);

  /// Register an object (or array) for cleanup at process
  /// termination.
  /**
   * @a cleanup_hook points to a (global, or static member) function that
   * is called for the object or array when it to be destroyed.  It
   * may perform any necessary cleanup specific for that object or its
   * class.  @a param is passed as the second parameter to the
   * cleanup_hook function; the first parameter is the object (or
   * array) to be destroyed.  @a cleanup_hook, for example, may delete
   * the object (or array).  For OS's that do not have processes, this
   * function is the same as @c at_thread_exit.  Returns 0 on success.
   * On failure, returns -1 and sets errno to: EAGAIN if shutting
   * down, ENOMEM if insufficient virtual memory, or EEXIST if the
   * object (or array) had already been registered.
   */
  static int at_exit (void *object,
                      ACE_CLEANUP_FUNC cleanup_hook,
                      void *param,
                      const char* name);

protected:

  /// Force allocation on the heap.
  //@{
  TAO_Singleton_Manager (void);
  ~TAO_Singleton_Manager (void);
  //@}

private:

  /// Disallow copying by not implementing the following ...
  //@{
  TAO_Singleton_Manager (const TAO_Singleton_Manager &);
  TAO_Singleton_Manager &operator= (const TAO_Singleton_Manager &);
  //@}

  /// Register an object or array for deletion at program termination.
  /// See description of static version above for return values.
  int at_exit_i (void *object, ACE_CLEANUP_FUNC cleanup_hook, void *param, const char* name);

private:
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

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/TAO_Singleton_Manager.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_SINGLETON_MANAGER_H */
