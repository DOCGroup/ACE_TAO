// $Id$

#define ACE_BUILD_DLL

#include "ace/Object_Manager.h"
#include "ace/Service_Repository.h"

#if !defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_Object_Manager *ACE_Object_Manager::instance_ = 0;

ACE_Object_Manager::ACE_Object_Manager (void)
{
}

ACE_Object_Manager::~ACE_Object_Manager (void)
{
  void *p;
  int i;

  // Delete all registered objects and arrays.
  while ((i = registered_objects_.dequeue_head (p)) != -1)
    delete p;

  while ((i = registered_arrays_.dequeue_head (p)) != -1)
    delete [] p;

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
ACE_Object_Manager::delete_at_exit_i (void *object)
{
  // Check for already in queue, and return 1 if so.
  ACE_Unbounded_Queue_Iterator<void *> i (registered_objects_);
  void **obj;

  while (i.next (obj))
    {
      i.advance ();

      if (obj == object)
	// The object has already been registered.
	return 1;
    }

  // Returns -1 if unable to allocate storage.
  return registered_objects_.enqueue_tail (object);
}

int
ACE_Object_Manager::delete_array_at_exit_i (void *array)
{
  // Check for already in queue, and return 1 if so.
  ACE_Unbounded_Queue_Iterator<void *> i (registered_arrays_);
  void **obj;

  while (i.next (obj))
    {
      i.advance ();

      if (obj == array)
	// The array has already been registered.
	return 1;
    }

  // Returns -1 if unable to allocate storage.
  return registered_arrays_.enqueue_tail (array);
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Unbounded_Queue<void *>;
template class ACE_Unbounded_Queue_Iterator<void *>;
template class ACE_Node<void *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
