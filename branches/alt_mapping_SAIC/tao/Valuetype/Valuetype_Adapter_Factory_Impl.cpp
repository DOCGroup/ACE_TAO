// -*- C++ -*-
// $Id$

#include "tao/Valuetype/Valuetype_Adapter_Impl.h"
#include "tao/Valuetype/Valuetype_Adapter_Factory_Impl.h"

#include "tao/ORB_Core.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Valuetype_Adapter_Factory_Impl::~TAO_Valuetype_Adapter_Factory_Impl (void)
{
}

TAO_Valuetype_Adapter *
TAO_Valuetype_Adapter_Factory_Impl::create (void)
{
  TAO_Valuetype_Adapter_Impl *nva = 0;
  ACE_NEW_THROW_EX (nva,
                    TAO_Valuetype_Adapter_Impl (),
                    CORBA::NO_MEMORY ());

  return nva;
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_Valuetype_Adapter_Factory_Impl::Initializer (void)
{
  TAO_ORB_Core::valuetype_adapter_factory_name (
      "Concrete_Valuetype_Adapter_Factory"
    );

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Valuetype_Adapter_Factory_Impl
      );
}


ACE_STATIC_SVC_DEFINE (
    TAO_Valuetype_Adapter_Factory_Impl,
    ACE_TEXT ("Concrete_Valuetype_Adapter_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Valuetype_Adapter_Factory_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_Valuetype, TAO_Valuetype_Adapter_Factory_Impl)
TAO_END_VERSIONED_NAMESPACE_DECL
