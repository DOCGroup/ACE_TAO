// -*- C++ -*-
//
// $Id$

ACE_INLINE PortableServer::POA_ptr
TAO_Regular_POA::the_parent (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->parent_ != 0)
    return PortableServer::POA::_duplicate (this->parent_);
  else
    return PortableServer::POA::_nil ();
}

