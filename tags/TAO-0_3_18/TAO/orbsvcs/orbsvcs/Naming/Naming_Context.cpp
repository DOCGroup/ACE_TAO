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

TAO_Naming_Context::TAO_Naming_Context (TAO_Naming_Context_Impl *impl)
  : impl_ (impl)
{
}

TAO_Naming_Context::~TAO_Naming_Context (void)
{
  delete impl_;
}

PortableServer::POA_ptr
TAO_Naming_Context::_default_POA (CORBA::Environment &/*env*/)
{
  return impl_->_default_POA ();
}

void
TAO_Naming_Context::bind (const CosNaming::Name &n,
                          CORBA::Object_ptr obj,
                          CORBA::Environment &IT_env)
{
  impl_->bind (n, obj, IT_env);
}

void
TAO_Naming_Context::rebind (const CosNaming::Name &n,
                            CORBA::Object_ptr obj,
                            CORBA::Environment &IT_env)
{
  impl_->rebind (n, obj, IT_env);
}

void
TAO_Naming_Context::bind_context (const CosNaming::Name &n,
                                  CosNaming::NamingContext_ptr nc,
                                  CORBA::Environment &IT_env)
{
  impl_->bind_context (n, nc, IT_env);
}

void
TAO_Naming_Context::rebind_context (const CosNaming::Name &n,
                                    CosNaming::NamingContext_ptr nc,
                                    CORBA::Environment &IT_env)
{
  impl_->rebind_context (n, nc, IT_env);
}

CORBA::Object_ptr
TAO_Naming_Context::resolve (const CosNaming::Name &n,
                             CORBA::Environment &IT_env)
{
  return impl_->resolve (n, IT_env);
}

void
TAO_Naming_Context::unbind (const CosNaming::Name &n,
                            CORBA::Environment &IT_env)
{
  impl_->unbind (n, IT_env);
}

CosNaming::NamingContext_ptr
TAO_Naming_Context::new_context (CORBA::Environment &IT_env)
{
  return impl_->new_context (IT_env);
}

CosNaming::NamingContext_ptr
TAO_Naming_Context::bind_new_context (const CosNaming::Name &n,
                                      CORBA::Environment &IT_env)
{
  return impl_->bind_new_context (n, IT_env);
}

void
TAO_Naming_Context::destroy (CORBA::Environment &IT_env)
{
  impl_->destroy (IT_env);
}

void
TAO_Naming_Context::list (CORBA::ULong how_many,
                          CosNaming::BindingList_out bl,
                          CosNaming::BindingIterator_out bi,
                          CORBA::Environment &IT_env)
{
  impl_->list (how_many, bl, bi, IT_env);
}
