/* -*- C++ -*- */

// =====================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Lookup.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// =====================================================================

#if !defined (TAO_LOOKUP_C)
#define TAO_LOOKUP_C

#if defined (OS_NO_NAMESPACE)
#define queue sillything
#define map mommy
#endif /* OS_NO_NAMESPACE */

#include "Lookup.h"
#include "ace/Auto_Ptr.h"
#include <iostream.h>

#if defined (OS_NO_NAMESPACE)
#undef queue
#undef map
#endif /* OS_NO_NAMESPACE */

#include "Query_Only_Offer_Iterator.h"
#include "Register_Offer_Iterator.h"

template <class TRADER>
const char* TAO_Lookup<TRADER>::NAME = "Trader: Lookup";

template <class TRADER>
TAO_Lookup<TRADER>::TAO_Lookup (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components<POA_CosTrading::Lookup> (trader.trading_components ()),
    TAO_Support_Attributes<POA_CosTrading::Lookup> (trader.support_attributes ()),
    TAO_Import_Attributes<POA_CosTrading::Lookup> (trader.import_attributes ())
{
}

template <class TRADER>
TAO_Lookup<TRADER>::~TAO_Lookup (void)
{
}

template <class TRADER> void
TAO_Lookup<TRADER>::
query (const char *type,
       const char *constraint,
       const char *preferences,
       const CosTrading::PolicySeq &in_policies,
       const CosTrading::Lookup::SpecifiedProps &desired_props,
       CORBA::ULong how_many,
       CosTrading::OfferSeq_out returned_offers,
       CosTrading::OfferIterator_out returned_offer_iterator,
       CosTrading::PolicyNameSeq_out returned_limits_applied,
       CORBA::Environment& env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType,
		   CosTrading::UnknownServiceType,
		   CosTrading::IllegalConstraint,
		   CosTrading::Lookup::IllegalPreference,
		   CosTrading::Lookup::IllegalPolicyName,
		   CosTrading::Lookup::PolicyTypeMismatch,
		   CosTrading::Lookup::InvalidPolicyValue,
		   CosTrading::IllegalPropertyName,
		   CosTrading::DuplicatePropertyName,
		   CosTrading::DuplicatePolicyName))
{    
  // Initializing out parameters
  returned_offers = new CosTrading::OfferSeq;
  returned_offer_iterator = CosTrading::OfferIterator::_nil ();

  // Get service type map
  TRADER::SERVICE_TYPE_MAP& service_type_map =
    this->trader_.service_type_map ();
  
  // If the type is invalid or unknown, let us know now.
  TAO_Support_Attributes_Impl support_attrs =
    this->trader_.support_attributes ();  
  CosTrading::TypeRepository_ptr type_repos =
    support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, env);
  TAO_CHECK_ENV_RETURN (env,);
  TAO_Policies policies (this->trader_, in_policies, env);
  TAO_CHECK_ENV_RETURN (env,);

  // If type is not found, there is nothing to consider - return.
  // Else we found the service type....proceed with lookup.
  // We will store the matched offers in here.
  LOOKUP_OFFER_LIST ordered_offers;
  
  // Perform the lookup, placing the ordered results in ordered_offers.
  this->perform_lookup (type,
			constraint,
			preferences,
			service_type_map,
			rep,
			policies,
			ordered_offers,
			env);      
  TAO_CHECK_ENV_RETURN (env,);
  
  // Fill the return sequence and iterator with the bountiful results.
  this->fill_receptacles (type,
			  ordered_offers,
			  how_many,			  
			  desired_props,
			  returned_offers,
			  returned_offer_iterator,
			  env);
  TAO_CHECK_ENV_RETURN (env,);

  // Return the limits applied during the course of the lookup.
  returned_limits_applied = policies.limits_applied ();
}

