/* -*- C++ -*- */
// $Id$

// Note: don't explicitly initialize "object_", because TYPE may not
// have a default constructor.  Let the compiler figure it out . . .
template <class TYPE>
ACE_INLINE
ACE_Managed_Cleanup<TYPE>::ACE_Managed_Cleanup (void)
{
}

template <class TYPE>
ACE_INLINE
TYPE &
ACE_Managed_Cleanup<TYPE>::object (void)
{
  return this->object_;
}

template <class TYPE>
ACE_INLINE
TYPE *
ACE_Managed_Object<TYPE>::get_preallocated_object
  (ACE_Object_Manager::Preallocated_Object id)
{
  // The preallocated objects are in a separate, "read-only" array so
  // that this function doesn't need a lock.  Also, because it is
  // intended _only_ for use with hard-code values, it performs no
  // range checking on "id".

  // Cast the return type of the the object pointer based
  // on the type of the function template parameter.
  return &((ACE_Managed_Cleanup<TYPE> *)
           ACE_Object_Manager::preallocated_object[id])->object ();
}

template <class TYPE>
ACE_INLINE
TYPE *
ACE_Managed_Object<TYPE>::get_preallocated_array
  (ACE_Object_Manager::Preallocated_Array id)
{
  // The preallocated array are in a separate, "read-only" array so
  // that this function doesn't need a lock.  Also, because it is
  // intended _only_ for use with hard-code values, it performs no
  // range checking on "id".

  // Cast the return type of the the object pointer based
  // on the type of the function template parameter.
  return &((ACE_Managed_Cleanup<TYPE> *)
           ACE_Object_Manager::preallocated_array[id])->object ();
}
