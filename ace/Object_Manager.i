/* -*- C++ -*- */
// $Id$

ACE_INLINE
int
ACE_Object_Manager::delete_at_exit (void *object)
{
  return ACE_Object_Manager::instance ()->delete_at_exit_ (object);
}

ACE_INLINE
int
ACE_Object_Manager::delete_array_at_exit (void *array)
{
  return ACE_Object_Manager::instance ()->delete_array_at_exit_ (array);
}
