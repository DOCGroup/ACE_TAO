/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Attributes.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_ATTRIBUTES_H
#define TAO_ATTRIBUTES_H

#include "orbsvcs/CosTradingS.h"

class TAO_Trader_Base;
 
class TAO_Export TAO_Support_Attributes_Impl
  // = TITLE
  //     This class stores, allows access to and modification of 
  //     trader's support attributes.
  //
  // = DESCRIPTION
  //     Contains a reference to the trader from which it is
  //     used so it can use trader's lock.
{
public:
  // = Initialization and termination methods.
  TAO_Support_Attributes_Impl (TAO_Trader_Base &trader);
  ~TAO_Support_Attributes_Impl ();

  // = Accessor methods.
  CORBA::Boolean supports_modifiable_properties (void) const;
  void supports_modifiable_properties (CORBA::Boolean);

  CORBA::Boolean supports_dynamic_properties (void) const;
  void supports_dynamic_properties (CORBA::Boolean);

  CORBA::Boolean supports_proxy_offers (void) const;
  void supports_proxy_offers (CORBA::Boolean);

  CosTrading::TypeRepository_ptr type_repos (void) const;
  void type_repos (CosTrading::TypeRepository_ptr);

private:

  TAO_Trader_Base &trader_;
  // A reference to the trader (needed for obtaining the lock.)

  CORBA::Boolean supports_modifiable_properties_;
  // Indicator of whether the trader supports property modification.

  CORBA::Boolean supports_dynamic_properties_;
  // Indicator of whether the trader supports dynamic properties.

  CORBA::Boolean supports_proxy_offers_;
  // Indicator of whether the trader supports proxy offers.

  CORBA::Object_var type_repos_;
  // A reference to the TypeRepostitory used by the trader.
};

class TAO_Export TAO_Link_Attributes_Impl
  // = TITLE
  //     This class stores, allows access to and modification of 
  //     trader's link attributes.
  //
  // = DESCRIPTION
  //     Contains a reference to the trader from which it is
  //     used so it can use trader's lock.
{
public:
  // = Initialization and termination methods.

  TAO_Link_Attributes_Impl (TAO_Trader_Base &trader);
  ~TAO_Link_Attributes_Impl ();

  // = Accessor methods.
  
  CosTrading::FollowOption max_link_follow_policy (void) const;
  void  max_link_follow_policy (CosTrading::FollowOption);

private:

  TAO_Trader_Base &trader_;
  // A reference to the trader (needed for obtaining the lock.)

  CosTrading::FollowOption max_link_follow_policy_;
  // 
};

