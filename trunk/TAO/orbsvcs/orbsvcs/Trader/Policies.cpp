// $Id$

#include "Policies.h"

const char* TAO_Policies::POLICY_NAMES[] = 
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

TAO_Policies::TAO_Policies (TAO_Trader_Base& trader,
			    const CosTrading::PolicySeq& policies,
			    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPolicyName,
		  CosTrading::DuplicatePolicyName))
  : trader_ (trader),
    policies_ (USE_PROXY_OFFERS + 1)
{
  for (int i = 0; i <= USE_PROXY_OFFERS; i++)
    this->policies_[i] = 0;

  for (int j = 0; j < policies.length (); j++)
    {      
      const char* pol_name = (const char*) policies[j].name;
      int length = (pol_name == 0) ? 0 : ACE_OS::strlen (pol_name),
	index = -1;

      if (length < ACE_OS::strlen (POLICY_NAMES[HOP_COUNT]))
	TAO_THROW (CosTrading::Lookup::IllegalPolicyName (pol_name));
      
      switch (pol_name[0])
	{
	case 'e':
	  index = EXACT_TYPE_MATCH;
	  break;
	case 'h':
	  index = HOP_COUNT;
	  break;
	case 'l':
	  index = LINK_FOLLOW_RULE;
	  break;
	case 'm':
	  index = MATCH_CARD;
	  break;
	case 'r':
	  index = RETURN_CARD;
	  break;
	case 's':
	  index = SEARCH_CARD;
	  break;
	case 'u':
	  if (pol_name[4] == 'd')
	    index = USE_DYNAMIC_PROPERTIES;
	  if (pol_name[4] == 'm')
	    index = USE_MODIFIABLE_PROPERTIES;
	  if (pol_name[4] == 'p')
	    index = USE_PROXY_OFFERS;
	}

      // Match the name of the policy, and insert its value into the
      // vector. 
      if (index == -1 || ::strcmp (POLICY_NAMES[index], pol_name) != 0)
	TAO_THROW (CosTrading::Lookup::IllegalPolicyName (pol_name));
      else
	{
	  if (this->policies_[index] != 0)
	    TAO_THROW (CosTrading::DuplicatePolicyName (pol_name));
	  else
	    this->policies_[index] = (CosTrading::Policy *) &(policies[j]);
	}
    }
}

CORBA::ULong
TAO_Policies::ulong_prop (POLICY_TYPE pol,
			  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  CORBA::ULong return_value = 0, max_value;
  TAO_Import_Attributes_Impl& import_attrs =
    this->trader_.import_attributes ();

  // Discover the default values for each of the possible cardinality
  // policies. 
  switch (pol)
    {
    case SEARCH_CARD:
      return_value = import_attrs.def_search_card ();
      max_value = import_attrs.max_search_card ();
      break;
    case MATCH_CARD:
      return_value = import_attrs.def_match_card ();
      max_value = import_attrs.max_match_card ();
      break;
    case RETURN_CARD:
      return_value = import_attrs.def_return_card ();
      max_value = import_attrs.max_return_card ();
      break;
    }
  
  if (this->policies_[pol] != 0)
    {
      // Extract the desired policy value.
      CosTrading::Policy* policy = this->policies_[pol];
      CosTrading::PolicyValue& value = policy->value;
      CORBA::TypeCode* type = value.type ();
      
      if (!type->equal (CORBA::_tc_ulong, _env))
	TAO_THROW_RETURN (CosTrading::Lookup::PolicyTypeMismatch (*policy),
			  return_value);
      else
	value >>= return_value;

      if (max_value < return_value)
	return_value = max_value;
      else
	this->limits_.push_back ((char *) POLICY_NAMES[pol]);
    }
  
  return return_value;
}

CORBA::ULong
TAO_Policies::search_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->ulong_prop (SEARCH_CARD, _env);
}

CORBA::ULong
TAO_Policies::match_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->ulong_prop (MATCH_CARD, _env);
}

CORBA::ULong
TAO_Policies::return_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->ulong_prop (RETURN_CARD, _env);
}

