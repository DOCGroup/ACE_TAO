// $Id$

#include "ace/Log_Msg.h"
#include "ace/Synch.h"

#include "tao/TAO_Singleton_Manager.h"

#include "tao/Exception.h"
#include "tao/Typecode.h"

#if !defined (__ACE_INLINE__)
# include "tao/TAO_Singleton_Manager.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao, TAO_Singleton_Manager, "$Id$")


extern "C" void
TAO_Singleton_Manager_cleanup_destroyer (void *, void *)
{
  if (TAO_Singleton_Manager::instance_)
    (void) TAO_Singleton_Manager::instance ()->fini ();
}

TAO_Singleton_Manager *TAO_Singleton_Manager::instance_ = 0;

void *TAO_Singleton_Manager::preallocated_object[
  TAO_Singleton_Manager::TAO_PREALLOCATED_OBJECTS] = { 0 };

TAO_Singleton_Manager::TAO_Singleton_Manager (void)
  // default_mask_ isn't initialized, because it's defined by <init>.
  : thread_hook_ (0),
    exit_info_ (),
    registered_with_object_manager_ (0)
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  , internal_lock_ (new ACE_Recursive_Thread_Mutex)
# endif /* ACE_MT_SAFE */
{
  // Be sure that no further instances are created via instance ().
  if (instance_ == 0)
    instance_ = this;

  (void) this->init ();
}

TAO_Singleton_Manager::~TAO_Singleton_Manager (void)
{
  this->dynamically_allocated_ = 0;   // Don't delete this again in fini()
  (void) this->fini ();
}

sigset_t *
TAO_Singleton_Manager::default_mask (void)
{
  return TAO_Singleton_Manager::instance ()->default_mask_;
}

ACE_Thread_Hook *
TAO_Singleton_Manager::thread_hook (void)
{
  return TAO_Singleton_Manager::instance ()->thread_hook_;
}

ACE_Thread_Hook *
TAO_Singleton_Manager::thread_hook (ACE_Thread_Hook *new_thread_hook)
{
  TAO_Singleton_Manager *tao_om = TAO_Singleton_Manager::instance ();
  ACE_Thread_Hook *old_hook = tao_om->thread_hook_;
  tao_om->thread_hook_ = new_thread_hook;
  return old_hook;
}

TAO_Singleton_Manager *
TAO_Singleton_Manager::instance (void)
{
  // This function should be called during construction of static
  // instances, or before any other threads have been created in the
  // process.  So, it's not thread safe.

  if (instance_ == 0)
    {
      TAO_Singleton_Manager *instance_pointer;

      ACE_NEW_RETURN (instance_pointer,
                      TAO_Singleton_Manager,
                      0);
      ACE_ASSERT (instance_pointer == instance_);

      instance_pointer->dynamically_allocated_ = 1;

    }

  return instance_;
}

int
TAO_Singleton_Manager::init (void)
{
  // Register the TAO_Singleton_Manager with the ACE_Object_Manager.
  int register_with_object_manager = 1;

  return this->init (register_with_object_manager);
}

int
TAO_Singleton_Manager::init (int register_with_object_manager)
{
  if (this->starting_up_i ())
    {
      // First, indicate that this TAO_Singleton_Manager instance is being
      // initialized.
      this->object_manager_state_ = OBJ_MAN_INITIALIZING;

      if (this == instance_)
        {
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
          // @@ No MT-specific pre-allocated objects.
# endif /* ACE_MT_SAFE */
        }

      ACE_NEW_RETURN (this->default_mask_, sigset_t, -1);
      ACE_OS::sigfillset (this->default_mask_);

      if (register_with_object_manager == 1
          && ACE_Object_Manager::instance ()->at_exit (
               this,
               (ACE_CLEANUP_FUNC) TAO_Singleton_Manager_cleanup_destroyer,
               0) != 0)
        return -1;

      this->registered_with_object_manager_ =
        register_with_object_manager;

      // Finally, indicate that the TAO_Singleton_Manager instance has
      // been initialized.
      this->object_manager_state_ = OBJ_MAN_INITIALIZED;

      return 0;
    }
  else if (this->registered_with_object_manager_
           != register_with_object_manager)
    {
      // An attempt was made to register the TAO_Singleton_Manager
      // with a manager of a different type from the one it is
      // currently registered with.  This indicates a problem with the
      // caller's logic.

      errno = EINVAL;
      return -1;
    }

  // Had already initialized.
  return 1;
}

// Clean up a TAO_Singleton_Manager.  There can be instances of this object
// other than The Instance.  This can happen if a user creates one for some
// reason.  All objects clean up their per-object information and managed
// objects, but only The Instance cleans up the static preallocated objects.
int
TAO_Singleton_Manager::fini (void)
{
  if (instance_ == 0  ||  this->shutting_down_i ())
    // Too late.  Or, maybe too early.  Either fini () has already
    // been called, or init () was never called.
    return this->object_manager_state_ == OBJ_MAN_SHUT_DOWN  ?  1  :  -1;

  // No mutex here.  Only the main thread should destroy the singleton
  // TAO_Singleton_Manager instance.

  // Indicate that the TAO_Singleton_Manager instance is being shut
  // down.  This object manager should be the last one to be shut
  // down.
  this->object_manager_state_ = OBJ_MAN_SHUTTING_DOWN;

  // If another Object_Manager has registered for termination, do it.
  if (this->next_)
    {
      this->next_->fini ();
      this->next_ = 0;  // Protect against recursive calls.
    }

  // Call all registered cleanup hooks, in reverse order of
  // registration.
  this->exit_info_.call_hooks ();

  // Only clean up preallocated objects when the singleton Instance is being
  // destroyed.
  if (this == instance_)
    {
#if ! defined (ACE_HAS_STATIC_PREALLOCATION)
      // Cleanup the dynamically preallocated objects.
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      // @@ No MT-specific preallocated objects yet.
# endif  /* ACE_MT_SAFE */
      // @@ No preallocated objects yet.
#endif  /* ! ACE_HAS_STATIC_PREALLOCATION */
    }

  delete this-> default_mask_;
  this->default_mask_ = 0;

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  delete this->internal_lock_;
  this->internal_lock_ = 0;
#endif /* ACE_MT_SAFE */

  // Indicate that this TAO_Singleton_Manager instance has been shut down.
  this->object_manager_state_ = OBJ_MAN_SHUT_DOWN;

  if (this == instance_)
    instance_ = 0;

  if (this->dynamically_allocated_)
    {
      delete this;
    }

  // Clean up all ORB owned Exceptions (before TypeCode clean up).
  TAO_Exceptions::fini ();

  // Clean up all ORB owned TypeCodes.
  TAO_TypeCodes::fini ();

  return 0;
}

int
TAO_Singleton_Manager::starting_up (void)
{
  return TAO_Singleton_Manager::instance_
    ? instance_->starting_up_i ()
    : 1;
}

int
TAO_Singleton_Manager::shutting_down (void)
{
  return TAO_Singleton_Manager::instance_
    ? instance_->shutting_down_i ()
    : 1;
}

int
TAO_Singleton_Manager::at_exit_i (void *object,
                                  ACE_CLEANUP_FUNC cleanup_hook,
                                  void *param)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
    *instance_->internal_lock_, -1));

  if (this->shutting_down_i ())
    {
      errno = EAGAIN;
      return -1;
    }

  if (this->exit_info_.find (object))
    {
      // The object has already been registered.
      errno = EEXIST;
      return -1;
    }

  return this->exit_info_.at_exit_i (object, cleanup_hook, param);
}