class TAO_Export TAO_Import_Attributes_Impl
  // = TITLE
  //     This class stores, allows access to and modification of 
  //     trader's import attributes.
  //
  // = DESCRIPTION
  //     Contains a reference to the trader from which it is
  //     used so it can use trader's lock.
{
public:
  // = Initialization and termination methods.

  TAO_Import_Attributes_Impl (TAO_Trader_Base &trader);

  ~TAO_Import_Attributes_Impl (void);

  // = Accessor methods.
  
  CORBA::ULong def_search_card (void) const;
  void def_search_card (CORBA::ULong);
	
  CORBA::ULong max_search_card (void) const;
  void max_search_card (CORBA::ULong);

  CORBA::ULong def_match_card (void) const;
  void def_match_card (CORBA::ULong);

  CORBA::ULong max_match_card (void) const;
  void max_match_card (CORBA::ULong);

  CORBA::ULong def_return_card (void) const;
  void def_return_card (CORBA::ULong);

  CORBA::ULong max_return_card (void) const;
  void max_return_card (CORBA::ULong);

  CORBA::ULong max_list (void) const;
  void max_list (CORBA::ULong);

  CORBA::ULong def_hop_count (void) const;
  void def_hop_count (CORBA::ULong);

  CORBA::ULong max_hop_count (void) const;
  void max_hop_count (CORBA::ULong);

  CosTrading::FollowOption def_follow_policy (void) const;
  void def_follow_policy (CosTrading::FollowOption);

  CosTrading::FollowOption max_follow_policy (void) const;
  void max_follow_policy (CosTrading::FollowOption);

private:

  TAO_Trader_Base &trader_;

  CORBA::ULong def_search_card_;
  // Upper bound of offers to be searched if <search_card>
  // is not specified.  

  CORBA::ULong max_search_card_;
  // Maximum upper bound of offers to be searched.

  CORBA::ULong def_match_card_;
  // Upper bound of matched offers to be ordered if 
  // <match_card> is not specified).  

  CORBA::ULong max_match_card_; 
  // Maximum upper bound of matched offers to be ordered.

  CORBA::ULong def_return_card_;
  // Upper bound of ordered offers to be returned if 
  // <returned_card> is not specified.

  CORBA::ULong max_return_card_; 
  // Maximum upper bound of ordered offers to be returned.

  CORBA::ULong max_list_;
  // Upper bound of depth
    
  CORBA::ULong def_hop_count_;
  // Upper bound of depth of links to be traversed if <hop_count> is
  // not specified.

  CORBA::ULong max_hop_count_;
  // Maximum upper bound of depth of links to be traversed.
    
  CosTrading::FollowOption def_follow_policy_;
  // Default link follow policy for a particular trader.

  CosTrading::FollowOption max_follow_policy_;
  // Limiting link follow policy for all links of the trader -
  // overrides both link and importer policies.
};

class TAO_Export TAO_Trading_Components_Impl
  //
  // = TITLE
  //     Set/get methods for references to various
  //     interfaces of the trader.
  //
  // = DESCRIPTION
  //     Note, this class is for use in local address space only and is NOT a
  //     direct implementation of IDL methods. (Implementation of 
  //     IDL methods in CosTrading::Trader_Components would need
  //     add a _duplicate call for each get method). 
{
public:

  // = Initialization and termination methods.
  TAO_Trading_Components_Impl (TAO_Trader_Base &trader);
  ~TAO_Trading_Components_Impl (void);

  // = CosTrading::TraderComponents methods. 

  CosTrading::Lookup_ptr lookup_if (void) const;
  // Returns an object reference to the Lookup interface of the trader.
  // Returns nil if the trader does not support Lookup interface.

  void lookup_if (CosTrading::Lookup_ptr);
  // Set the reference to the Lookup interface.
  
  CosTrading::Register_ptr register_if (void) const;
  // Returns object reference for the Register interface of the trader.
  // Returns nil if the trader does not support Register interface.

  void register_if (CosTrading::Register_ptr);
  // Set the reference to the Register interface of the trader.
  
  CosTrading::Link_ptr link_if (void) const;
  // Returns object reference for the Link interface of the trader.
  // Returns nil if the trader does not support Link interface.

  void link_if (CosTrading::Link_ptr);
  // Set the reference to the Link interface of the trader.

  CosTrading::Proxy_ptr proxy_if (void) const;
  // Returns object reference to the Proxy interface of the trader.
  // Returns nil if the trader does not support Proxy interface. 

  void proxy_if (CosTrading::Proxy_ptr);
  // Set the reference to the Proxy interface of the trader.
  
  CosTrading::Admin_ptr admin_if (void) const;
  // Returns object reference for the Admin interface of the trader.
  // Returns nil if the trader does not support Admin interface.

  void admin_if (CosTrading::Admin_ptr);
  // Set the reference to the Admin interface of the trader.

private:

  TAO_Trader_Base &trader_;

  CosTrading::Lookup_var lookup_;
  CosTrading::Register_var register_;
  CosTrading::Link_var link_;
  CosTrading::Proxy_var proxy_;
  CosTrading::Admin_var admin_;
};


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
#include "Attributes.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_ATTRIBUTES_H */
