/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Trader_Interfaces.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#if !defined (TAO_TRADER_INTERFACES_C)
#define TAO_TRADER_INTERFACES_C

#include "Trader_Interfaces.h"

  // *************************************************************
  // TAO_Lookup
  // *************************************************************

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
  returned_limits_applied = new CosTrading::PolicyNameSeq;
  
  // Get service type map
  Offer_Database& offer_database = this->trader_.offer_database ();

  TAO_Policies policies (this->trader_, in_policies, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  
  // If the importer has specified a starting trader, foward the
  // query.
  CosTrading::TraderName* trader_name = policies.starting_trader (env);
  TAO_CHECK_ENV_RETURN_VOID (env);

  if (trader_name != 0)
    {
      CosTrading::TraderName_var starting_trader (trader_name);
      this->forward_query (starting_trader.in (),
			   type,
			   constraint,
			   preferences,
			   policies,
			   desired_props,
			   how_many,
			   returned_offers,
			   returned_offer_iterator,
			   returned_limits_applied,
			   env);
      return;
    }
  
  // If a federated query returns to us, ignore it to prevent
  // redundant results.
  if (this->duplicate_stem_id (policies, env))
    return;

  // If the type is invalid or unknown, let us know now.
  const TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();  
  CosTrading::TypeRepository_ptr type_repos =
    support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  
  // If type is not found, there is nothing to consider - return.
  // Else we found the service type....proceed with lookup.
  // We will store the matched offers in here.
  Offer_Queue ordered_offers;
  
  // Perform the lookup, placing the ordered results in ordered_offers.
  this->perform_lookup (type,
			constraint,
			preferences,
			offer_database,
			rep,
			policies,
			ordered_offers,
			returned_limits_applied,
			env);      
  TAO_CHECK_ENV_RETURN_VOID (env);
  
  // Fill the return sequence and iterator with the bountiful results.
  CORBA::ULong offers_returned = 
    this->fill_receptacles (type,
			    ordered_offers,
			    how_many,			  
			    desired_props,
			    returned_offers,
			    returned_offer_iterator,
			    env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  
  // Determine if we should perform a federated query, and if so
  // construct a sequence of links to follow.
  CosTrading::LinkNameSeq* links = 0;
  CORBA::Boolean should_follow =
    this->retrieve_links (policies,
			  offers_returned,
			  CosTrading::LinkNameSeq_out (links),
			  env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  
  if (should_follow && links != 0)
    {
      // Perform the sequence of fedrated queries.
      CosTrading::LinkNameSeq_var links_to_follow (links);  
      this->federated_query (*links,
			     type,
			     constraint,
			     preferences,
			     policies,
			     desired_props,
			     how_many - returned_offers->length (),
			     returned_offers,
			     returned_offer_iterator,
			     returned_limits_applied,
			     env);
    }
  TAO_CHECK_ENV_RETURN_VOID (env);
}

template <class TRADER> void
TAO_Lookup<TRADER>::
perform_lookup (const char* type,
		const char* constraint,
		const char* preferences,
		Offer_Database& offer_database,
		CosTradingRepos::ServiceTypeRepository_ptr rep,		
		TAO_Policies& policies,
		Offer_Queue& ordered_offers,
    CosTrading::PolicyNameSeq_out returned_limits_applied,
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
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var
    type_struct (rep->fully_describe_type (type, env));
  TAO_CHECK_ENV_RETURN_VOID (env);
  TAO_Offer_Filter offer_filter (type_struct.ptr (), policies, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  TAO_Constraint_Validator validator (type_struct.ptr ());
  TAO_Constraint_Interpreter constr_inter (validator, constraint, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  TAO_Preference_Interpreter pref_inter (validator, preferences, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  CORBA::ULong return_card = policies.return_card (env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  
  // Try to find the map of offers of desired service type.
  this->lookup_one_type (type,
			 offer_database,
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
      TAO_CHECK_ENV_RETURN_VOID (env);
      this->lookup_all_subtypes (type,
				 type_struct->incarnation,
				 offer_database,
				 rep,
				 constr_inter,
				 pref_inter,
				 offer_filter);
    }
  TAO_CHECK_ENV_RETURN_VOID (env);

  // Pull the matched offers out of the pref_inter in order, and stick 
  // them in a queue. The offers in the queue will be emptied into
  // the return sequence and iterator for later purusal by the
  // importer. Only prepare for the importer no more offers than the
  // return cardinality default or policy allows.
  CORBA::ULong num_offers = pref_inter.num_offers ();
  for (CORBA::ULong i = 0; i < num_offers && i < return_card; i++)
    {
      CosTrading::Offer* offer;
      CosTrading::OfferId offer_id;
      if (pref_inter.remove_offer (offer_id, offer) == 0)
	{
	  Offer_Info offer_info;
	  offer_info.offer_id_ = offer_id;
	  offer_info.offer_ptr_ = offer;
	  ordered_offers.enqueue_tail (offer_info);
	}
      else
	break;
    }

  // Take note of the limits applied in this query.
  returned_limits_applied = offer_filter.limits_applied ();
}

template <class TRADER> void
TAO_Lookup<TRADER>::
lookup_one_type (const char* type,
		 Offer_Database& offer_database,
		 TAO_Constraint_Interpreter& constr_inter,
		 TAO_Preference_Interpreter& pref_inter,
		 TAO_Offer_Filter& offer_filter)
{
  // Retrieve an iterator over the offers for a given type.
  Offer_Database::offer_iterator offer_iter (type, offer_database);

  while (offer_filter.ok_to_consider_more () &&
	 offer_iter.has_more_offers ())
    {
      // For each offer in the iterator, attempt to match it with
      // the constraints passed to the Query method. If it matches 
      // the constraint, use the TAO_Preference_Interpreter to
      // order the matched offers with respect to the preference
      // string passed to the method. All the while the offer
      // iterator ensures we don't exceed the match cardinality
      // constraints. 
      CosTrading::Offer* offer = offer_iter.get_offer ();
      
      TAO_Constraint_Evaluator evaluator (offer);
      if (offer_filter.ok_to_consider (offer) &&
	  constr_inter.evaluate (evaluator))
	{
	  // Shove the offer and its id into the preference
	  // ordering object, pref_inter.
	  CosTrading::OfferId offer_id = offer_iter.get_id ();
	  pref_inter.order_offer (offer_id, offer, evaluator);
	  offer_filter.matched_offer ();
	}
      
      offer_iter.next_offer ();
    }
}

template <class TRADER> void
TAO_Lookup<TRADER>::
lookup_all_subtypes (const char* type,
		     CosTradingRepos::ServiceTypeRepository::IncarnationNumber& inc_num,
		     Offer_Database& offer_database,
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

  CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes sst;
  CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq_var all_types;
  
  // Optimization: Since a subtype can't have a higher incarnation
  // number than a supertype, we don't need to consider those
  // types with lower incarnation numbers.  
  sst._d (CosTradingRepos::ServiceTypeRepository::since);
  sst.incarnation (inc_num);  
  
  //  TAO_TRY
  //    {
  CORBA::Environment env;
  all_types = rep->list_types (sst, env);
  //      TAO_CHECK_ENV;
  //    }
  //  TAO_CATCHANY { return; }
  //  TAO_ENDTRY;  

  // Scan all types inserted after the super types. If the transitive
  // closure of a type's super type relation includes the super type
  // being considered, then perform a search on that type.
  CORBA::ULong num_types = all_types->length ();
  for (CORBA::ULong i = 0;
       i < num_types && offer_filter.ok_to_consider_more ();
       i++)
    {
      CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct;
      
      TAO_TRY
	{
	  // Obtain a description of the prospective type.
	  type_struct = rep->fully_describe_type (all_types[i], TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}
      TAO_CATCHANY
	{
	  break;
	}
      TAO_ENDTRY;

      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq&
	super_types = type_struct->super_types;
      CORBA::ULong num_super_types = super_types.length ();

      for (CORBA::ULong j = 0; j < num_super_types; j++)
	{
	  if (ACE_OS::strcmp (type_struct->super_types[j], type) == 0)
	    {
	      // Egads, a subtype!
	      this->lookup_one_type (all_types[i],
				     offer_database,
				     constr_inter,
				     pref_inter,
				     offer_filter);
	      break;
	    }
	}
    }  
}


template <class TRADER> int
TAO_Lookup<TRADER>::
fill_receptacles (const char* type,
		  Offer_Queue& ordered_offers,
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
  Offer_Queue::ITERATOR ordered_offers_iterator (ordered_offers);
  TAO_Property_Filter prop_filter (desired_props, env);
  TAO_CHECK_ENV_RETURN (env, 0);
  
  // RETURNING: Calculate how many offers go into the sequence
  //  Calculate how many go into the iterator
  CORBA::ULong size = ordered_offers.size ();
  CORBA::ULong offers_in_sequence = (how_many < size) ? how_many : size;
  CORBA::ULong offers_in_iterator = size - offers_in_sequence;
  CORBA::ULong total_offers = offers_in_sequence + offers_in_iterator;
  
  offers->length (offers_in_sequence);
  
  // Add to the sequence, filtering out the undesired properties.
  for (CORBA::ULong i = 0;
       i < offers_in_sequence;
       ordered_offers_iterator.advance (), i++)
    {
      Offer_Info* offer_info_ptr = 0;
      ordered_offers_iterator.next (offer_info_ptr);
      CosTrading::Offer& source = *offer_info_ptr->offer_ptr_;
      CosTrading::Offer& destination = (*offers)[i];
      prop_filter.filter_offer (source, destination);

      CORBA::string_free (offer_info_ptr->offer_id_);
    }
    
  // Any remaining offers go into iterator
  if (offers_in_iterator > 0)
    {
      // Create an iterator implementation 
      TAO_Offer_Iterator *oi =
	this->create_offer_iterator (type, prop_filter);
      offer_itr = oi->_this (env);
      TAO_CHECK_ENV_RETURN (env,total_offers - offers_in_iterator);
      
      // Add to the iterator
      for (i = 0;
	   i < offers_in_iterator;
	   ordered_offers_iterator.advance (), i++)
	{
	  Offer_Info* offer_info_ptr = 0;
	  ordered_offers_iterator.next (offer_info_ptr);      
	  oi->add_offer (offer_info_ptr->offer_id_,
			 offer_info_ptr->offer_ptr_);
	}
    }

  return total_offers;
}

template <class TRADER>
TAO_Offer_Iterator *
TAO_Lookup<TRADER>::
create_offer_iterator (const char *type,
		       const TAO_Property_Filter& pfilter)
{
  // This is the factory method that creates the appropriate type of
  // offer iterator. If there's no Register interface, then we can
  // just stick the offers directly into an iterator, since these
  // offers will never be removed from the Trader. If there's a
  // Register interface, then there's a chance that by the time the
  // importer calls the next_n method on the iterator that the offer
  // will have been withdrawn. So the Register_Offer_Iterator retains only
  // the offer ids, and will recognize when an offer id no longer
  // identifies an offer in the trader.

  // We pass the property filter to the iterators, so when the iterators
  // return the offers, they can remove the undesirable properties
  // from those offers.  
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

template <class TRADER> CORBA::Boolean
TAO_Lookup<TRADER>::duplicate_stem_id (TAO_Policies& policies,
				       CORBA::Environment& _env)
{
  // Determine whether the stem_id passed to this query is one we've 
  // already seen. If this is the case, then we shouldn't pursue this
  // query any further.
  CORBA::Boolean return_value = CORBA::B_FALSE;
  CosTrading::Admin::OctetSeq_var request_id = policies.request_id (_env);
  TAO_CHECK_ENV_RETURN (_env, return_value);

  // If the stem_id was provided and is a duplicate, return true.
  if ((request_id.ptr () != 0) &&
      this->request_ids_.insert (request_id) == 1)
    return_value = CORBA::B_TRUE;

  return return_value;
}

template <class TRADER> CORBA::Boolean
TAO_Lookup<TRADER>::retrieve_links (TAO_Policies& policies,
				    CORBA::ULong offers_returned,
				    CosTrading::LinkNameSeq_out links,
				    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::Lookup::PolicyTypeMismatch))
{
  CORBA::Boolean should_follow = CORBA::B_FALSE;
  CosTrading::FollowOption follow_rule = policies.link_follow_rule (_env);
  TAO_CHECK_ENV_RETURN (_env, should_follow);
  
  // Determine whether or not a federated query is warranted. A query
  // is waranted if the follow_rule governing this query is 'always'
  // or if_no_local and the local query returned nothing.
  if ((follow_rule == CosTrading::always ||
       (follow_rule == CosTrading::if_no_local && offers_returned == 0))
      && policies.hop_count (_env) > 0)
    should_follow = CORBA::B_TRUE;

  if (should_follow)
    {
      // Grab the names of all the links in the trader, and push
      // the suitable ones onto <valid_links>.
      CosTrading::Link_ptr link_interface
	= this->trader_.trading_components ().link_if ();
      ACE_Unbounded_Queue<CosTrading::LinkName> valid_links;
      CosTrading::LinkNameSeq_var link_path =
	link_interface->list_links (_env);
      TAO_CHECK_ENV_RETURN (_env, should_follow);
      
      // Determine which of the links registered with the Link
      // interface are suitable to follow. 
      for (int i = link_path->length () - 1; i >= 0; i--)
	{
	  CosTrading::FollowOption link_rule =
	    policies.link_follow_rule ((const char*) link_path[i], _env);
	  if (link_rule == CosTrading::always ||
	      (link_rule == CosTrading::if_no_local &&
	       offers_returned == 0))
	    valid_links.enqueue_tail ((char *)((const char*) link_path[i]));
	}

      // Collect those valid links into a sequence suitable for
      // passing into the federated_query method.
      CosTrading::LinkName link_name = 0,
	*link_buf = CosTrading::LinkNameSeq::allocbuf (valid_links.size ());

      if (link_buf != 0)
	{
	  for (i = valid_links.size () - 1; i >= 0; i--)
	    {	  
	      valid_links.dequeue_head (link_name);
	      link_buf[i] = CORBA::string_dup (link_name);
	    }
	  
	  ACE_NEW_RETURN (links,
			  CosTrading::LinkNameSeq (valid_links.size (),
						   valid_links.size (),
						   link_buf,
						   CORBA::B_TRUE),
			  0);
	}
    }      

  return should_follow;
}


template <class TRADER> void
TAO_Lookup<TRADER>::
federated_query (const CosTrading::LinkNameSeq& links,
		 const char *type,
		 const char *constr,
		 const char *pref,
		 TAO_Policies& policies,
		 const CosTrading::Lookup::SpecifiedProps& desired_props,
		 CORBA::ULong how_many,
		 CosTrading::OfferSeq_out offers,
		 CosTrading::OfferIterator_out offer_iter,
		 CosTrading::PolicyNameSeq_out limits,
		 CORBA::Environment& _env) 
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
  // The general idea here is this: We've assembled a number of links
  // to follow, and we'll query each of them in turn. When we've
  // assembled <return_card> number of offers, we've hit the ceiling
  // and we back out. On each query we adjust the policies for the new 
  // trader by reducing the return_card, hop_count, etc..., and merge
  // the results from the new query with the results from the previous 
  // queries.
  
  // We'll need the link and admin interfaces for this part of the
  // federated query.
  CORBA::ULong total_returned = 0,
    return_card = policies.return_card (_env);
  CosTrading::Link_ptr link_interface
    = this->trader_.trading_components ().link_if ();
  CosTrading::Admin_ptr admin_interface
    = this->trader_.trading_components ().admin_if ();

  // Begin collecting all the various offer_iterators into a
  // collection. The end result is a distributed tree of offer
  // iterators, which if traversed in its entirety is probably hugely
  // inefficient, but oh well, I can't think of a better solution.
  TAO_Offer_Iterator_Collection* offer_iter_collection;
  ACE_NEW (offer_iter_collection, TAO_Offer_Iterator_Collection);

  offer_iter_collection->add_offer_iterator (offer_iter);
  total_returned = offers->length () +
    offer_iter_collection->max_left (_env);

  for (int i = links.length () - 1; i >= 0; i--)
    {
      CosTrading::OfferSeq* out_offers = 0;
      CosTrading::OfferIterator* out_offer_iter = 0;
      CosTrading::PolicyNameSeq* out_limits = 0; 
      CosTrading::OfferSeq_var out_offers_var (out_offers);
      CosTrading::OfferIterator_var out_offer_iter_var (out_offer_iter);
      CosTrading::PolicyNameSeq_var out_limits_var (out_limits);
	    
      TAO_TRY
	{
	  // Obtain information about the link we're traversing.
	  CosTrading::Link::LinkInfo_var link_info =
	    link_interface->describe_link (links[i], TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  // Prepare a set of policies to pass to the next trader.
	  CosTrading::PolicySeq_var new_pols =
	    policies.policies_to_pass (link_info->def_pass_on_follow_rule,
				       total_returned,
				       admin_interface);
	  
	  // Perform the federated query.
	  link_info->target->
	    query (type,
		   constr,
		   pref,
		   new_pols.in (),
		   desired_props,
		   how_many - offers->length (),
		   CosTrading::OfferSeq_out (out_offers),
		   CosTrading::OfferIterator_out (out_offer_iter),
		   CosTrading::PolicyNameSeq_out (out_limits),
		   _env);
	  TAO_CHECK_ENV;
	  
	  // Merge the results.
	  CORBA::ULong offset = offers->length ();
	  offers->length (out_offers->length () + offset);
	  offer_iter_collection->add_offer_iterator (out_offer_iter);
	  for (int j = out_offers->length () - 1; j >= 0; j--)
	    offers[j + offset] = out_offers_var[j];

	  // Concatenate the limits applied.
	  offset = limits->length ();
	  offers->length (out_limits->length () + offset);
	  for (j = out_limits->length () - 1; j >= 0; j--)
	    limits[j + offset] = out_limits_var[j];

	  // If we've obtained all the offers we need, let's blow this 
	  // joint. 
	  if (total_returned >= return_card)
	    break;
	}
      TAO_CATCHANY
	{
	  break;
	}
      TAO_ENDTRY;
    }

  // Return the collection of offer iterators.
  offer_iter = offer_iter_collection->_this (_env);
}

template <class TRADER> void
TAO_Lookup<TRADER>::
forward_query (const CosTrading::TraderName& starting_trader,
		 const char *type,
		 const char *constr,
		 const char *pref,
		 TAO_Policies& policies,
		 const CosTrading::Lookup::SpecifiedProps& desired_props,
		 CORBA::ULong how_many,
		 CosTrading::OfferSeq_out offers,
		 CosTrading::OfferIterator_out offer_itr,
		 CosTrading::PolicyNameSeq_out limits_applied,
		 CORBA::Environment& _env)
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
  // Forward this query to the next link in the starting_trader sequence.
  CosTrading::Link_ptr link_interface
    = this->trader_.trading_components ().link_if ();

  TAO_TRY
    {  
      CosTrading::Link::LinkInfo_var link_info =
	link_interface->describe_link (starting_trader[0], TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      CosTrading::PolicySeq_var in_policies = policies.policies_to_forward ();

      // Perform forwarding query.
      link_info->target->query (type,
				constr,
				pref,
				in_policies.in (),
				desired_props,
				how_many,
				offers,
				offer_itr,
				limits_applied,
				_env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
    }
  TAO_CATCHANY
    {
      TAO_THROW (CosTrading::Lookup::InvalidPolicyValue ());
    }
  TAO_ENDTRY;
}

  // *************************************************************
  // TAO_Register
  // *************************************************************

template <class TRADER>
TAO_Register<TRADER>::TAO_Register (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components<POA_CosTrading::Register> (trader.trading_components ()),
    TAO_Support_Attributes<POA_CosTrading::Register> (trader.support_attributes ())
{
}

template <class TRADER>
TAO_Register<TRADER>::~TAO_Register (void)
{
}

template <class TRADER> CosTrading::OfferId 
TAO_Register<TRADER>::export (CORBA::Object_ptr reference, 
			      const char *type, 
			      const CosTrading::PropertySeq &properties,
			      CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::Register::InvalidObjectRef, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::Register::InterfaceTypeMismatch, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::DuplicatePropertyName))
{
  // For robustness purposes --
  if (CORBA::is_nil (reference))
    TAO_THROW_RETURN (CosTrading::Register::InvalidObjectRef (), 0);
  
  // Get service type map
  Offer_Database &offer_database = this->trader_.offer_database ();

  CosTrading::Offer offer;
  TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();  
  CosTrading::TypeRepository_ptr type_repos =
    support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);
  
  // Yank our friend, the type struct, and confirm that the given
  // properties match the type definition.
  CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct =
    rep->fully_describe_type (type, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);
  
  // Oops the type is masked, we shouldn't let exporters know the type 
  // exists.
  if (type_struct->masked)
    TAO_THROW_RETURN (CosTrading::UnknownServiceType (type), 0);

  // TAO-specific way to determine if an object is derived from or is
  // an interface type.
  if (! reference->_is_a (type_struct->if_name, _env))
    TAO_THROW_RETURN (CosTrading::Register::
		      InterfaceTypeMismatch (type, reference), 0);
  
  // Validate that the properties defined for this offer are correct
  // to their types and strength.
  this->validate_properties (type, type_struct,
			     (CosTrading::PropertySeq&) properties, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);

  offer.reference = reference->_duplicate (reference);
  offer.properties = properties;
  
  // Insert the offer into the underlying type map.
  CosTrading::OfferId id = offer_database.insert_offer (type, offer);
  
  if (id == 0)
    {
      // Add type, if it's already been added in that split second
      // since we've released the lock, nothing bad will happen.
      id = offer_database.insert_offer (type, offer);
    }
  
  return id;
}

template <class TRADER> void 
TAO_Register<TRADER>::withdraw (const char *id,
				CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::IllegalOfferId, 
		   CosTrading::UnknownOfferId, 
		   CosTrading::Register::ProxyOfferId))
{
  // Get service type map.
  Offer_Database &offer_database = this->trader_.offer_database ();
  offer_database.remove_offer ((CosTrading::OfferId) id, _env);
}

template <class TRADER> CosTrading::Register::OfferInfo * 
TAO_Register<TRADER>::describe (const char *id,
				CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Register::ProxyOfferId))
{
  // Get service type map.
  char* type = 0;
  Offer_Database &offer_database = this->trader_.offer_database ();

  // Perform a lookup to find the offer.
  CosTrading::Offer* offer =
    offer_database.lookup_offer ((CosTrading::OfferId) id, type, _env);
  TAO_CHECK_ENV_RETURN (_env, (CosTrading::Register::OfferInfo *) 0);
      
  CosTrading::Register::OfferInfo *offer_info = 
    new CosTrading::Register::OfferInfo ();

  offer_info->reference = offer->reference->_duplicate (offer->reference);
  offer_info->type = CORBA::string_dup (type);
  offer_info->properties = offer->properties;

  return offer_info;
}

template <class TRADER> void 
TAO_Register<TRADER>::modify (const char *id, 
			      const CosTrading::PropertyNameSeq& del_list, 
			      const CosTrading::PropertySeq& modify_list,
			      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::NotImplemented, 
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Register::ProxyOfferId, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::Register::UnknownPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::Register::MandatoryProperty, 
		  CosTrading::Register::ReadonlyProperty, 
		  CosTrading::DuplicatePropertyName))
{
  // Throw an exception if the trader is not configured
  // to support properties modification.
  if (! this->supports_modifiable_properties (_env))
    TAO_THROW (CosTrading::NotImplemented ());

  char* type = 0;
  TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();  
  CosTrading::TypeRepository_ptr type_repos = support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  Offer_Database &offer_database = this->trader_.offer_database ();
      
  CosTrading::Offer* offer =
    offer_database.lookup_offer ((CosTrading::OfferId) id, type, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  
  if (offer != 0)
    {
      // Yank our friend, the type struct.
      CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct = rep->describe_type (type, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      TAO_Offer_Modifier offer_mod (type, type_struct, *offer);
      
      // Delete, add, and change properties of the offer.
      this->validate_properties (type, type_struct,
				 (CosTrading::PropertySeq) modify_list, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      offer_mod.delete_properties (del_list, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      offer_mod.merge_properties (modify_list, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      
      // Alter our reference to the offer. 
      offer_mod.affect_change ();
    }
}

template <class TRADER> void 
TAO_Register<TRADER>::withdraw_using_constraint (const char *type, 
						 const char *constr,
						 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::IllegalConstraint, 
		  CosTrading::Register::NoMatchingOffers))
{
  int num_removed = 0;
  TAO_Support_Attributes_Impl&
    support_attrs = this->trader_.support_attributes ();
  CosTrading::TypeRepository_ptr type_repos = support_attrs.type_repos ();
  CosTradingRepos::ServiceTypeRepository_ptr rep = 
    CosTradingRepos::ServiceTypeRepository::_narrow (type_repos, _env);
  Offer_Database &offer_database =  this->trader_.offer_database ();
  CORBA::Boolean dp_support = support_attrs.supports_dynamic_properties ();
  ACE_Unbounded_Queue<CosTrading::OfferId_var> ids;
  
  // Retrieve the type struct
  CosTradingRepos::ServiceTypeRepository::TypeStruct*
    type_struct = rep->describe_type (type, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  // Try to find the map of offers of desired service type.
  Offer_Database::offer_iterator offer_iter (type, offer_database);
  TAO_Constraint_Validator validator (type_struct);
  TAO_Constraint_Interpreter constr_inter (validator, constr, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  
  while (offer_iter.has_more_offers ())
    {
      CosTrading::Offer* offer = offer_iter.get_offer ();
      // Add offer if it matches the constraints
      
      TAO_Constraint_Evaluator evaluator (offer, dp_support);
      if (constr_inter.evaluate (evaluator))
	ids.enqueue_tail (offer_iter.get_id ());
      
      offer_iter.next_offer ();
    }      
  
  if (ids.size () == 0)
    TAO_THROW (CosTrading::Register::NoMatchingOffers (constr));
  else
    {      
      while (! ids.is_empty ())
	{
	  CosTrading::OfferId_var offer_id;
	  
	  ids.dequeue_head (offer_id);
	  offer_database.remove_offer (offer_id, _env);
	}
    }
}
           
template <class TRADER> CosTrading::Register_ptr 
TAO_Register<TRADER>::resolve (const CosTrading::TraderName &name,
			       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::Register::IllegalTraderName, 
		  CosTrading::Register::UnknownTraderName, 
		  CosTrading::Register::RegisterNotSupported))
{
  // Determine if the first link is a legal link name.
  if (! TAO_Trader_Base::is_valid_identifier_name (name[0]))
    TAO_THROW_RETURN (CosTrading::Register::IllegalTraderName (name),
		      CosTrading::Register::_nil ());

  // Grab a reference to the link interface, and get a link description.
  CosTrading::Link_var link (this->link_if (_env));
  TAO_CHECK_ENV_RETURN (_env, CosTrading::Register::_nil ());
  CosTrading::Link::LinkInfo_var link_info;
  
  TAO_TRY
    {
      // Ensure that the link to the next trader exists.
      link_info = link->describe_link (name[0], TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_THROW_RETURN (CosTrading::Register::UnknownTraderName (name),
			CosTrading::Register::_nil ());
    }
  TAO_ENDTRY;
  
  // Ensure that the register pointer isn't nil.
  if (link_info->target_reg.in() == CosTrading::Register::_nil ())
    TAO_THROW_RETURN (CosTrading::Register::RegisterNotSupported (name),
		      CosTrading::Register::_nil ());

  CosTrading::Register_ptr return_value = link_info->target_reg;
  
  if (name.length () > 1)
    {
      // Create a new Trader Name with the first link removed.
      CosTrading::TraderName trader_name (name.length () - 1);
      for (int i = trader_name.length () - 1; i >= 0; i--)
	trader_name[i] = name[i + 1];

      return_value = link_info->target_reg->resolve (trader_name, _env);
      TAO_CHECK_ENV_RETURN (_env, CosTrading::Register::_nil ());
    }

  return return_value;
}

template <class TRADER> void
TAO_Register<TRADER>::
validate_properties (const char* type, 
		     CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
		     CosTrading::PropertySeq& properties,
		     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::DuplicatePropertyName))
{
  typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;
  
  int length = properties.length ();
  SERVICE_TYPE_REPOS::PropStructSeq& prop_types = type_struct->props;
  TAO_Property_Evaluator_By_Name prop_eval (properties, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  
  // Perform property validation
  length = prop_types.length ();
  for (int i = 0; i < length; i++)
    {
      SERVICE_TYPE_REPOS::PropStruct& prop_struct = prop_types[i]; 
      const char* prop_name  = prop_struct.name;
      
      // Obtain the type of the exported property.
      CORBA::TypeCode* prop_type = prop_eval.property_type (prop_name);
      
      if (prop_type == CORBA::TypeCode::_nil ())
	{
	  // Offer cannot have a missing mandatory property.
	  if (prop_types[i].mode == SERVICE_TYPE_REPOS::PROP_MANDATORY)
	    TAO_THROW (CosTrading::MissingMandatoryProperty (type, prop_name));
	}
      else
	{
	  if (! prop_type->equal (prop_struct.value_type, _env))
	    {
	      // Offer cannot redefine the type of an property. 
	      const CosTrading::Property* prop = prop_eval.get_property (prop_name);
	      TAO_THROW (CosTrading::PropertyTypeMismatch (type, *prop));
	    }
	  else if (prop_struct.mode == SERVICE_TYPE_REPOS::PROP_READONLY &&
		   prop_eval.is_dynamic_property (prop_name))
	    TAO_THROW (CosTrading::ReadonlyDynamicProperty (type, prop_name));
	}
    }
}

  // *************************************************************
  // TAO_Admin
  // *************************************************************

template <class TRADER>  
TAO_Admin<TRADER>::TAO_Admin (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components <POA_CosTrading::Admin> (trader.trading_components ()),
    TAO_Import_Attributes <POA_CosTrading::Admin> (trader.import_attributes ()),
    TAO_Support_Attributes <POA_CosTrading::Admin> (trader.support_attributes ()),
    TAO_Link_Attributes <POA_CosTrading::Admin> (trader.link_attributes ()),
    sequence_number_ (0)
{
  // Because a servant is uniquely identified by a POA name and an
  // ObjectID number, the concatenation of the two prefixed before a
  // sequence number will ensure the request_id_stem space between
  // traders will not overlap. The sequence number space will be four
  // octets.
  CORBA::Environment _env;
  PortableServer::POA_var poa = this->_default_POA (_env);
  PortableServer::ObjectId_var id = poa->servant_to_id (this, _env);
  CORBA::String_var poa_name = poa->the_name (_env);
  int name_length = ACE_OS::strlen (poa_name.in ()),
    id_length = id->length (),
    total_length = name_length + id_length + sizeof (CORBA::ULong);
  
  this->stem_id_.length (total_length);
  // @@ Commented it out until a fix is found
  
  for (int i = total_length - 1, j = name_length - 1; j >= 0; i--, j--) 
    //this->stem_id_[i] = (CORBA::Octet) poa_name[j];
    ;


  for (j = id_length - 1; j >= 0; j--, i--)
    this->stem_id_[i] = id[j];
}

template <class TRADER>
TAO_Admin<TRADER>::~TAO_Admin (void)
{
}

template <class TRADER> CosTrading::Admin::OctetSeq * 
TAO_Admin<TRADER>::request_id_stem (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  // Add one to the sequence_number and concatenate it to the unique
  // prefix. The sequence number is four octets long.
  for (int i = sizeof (CORBA::ULong) - 1; i >= 0; i--)
    this->stem_id_[i] = (this->sequence_number_ >> (8*i)) & 0xff;

  // Increment the sequence number and return a copy of the stem_id.
  this->sequence_number_++;
  return new CosTrading::Admin::OctetSeq (this->stem_id_);
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_def_search_card (CORBA::ULong value,
					CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException)) 
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_search_card ();

  this->trader_.import_attributes ().def_search_card (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_max_search_card (CORBA::ULong value,
					CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_search_card ();

  this->trader_.import_attributes ().max_search_card (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_def_match_card (CORBA::ULong value,
				       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_match_card ();

  this->trader_.import_attributes ().def_match_card (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_max_match_card (CORBA::ULong value,
				       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_match_card ();

  this->trader_.import_attributes ().max_match_card (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_def_return_card (CORBA::ULong value,
					CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_return_card ();
  
  this->trader_.import_attributes ().def_return_card (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_max_return_card (CORBA::ULong value,
					CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_return_card ();
  
  this->trader_.import_attributes ().max_return_card (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_max_list (CORBA::ULong value,
				 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_list ();
  
  this->trader_.import_attributes ().max_list (value);
  return return_value;
}

template <class TRADER> CORBA::Boolean 
TAO_Admin<TRADER>::
set_supports_modifiable_properties (CORBA::Boolean value,
				    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_modifiable_properties ();
  
  this->trader_.support_attributes ().supports_modifiable_properties (value);
  return return_value;
}

template <class TRADER> CORBA::Boolean 
TAO_Admin<TRADER>::
set_supports_dynamic_properties (CORBA::Boolean value,
				 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_dynamic_properties ();
  
  this->trader_.support_attributes ().supports_dynamic_properties (value);
  return return_value;
}

template <class TRADER> CORBA::Boolean 
TAO_Admin<TRADER>::set_supports_proxy_offers (CORBA::Boolean value,      
					      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_proxy_offers ();
  
  this->trader_.support_attributes ().supports_proxy_offers (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_def_hop_count (CORBA::ULong value,
				      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_hop_count ();
  
  this->trader_.import_attributes ().def_hop_count (value);
  return return_value;
}

template <class TRADER> CORBA::ULong 
TAO_Admin<TRADER>::set_max_hop_count (CORBA::ULong value,
				      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_hop_count ();
    
  this->trader_.import_attributes ().max_hop_count (value);
  return return_value;
}

template <class TRADER> CosTrading::FollowOption 
TAO_Admin<TRADER>::set_def_follow_policy (CosTrading::FollowOption policy,
					  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::FollowOption return_value =
    this->trader_.import_attributes ().def_follow_policy ();
    
  this->trader_.import_attributes ().def_follow_policy (policy);
  return return_value;
}

template <class TRADER> CosTrading::FollowOption 
TAO_Admin<TRADER>::set_max_follow_policy (CosTrading::FollowOption policy,
					  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::FollowOption return_value =
    this->trader_.import_attributes ().max_follow_policy ();
    
  this->trader_.import_attributes ().max_follow_policy (policy);
  return return_value;
}

template <class TRADER> CosTrading::FollowOption 
TAO_Admin<TRADER>::
set_max_link_follow_policy (CosTrading::FollowOption policy,
			    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::FollowOption return_value =
    this->trader_.link_attributes ().max_link_follow_policy ();
  
  this->trader_.link_attributes ().max_link_follow_policy (policy);
  return return_value;
}

template <class TRADER> CosTrading::TypeRepository_ptr 
TAO_Admin<TRADER>::
set_type_repos (CosTrading::TypeRepository_ptr repository,
		CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::TypeRepository_ptr return_value =
    this->trader_.support_attributes ().type_repos ();
  
  this->trader_.support_attributes ().type_repos (repository);
  return return_value;
}

template <class TRADER> CosTrading::Admin::OctetSeq* 
TAO_Admin<TRADER>::set_request_id_stem (const CosTrading::Admin::OctetSeq& stem,
					CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

template <class TRADER> void 
TAO_Admin<TRADER>::list_offers (CORBA::ULong how_many, 
				CosTrading::OfferIdSeq_out ids, 
				CosTrading::OfferIdIterator_out id_itr,
				CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,) 
		  CosTrading::NotImplemented)
{
  // This method only applies when the register interface is implemented
  if (CORBA::is_nil(this->trader_.trading_components().register_if()))
    TAO_THROW (CosTrading::NotImplemented());

  TRADER::Offer_Database& type_map = this->trader_.offer_database ();

  CosTrading::OfferIdIterator_ptr oi =
    type_map.retrieve_all_offer_ids ()->_this (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
      
  id_itr = CosTrading::OfferIdIterator::_nil ();
  if (how_many > 0)
    {      
      if (oi->next_n (how_many, ids, _env) == CORBA::B_FALSE)
	{
	  // No more items left in the iterator.
	  oi->destroy (_env);
	  oi = CosTrading::OfferIdIterator::_nil ();
	}
      else
	id_itr = oi;
    }
  else
    ids = new CosTrading::OfferIdSeq(0);
}

template <class TRADER> void 
TAO_Admin<TRADER>::list_proxies (CORBA::ULong how_many, 
				 CosTrading::OfferIdSeq_out ids, 
				 CosTrading::OfferIdIterator_out id_itr,
				 CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::NotImplemented))
{
  TAO_THROW (CosTrading::NotImplemented ());
}

  // *************************************************************
  // TAO_Link
  // *************************************************************


template <class TRADER, class MAP_LOCK_TYPE>  
TAO_Link<TRADER,MAP_LOCK_TYPE>::TAO_Link (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components <POA_CosTrading::Link> (trader.trading_components ()),
    TAO_Link_Attributes <POA_CosTrading::Link> (trader.link_attributes ()),
    TAO_Support_Attributes <POA_CosTrading::Link> (trader.support_attributes ())
{
}

template <class TRADER, class MAP_LOCK_TYPE>
TAO_Link<TRADER,MAP_LOCK_TYPE>::~TAO_Link (void)
{
}

template <class TRADER, class MAP_LOCK_TYPE> void
TAO_Link<TRADER, MAP_LOCK_TYPE>::
add_link (const char *name, 
	  CosTrading::Lookup_ptr target, 
	  CosTrading::FollowOption def_pass_on_follow_rule, 
	  CosTrading::FollowOption limiting_follow_rule,
	  CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::DuplicateLinkName, 
		  CosTrading::InvalidLookupRef, 
		  CosTrading::Link::DefaultFollowTooPermissive, 
		  CosTrading::Link::LimitingFollowTooPermissive))
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::Link::IllegalLinkName (name));

  // Ensure this isn't a duplicate link name.
  TAO_String_Hash_Key link_name (name);
  if (this->links_.find (link_name) == -1)
    TAO_THROW (CosTrading::Link::DuplicateLinkName (name));

  // Ensure the lookup_ptr isn't nil.
  if (target == CosTrading::Lookup::_nil())
    TAO_THROW (CosTrading::InvalidLookupRef (target));

  // Ensure that the default link behavior isn't stronger than the
  // limiting link behavior.
  if (def_pass_on_follow_rule > limiting_follow_rule)
    TAO_THROW (CosTrading::Link::DefaultFollowTooPermissive
	       (def_pass_on_follow_rule, limiting_follow_rule));

  // Ensure that the limiting link behavior for this link doesn't
  // exceed the maximum allowed for a link.
  if (limiting_follow_rule < this->max_link_follow_policy (_env))
    TAO_THROW (CosTrading::Link::LimitingFollowTooPermissive
	       (limiting_follow_rule, this->max_link_follow_policy (_env)));

  // Create a link info structure for this link of the federation.
  CosTrading::Link::LinkInfo link_info;
  link_info.target = target;
  link_info.target_reg = target->register_if (_env);
  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;
  TAO_CHECK_ENV_RETURN_VOID (_env);

  // Insert this link into the collection of links.
  this->links_.bind (link_name, link_info);
}

template <class TRADER, class MAP_LOCK_TYPE> void
TAO_Link<TRADER,MAP_LOCK_TYPE>::remove_link (const char *name,
			       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName))
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::Link::IllegalLinkName (name));

  // Ensure this isn't a duplicate link name.
  TAO_String_Hash_Key link_name (name);
  if (this->links_.find (link_name) == -1)
    TAO_THROW (CosTrading::Link::UnknownLinkName (name));

  // Erase the link state from the map.
  this->links_.unbind (link_name);
}

template <class TRADER, class MAP_LOCK_TYPE> CosTrading::Link::LinkInfo * 
TAO_Link<TRADER,MAP_LOCK_TYPE>::describe_link (const char *name,
					       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CosTrading::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName))
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::Link::IllegalLinkName (name), 0);
  
  // Ensure this isn't a duplicate link name.
  Links::ENTRY* link_entry;
  TAO_String_Hash_Key link_name (name);
  if (this->links_.find (link_name, link_entry) == -1)
    TAO_THROW_RETURN (CosTrading::Link::UnknownLinkName (name), 0);

  // return the link infor for this link name.
  return &(link_entry->int_id_);
}

template <class TRADER, class MAP_LOCK_TYPE> CosTrading::LinkNameSeq* 
TAO_Link<TRADER,MAP_LOCK_TYPE>::list_links (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  // Allocate space for the link names.
  CORBA::ULong size = this->links_.current_size (), i = 0;
  CosTrading::LinkName* link_seq =
    CosTrading::LinkNameSeq::allocbuf (size);

  // Copy the link names into the buffer.
  for (Links::iterator links_iter (this->links_);
       ! links_iter.done ();
       links_iter++)
    link_seq[i++] = CORBA::string_dup ((*links_iter).ext_id_.in ());

  // Return a sequence of the buf names.
  return new CosTrading::LinkNameSeq (i, i, link_seq, CORBA::B_TRUE);
}

template <class TRADER, class MAP_LOCK_TYPE> void
TAO_Link<TRADER,MAP_LOCK_TYPE>::
modify_link (const char *name, 
	     CosTrading::FollowOption def_pass_on_follow_rule, 
	     CosTrading::FollowOption limiting_follow_rule,
	     CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName, 
		  CosTrading::Link::DefaultFollowTooPermissive, 
		  CosTrading::Link::LimitingFollowTooPermissive))
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::Link::IllegalLinkName (name));

  // Ensure this isn't a duplicate link name.
  Links::ENTRY* link_entry = 0;
  TAO_String_Hash_Key link_name (name);
  if (this->links_.find (link_name, link_entry) == -1)
    TAO_THROW (CosTrading::Link::UnknownLinkName (name));

  // Ensure that the default link behavior isn't stronger than the
  // limiting link behavior.
  if (def_pass_on_follow_rule > limiting_follow_rule)
    TAO_THROW (CosTrading::Link::DefaultFollowTooPermissive
	       (def_pass_on_follow_rule, limiting_follow_rule));

  // Ensure that the limiting link behavior for this link doesn't
  // exceed the maximum allowed for a link.
  if (limiting_follow_rule < this->max_link_follow_policy (_env))
    TAO_THROW (CosTrading::Link::LimitingFollowTooPermissive
	       (limiting_follow_rule, this->max_link_follow_policy (_env)));

  // Adjust the link settings
  CosTrading::Link::LinkInfo& link_info = link_entry->int_id_;
  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;
}

  // *************************************************************
  // TAO_Proxy
  // *************************************************************

template <class TRADER>  
TAO_Proxy<TRADER>::TAO_Proxy (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components <POA_CosTrading::Proxy> (trader.trading_components ()),
    TAO_Support_Attributes <POA_CosTrading::Proxy> (trader.support_attributes ())
{
}

template <class TRADER>
TAO_Proxy<TRADER>::~TAO_Proxy (void)
{
}

template <class TRADER> CosTrading::OfferId 
TAO_Proxy<TRADER>::export_proxy (CosTrading::Lookup_ptr target, 
				 const char *type, 
				 const CosTrading::PropertySeq& properties, 
				 CORBA::Boolean if_match_all, 
				 const char * recipe, 
				 const CosTrading::PolicySeq& policies_to_pass_on,
				 CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalServiceType, 
		  CosTrading::UnknownServiceType, 
		  CosTrading::InvalidLookupRef, 
		  CosTrading::IllegalPropertyName, 
		  CosTrading::PropertyTypeMismatch, 
		  CosTrading::ReadonlyDynamicProperty, 
		  CosTrading::MissingMandatoryProperty, 
		  CosTrading::Proxy::IllegalRecipe, 
		  CosTrading::DuplicatePropertyName, 
		  CosTrading::DuplicatePolicyName))
{
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), 0);
}

template <class TRADER> void
TAO_Proxy<TRADER>::withdraw_proxy (const char *id,
				   CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Proxy::NotProxyOfferId))
{
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));
}

template <class TRADER> CosTrading::Proxy::ProxyInfo *
TAO_Proxy<TRADER>::describe_proxy (const char *id,
				   CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::IllegalOfferId, 
		  CosTrading::UnknownOfferId, 
		  CosTrading::Proxy::NotProxyOfferId))
{
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), 0);
}

template <class TRADER> void
TAO_Proxy<TRADER>::list_proxies (CORBA::ULong how_many,
				 CosTrading::OfferIdSeq*& ids,
				 CosTrading::OfferIdIterator_ptr& id_itr,
				 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::NotImplemented))
{
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));
}

#endif /* TAO_TRADER_INTERFACES_C */
