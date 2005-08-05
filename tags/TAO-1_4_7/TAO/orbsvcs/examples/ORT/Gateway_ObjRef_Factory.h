// -*- C++ -*-
//
// $Id$

#ifndef GATEWAY_OBJREF_FACTORY_H
#define GATEWAY_OBJREF_FACTORY_H

#include "ObjectReferenceFactoryC.h"
#include "GatewayC.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class Gateway_ObjRef_Factory
  : public CORBA::DefaultValueRefCountBase,
    public virtual OBV_ORT::ObjectReferenceFactory
{
public:

  Gateway_ObjRef_Factory (
    Gateway::Object_Factory_ptr gateway_object_factory,
    PortableInterceptor::ObjectReferenceFactory *old_factory);

  virtual CORBA::Object_ptr make_object (
      const char *repository_id,
      const PortableInterceptor::ObjectId &id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  Gateway::Object_Factory_var gateway_object_factory_;

  PortableInterceptor::ObjectReferenceFactory_var old_factory_;
};

#endif /* GATEWAY_OBJREF_FACTORY_H */
