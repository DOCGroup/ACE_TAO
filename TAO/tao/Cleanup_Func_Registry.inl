// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE size_t
TAO_Cleanup_Func_Registry::size () const
{
  return this->cleanup_funcs_.size ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
