// $Id$

#include "Policy_Manager.h"

TAO_Policy_Manager::TAO_Policy_Manager (int num_policies)
  : policies_ (num_policies),
    num_policies_ (0)
{
  for (int i = 0; i < TAO_Policies::REQUEST_ID + 1; i++)
    this->poltable_[i] = -1;
}

void
TAO_Policy_Manager::search_card (CORBA::ULong scard)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::SEARCH_CARD);  
  policy.value <<= scard;    
}
  
void
TAO_Policy_Manager::match_card (CORBA::ULong mcard)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::MATCH_CARD);  
  policy.value <<= mcard;  
}

void
TAO_Policy_Manager::return_card (CORBA::ULong rcard)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::RETURN_CARD);  
  policy.value <<= rcard;  
}

void
TAO_Policy_Manager::use_modifiable_properties (CORBA::Boolean mod_props)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::USE_MODIFIABLE_PROPERTIES);  
  policy.value <<= CORBA::Any::from_boolean (mod_props);  
}

void
TAO_Policy_Manager::use_dynamic_properties (CORBA::Boolean dyn_props)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::USE_DYNAMIC_PROPERTIES);  
  policy.value <<= CORBA::Any::from_boolean (dyn_props);  
}

void
TAO_Policy_Manager::use_proxy_offers (CORBA::Boolean prox_offs)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::USE_PROXY_OFFERS);  
  policy.value <<= CORBA::Any::from_boolean (prox_offs);  
}

void
TAO_Policy_Manager::starting_trader (CosTrading::TraderName* name)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::STARTING_TRADER);  
  policy.value <<= *name;  
}

void
TAO_Policy_Manager::
link_follow_rule (CosTrading::FollowOption follow_option)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::LINK_FOLLOW_RULE);  
  policy.value <<= follow_option;  
}

void
TAO_Policy_Manager::hop_count (CORBA::ULong hop_count)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::HOP_COUNT);  
  policy.value <<= hop_count;  
}

void
TAO_Policy_Manager::exact_type_match (CORBA::Boolean exact_type)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::EXACT_TYPE_MATCH);  
  policy.value <<= CORBA::Any::from_boolean (exact_type);  
}

void
TAO_Policy_Manager::request_id (CosTrading::Admin::OctetSeq* request_id)
{
  CosTrading::Policy& policy =
    this->fetch_next_policy (TAO_Policies::REQUEST_ID);  
  policy.value <<= *request_id;
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
TAO_Policy_Manager::fetch_next_policy (POLICY_TYPE pol_type)
{
  CORBA::ULong index = 0;

  if (this->poltable_[pol_type] == -1)
    {
      int length = this->policies_.length ();
      this->num_policies_++;
      
      if (length < this->num_policies_)
	this->policies_.length (this->num_policies_);
      
      index = this->num_policies_ - 1;
      this->policies_[index].name = TAO_Policies::POLICY_NAMES[pol_type];
      this->poltable_[pol_type] = index;
    }
  else
    index = this->poltable_[pol_type];

  return this->policies_[index];
}
