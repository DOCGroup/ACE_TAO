// $Id$

#include "Policy_Manager.h"

const char* TAO_Policy_Manager::POLICY_NAMES[] = 
{
  "exact_type_match",
  "hop_count",
  "link_follow_rule",
  "match_card",
  "return_card",
  "search_card",
  "starting_trader",
  "use_dynamic_properties",
  "use_modifiable_properties",
  "use_proxy_offers"
};


TAO_Policy_Manager::TAO_Policy_Manager (int num_policies)
  : policies_ (num_policies),
    num_policies_ (0)
{
}

void
TAO_Policy_Manager::search_card (CORBA::ULong scard)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[SEARCH_CARD];
  policy.value <<= scard;    
}
  
void
TAO_Policy_Manager::match_card (CORBA::ULong mcard)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = (const char *) POLICY_NAMES[MATCH_CARD];
  policy.value <<= mcard;  
}

void
TAO_Policy_Manager::return_card (CORBA::ULong rcard)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[RETURN_CARD];
  policy.value <<= rcard;  
}

void
TAO_Policy_Manager::use_modifiable_properties (CORBA::Boolean mod_props)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = (const char *) POLICY_NAMES[USE_MODIFIABLE_PROPERTIES];
  policy.value <<= CORBA::Any::from_boolean (mod_props);  
}

void
TAO_Policy_Manager::use_dynamic_properties (CORBA::Boolean dyn_props)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = (const char *) POLICY_NAMES[USE_DYNAMIC_PROPERTIES];
  policy.value <<= CORBA::Any::from_boolean (dyn_props);  
}

void
TAO_Policy_Manager::use_proxy_offers (CORBA::Boolean prox_offs)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[USE_PROXY_OFFERS];
  policy.value <<= CORBA::Any::from_boolean (prox_offs);  
}

void
TAO_Policy_Manager::starting_trader (CosTrading::TraderName* name)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[STARTING_TRADER];
  policy.value <<= *name;  
}

void
TAO_Policy_Manager::
link_follow_rule (CosTrading::FollowOption follow_option)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[LINK_FOLLOW_RULE];
  policy.value <<= follow_option;  
}

void
TAO_Policy_Manager::hop_count (CORBA::ULong hop_count)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[HOP_COUNT];
  policy.value <<= hop_count;  
}

void
TAO_Policy_Manager::exact_type_match (CORBA::Boolean exact_type)
{
  CosTrading::Policy& policy = this->fetch_next_policy ();  
  policy.name = POLICY_NAMES[EXACT_TYPE_MATCH];
  policy.value <<= CORBA::Any::from_boolean (exact_type);  
}

TAO_Policy_Manager::operator const CosTrading::PolicySeq& (void) const
{
  return this->policies_;
}

const CosTrading::PolicySeq&
TAO_Policy_Manager::policy_seq (void) const
{
  return this->policies_;
}

CosTrading::Policy&
TAO_Policy_Manager::fetch_next_policy (void)
{
  int length = this->policies_.length ();

  this->num_policies_++;
  if (length < this->num_policies_)
    this->policies_.length (this->num_policies_);

  return this->policies_[this->num_policies_ - 1];
}
