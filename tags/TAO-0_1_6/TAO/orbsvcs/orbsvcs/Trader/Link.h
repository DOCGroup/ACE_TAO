/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Link.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>,
//    Irfan Pyarali <irfan@cs.wustl.edu>,
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_LINK_H
#define TAO_LINK_H

#include "CosTradingS.h"

template <class TRADER>
class TAO_Link : public POA_CosTrading::Link
{
public:

  TAO_Link (const TRADER &trader);

  ~TAO_Link (void);
		
  virtual void add_link (const char *name, 
			 CosTrading::Lookup_ptr target, 
			 CosTrading::FollowOption def_pass_on_follow_rule, 
			 CosTrading::FollowOption limiting_follow_rule,
			 CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException,
		    CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::DuplicateLinkName, 
		    CosTrading::InvalidLookupRef, 
		    CosTrading::Link::DefaultFollowTooPermissive, 
		    CosTrading::Link::LimitingFollowTooPermissive);

  virtual void remove_link (const char *name,
			    CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException,
		    CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::UnknownLinkName);

  virtual CosTrading::Link::LinkInfo* describe_link (const char *name,
						     CORBA::Environment&) 
    TAO_THROW_SPEC (CORBA::SystemException, 
		    CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::UnknownLinkName);

  virtual CosTrading::LinkNameSeq* list_links (CORBA::Environment& _env)
    TAO_THROW_SPEC (CORBA::SystemException);

  virtual void modify_link (const char *name, 
			    CosTrading::FollowOption def_pass_on_follow_rule, 
			    CosTrading::FollowOption limiting_follow_rule,
			    CORBA::Environment& _env) 
    TAO_THROW_SPEC (CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::UnknownLinkName, 
		    CosTrading::Link::DefaultFollowTooPermissive, 
		    CosTrading::Link::LimitingFollowTooPermissive);

   // = CosTrading::TraderComponents methods. 

  virtual CosTrading::Lookup_ptr lookup_if (CORBA::Environment& _env)
      TAO_THROW_SPEC (CORBA::SystemException);
  // Returns an object reference to the Lookup interface of the trader.
  // Returns nil if the trader does not support Lookup interface.
  
  virtual CosTrading::Register_ptr register_if (CORBA::Environment& _env)
      TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference for the Register interface of the trader.
  // Returns nil if the trader does not support Register interface.
  
  virtual CosTrading::Link_ptr link_if (CORBA::Environment& _env)
      TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference for the Link interface of the trader.
  // Returns nil if the trader does not support Link interface.

  virtual CosTrading::Proxy_ptr proxy_if (CORBA::Environment& _env) 
      TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference to the Proxy interface of the trader.
  // Returns nil if the trader does not support Proxy interface. 
  
  virtual CosTrading::Admin_ptr admin_if (CORBA::Environment& _env) 
      TAO_THROW_SPEC (CORBA::SystemException);
  // Returns object reference for the Admin interface of the trader.
  // Returns nil if the trader does not support Admin interface.
		
  // = CosTrading::SupportAttributes methods.

  virtual CORBA::Boolean supports_modifiable_properties (CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException);
    
  virtual CORBA::Boolean supports_dynamic_properties (CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException);
    
  virtual CORBA::Boolean supports_proxy_offers (CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException);
    
  virtual CosTrading::TypeRepository_ptr type_repos (CORBA::Environment& _env) 
    TAO_THROW_SPEC (CORBA::SystemException);

  // = CosTrading::LinkAttributes methods.
  
  virtual CosTrading::FollowOption max_link_follow_policy (CORBA::Environment& _env)
    TAO_THROW_SPEC (CORBA::SystemException);

private:
  TRADER &trader_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Link.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_LINK_H */
