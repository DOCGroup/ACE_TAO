// $Id$

#define ACE_BUILD_DLL

#if !defined (ACE_MANAGED_OBJECT_CPP)
#define ACE_MANAGED_OBJECT_CPP

#include "ace/Object_Manager.h"
#include "ace/Managed_Object.h"
#include "ace/Synch.h"

template <class TYPE>
ACE_Managed_Cleanup<TYPE>::ACE_Managed_Cleanup (void)
  : object_ ()
{
}

template <class TYPE>
TYPE &
ACE_Managed_Cleanup<TYPE>::object (void)
{
  return this->object_;
}

template <class TYPE>
int
ACE_Managed_Object<TYPE>::get_object (int &id, TYPE *&object)
{
  // Use the ACE_Object_Manager instance's lock.
  ACE_MT (ACE_Thread_Mutex &lock = *ACE_Object_Manager::instance ()->lock_);
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, lock, -1));

  if (id == 0)
    {
      // User is requesting a new object.
      if (ACE_Object_Manager::next_managed_object < ACE_MAX_MANAGED_OBJECTS)
        {
          // Allocate a new object, store it in the table, and return it.
          ACE_NEW_RETURN (object, TYPE, -1);
          if (object == 0)
            {
              errno = ENOMEM;
              return -1;
            }
          else
            {
              id = ACE_Object_Manager::next_managed_object;
              ACE_Object_Manager::managed_object[
                ACE_Object_Manager::next_managed_object++] = object;
              return 0;
            }
        }
      else
        {
          // No more managed_object table slots.
          object = 0;
          errno = ENOSPC;
          return -1;
        }
    }
  else if (id < 0  ||  (u_int) id >= ACE_Object_Manager::next_managed_object)
    {
      // Unknown, non-zero, or negative id.
      object = 0;
      errno = ENOENT;
      return -1;
    }
  else
    {
      // id is known, so return the object.  Cast its type based
      // on the type of the function template parameter.
      object = (TYPE *) ACE_Object_Manager::managed_object[id - 1];
      return 0;
    }
}

template <class TYPE>
TYPE *
ACE_Managed_Object<TYPE>::get_object (int id)
{
  // Use the ACE_Object_Manager instance's lock.
  ACE_MT (ACE_Thread_Mutex &lock = *ACE_Object_Manager::instance ()->lock_);
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, lock, 0));

  if (id <= 0  ||  (u_int) id > ACE_Object_Manager::next_managed_object)
    {
      // Unknown or invalid managed object id.
      errno = ENOENT;
      return 0;
    }
  else
    {
      // id is known, so return the object.  Cast its type based
      // on the type of the function template parameter.
      return &((ACE_Managed_Cleanup<TYPE> *)
               ACE_Object_Manager::managed_object[id - 1])->object ();
    }
}

#endif /* ACE_MANAGED_OBJECT_CPP */