template <class TRADER> void
TAO_Lookup<TRADER>::
perform_lookup (const char* type,
		const char* constraint,
		const char* preferences,
		SERVICE_TYPE_MAP& service_type_map,
		CosTradingRepos::ServiceTypeRepository_ptr rep,		
		TAO_Policies& policies,
		LOOKUP_OFFER_LIST& ordered_offers,
		CORBA::Environment& env)
  TAO_THROW_SPEC ((CosTrading::IllegalConstraint,
		  CosTrading::Lookup::IllegalPreference,
		  CosTrading::Lookup::PolicyTypeMismatch,
		  CosTrading::Lookup::InvalidPolicyValue,
		  CosTrading::IllegalServiceType,
		  CosTrading::UnknownServiceType))
{    
  // TAO_Offer_Filter -- ensures that we don't consider offers with
  // modifiable or dynamic properties if the Trader doesn't support
  // them, or the importer has turned them off using policies.
  // TAO_Constraint_Validator -- validates the constraint with the
  // property types in the supplied type.
  // TAO_Constraint_Interpreter -- parses the constraint string, and
  // determines whether an offer meets those constraints.
  // TAO_Preference_Interpreter -- parses the preference string and
  // orders offers according to those constraints.
  TYPE_STRUCT type_struct (rep->fully_describe_type (type, env));
  TAO_CHECK_ENV_RETURN (env,);
  TAO_Offer_Filter offer_filter (type_struct.ptr (), policies, env);
  TAO_Constraint_Validator validator (type_struct.ptr ());
  TAO_Constraint_Interpreter constr_inter (validator, constraint, env);
  TAO_CHECK_ENV_RETURN (env,);
  TAO_Preference_Interpreter pref_inter (validator, preferences, env);
  TAO_CHECK_ENV_RETURN (env,);
  CORBA::ULong return_card = policies.return_card (env);
  TAO_CHECK_ENV_RETURN (env,);
  
  // Try to find the map of offers of desired service type.
  this->lookup_one_type (type,
			 service_type_map,
			 constr_inter,
			 pref_inter,
			 offer_filter);

  // If the importer hasn't demanded an exact match search, we search
  // all the subtypes of the supplied type. NOTE: Only the properties
  // belonging to the provided type are considered on
  // subtypes. Additional properties on the subtype are generally
  // ignored. This is as it should be, consistent with the notions of
  // type inheritence.
  if (! policies.exact_type_match (env))
    {
      TAO_CHECK_ENV_RETURN (env,);
      this->lookup_all_subtypes (type,
				 service_type_map,
				 rep,
				 constr_inter,
				 pref_inter,
				 offer_filter);
    }
  TAO_CHECK_ENV_RETURN (env,);

  // Pull the matched offers out of the pref_inter in order, and stick 
  // them in a queue. The offers in the queue will be emptied into
  // the return sequence and iterator for later purusal by the
  // importer. Only prepare for the importer no more offers than the
  // return cardinality default or policy allows.
  int num_offers = pref_inter.num_offers ();
  for (int i = 0; i < num_offers && i < return_card; i++)
    {
      CosTrading::Offer* offer;
      CosTrading::OfferId offer_id;
      if (pref_inter.remove_offer (offer_id, offer))
	ordered_offers.push_back (make_pair (offer_id, offer));
      else
	break;
    }
}

template <class TRADER> void
TAO_Lookup<TRADER>::
lookup_one_type (const char* type,
		 SERVICE_TYPE_MAP& service_type_map,
		 TAO_Constraint_Interpreter& constr_inter,
		 TAO_Preference_Interpreter& pref_inter,
		 TAO_Offer_Filter& offer_filter)
{
  // Retrieve an iterator over the offers for a given type.
  auto_ptr<LOCAL_OFFER_ITER>
    offer_iter (service_type_map.get_offers (type));

  if (offer_iter.get () != 0)
    {
      while (offer_filter.ok_to_consider_more () &&
	     offer_iter->has_more_offers ())
	{
	  // For each offer in the iterator, attempt to match it with
	  // the constraints passed to the Query method. If it matches 
	  // the constraint, use the TAO_Preference_Interpreter to
	  // order the matched offers with respect to the preference
	  // string passed to the method. All the while the offer
	  // iterator ensures we don't exceed the match cardinality
	  // constraints. 
	  CosTrading::Offer* offer = offer_iter->get_offer ();
	  
	  TAO_Constraint_Evaluator evaluator (offer);      
	  if (offer_filter.ok_to_consider (offer) &&
	      constr_inter.evaluate (evaluator))
	    {
	      // Shove the offer and its id into the preference
	      // ordering object, pref_inter.
	      CosTrading::OfferId offer_id = offer_iter->get_id ();
	      pref_inter.order_offer (offer_id, offer, evaluator);
	      offer_filter.matched_offer ();
	    }
      
	  offer_iter->next_offer ();
	}
    }
}

