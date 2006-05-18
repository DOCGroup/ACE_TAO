// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE PortableServer::POA_ptr
TAO_Regular_POA::the_parent (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->parent_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
