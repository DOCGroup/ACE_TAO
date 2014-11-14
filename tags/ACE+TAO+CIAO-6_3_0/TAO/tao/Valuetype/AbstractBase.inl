// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::AbstractBase_ptr
CORBA::AbstractBase::_duplicate (CORBA::AbstractBase_ptr obj)
{
  if (obj)
    {
      obj->_add_ref ();
    }

  return obj;
}

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
  if (this->is_objref_)
    {
      if (!CORBA::is_nil (this->equivalent_obj_.in ()))
        {
          return this->equivalent_obj_->_stubobj ();
        }
    }
  return 0;
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
CORBA::Object_ptr
CORBA::AbstractBase::equivalent_objref (void)
{
  return this->equivalent_obj_.in ();
}

ACE_INLINE void
CORBA::AbstractBase::_decr_refcount (void)
{
  this->_remove_ref ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