template <class TRADER> void
TAO_Lookup<TRADER>::
lookup_all_subtypes (const char* type,
		     SERVICE_TYPE_MAP& service_type_map,
		     CosTradingRepos::ServiceTypeRepository_ptr rep,
		     TAO_Constraint_Interpreter& constr_inter,
		     TAO_Preference_Interpreter& pref_inter,
		     TAO_Offer_Filter& offer_filter)
{
  // BEGIN SPEC
  // The trader may return a service offer of a subtype of the "type"
  // requested. Sub-typing of service types is discussed in "Service
  // Types" on page 16-4. A service subtype can be described by the
  // properties of its supertypes. This ensures that a well-formed query
  // for the "type" is also a well-formed query with respect to any
  // subtypes. However, if the importer specifies the policy of
  // exact_type_match = TRUE, then only offers with the exact (no
  // subtype) service type requested are returned.  
  // END SPEC

  // The algorithm works as follows:
  // Starting with the asked for type, iterate through all other types
  // looking for those that have it as their direct supertype. Upon
  // finding a direct subtype of the top-level types, consider all of
  // that type's offer and stick it onto the list of subtypes. On the
  // second iteration, locate subtypes of those subtypes. This
  // proceeds until all of the subtypes for the original class have
  // been located and their offers considered, or we've exhausted the
  // cardinality constraints.
  
  list<char*> sub_types, unconsidered_types;
  CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes sst;
  TYPE_NAME_SEQ all_types (service_type_map.list_all_types ());
  
  // All types save the supertype are initially unconsidered.
  sub_types.push_back ((char *) type);
  for (int i = all_types->length () - 1; i >= 0; i--)
    unconsidered_types.push_back ((char*)((const char*) all_types[i]));
  unconsidered_types.remove ((char *) type);

  // Iterate over the remaining subtypes to locate their subtypes.
  // We could meet our cardinality constraints prior searching all
  // types, at which point the algorithm ends gracefully.
  while (! sub_types.empty () && offer_filter.ok_to_consider_more ())  
  {
    // For each potential supertype, iterate over the remaining types.
    const char* super_type = sub_types.front ();
    sub_types.pop_front ();
    for (int j = unconsidered_types.size () - 1;
	 j >= 0 && offer_filter.ok_to_consider_more ();
	 j--)
      {
	TYPE_STRUCT type_struct;
	CORBA::Boolean is_sub_type = 0;
	char* type_name = unconsidered_types.front ();
	unconsidered_types.pop_front ();
	
	TAO_TRY
	  {
	    type_struct = rep->describe_type (type_name, TAO_TRY_ENV);
	  }
	TAO_CATCHANY
	  {
	    break;
	  }
	TAO_ENDTRY;

	// Determine if the prospective type is a subtype of the given one.
	for (int k = type_struct->super_types.length () - 1;
	     k >= 0 &&
	       ACE_OS::strcmp (type_struct->super_types[k], super_type);
	     k--)
	  ;

	// If this type isn't a subtype, return it to the queue for
	// later consideration.
	if (k < 0)
	  unconsidered_types.push_back (type_name);
	else
	  {
	    // Otherwise, perform a constraint match on the type, and
	    // add it to the queue of potential supertypes.
	    this->lookup_one_type (type_name,
				   service_type_map,
				   constr_inter,
				   pref_inter,
				   offer_filter);
	    sub_types.push_back (type_name);
	  }
      }
  }
}


