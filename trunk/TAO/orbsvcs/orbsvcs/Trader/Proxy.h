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

#include "CosTradingS.h"

template <class TRADER>
class TAO_Proxy : public POA_CosTrading::Proxy
{
public:

  TAO_Proxy (const TRADER &trader);

  ~TAO_Proxy (void);

  virtual CosTrading::OfferId
    export_proxy (CosTrading::Lookup_ptr target, 
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
		    CosTrading::DuplicatePolicyName);

  virtual void withdraw_proxy (const char *id,
			       CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException, 
		    CosTrading::IllegalOfferId, 
		    CosTrading::UnknownOfferId, 
		    CosTrading::Proxy::NotProxyOfferId);

  virtual CosTrading::Proxy::ProxyInfo *
  describe_proxy (const char *id,
		  CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException,
		    CosTrading::IllegalOfferId, 
		    CosTrading::UnknownOfferId, 
		    CosTrading::Proxy::NotProxyOfferId);

   // = CosTrading::TraderComponents methods. 

  virtual void list_proxies (CORBA::ULong how_many,
			     CosTrading::OfferIdSeq*& ids,
			     CosTrading::OfferIdIterator_ptr& id_itr,
			     CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException,
		    CosTrading::NotImplemented);

  virtual CosTrading::Lookup_ptr lookup_if (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  // Returns an object reference to the Lookup interface of the trader.
  // Returns nil if the trader does not support Lookup interface.
  
  virtual CosTrading::Register_ptr register_if (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference for the Register interface of the trader.
  // Returns nil if the trader does not support Register interface.
  
  virtual CosTrading::Link_ptr link_if (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference for the Link interface of the trader.
  // Returns nil if the trader does not support Link interface.

  virtual CosTrading::Proxy_ptr proxy_if (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference to the Proxy interface of the trader.
  // Returns nil if the trader does not support Proxy interface. 
  
  virtual CosTrading::Admin_ptr admin_if (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference for the Admin interface of the trader.
  // Returns nil if the trader does not support Admin interface.
		
  // = CosTrading::SupportAttributes methods.

  virtual CORBA::Boolean supports_modifiable_properties (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  
  virtual CORBA::Boolean supports_dynamic_properties (CORBA::Environment& env)
  TAO_THROW_SPEC (CORBA::SystemException);
  
  virtual CORBA::Boolean supports_proxy_offers (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
  
  virtual CosTrading::TypeRepository_ptr type_repos (CORBA::Environment& env)
    TAO_THROW_SPEC (CORBA::SystemException);
		
private:
  TRADER &trader_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Proxy.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */


#endif /* TAO_PROXY_H */
