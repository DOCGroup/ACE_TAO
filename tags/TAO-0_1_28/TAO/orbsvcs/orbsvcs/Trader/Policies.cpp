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
  "use_proxy_offers",
  "request_id"
};

TAO_Policies::TAO_Policies (TAO_Trader_Base& trader,
			    const CosTrading::PolicySeq& policies,
			    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPolicyName,
		  CosTrading::DuplicatePolicyName))
  : trader_ (trader)
{
  for (int i = 0; i < TAO_NUM_POLICIES; i++)
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
	  if (pol_name[2] == 't')
	    index = RETURN_CARD;
	  else if (pol_name[2] == 'q')
	    index = REQUEST_ID;
	  break;
	case 's':
	  if (pol_name[1] == 't')
	    index == STARTING_TRADER;
	  else if (pol_name[1] == 'e')
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

TAO_Policies::~TAO_Policies (void)
{
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
    case HOP_COUNT:
      return_value = import_attrs.def_hop_count ();
      max_value = import_attrs.max_hop_count ();
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
  CORBA::Boolean def_value = CORBA::B_TRUE,
    return_value = CORBA::B_TRUE;
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
    case EXACT_TYPE_MATCH:
      def_value = CORBA::B_FALSE;
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
        value >>= CORBA::Any::to_boolean (return_value);

      if (def_value == CORBA::B_FALSE &&
	  pol != EXACT_TYPE_MATCH)
	return_value = CORBA::B_FALSE;
    }
  else
    return_value = def_value;

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
  CosTrading::TraderName* trader_name = 0;
  
  if (this->policies_[STARTING_TRADER] != 0)
    {
      CosTrading::Policy* policy = this->policies_[STARTING_TRADER];
      CosTrading::PolicyValue& value = policy->value;
      CORBA::TypeCode* type = value.type ();
      
      if (!type->equal (CosTrading::_tc_TraderName, _env))
	TAO_THROW_RETURN (CosTrading::Lookup::PolicyTypeMismatch (*policy),
			  trader_name);
      else
	{
	  //	  ACE_NEW_RETURN (trader_name, CosTrading::TraderName, 0);
	  value >>= trader_name;
	}
    }

  return trader_name;
}

CosTrading::FollowOption
TAO_Policies::link_follow_rule (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  CosTrading::FollowOption return_value = 
    this->trader_.import_attributes ().def_follow_policy ();

  if (this->policies_[LINK_FOLLOW_RULE] != 0)
    {
      CosTrading::FollowOption max_follow_policy = 
	this->trader_.import_attributes ().max_follow_policy ();

      CosTrading::Policy* policy = this->policies_[LINK_FOLLOW_RULE];
      CosTrading::PolicyValue& value = policy->value;
      CORBA::TypeCode* type = value.type ();

      // Extract the link follow rule 
      if (!type->equal (CosTrading::_tc_FollowOption, _env))
	TAO_THROW_RETURN (CosTrading::Lookup::PolicyTypeMismatch (*policy),
			  return_value);
      else
	value >>= return_value;

      if (return_value > max_follow_policy)
	return_value = max_follow_policy;
    }

  return return_value;
}

CosTrading::FollowOption
TAO_Policies::link_follow_rule (const char* link_name,
				CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
		   CosTrading::Lookup::InvalidPolicyValue,
		   CosTrading::Link::IllegalLinkName,
		   CosTrading::Link::UnknownLinkName))
{
  CosTrading::FollowOption return_value = CosTrading::local_only;
  CosTrading::Link_var link
    (this->trader_.trading_components ().link_if ());

  if (link.in() != CosTrading::Link::_nil ())
    {
      CosTrading::Link::LinkInfo_var
	link_info (link->describe_link (link_name, _env));
      TAO_CHECK_ENV_RETURN (_env, return_value);
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
	    value >>= query_link_follow_rule;
	  
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
  return this->ulong_prop (HOP_COUNT, _env);
}

CosTrading::Admin::OctetSeq*
TAO_Policies::request_id (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch))
{
  CosTrading::Admin::OctetSeq* request_id = 0;
  
  if (this->policies_[REQUEST_ID] != 0)
    {
      CosTrading::Policy* policy = this->policies_[REQUEST_ID];
      CosTrading::PolicyValue& value = policy->value;
      CORBA::TypeCode* type = value.type ();
      
      if (!type->equal (CosTrading::Admin::_tc_OctetSeq, _env))
	TAO_THROW_RETURN (CosTrading::Lookup::PolicyTypeMismatch (*policy),
			  request_id);
      else
	{
	  //ACE_NEW_RETURN (request_id, CosTrading::Admin::OctetSeq, 0);
	  value >>= request_id;
	}
    }

  return request_id;
}