template <class TRADER> void
TAO_Lookup<TRADER>::
fill_receptacles (const char* type,
		  LOOKUP_OFFER_LIST& ordered_offers,
		  CORBA::ULong how_many,		  
		  const CosTrading::Lookup::SpecifiedProps& desired_props,
		  CosTrading::OfferSeq*& offers,
		  CosTrading::OfferIterator_ptr& offer_itr,
		  CORBA::Environment& env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		  CosTrading::DuplicatePropertyName))
{
  // BEGIN SPEC
  // The returned offers are passed back in one of two ways (or a
  // combination of both). ° The "offers" return result conveys a list
  // of offers and the "offer_itr" is a reference to an interface at
  // which offers can be obtained. ° The "how_many" parameter states
  // how many offers are to be returned via the "offers" result, any
  // remaining offers are available via the iterator interface. If the
  // "how_many" exceeds the number of offers to be returned, then the
  // "offer_itr" will be nil.
  // END SPEC
  
  // Ordered offers iterator.
  LOOKUP_OFFER_LIST::iterator ordered_offers_iterator = 
    ordered_offers.begin ();
  TAO_Property_Filter prop_filter (desired_props, env);
  TAO_CHECK_ENV_RETURN (env,);
  
  // RETURNING: Calculate how many offers go into the sequence
  //  Calculate how many go into the iterator
  int size = ordered_offers.size ();
  CORBA::ULong offers_in_sequence = (how_many < size) ? how_many : size;
  CORBA::ULong offers_in_iterator = size - offers_in_sequence;

  offers->length (offers_in_sequence);
  
  // Add to the sequence, filtering out the undesired properties.
  for (int i = 0; i < offers_in_sequence; ordered_offers_iterator++, i++)
    {
      CosTrading::Offer& source = *((*ordered_offers_iterator).second);
      CosTrading::Offer& destination = (*offers)[i];
      prop_filter.filter_offer (source, destination);
    }
    
  // Any remaining offers go into iterator
  if (offers_in_iterator > 0)
    {
      // Create an iterator implementation 
      TAO_Offer_Iterator *oi =
	this->create_offer_iterator (type, prop_filter);  
      offer_itr = oi->_this (env);
      TAO_CHECK_ENV_RETURN (env,);
      
      // Add to the iterator
      for (i = 0;
	   i < offers_in_iterator;
	   ordered_offers_iterator++, i++)
	{
	  oi->add_offer ((*ordered_offers_iterator).first, 
			 (*ordered_offers_iterator).second);
	}
    }
}

template <class TRADER>
TAO_Offer_Iterator *
TAO_Lookup<TRADER>::
create_offer_iterator (const char *type,
		       const TAO_Property_Filter& pfilter)
{
  // This is the factory method that creates the appropriate type of
  // offer iterator. If there's no Register interface, then we can
  // just stick the offers directly into an iterator, since this
  // offers will never be removed from the Trader. If there's a
  // Register interface, then there's a chance that by the time the
  // importer called the next_n method on the iterator that the offer
  // has been withdrawn. So the Register_Offer_Iterator retains only
  // the offer ids, and will recognize when an offer id no longer
  // identifies an offer in the trader.

  // We pass the property filter to the iterators, so when they
  // return the offers, they can remove the undesirable properties
  // from them.  
  TAO_Offer_Iterator* iterator = 0;
  
  if (CORBA::is_nil (this->trader_.trading_components ().register_if ()))
    iterator =  new TAO_Query_Only_Offer_Iterator (pfilter);
  else
    {
      iterator =
	new TAO_Register_Offer_Iterator<TRADER> (this->trader_, pfilter);
    }

  return iterator;
}

#endif /* TAO_LOOKUP_C */
