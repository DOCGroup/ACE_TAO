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

#include "stl.h"
#include "orbsvcs/CosTradingC.h"
#include "Policies.h"

class TAO_Policy_Manager
// = TITLE
//
// This class is a utility for clients using the CosTrading::Lookup
// interface that helps them build a policy sequence without violating 
// syntax rules and having to mess with typecodes.
{
public:

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

  void request_id (CosTrading::Admin::OctetSeq* reqiest_id);
  
  void exact_type_match (CORBA::Boolean exact_type);

  operator const CosTrading::PolicySeq& (void) const;

  const CosTrading::PolicySeq& policy_seq (void) const;
  // Return a PolicySeq suitable for passing to the query method of
  // the Lookup interface.
  
private:

  CosTrading::Policy& fetch_next_policy (TAO_Policies::POLICY_TYPE pol_type);
  
  int poltable_[TAO_Policies::REQUEST_ID + 1];
  
  CosTrading::PolicySeq policies_;

  CORBA::ULong num_policies_;
};

#endif /* TAO_POLICY_MANAGER_H */
