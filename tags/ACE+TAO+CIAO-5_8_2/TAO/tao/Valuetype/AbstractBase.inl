// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::AbstractBase_ptr
CORBA::AbstractBase::_nil (void)
{
  return static_cast <CORBA::AbstractBase_ptr> (0);
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA::AbstractBase::_narrow (CORBA::AbstractBase_ptr obj)
{
  return CORBA::AbstractBase::_duplicate (obj);
}

ACE_INLINE
CORBA::Boolean
CORBA::AbstractBase::_is_objref (void) const
{
  return this->is_objref_;
}

ACE_INLINE
TAO_Stub *
CORBA::AbstractBase::_stubobj (void) const
{
  return this->concrete_stubobj_;
}

ACE_INLINE
CORBA::Boolean
CORBA::AbstractBase::_is_collocated (void) const
{
  return this->is_collocated_;
}

ACE_INLINE
TAO_Abstract_ServantBase *
CORBA::AbstractBase::_servant (void) const
{
  return this->servant_;
}

ACE_INLINE
CORBA::Boolean
CORBA::AbstractBase::_is_local (void) const
{
  return this->is_local_;
}

ACE_INLINE void
CORBA::AbstractBase::_decr_refcount (void)
{
  this->_remove_ref ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
