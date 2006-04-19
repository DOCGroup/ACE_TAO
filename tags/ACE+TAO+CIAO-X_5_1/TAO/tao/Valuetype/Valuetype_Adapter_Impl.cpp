// $Id$

#include "tao/Valuetype/Valuetype_Adapter_Impl.h"
#include "tao/Valuetype/AbstractBase.h"
#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/ValueFactory_Map.h"

#include "tao/ORB_Core.h"


ACE_RCSID (Valuetype,
           Valuetype_Adapter_Impl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Valuetype_Adapter_Impl::~TAO_Valuetype_Adapter_Impl (void)
{
}

CORBA::Object_ptr
TAO_Valuetype_Adapter_Impl::abstractbase_to_object (
    CORBA::AbstractBase_ptr p
  )
{
  return p->_to_object ();
}

CORBA::Boolean
TAO_Valuetype_Adapter_Impl::stream_to_value (TAO_InputCDR &cdr,
                                             CORBA::ValueBase *& val)
{
  return cdr >> val;
}

CORBA::Boolean
TAO_Valuetype_Adapter_Impl::stream_to_abstract_base (
    TAO_InputCDR &cdr,
    CORBA::AbstractBase_ptr & obj
  )
{
  return cdr >> obj;
}

CORBA::ULong
TAO_Valuetype_Adapter_Impl::type_info_single (void) const
{
  return TAO_OBV_GIOP_Flags::Type_info_single;
}

int
TAO_Valuetype_Adapter_Impl::vf_map_rebind (const char *repo_id,
                                           CORBA::ValueFactory &factory)
{
  return TAO_VALUEFACTORY_MAP::instance ()->rebind (repo_id,
                                                    factory);
}

int
TAO_Valuetype_Adapter_Impl::vf_map_unbind (const char *repo_id)

{
  CORBA::ValueFactory fac;
  return TAO_VALUEFACTORY_MAP::instance ()->unbind (repo_id,
                                                    fac);
}

CORBA::ValueFactory
TAO_Valuetype_Adapter_Impl::vf_map_find (const char *repo_id)
{
  CORBA::ValueFactory factory = 0;
  (void) TAO_VALUEFACTORY_MAP::instance ()->find (repo_id,
                                                  factory);

  return factory;
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_Valuetype_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::valuetype_adapter_name (
      "Concrete_Valuetype_Adapter"
    );

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Valuetype_Adapter_Impl
      );
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
    TAO_Valuetype_Adapter_Impl,
    ACE_TEXT ("Concrete_Valuetype_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Valuetype_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_Valuetype, TAO_Valuetype_Adapter_Impl)
