/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Proxy.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_PROXY_H
#define TAO_PROXY_H

#include "Attributes_T.h"

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

template <class TRADER>
class TAO_Proxy : 
  public TAO_Trader_Components <POA_CosTrading::Proxy>,
  public TAO_Support_Attributes <POA_CosTrading::Proxy>
{
public:

  TAO_Proxy (TRADER &trader);

  ~TAO_Proxy (void);

  virtual CosTrading::OfferId
    export_proxy (CosTrading::Lookup_ptr target, 
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
		    CosTrading::DuplicatePolicyName));

  virtual void withdraw_proxy (const char *id,
			       CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::IllegalOfferId, 
		    CosTrading::UnknownOfferId, 
		    CosTrading::Proxy::NotProxyOfferId));

  virtual CosTrading::Proxy::ProxyInfo *
  describe_proxy (const char *id,
		  CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::IllegalOfferId, 
		    CosTrading::UnknownOfferId, 
		    CosTrading::Proxy::NotProxyOfferId));

   // = CosTrading::TraderComponents methods. 

  virtual void list_proxies (CORBA::ULong how_many,
			     CosTrading::OfferIdSeq*& ids,
			     CosTrading::OfferIdIterator_ptr& id_itr,
			     CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::NotImplemented));
				
private:
  TRADER &trader_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Proxy.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

#endif /* TAO_PROXY_H */
