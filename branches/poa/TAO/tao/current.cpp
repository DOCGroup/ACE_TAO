// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     current.cpp
//
// = VERSION
//     $Id$
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#include "tao/corba.h"
#include "tao/current.h"

TAO_POA_Current::TAO_POA_Current (void)
  : poa_impl_ (0),
    object_id_ (0),
    in_upcall_ (0),
    object_key_ (0)
{
}

TAO_POA_Current::~TAO_POA_Current (void)
{
}

PortableServer::POA_ptr
TAO_POA_Current::get_POA (CORBA_Environment &_env)
{
  if (! this->context_is_valid ())
    TAO_THROW_RETURN (PortableServer::Current::NoContext, PortableServer::POA::_nil ());
  
  PortableServer::POA_var result = poa_impl_->_this (env);
  if (env.exception () != 0)
    return PortableServer::POA::_nil ();

  return result._retn ();
}

PortableServer::ObjectId *
TAO_POA_Current::get_object_id (CORBA_Environment &_env)
{
  if (! this->context_is_valid ())
    TAO_THROW_RETURN (PortableServer::Current::NoContext, 0);

  // Create a new one and pass it back
  return new PortableServer::ObjectId (*object_id_);
}

