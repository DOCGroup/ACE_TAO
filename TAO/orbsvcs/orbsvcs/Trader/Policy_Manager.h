/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    ACE_Policies.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ======================================================================= 

#ifndef TAO_POLICY_MANAGER_H
#define TAO_POLICY_MANAGER_H

#if defined (OS_NO_NAMESPACE)
#define queue foobar
#endif /* OS_NO_NAMESPACE */

#include "orbsvcs/CosTradingC.h"

#if defined (OS_NO_NAMESPACE)
#undef queue
#endif /* OS_NO_NAMESPACE */

class TAO_Policy_Manager
// = TITLE
//
// This class is a utility for clients using the CosTrading::Lookup
// interface that helps them build a policy sequence without violating 
// syntax rules and having to mess with typecodes.
{
public:

  enum POLICY_TYPE
  {
    EXACT_TYPE_MATCH,
    HOP_COUNT,
    LINK_FOLLOW_RULE,
    MATCH_CARD,
    RETURN_CARD,
    SEARCH_CARD,
    STARTING_TRADER,
    USE_DYNAMIC_PROPERTIES,
    USE_MODIFIABLE_PROPERTIES,
    USE_PROXY_OFFERS
  };

  static const char * POLICY_NAMES[];
  
  TAO_Policy_Manager (int num_policies = 0);

  // = Routines to set policies.
  
  void search_card (CORBA::ULong scard);
  
  void match_card (CORBA::ULong mcard);
    
  void return_card (CORBA::ULong rcard);

  void use_modifiable_properties (CORBA::Boolean mod_props);

  void use_dynamic_properties (CORBA::Boolean dyn_props);

  void use_proxy_offers (CORBA::Boolean prox_offs);

  void starting_trader (CosTrading::TraderName* name);
  
  void link_follow_rule (CosTrading::FollowOption follow_option);

  void hop_count (CORBA::ULong hop_count);

  void exact_type_match (CORBA::Boolean exact_type);

  operator const CosTrading::PolicySeq& (void) const;

  const CosTrading::PolicySeq& policy_seq (void) const;
  // Return a PolicySeq suitable for passing to the query method of
  // the Lookup interface.
  
private:

  CosTrading::Policy& fetch_next_policy (void);
  
  int num_policies_;
  
  CosTrading::PolicySeq policies_;
};

#endif /* TAO_POLICY_MANAGER_H */
