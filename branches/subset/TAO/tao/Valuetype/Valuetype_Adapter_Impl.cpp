// $Id$

#include "Valuetype_Adapter_Impl.h"
#include "AbstractBase.h"
#include "ValueBase.h"
#include "ValueFactory_Map.h"
#include "tao/ORB_Core.h"

ACE_RCSID (Valuetype,
           Valuetype_Adapter_Impl,
           "$Id$")

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

CORBA::ULong
TAO_Valuetype_Adapter_Impl::type_info_single (void) const
{
  return TAO_OBV_GIOP_Flags::Type_info_single;
}

TAO_ValueFactory_Map *
TAO_Valuetype_Adapter_Impl::valuefactory_map_instance (void)
{
  return TAO_VALUEFACTORY_MAP::instance ();
}
    
int 
TAO_Valuetype_Adapter_Impl::vf_map_rebind (TAO_ValueFactory_Map *map,
                                           const char *repo_id,
                                           CORBA::ValueFactory &factory)
{
  return map->rebind (repo_id,
                      factory);
}

int 
TAO_Valuetype_Adapter_Impl::vf_map_find (TAO_ValueFactory_Map *map,
                                         const char *repo_id,
                                         CORBA::ValueFactory &factory)
{
  return map->find (repo_id,
                    factory);
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

ACE_STATIC_SVC_DEFINE (
    TAO_Valuetype_Adapter_Impl,
    ACE_TEXT ("Concrete_Valuetype_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Valuetype_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_Valuetype, TAO_Valuetype_Adapter_Impl)

