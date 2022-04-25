// -*- C++ -*-
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
CORBA::AbstractBase::_nil ()
{
  return nullptr;
}

ACE_INLINE CORBA::AbstractBase_ptr
CORBA::AbstractBase::_narrow (CORBA::AbstractBase_ptr obj)
{
  return CORBA::AbstractBase::_duplicate (obj);
}

ACE_INLINE
CORBA::Boolean
CORBA::AbstractBase::_is_objref () const
{
  return this->is_objref_;
}

ACE_INLINE
TAO_Stub *
CORBA::AbstractBase::_stubobj () const
{
  if (this->is_objref_)
    {
      if (!CORBA::is_nil (this->equivalent_obj_.in ()))
        {
          return this->equivalent_obj_->_stubobj ();
        }
    }
  return nullptr;
}

ACE_INLINE
CORBA::Boolean
CORBA::AbstractBase::_is_collocated () const
{
  return this->is_collocated_;
}

ACE_INLINE
TAO_Abstract_ServantBase *
CORBA::AbstractBase::_servant () const
{
  return this->servant_;
}

ACE_INLINE
CORBA::Object_ptr
CORBA::AbstractBase::equivalent_objref ()
{
  return this->equivalent_obj_.in ();
}

ACE_INLINE void
CORBA::AbstractBase::_decr_refcount ()
{
  this->_remove_ref ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
