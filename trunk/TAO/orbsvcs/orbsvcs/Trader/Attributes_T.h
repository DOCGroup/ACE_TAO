/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Attributes_T.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_ATTRIBUTES_T_H
#define TAO_ATTRIBUTES_T_H

#include "Attributes.h"

template <class IF>
class TAO_Trader_Components : public virtual IF
{
public:

  TAO_Trader_Components (const TAO_Trading_Components_Impl& comps);
  
  // = CosTrading::TraderComponents methods. 
  virtual CosTrading::Lookup_ptr lookup_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns an object reference to the Lookup interface of the trader.
  // Returns nil if the trader does not support Lookup interface.
  
  virtual CosTrading::Register_ptr register_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns object reference for the Register interface of the trader.
  // Returns nil if the trader does not support Register interface.
  
  virtual CosTrading::Link_ptr link_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException)); 
  // Returns object reference for the Link interface of the trader.
  // Returns nil if the trader does not support Link interface.

  virtual CosTrading::Proxy_ptr proxy_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns object reference to the Proxy interface of the trader.
  // Returns nil if the trader does not support Proxy interface. 
   
  virtual CosTrading::Admin_ptr admin_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns object reference for the Admin interface of the trader.
  // Returns nil if the trader does not support Admin interface.

private:

  const TAO_Trading_Components_Impl& comps_;
};

template <class IF>
class TAO_Support_Attributes : public virtual IF
{
public:

  TAO_Support_Attributes (const TAO_Support_Attributes_Impl& attrs);

  // = CosTrading::SupportAttributes methods.
  
  virtual CORBA::Boolean supports_modifiable_properties (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Boolean supports_dynamic_properties (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Boolean supports_proxy_offers (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CosTrading::TypeRepository_ptr type_repos (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
private:

  const TAO_Support_Attributes_Impl& attrs_;
};

template <class IF>
class TAO_Import_Attributes : public virtual IF
{
public:

  TAO_Import_Attributes (const TAO_Import_Attributes_Impl& attrs);

  // = CosTrading::ImportAttributes methods.
  
  virtual CORBA::ULong def_search_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_search_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // Search cardinality determines the maximum number of offers searched
  // before not considering other offers. 
  
  virtual CORBA::ULong def_match_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_match_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // Match cardinality determines the maximum number of offers
  // matched to the constraints before not considering other offers..
  
  virtual CORBA::ULong def_return_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_return_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // Return cardinality determines the maximum number of offers marked 
  // to return before not considering other offers.

  
  virtual CORBA::ULong max_list (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong def_hop_count (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_hop_count (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CosTrading::FollowOption def_follow_policy (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CosTrading::FollowOption max_follow_policy (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  
private:

  const TAO_Import_Attributes_Impl& attrs_;
};

template <class IF>
class TAO_Link_Attributes : public virtual IF
{
public:

  TAO_Link_Attributes (const TAO_Link_Attributes_Impl& attrs);

  // = CosTrading::LinkAttributes methods
  virtual CosTrading::FollowOption max_link_follow_policy (CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
    
private:

  const TAO_Link_Attributes_Impl& attrs_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Attributes_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_ATTRIBUTES_T_H */
