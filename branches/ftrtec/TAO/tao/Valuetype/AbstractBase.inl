// -*- C++ -*-
//
// $Id$

ACE_INLINE 
CORBA::AbstractBase_ptr
CORBA::AbstractBase::_nil (void)
{
  return (CORBA::AbstractBase_ptr)0;
}

/// Just call _duplicate and let it decide what to do.
ACE_INLINE 
CORBA::AbstractBase_ptr
CORBA::AbstractBase::_narrow (CORBA::AbstractBase_ptr obj
                              ACE_ENV_ARG_DECL_NOT_USED)
{
  return CORBA::AbstractBase::_duplicate (obj);
}

/// Same for this one.
ACE_INLINE 
CORBA::AbstractBase_ptr
CORBA::AbstractBase::_unchecked_narrow (CORBA::AbstractBase_ptr obj
                                        ACE_ENV_ARG_DECL_NOT_USED)
{
  return CORBA::AbstractBase::_duplicate (obj);
}

ACE_INLINE
CORBA::Boolean
CORBA::AbstractBase::_is_a (const char *type_id
                            ACE_ENV_ARG_DECL_NOT_USED)
{
  return ! ACE_OS::strcmp (type_id,
                           "IDL:omg.org/CORBA/AbstractBase:1.0");
}

ACE_INLINE
const char *
CORBA::AbstractBase::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/AbstractBase:1.0";
}

ACE_INLINE
const char *
CORBA::AbstractBase::_tao_obv_repository_id (void) const
{
  return "IDL:omg.org/CORBA/AbstractBase:1.0";
}

ACE_INLINE
void *
CORBA::AbstractBase::_tao_obv_narrow (ptrdiff_t /* type_id */)
{
  return this;
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

