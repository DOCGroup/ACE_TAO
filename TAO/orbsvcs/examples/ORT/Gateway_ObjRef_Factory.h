// -*- C++ -*-
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
  Gateway_ObjRef_Factory (Gateway_ObjRef_Factory const &) = delete;
  Gateway_ObjRef_Factory (Gateway_ObjRef_Factory &&) = delete;
  Gateway_ObjRef_Factory &operator= (Gateway_ObjRef_Factory const &) = delete;
  Gateway_ObjRef_Factory &operator= (Gateway_ObjRef_Factory &&) = delete;

  ::CORBA::ValueBase *_copy_value () override;

  CORBA::Object_ptr make_object (
      char const *repository_id,
      PortableInterceptor::ObjectId const &id) override;

private:
  Gateway::Object_Factory_var gateway_object_factory_;

  PortableInterceptor::ObjectReferenceFactory_var old_factory_;
};

#endif /* GATEWAY_OBJREF_FACTORY_H */