CosTrading::PolicySeq*
TAO_Policies::policies_to_forward (void)
{
  // Create a new policy sequence, shortening the starting trader
  // policy by one link.
  
  CORBA::Environment env;
  CORBA::ULong counter = 0;
  CosTrading::Policy* policy_buffer =
    CosTrading::PolicySeq::allocbuf (REQUEST_ID + 1);
  
  if (policy_buffer == 0)
    return 0;
  
  for (int i = EXACT_TYPE_MATCH; i <= REQUEST_ID; i++)
    {
      CosTrading::Policy& new_policy = policy_buffer[counter];
      
      if (this->policies_[i] != 0)
	{
	  // Copy in the existing policies.
	  new_policy.name = this->policies_[i]->name;

	  if (i == STARTING_TRADER)
	    {
	      TAO_TRY
		{
		  // Eliminate the first link of the trader name.
		  CosTrading::TraderName* trader_name =
		    this->starting_trader (env);
		  CORBA::ULong length = trader_name->length ();
		  
		  for (int j = 1; j < length; j++)
		    trader_name[j - 1] = trader_name[j];		  
		  trader_name->length (length - 1);

		  new_policy.value <<= *trader_name;
		}
	      TAO_CATCHANY {}
	      TAO_ENDTRY;	      
	    }
	  else
	    new_policy.value = this->policies_[i]->value;
	}
    }

  // Create the new sequence
  return new CosTrading::PolicySeq (REQUEST_ID + 1, counter,
				    policy_buffer, CORBA::B_TRUE);
}

CosTrading::PolicySeq*
TAO_Policies::
policies_to_pass (CosTrading::FollowOption def_pass_on_follow_rule,
		  CORBA::ULong offers_returned,
		  CosTrading::Admin_ptr admin_if)
{
  // Create a new policy sequence to pass to a federated
  // query. Decrement the hop count, add a stem id if none exists, and 
  // add a follow rule if none exists. Also adjust the return_card to
  // account for previous queries.
  CORBA::Environment env;
  CORBA::ULong counter = 0;
  CosTrading::Policy* policy_buffer =
    CosTrading::PolicySeq::allocbuf (REQUEST_ID + 1);

  if (policy_buffer == 0)
    return 0;
  
  for (int i = EXACT_TYPE_MATCH; i <= REQUEST_ID; i++)
    {
      CosTrading::Policy& new_policy = policy_buffer[counter];
      
      if (this->policies_[i] != 0)
	{
	  // Add an existing query.
	  new_policy.name = this->policies_[i]->name;
	  new_policy.value = this->policies_[i]->value;
	}
      else
	{	  
	  if (i == LINK_FOLLOW_RULE)
	    {
	      // Add a link follow rule if one didn't exist.
	      new_policy.name = POLICY_NAMES[i];
	      new_policy.value <<= def_pass_on_follow_rule;
	    }
	  else if (i == REQUEST_ID)
	    {
	      // Add the request id if one didn't exist.
	      new_policy.name = POLICY_NAMES[i];
	      new_policy.value <<= *(admin_if->request_id_stem (env));
	    }
	}    

      if (i == HOP_COUNT)
	{
	  // Decrement the hop count.
	  new_policy.name =  POLICY_NAMES[i];
	  new_policy.value <<= this->hop_count (env) - 1;
	}
      else if (i == RETURN_CARD)
	{
	  // Adjust the return card.
	  new_policy.name =  POLICY_NAMES[i];
	  new_policy.value <<= this->return_card (env) - offers_returned;
	}
    }

  // Create the new sequence.
  return new CosTrading::PolicySeq (REQUEST_ID + 1, counter,
				    policy_buffer, CORBA::B_TRUE);
}
