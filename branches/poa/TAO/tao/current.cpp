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

TAO_POA_Current_Impl::TAO_POA_Current_Impl (void)
  : poa_impl_ (0),
    object_id_ (0),
    in_upcall_ (0),
    object_key_ (0)
{}

TAO_POA_Current_Impl::~TAO_POA_Current_Impl (void)
{}

PortableServer::POA *
TAO_POA_Current_Impl::get_POA (CORBA_Environment &_env)
{
  if (! this->context_is_valid ())
    TAO_THROW_RETURN (PortableServer::Current::NoContext, 0);

  return poa_impl_->_this ();
}

PortableServer::ObjId *
TAO_POA_Current_Impl::get_object_id (CORBA_Environment &_env)
{
  if (! this->context_is_valid ())
    TAO_THROW_RETURN (PortableServer::Current::NoContext, 0);

  // Create a new one and pass it back
  return new PortableServer::ObjId_out(object_id_);
}
