// $Id$

#ifndef GATEWAY_OBJREF_FACTORY_H
#define GATEWAY_OBJREF_FACTORY_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/ValueBase.h"

#include "Object_Factory_i.h"

#include "GatewayS.h"

class Gateway_ObjRef_Factory
  : public CORBA_DefaultValueRefCountBase,
    public virtual PortableInterceptor::ObjectReferenceFactory
{

 public:

  Gateway_ObjRef_Factory (Gateway::Object_Factory_ptr gateway_object_factory,
                          PortableInterceptor::ObjectReferenceFactory *old_factory);

  virtual CORBA::Object_ptr make_object (const char *repository_id,
                                         const PortableInterceptor::ObjectId &id
                                        TAO_ENV_ARG_DECL);

 private:

  Gateway::Object_Factory_var gateway_object_factory_;

  PortableInterceptor::ObjectReferenceFactory_var old_factory_;
};

#endif /* GATEWAY_OBJREF_FACTORY_H */
