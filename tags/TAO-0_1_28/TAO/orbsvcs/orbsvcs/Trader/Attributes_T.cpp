// Attributes_T.cpp
// $Id$

#if !defined (TAO_ATTRIBUTES_C)
#define TAO_ATTRIBUTES_C

#include "Attributes_T.h"

template <class IF>
TAO_Trader_Components<IF>::
TAO_Trader_Components (const TAO_Trading_Components_Impl& comps) 
  : comps_ (comps)
{
}

template <class IF> CosTrading::Lookup_ptr
TAO_Trader_Components<IF>::lookup_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Lookup::_duplicate (this->comps_.lookup_if ());
}

template <class IF> CosTrading::Register_ptr
TAO_Trader_Components<IF>::register_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Returning Register Interface.\n"));
  return CosTrading::Register::_duplicate (this->comps_.register_if ());
}

template <class IF> CosTrading::Admin_ptr
TAO_Trader_Components<IF>::admin_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Admin::_duplicate (this->comps_.admin_if ());
}

template <class IF> CosTrading::Proxy_ptr
TAO_Trader_Components<IF>::proxy_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Proxy::_duplicate (this->comps_.proxy_if ());
}

template <class IF> CosTrading::Link_ptr
TAO_Trader_Components<IF>::link_if (CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Link::_duplicate (this->comps_.link_if ());
}
template <class IF> 
TAO_Support_Attributes<IF>::
TAO_Support_Attributes (const TAO_Support_Attributes_Impl& attrs) 
  : attrs_ (attrs)
{
}

template <class IF> CORBA::Boolean 
TAO_Support_Attributes<IF>::supports_modifiable_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_modifiable_properties ();
}

template <class IF> CORBA::Boolean 
TAO_Support_Attributes<IF>::supports_dynamic_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_dynamic_properties ();
}

template <class IF> CORBA::Boolean 
TAO_Support_Attributes<IF>::supports_proxy_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_proxy_offers ();
}

template <class IF> CosTrading::TypeRepository_ptr 
TAO_Support_Attributes<IF>::type_repos (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Returning Service Type Repository.\n"));
  return CosTrading::TypeRepository::_duplicate (this->attrs_.type_repos ());
}

template <class IF> 
TAO_Import_Attributes<IF>::
TAO_Import_Attributes (const TAO_Import_Attributes_Impl& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_search_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_search_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_search_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_search_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_match_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_match_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_match_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_match_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_return_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_return_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_return_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_return_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_list (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_list ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_hop_count (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_hop_count ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_hop_count (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_hop_count ();
}

template <class IF> CosTrading::FollowOption 
TAO_Import_Attributes<IF>::def_follow_policy (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_follow_policy ();
}

template <class IF> CosTrading::FollowOption 
TAO_Import_Attributes<IF>::max_follow_policy (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_follow_policy ();
}

template <class IF> 
TAO_Link_Attributes<IF>::
TAO_Link_Attributes (const TAO_Link_Attributes_Impl& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CosTrading::FollowOption
TAO_Link_Attributes<IF>::max_link_follow_policy (CORBA::Environment &env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_link_follow_policy ();
}

#endif /* TAO_ATTRIBUTES_C */
