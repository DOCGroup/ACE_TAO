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
TAO_Proxy<TRADER>::TAO_Proxy (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components <POA_CosTrading::Proxy> (trader.trading_components ()),
    TAO_Support_Attributes <POA_CosTrading::Proxy> (trader.support_attributes ())
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
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::InvalidLookupRef, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::Proxy::IllegalRecipe, 
		  CosTrading::DuplicatePropertyName, 
		  CosTrading::DuplicatePolicyName))
{
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), 0);
}

template <class TRADER> void
TAO_Proxy<TRADER>::withdraw_proxy (const char *id,
				   CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Proxy::NotProxyOfferId))
{
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));
}

template <class TRADER> CosTrading::Proxy::ProxyInfo *
TAO_Proxy<TRADER>::describe_proxy (const char *id,
				   CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Proxy::NotProxyOfferId))
{
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), 0);
}

template <class TRADER> void
TAO_Proxy<TRADER>::list_proxies (CORBA::ULong how_many,
				 CosTrading::OfferIdSeq*& ids,
				 CosTrading::OfferIdIterator_ptr& id_itr,
				 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::NotImplemented))
{
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));
}


#endif /* TAO_PROXY_C */
