// $Id$

#include "ace/Object_Manager.h"
#include "ace/Service_Repository.h"

ACE_Object_Manager *ACE_Object_Manager::instance_ = 0;

ACE_Object_Manager::ACE_Object_Manager ()
  : registered_objects_ (),
    registered_arrays_ ()
{
}


ACE_Object_Manager::~ACE_Object_Manager ()
{
  void *p;
  int i;

  // Delete all registered objects and arrays.
  while ((i = registered_objects_.dequeue_head (p)) != -1)
    {
      delete p;
    }

  while ((i = registered_arrays_.dequeue_head (p)) != -1)
    {
      delete [] p;
    }


  // This call closes and deletes all ACE library services and singletons.
  ACE_Service_Config::close ();
}


ACE_Object_Manager *
ACE_Object_Manager::instance ()
{
  // This function should be call during construction of static instances,
  // so it's not thread safe.

  if (instance_ == 0)
    {
      ACE_NEW_RETURN (instance_, ACE_Object_Manager (), 0);
    }

  return instance_;
}


void
ACE_Object_Manager::delete_at_exit (void *object)
{
  registered_objects_.enqueue_tail (object);
}


void
ACE_Object_Manager::delete_array_at_exit (void *array)
{
  registered_arrays_.enqueue_tail (array);
}


#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Unbounded_Queue<void *>;
template class ACE_Unbounded_Queue_Iterator<void *>;
template class ACE_Node<void *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
