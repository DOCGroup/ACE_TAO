/* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    Trading
// 
// = FILENAME
//    Proxy.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================

#if !defined (TAO_PROXY_C)
#define TAO_PROXY_C

#include "Proxy.h"

template <class TRADER>  
TAO_Proxy<TRADER>::TAO_Proxy (const TRADER &trader)
  : trader_ (trader)
{
}

template <class TRADER>
TAO_Proxy<TRADER>::~TAO_Proxy (void)
{
}

template <class TRADER> CosTrading::OfferId 
TAO_Proxy<TRADER>::export_proxy (CosTrading::Lookup_ptr target, 
				 const char *type, 
				 const CosTrading::PropertySeq& properties, 
				 CORBA::Boolean if_match_all, 
				 const char * recipe, 
				 const CosTrading::PolicySeq& policies_to_pass_on,
				 CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::InvalidLookupRef, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::Proxy::IllegalRecipe, 
		  CosTrading::DuplicatePropertyName, 
		  CosTrading::DuplicatePolicyName)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER> void
TAO_Proxy<TRADER>::withdraw_proxy (const char *id,
				   CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException,
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Proxy::NotProxyOfferId)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER> CosTrading::Proxy::ProxyInfo *
TAO_Proxy<TRADER>::describe_proxy (const char *id,
				   CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException,
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Proxy::NotProxyOfferId)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER>
CosTrading::Lookup_ptr
TAO_Proxy<TRADER>::lookup_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Lookup::_duplicate (this->trader_.trading_components ().lookup_if ());
}

template <class TRADER>
CosTrading::Register_ptr
TAO_Proxy<TRADER>::register_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Register::_duplicate (this->trader_.trading_components ().register_if ());
}

template <class TRADER>
CosTrading::Link_ptr
TAO_Proxy<TRADER>::link_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Link::_duplicate (this->trader_.trading_components ().link_if ());
}

template <class TRADER>
CosTrading::Proxy_ptr
TAO_Proxy<TRADER>::proxy_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Proxy::_duplicate (this->trader_.trading_components ().proxy_if ());
}

template <class TRADER>
CosTrading::Proxy_ptr
TAO_Proxy<TRADER>::admin_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Admin::_duplicate (this->trader_.trading_components ().admin_if ());
}

template <class TRADER>
CORBA::Boolean 
TAO_Proxy<TRADER>::supports_modifiable_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return this->trader_.support_attributes ().supports_modifiable_properties ();
}
  
template <class TRADER>
CORBA::Boolean 
TAO_Proxy<TRADER>::supports_dynamic_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return this->trader_.support_attributes ().supports_dynamic_properties ();
}

template <class TRADER>
CORBA::Boolean 
TAO_Proxy<TRADER>::supports_proxy_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return this->trader_.support_attributes ().supports_proxy_offers ();
}

#endif /* TAO_PROXY_C */
