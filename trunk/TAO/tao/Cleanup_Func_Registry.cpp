// -*- C++ -*-
//
// $Id$

#include "Cleanup_Func_Registry.h"

#if !defined (__ACE_INLINE__)
# include "Cleanup_Func_Registry.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, Cleanup_Func_Registry, "$Id$")

TAO_Cleanup_Func_Registry::TAO_Cleanup_Func_Registry (void)
  : cleanup_funcs_ ()
{
}

TAO_Cleanup_Func_Registry::~TAO_Cleanup_Func_Registry (void)
{
}

int
TAO_Cleanup_Func_Registry::register_cleanup_function (
  ACE_CLEANUP_FUNC func,
  size_t &slot_id)
{
  size_t slot = this->cleanup_funcs_.size ();

  if (this->cleanup_funcs_.size (slot + 1) != 0)
    return -1;

  this->cleanup_funcs_[slot] = func;
  slot_id = slot;

  return 0;
}

void
TAO_Cleanup_Func_Registry::cleanup (ACE_Array_Base<void *> &ts_objects)
{
  size_t len = ts_objects.size ();

  // The allocated slot may never been used.  It is therefore possible
  // that the TSS array size maybe less than the cleanup function
  // size.  However, there is still a one-to-one correspondence
  // between cleanup_func[foo] and ts_object[foo].

  ACE_ASSERT (len <= this->cleanup_funcs_.size ());

  /// Cleanup each TSS object.
  for (size_t i = 0; i < len; ++i)
    {
      ACE_CLEANUP_FUNC destructor = this->cleanup_funcs_[i];
      destructor (ts_objects[i], 0);
    }
}