CORBA::Boolean
TAO_Policies::boolean_prop (POLICY_TYPE pol,
			    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  CORBA::Boolean def_value = (CORBA::Boolean) 1,
    return_value = (CORBA::Boolean) 1;
  TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();

  switch (pol)
    {
    case USE_MODIFIABLE_PROPERTIES:
      def_value = support_attrs.supports_modifiable_properties ();
      break;
    case USE_DYNAMIC_PROPERTIES:
      def_value = support_attrs.supports_dynamic_properties ();
      break;
    case USE_PROXY_OFFERS:
      def_value = support_attrs.supports_proxy_offers ();
      break;
    }
  
  if (this->policies_[pol] != 0)
    {
      CosTrading::Policy* policy = this->policies_[pol];
      CosTrading::PolicyValue& value = policy->value;
      CORBA::TypeCode* type = value.type ();
      
      if (!type->equal (CORBA::_tc_boolean, _env))
	TAO_THROW_RETURN (CosTrading::Lookup::PolicyTypeMismatch (*policy),
			  return_value);
      else
	value >>= to_boolean (return_value);

      if (def_value == (CORBA::Boolean) 0)
	return_value = (CORBA::Boolean) 0;
      else
	this->limits_.push_back ((char *) POLICY_NAMES[pol]);
    }

  return return_value;
}


CORBA::Boolean
TAO_Policies::use_modifiable_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->boolean_prop (USE_MODIFIABLE_PROPERTIES, _env);
}

CORBA::Boolean
TAO_Policies::use_dynamic_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->boolean_prop (USE_DYNAMIC_PROPERTIES, _env);
}

CORBA::Boolean
TAO_Policies::use_proxy_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->boolean_prop (USE_PROXY_OFFERS, _env);
}

CORBA::Boolean
TAO_Policies::exact_type_match (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return this->boolean_prop (EXACT_TYPE_MATCH, _env);
}


CosTrading::TraderName*
TAO_Policies::starting_trader (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
	 CosTrading::Lookup::InvalidPolicyValue))
{
  return 0;
}

CosTrading::FollowOption
TAO_Policies::link_follow_rule (const char* link_name,
				CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
		   CosTrading::Lookup::InvalidPolicyValue))
{
  CosTrading::FollowOption return_value = CosTrading::local_only;
  CosTrading::Link_var link
    (this->trader_.trading_components ().link_if ());
  TAO_CHECK_ENV_RETURN (_env, return_value);

  if (link != CosTrading::Link::_nil ())
    {
      CosTrading::Link::LinkInfo_var
	link_info (link->describe_link (link_name, _env));
      CosTrading::FollowOption trader_max_follow_policy = 
	this->trader_.import_attributes ().max_follow_policy ();

      if (this->policies_[LINK_FOLLOW_RULE] != 0)
	{
	  CosTrading::FollowOption query_link_follow_rule;
	  CosTrading::FollowOption link_limiting_follow_rule =
	    link_info->limiting_follow_rule;
	  CosTrading::Policy* policy = this->policies_[LINK_FOLLOW_RULE];
	  CosTrading::PolicyValue& value = policy->value;
	  CORBA::TypeCode* type = value.type ();

	  // Extract the link follow rule 
	  if (!type->equal (CosTrading::_tc_FollowOption, _env))
	    TAO_THROW_RETURN (CosTrading::Lookup::PolicyTypeMismatch (*policy),
			      return_value);
	  else
	    //	    value >>= query_link_follow_rule;
	    ;

	  return_value = (query_link_follow_rule < trader_max_follow_policy) 
	    ? query_link_follow_rule : trader_max_follow_policy;
	  return_value = (return_value < link_limiting_follow_rule)
	    ? return_value : link_limiting_follow_rule;
	}
      else
	{
	  CosTrading::FollowOption link_def_follow_rule =
	    this->trader_.import_attributes ().max_follow_policy ();

	  return_value = (link_def_follow_rule < trader_max_follow_policy)
	    ? link_def_follow_rule : trader_max_follow_policy;
	}           
    }

  return return_value;
}

CORBA::ULong
TAO_Policies::hop_count (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  return (CORBA::ULong) 0;
}


CosTrading::PolicyNameSeq*
TAO_Policies::limits_applied (void)
{
  int i = 0;
  CORBA::ULong size = this->limits_.size ();
  CosTrading::PolicyName* temp = CosTrading::PolicyNameSeq::allocbuf (size);

  for (POL_QUEUE::iterator p_iter = this->limits_.begin();
       p_iter != this->limits_.end ();
       p_iter++)  
    temp[i++] = CORBA::string_dup (*p_iter);
  
  return new CosTrading::PolicyNameSeq (i, i, temp, 1);
}
