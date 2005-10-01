// -*- C++ -*-
//
// $Id$

ACE_INLINE size_t
TAO_Cleanup_Func_Registry::size (void) const
{
  return this->cleanup_funcs_.size ();
}
