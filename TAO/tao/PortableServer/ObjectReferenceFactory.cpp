// -*- C++ -*-
//
// $Id$

#include "tao/PortableServer/ObjectReferenceFactory.h"

ACE_RCSID (PortableServer,
           ObjectReferenceFactory,
           "$Id$")

#include "tao/PortableServer/PortableServerC.h"
#include "tao/PortableServer/POA.h"

# if !defined (__ACE_INLINE__)
#   include "tao/PortableServer/ObjectReferenceFactory.i"
# endif /* !__ACE_INLINE__ */

TAO_ObjectReferenceFactory::TAO_ObjectReferenceFactory (TAO_POA *poa)
  : poa_ (poa),
    servant_ (0),
    make_object_invoker_ (0)
{
  // Constructor
}

CORBA::Object *
TAO_ObjectReferenceFactory::make_object (
    const char *intf,
    const PortableInterceptor::ObjectId & id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  PortableServer::ObjectId oid;
  oid.replace (id.maximum (),
               id.length (),
               ACE_const_cast (CORBA::Octet *,id.get_buffer ()),
               0);

  // Create a reference
  CORBA::Object_var object =
    this->poa_->invoke_key_to_object (intf,
                                      oid
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return object._retn ();
}

void
TAO_ObjectReferenceFactory::set_servant (PortableServer::Servant
                                         servant)
{
  this->servant_ = servant;
}

void
TAO_ObjectReferenceFactory::make_object_invoker (CORBA::ULong identifier)
{
  this->make_object_invoker_ = identifier;
}
