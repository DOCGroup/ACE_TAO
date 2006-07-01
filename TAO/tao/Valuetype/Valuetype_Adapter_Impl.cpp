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

CORBA::Long
TAO_Valuetype_Adapter_Impl::type_info_single (void) const
{
  return TAO_OBV_GIOP_Flags::Type_info_single;
}

CORBA::Long
TAO_Valuetype_Adapter_Impl::type_info_implied (void) const
{
  return
    TAO_OBV_GIOP_Flags::Value_tag_base |
    TAO_OBV_GIOP_Flags::Type_info_none;
}

int
TAO_Valuetype_Adapter_Impl::vf_map_rebind (const char *repo_id,
                                           CORBA::ValueFactory &factory)
{
  return map_.rebind (repo_id, factory);
}

int
TAO_Valuetype_Adapter_Impl::vf_map_unbind (const char *repo_id)

{
  CORBA::ValueFactory fac;
  return map_.unbind (repo_id, fac);
}

CORBA::ValueFactory
TAO_Valuetype_Adapter_Impl::vf_map_find (const char *repo_id)
{
  CORBA::ValueFactory factory = 0;
  (void) map_.find (repo_id, factory);
  return factory;
}

TAO_END_VERSIONED_NAMESPACE_DECL
