// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Naming_Context.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "Naming_Context.h"

TAO_Naming_Context::TAO_Naming_Context (void)
  : impl_ (0)
{
}

TAO_Naming_Context::~TAO_Naming_Context (void)
{
  delete impl_;
}

void
TAO_Naming_Context::impl (TAO_Naming_Context_Impl *impl)
{
  impl_ = impl;
}

PortableServer::POA_ptr
TAO_Naming_Context::_default_POA (CORBA::Environment &/*env*/)
{
  return impl_->_default_POA ();
}

void
TAO_Naming_Context::bind (const CosNaming::Name &n,
                          CORBA::Object_ptr obj,
                          CORBA::Environment &ACE_TRY_ENV)
{
  impl_->bind (n, obj, ACE_TRY_ENV);
}

void
TAO_Naming_Context::rebind (const CosNaming::Name &n,
                            CORBA::Object_ptr obj,
                            CORBA::Environment &ACE_TRY_ENV)
{
  impl_->rebind (n, obj, ACE_TRY_ENV);
}

void
TAO_Naming_Context::bind_context (const CosNaming::Name &n,
                                  CosNaming::NamingContext_ptr nc,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  impl_->bind_context (n, nc, ACE_TRY_ENV);
}

void
TAO_Naming_Context::rebind_context (const CosNaming::Name &n,
                                    CosNaming::NamingContext_ptr nc,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  impl_->rebind_context (n, nc, ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_Naming_Context::resolve (const CosNaming::Name &n,
                             CORBA::Environment &ACE_TRY_ENV)
{
  return impl_->resolve (n, ACE_TRY_ENV);
}

void
TAO_Naming_Context::unbind (const CosNaming::Name &n,
                            CORBA::Environment &ACE_TRY_ENV)
{
  impl_->unbind (n, ACE_TRY_ENV);
}

CosNaming::NamingContext_ptr
TAO_Naming_Context::new_context (CORBA::Environment &ACE_TRY_ENV)
{
  return impl_->new_context (ACE_TRY_ENV);
}

CosNaming::NamingContext_ptr
TAO_Naming_Context::bind_new_context (const CosNaming::Name &n,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  return impl_->bind_new_context (n, ACE_TRY_ENV);
}

void
TAO_Naming_Context::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  impl_->destroy (ACE_TRY_ENV);
}

void
TAO_Naming_Context::list (CORBA::ULong how_many,
                          CosNaming::BindingList_out bl,
                          CosNaming::BindingIterator_out bi,
                          CORBA::Environment &ACE_TRY_ENV)
{
  impl_->list (how_many, bl, bi, ACE_TRY_ENV);
}
