// $Id$

#define ACE_BUILD_DLL

#include "ace/Object_Manager.h"
#include "ace/Service_Repository.h"

#if !defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

// Singleton pointer.
ACE_Object_Manager *ACE_Object_Manager::instance_ = 0;

ACE_Object_Manager::ACE_Object_Manager (void)
  : registered_objects_ ()
{
}

ACE_Object_Manager::~ACE_Object_Manager (void)
{
  object_info_t info;

  // Call all registered cleanup hooks, in reverse order
  // of registration.
  while (registered_objects_.dequeue_head (info) != -1)
    {
      (*info.cleanup_hook_) (info.object_, info.param_);
    }

  // This call closes and deletes all ACE library services and
  // singletons.
  ACE_Service_Config::close ();
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
  // Check for already in queue, and return 1 if so.
  object_info_t *info = 0;
  for (ACE_Unbounded_Queue_Iterator<object_info_t> iter (registered_objects_);
       iter.next (info) != 0;
       iter.advance ())
    {
      if (info->object_ == object)
        {
          // The object has already been registered.
          return 1;
        }
    }

  object_info_t new_info;
  new_info.object_ = object;
  new_info.cleanup_hook_ = cleanup_hook;
  new_info.param_ = param;

  // Returns -1 if unable to allocate storage.
  // Enqueue at the head and dequeue from the head to get LIFO ordering.
  return registered_objects_.enqueue_head (new_info);
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Unbounded_Queue<ACE_Object_Manager::object_info_t>;
template class ACE_Unbounded_Queue_Iterator<ACE_Object_Manager::object_info_t>;
template class ACE_Node<ACE_Object_Manager::object_info_t>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
