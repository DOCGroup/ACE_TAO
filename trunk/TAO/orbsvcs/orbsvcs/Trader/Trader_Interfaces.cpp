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
#include "Trader_T.h"
  // *************************************************************
  // TAO_Lookup
  // *************************************************************

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
TAO_Lookup (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader)
  : TAO_Trader_Components<POA_CosTrading::Lookup> (trader.trading_components ()),
    TAO_Support_Attributes<POA_CosTrading::Lookup> (trader.support_attributes ()),
    TAO_Import_Attributes<POA_CosTrading::Lookup> (trader.import_attributes ()),
    IDS_SAVED (100),
    trader_ (trader)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::~TAO_Lookup (void)
{
  ACE_GUARD (TRADER_LOCK_TYPE, trader_mon, this->lock_);
  for (Request_Ids::ITERATOR riter (this->request_ids_);
       ! riter.done ();
       riter.advance ())
    {
      CosTrading::Admin::OctetSeq** old_seq = 0;
      riter.next (old_seq);
      delete *old_seq;
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> void
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
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
  // Instantiate a class to help interpret query policies.
  TAO_Policies policies (this->trader_, in_policies, env);
  TAO_CHECK_ENV_RETURN_VOID (env);

  // If a federated query returns to us, ignore it to prevent
  // redundant results and infinite loops.
  CosTrading::Admin::OctetSeq* request_id = 0;
  if (this->seen_request_id (policies, request_id, env))
    return;
  TAO_CHECK_ENV_RETURN_VOID (env);

  // The presence of a link interface determines whether we should
  // attempt to forward or propagate queries.
  CosTrading::Link_ptr link_if =
    this->trader_.trading_components ().link_if ();

  // If the importer has specified a starting trader, foward the
  // query.
  CosTrading::TraderName* trader_name = policies.starting_trader (env);
  TAO_CHECK_ENV_RETURN_VOID (env);

  if (link_if != CosTrading::Link::_nil () && trader_name != 0)
    {
      CosTrading::PolicySeq policies_to_forward;
      policies.copy_to_forward (policies_to_forward, *trader_name);
      const char* next_hop = (*trader_name)[0];
      this->forward_query (next_hop,
                           type,
                           constraint,
                           preferences,
                           policies_to_forward,
                           desired_props,
                           how_many,
                           returned_offers,
                           returned_offer_iterator,
                           returned_limits_applied,
                           env);
      TAO_CHECK_ENV_RETURN_VOID (env);
      return;
    }

  // Retrieve the type description struct from the Service Type Repos.
  const TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var
    type_struct (rep->fully_describe_type (type, env));
  TAO_CHECK_ENV_RETURN_VOID (env);

  ACE_NEW (returned_offers, CosTrading::OfferSeq);

  // Obtain a reference to the offer database.
  TAO_Offer_Database<MAP_LOCK_TYPE>& offer_database = this->trader_.offer_database ();

  // TAO_Offer_Filter -- ensures that we don't consider offers with
  // modifiable or dynamic properties if the Trader doesn't support
  // them, or the importer has turned them off using policies.
  // TAO_Constraint_Validator -- validates the constraint with the
  // property types in the supplied type.
  // TAO_Constraint_Interpreter -- parses the constraint string, and
  // determines whether an offer meets those constraints.
  // TAO_Preference_Interpreter -- parses the preference string and
  // orders offers according to those constraints.
  TAO_Offer_Filter offer_filter (policies, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  TAO_Constraint_Validator validator (type_struct.in ());
  TAO_Constraint_Interpreter constr_inter (validator, constraint, env);
  TAO_CHECK_ENV_RETURN_VOID (env);
  TAO_Preference_Interpreter pref_inter (validator, preferences, env);
  TAO_CHECK_ENV_RETURN_VOID (env);

  // Try to find the map of offers of desired service type.
  offer_filter.configure_type (type_struct.ptr ());
  this->lookup_one_type (type,
                         offer_database,
                         constr_inter,
                         pref_inter,
                         offer_filter);

  if (! policies.exact_type_match (env))
    {
      // If the importer hasn't demanded an exact match search, we search
      // all the subtypes of the supplied type. NOTE: Only the properties
      // belonging to the provided type are considered on
      // subtypes. Additional properties on the subtype are generally
      // ignored. This is as it should be, consistent with the notions of
      // type inheritence.
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

  // Take note of the limits applied in this query.
  returned_limits_applied = offer_filter.limits_applied ();

  // Fill the return sequence and iterator with the bountiful results.
  CORBA::ULong offers_returned =
    this->fill_receptacles (type,
                            how_many,
                            desired_props,
                            policies,
                            pref_inter,
                            *returned_offers.ptr (),
                            returned_offer_iterator,
                            env);
  TAO_CHECK_ENV_RETURN_VOID (env);

  // The following steps are only appropriate for a linked trader.
  if (link_if != CosTrading::Link::_nil ())
    {
      // Determine if we should perform a federated query, and if so
      // construct a sequence of links to follow.
      CosTrading::LinkNameSeq_var links;
      CORBA::Boolean should_follow =
        this->retrieve_links (policies,
                              offers_returned,
                              CosTrading::LinkNameSeq_out (links.out ()),
                              env);
      TAO_CHECK_ENV_RETURN_VOID (env);

      if (should_follow && links->length () != 0)
        {
          // Query those links we've accumulated!
          this->federated_query (links.in (),
                                 policies,
                                 *request_id,
                                 pref_inter,
                                 type,
                                 constraint,
                                 preferences,
                                 desired_props,
                                 how_many,
                                 *returned_offers.ptr (),
                                 returned_offer_iterator.ptr (),
                                 *returned_limits_applied.ptr (),
                                 env);
          TAO_CHECK_ENV_RETURN_VOID (env);
        }
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
lookup_one_type (const char* type,
                 TAO_Offer_Database<MAP_LOCK_TYPE>& offer_database,
                 TAO_Constraint_Interpreter& constr_inter,
                 TAO_Preference_Interpreter& pref_inter,
                 TAO_Offer_Filter& offer_filter)
{
  // Retrieve an iterator over the offers for a given type.
  // @@ Would have used Offer_Database::offer_iterator for less
  // coupling between TAO_Lookup and Offer_Database, but g++ barfs on
  // that.
  TAO_Offer_Database<MAP_LOCK_TYPE>::offer_iterator
    offer_iter (type, offer_database);

  while (offer_filter.ok_to_consider_more () &&
         offer_iter.has_more_offers ())
    {
      // For each offer in the iterator, attempt to match it with
      // the constraints passed to the Query method. If it matches
      // the constraint, use the TAO_Preference_Interpreter to
      // order the matched offers with respect to the preference
      // string passed to the method. All the while the offer
      // filter ensures we don't exceed the match cardinality
      // constraints.
      CosTrading::Offer* offer = offer_iter.get_offer ();

      TAO_Constraint_Evaluator evaluator (offer);
      if (offer_filter.ok_to_consider (offer) &&
          constr_inter.evaluate (evaluator))
        {
          // Shove the offer and its id into the preference
          // ordering object, pref_inter.
          CosTrading::OfferId offer_id = offer_iter.get_id ();
          pref_inter.order_offer (evaluator, offer, offer_id);
          offer_filter.matched_offer ();
        }

      offer_iter.next_offer ();
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
lookup_all_subtypes (const char* type,
                     CosTradingRepos::ServiceTypeRepository::IncarnationNumber& inc_num,
                     TAO_Offer_Database<MAP_LOCK_TYPE>& offer_database,
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

          CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq&
            super_types = type_struct->super_types;
          CORBA::ULong num_super_types = super_types.length ();

          for (CORBA::ULong j = 0; j < num_super_types; j++)
            {
              if (ACE_OS::strcmp (type_struct->super_types[j], type) == 0)
                {
                  // Egads, a subtype! This type has the type passed
                  // to query in its list of super_types.
                  offer_filter.configure_type (type_struct.ptr ());
                  this->lookup_one_type (all_types[i],
                                         offer_database,
                                         constr_inter,
                                         pref_inter,
                                         offer_filter);
                  break;
                }
            }
        }
      TAO_CATCHANY
        {
        }
      TAO_ENDTRY;
    }
}


template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
int
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
fill_receptacles (const char* type,
                  CORBA::ULong how_many,
                  const CosTrading::Lookup::SpecifiedProps& desired_props,
                  TAO_Policies& policies,
                  TAO_Preference_Interpreter& pref_inter,
                  CosTrading::OfferSeq& offers,
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

  TAO_Property_Filter prop_filter (desired_props, env);
  TAO_CHECK_ENV_RETURN (env, 0);

  // RETURNING: Calculate how many offers go into the sequence
  //  Calculate how many go into the iterator

  CORBA::ULong return_card = policies.return_card (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong i = 0;
  CORBA::ULong size = pref_inter.num_offers ();
  CORBA::ULong offers_in_sequence = (how_many < size) ? how_many : size;
  CORBA::ULong offers_in_iterator = size - offers_in_sequence;

  // Ensure the total number of offers returned doesn't exceed return_card.
  offers_in_sequence =
    (offers_in_sequence > return_card) ? return_card : offers_in_sequence;

  return_card -= offers_in_sequence;

  offers_in_iterator =
    (offers_in_iterator > return_card) ? return_card : offers_in_iterator;

  CORBA::ULong total_offers = offers_in_sequence + offers_in_iterator;
  offers.length (offers_in_sequence);

  // Add to the sequence, filtering out the undesired properties.
  for (i = 0; i < offers_in_sequence; i++)
    {
      CosTrading::Offer* offer = 0;
      CosTrading::OfferId offer_id = 0;

      // Pull the next ordered offer out of the preference interpreter.
      pref_inter.remove_offer (offer, offer_id);

      // Filter out the undesired properties.
      prop_filter.filter_offer (offer, offers[i]);
      CORBA::string_free (offer_id);
    }

  // Any remaining offers under the return_card go into iterator
  if (offers_in_iterator > 0)
    {
      // Create an iterator implementation
      TAO_Offer_Iterator *oi = this->create_offer_iterator (prop_filter);

      // Register it with the POA.
      offer_itr = oi->_this (env);
      TAO_CHECK_ENV_RETURN (env, total_offers - offers_in_iterator);

      // Add to the iterator
      for (i = 0; i < offers_in_iterator; i++)
        {
          CosTrading::Offer* offer = 0;
          CosTrading::OfferId offer_id = 0;

          // Pull the next ordered offer out of the preference
          // intrerpreter and add it to the offer iterator.
          pref_inter.remove_offer (offer, offer_id);
          oi->add_offer (offer_id, offer);
        }
    }

  // Clear the preference intrerpreter of superfluous items.
  CORBA::ULong num_offers = pref_inter.num_offers ();
  for (i = 0; i < num_offers; i++)
    {
      CosTrading::Offer* offer = 0;
      CosTrading::OfferId offer_id = 0;

      pref_inter.remove_offer (offer, offer_id);
      CORBA::string_free (offer_id);
    }

  return total_offers;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Offer_Iterator *
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
create_offer_iterator (const TAO_Property_Filter& pfilter)
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
    iterator = new TAO_Query_Only_Offer_Iterator (pfilter);
  else
    {
      iterator =
        new TAO_Register_Offer_Iterator<MAP_LOCK_TYPE> (this->trader_.offer_database (),
                                                        pfilter);
    }

  return iterator;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
retrieve_links (TAO_Policies& policies,
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
      CosTrading::Link_ptr link_if =
        this->trader_.trading_components ().link_if ();

      links = link_if->list_links (_env);
      TAO_CHECK_ENV_RETURN (_env, CORBA::B_FALSE);

      // Determine which of the links registered with the Link
      // interface are suitable to follow.
      CORBA::ULong i = 0, j = 0,
        length = links->length ();

      for (i = 0; i < length; i++)
        {
          // Grab the link information.
          CosTrading::Link::LinkInfo_var
            link_info (link_if->describe_link (links[i], _env));
          TAO_CHECK_ENV_RETURN (_env, should_follow);

          // Compute the link follow rule.
          CosTrading::FollowOption link_rule =
            policies.link_follow_rule (link_info.in (), _env);
          TAO_CHECK_ENV_RETURN (_env, should_follow);

          // Determine if the link follow rule applies.
          if (link_rule == CosTrading::always ||
              (link_rule == CosTrading::if_no_local &&
               offers_returned == 0))
            {
              // Add the link to the list of links to follow.
              if (i > j)
                links[j] = links[i];

              j++;
            }
        }

      links->length (j);
    }

  return should_follow;
}


template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
federated_query (const CosTrading::LinkNameSeq& links,
                 const TAO_Policies& policies,
                 const CosTrading::Admin::OctetSeq& request_id,
                 TAO_Preference_Interpreter& pref_inter,
                 const char *type,
                 const char *constr,
                 const char *pref,
                 const CosTrading::Lookup::SpecifiedProps& desired_props,
                 CORBA::ULong how_many,
                 CosTrading::OfferSeq& offers,
                 CosTrading::OfferIterator_ptr& offer_iter,
                 CosTrading::PolicyNameSeq& limits,
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
  // to follow, and we'll query each of them in turn. On each query we
  // adjust the policies for the new trader by reducing the hop_count,
  // changing the link_follow_rule etc..., and merge the results from
  // the new query with the results from the previous queries.

  // We'll need the link and admin interfaces for this part of the
  // federated query. It's ok to get the admin interface without
  // checking if it's nil, becase the conformance criteria dictate
  // that a trader can't implement the link interface without the
  // admin interface.
  CosTrading::Link_ptr link_interface
    = this->trader_.trading_components ().link_if ();

  // Begin collecting all the various offer_iterators into a
  // collection. The end result is a distributed tree of offer
  // iterators, which if traversed in its entirety is probably hugely
  // inefficient, but oh well, I can't think of a better solution.
  TAO_Offer_Iterator_Collection* offer_iter_collection;
  ACE_NEW (offer_iter_collection, TAO_Offer_Iterator_Collection);
  offer_iter_collection->add_offer_iterator (offer_iter);

  CosTrading::PolicySeq policies_to_pass;
  policies.copy_to_pass (policies_to_pass, request_id);

  for (int i = links.length () - 1; i >= 0; i--)
    {
      CosTrading::OfferSeq* out_offers = 0;
      CosTrading::OfferIterator* out_offer_iter = 0;
      CosTrading::PolicyNameSeq* out_limits = 0;

      TAO_TRY
        {
          // Obtain information about the link we're traversing.
          CosTrading::Link::LinkInfo_var link_info =
            link_interface->describe_link (links[i], TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Set the link follow policy for the query over the link.
          policies.copy_in_follow_option (policies_to_pass,
                                          link_info.in (),
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CosTrading::Lookup_var remote_lookup;
#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
          CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
          CORBA::Object_var obj =
            orb->string_to_object (link_info->target, TAO_TRY_ENV);
          TAO_CHECK_ENV;
          remote_lookup = CosTrading::Lookup::_narrow (obj.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;
#else
          remote_lookup = CosTrading::Lookup::_duplicate (link_info->target);
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */

          // Perform the federated query.
          remote_lookup->query (type,
                                constr,
                                pref,
                                policies_to_pass,
                                desired_props,
                                how_many - offers.length (),
                                CosTrading::OfferSeq_out (out_offers),
                                CosTrading::OfferIterator_out (out_offer_iter),
                                CosTrading::PolicyNameSeq_out (out_limits),
                                TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CORBA::ULong j = 0;
          CosTrading::OfferSeq_var out_offers_var (out_offers);
          CosTrading::PolicyNameSeq_var out_limits_var (out_limits);

          // Add another iterator to the collection.
          if (out_offer_iter != CosTrading::OfferIterator::_nil ())
            offer_iter_collection->add_offer_iterator (out_offer_iter);

          // Concatenate the limits applied.
          CORBA::ULong source_length = out_limits->length (),
            target_length = limits.length (),
            total_length = source_length + target_length;

          limits.length (total_length);
          for (j = 0; j < source_length; j++)
            limits[j + target_length] = out_limits_var[j];

          // Concatenate the sequence offers.
          source_length = out_offers->length ();
          target_length = offers.length ();
          total_length = source_length + target_length;

          offers.length (total_length);
          for (j = 0; j < source_length; j++)
            offers[j + target_length] = out_offers_var[j];
        }
      TAO_CATCHANY
        {
          // Ah, well, this query failed, move on to the next one.
        }
      TAO_ENDTRY;
    }

  // Sort the sequence in preference order.
  this->order_merged_sequence (pref_inter, offers);

  // Return the collection of offer iterators.
  offer_iter = offer_iter_collection->_this (_env);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
order_merged_sequence (TAO_Preference_Interpreter& pref_inter,
                       CosTrading::OfferSeq& offers)
{
  CORBA::ULong j = 0;
  CORBA::ULong length = offers.length ();

  // Grab ownership of the offers already in the target sequence.
  CosTrading::Offer* target_buf = offers.get_buffer (CORBA::B_TRUE);

  // Order the sequence.
  for (j = 0; j < length; j++)
    pref_inter.order_offer (&target_buf[j]);

  // Reallocate the sequence.
  offers.length (length);

  // Copy in the ordered offers.
  for (j = 0; j < length; j++)
    {
      CosTrading::Offer* offer = 0;
      pref_inter.remove_offer (offer);
      offers[j] = *offer;
    }

  // Release the orphaned memory.
  CosTrading::OfferSeq::freebuf (target_buf);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
forward_query (const char* next_hop,
               const char *type,
               const char *constr,
               const char *pref,
               const CosTrading::PolicySeq& policy_seq,
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
        link_interface->describe_link (next_hop, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosTrading::Lookup_var remote_lookup;
#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
      CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
      CORBA::Object_var obj = orb->string_to_object (link_info->target, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      remote_lookup = CosTrading::Lookup::_narrow (obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
#else
      remote_lookup = CosTrading::Lookup::_duplicate (link_info->target);
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */

      CORBA::Object_var us = this->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Boolean self_loop =
        remote_lookup->_is_equivalent (us.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (! self_loop)
        {
          // Perform forwarding query.
          remote_lookup->query (type,
                                constr,
                                pref,
                                policy_seq,
                                desired_props,
                                how_many,
                                offers,
                                offer_itr,
                                limits_applied,
                                _env);
          TAO_CHECK_ENV_RETURN_VOID (_env);
        }
      else
        {
          this->query (type,
                       constr,
                       pref,
                       policy_seq,
                       desired_props,
                       how_many,
                       offers,
                       offer_itr,
                       limits_applied,
                       _env);
          TAO_CHECK_ENV_RETURN_VOID (_env);
        }
    }
  TAO_CATCHANY
    {
      CosTrading::Policy policy;
      policy.name = TAO_Policies::POLICY_NAMES[TAO_Policies::STARTING_TRADER];
      policy.value <<= next_hop;
      TAO_THROW (CosTrading::Lookup::InvalidPolicyValue (policy));
    }
  TAO_ENDTRY;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
seen_request_id (TAO_Policies& policies,
                 CosTrading::Admin::OctetSeq*& seq,
                 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::Lookup::PolicyTypeMismatch))
{
  CORBA::Boolean return_value = CORBA::B_FALSE;

  seq = policies.request_id (_env);
  TAO_CHECK_ENV_RETURN (_env, CORBA::B_TRUE);

  if (seq == 0)
    {
      CosTrading::Admin_ptr admin_if =
        this->trader_.trading_components ().admin_if ();
      seq = admin_if->request_id_stem (_env);
      TAO_CHECK_ENV_RETURN (_env, CORBA::B_TRUE);
    }
  else
    {
      // Allocate memory so memory mangement is the same for both
      // cases.
      ACE_NEW_RETURN (seq,
                      CosTrading::Admin::OctetSeq (*seq),
                      CORBA::B_FALSE);
    }

  if (seq == 0)
    TAO_THROW_RETURN (CORBA::NO_MEMORY (CORBA::COMPLETED_NO), CORBA::B_TRUE);

  ACE_GUARD_RETURN (TRADER_LOCK_TYPE, trader_mon, this->lock_, CORBA::B_TRUE);
  for (Request_Ids::ITERATOR riter (this->request_ids_);
       ! riter.done ();
       riter.advance ())
    {
      CosTrading::Admin::OctetSeq** old_seq = 0;
      riter.next (old_seq);

      if (**old_seq == *seq)
        {
          return_value = CORBA::B_TRUE;
          break;
        }
    }

  if (return_value == CORBA::B_FALSE)
    {
      if (this->request_ids_.size () == IDS_SAVED)
        {
          CosTrading::Admin::OctetSeq* octet_seq = 0;
          this->request_ids_.dequeue_head (octet_seq);
          delete octet_seq;
        }

      this->request_ids_.enqueue_tail (seq);
    }

  return return_value;
}

  // *************************************************************
  // TAO_Register
  // *************************************************************

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::TAO_Register (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader)
  : TAO_Trader_Components<POA_CosTrading::Register> (trader.trading_components ()),
    TAO_Support_Attributes<POA_CosTrading::Register> (trader.support_attributes ()),
    trader_ (trader)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::~TAO_Register (void)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::OfferId
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::export (CORBA::Object_ptr reference,
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

  CosTrading::Offer* offer = 0;
  TAO_Support_Attributes_Impl& support_attrs =
    this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();

  // Yank our friend, the type struct, and confirm that the given
  // properties match the type definition.
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
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
  this->validate_properties (type, type_struct.ptr (), properties, _env);
  TAO_CHECK_ENV_RETURN (_env, 0);

  CORBA::ULong plength = properties.length ();
  ACE_NEW_RETURN (offer, CosTrading::Offer, 0);

  // No copying, no memory leaks.
  CosTrading::PropertySeq* hack_seq =
    ACE_const_cast (CosTrading::PropertySeq*, &properties);
  CosTrading::Property* pbuf = hack_seq->get_buffer (CORBA::B_TRUE);
  offer->properties.replace (plength, plength, pbuf, CORBA::B_TRUE);
  offer->reference = reference->_duplicate (reference);

  // Insert the offer into the underlying type map.
  CosTrading::OfferId id = offer_database.insert_offer (type, offer);

  return id;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::withdraw (const char *id,
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

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Register::OfferInfo *
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
describe (const char *id,
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

  CosTrading::Register::OfferInfo *offer_info = 0;
  ACE_NEW_RETURN (offer_info, CosTrading::Register::OfferInfo, 0);

  offer_info->reference = offer->reference->_duplicate (offer->reference.in ());
  offer_info->type = CORBA::string_dup (type);

  // Let the offer_info prop_seq "borrow" the sequence of properties.
  CORBA::ULong length = offer->properties.length ();
  CosTrading::Property* prop_buf = offer->properties.get_buffer ();
  offer_info->properties.replace (length, length, prop_buf, CORBA::B_FALSE);

  return offer_info;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
modify (const char *id,
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
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();
  Offer_Database &offer_database = this->trader_.offer_database ();

  CosTrading::Offer* offer = offer_database.
    lookup_offer (ACE_const_cast (CosTrading::OfferId, id), type, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  if (offer != 0)
    {
      // Yank our friend, the type struct.
      CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct
        = rep->fully_describe_type (type, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
      TAO_Offer_Modifier offer_mod (type, type_struct.in (), offer);

      CosTrading::PropertySeq* prop_seq =
        ACE_const_cast (CosTrading::PropertySeq*, &modify_list);

      // Delete, add, and change properties of the offer.
      offer_mod.delete_properties (del_list, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);

      offer_mod.merge_properties (*prop_seq, _env);
      TAO_CHECK_ENV_RETURN_VOID (_env);

      // Alter our reference to the offer. We do this last, since the
      // spec says: modify either suceeds completely or fails
      // completely.
      offer_mod.affect_change ();
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
withdraw_using_constraint (const char *type,
                           const char *constr,
                           CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTrading::UnknownServiceType,
                   CosTrading::IllegalConstraint,
                   CosTrading::Register::NoMatchingOffers))
{
  TAO_Support_Attributes_Impl&
    support_attrs = this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();
  Offer_Database &offer_database =  this->trader_.offer_database ();
  CORBA::Boolean dp_support = support_attrs.supports_dynamic_properties ();
  TAO_String_Queue ids;

  // Retrieve the type struct
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var
    type_struct = rep->fully_describe_type (type, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  // Try to find the map of offers of desired service type.
  // @@ Again, should be Offer_Database::offer_iterator
  TAO_Offer_Database<MAP_LOCK_TYPE>::offer_iterator
    offer_iter (type, offer_database);
  TAO_Constraint_Validator validator (type_struct.in ());
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

  if (ids.size () != 0)
    {
      while (! ids.is_empty ())
        {
          char* offer_id = 0;

          ids.dequeue_head (offer_id);
          offer_database.remove_offer (offer_id, _env);
          CORBA::string_free (offer_id);
        }
    }
  else
    TAO_THROW (CosTrading::Register::NoMatchingOffers (constr));
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Register_ptr
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
resolve (const CosTrading::TraderName &name,
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
  CosTrading::Link_ptr link_if =
    this->trader_.trading_components ().link_if ();

  // Ensure that the link interface is supported.
  if (link_if == CosTrading::Link::_nil ())
    return CosTrading::Register::_nil ();

  CosTrading::Link::LinkInfo_var link_info;
  CosTrading::Register_var remote_reg;

  TAO_TRY
    {
      // Ensure that the link to the next trader exists.
      link_info = link_if->describe_link (name[0], TAO_TRY_ENV);
      TAO_CHECK_ENV;

#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
      CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
      CORBA::Object_var obj = orb->string_to_object (link_info->target_reg, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      remote_reg = CosTrading::Register::_narrow (obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
#else
      remote_reg = CosTrading::Register::_narrow (link_info->target_reg);
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */

    }
  TAO_CATCHANY
    {
      TAO_THROW_RETURN (CosTrading::Register::UnknownTraderName (name),
                        CosTrading::Register::_nil ());
    }
  TAO_ENDTRY;

  // Ensure that the register pointer isn't nil.
  if (remote_reg.ptr () == CosTrading::Register::_nil ())
    TAO_THROW_RETURN (CosTrading::Register::RegisterNotSupported (name),
                      CosTrading::Register::_nil ());

  CosTrading::Register_ptr return_value = remote_reg.in ();

  if (name.length () > 1)
    {
      // Create a new Trader Name with the first link removed.
      CosTrading::TraderName trader_name (name.length () - 1);
      for (int i = trader_name.length () - 1; i >= 0; i--)
        trader_name[i] = name[i + 1];

      return_value = remote_reg->resolve (trader_name, _env);
      TAO_CHECK_ENV_RETURN (_env, CosTrading::Register::_nil ());
    }

  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
validate_properties (const char* type,
                     const CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
                     const CosTrading::PropertySeq& properties,
                     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
                  CosTrading::PropertyTypeMismatch,
                  CosTrading::ReadonlyDynamicProperty,
                  CosTrading::MissingMandatoryProperty,
                  CosTrading::DuplicatePropertyName))
{
  CORBA::ULong length = properties.length ();
  const CosTradingRepos::ServiceTypeRepository::PropStructSeq&
    prop_types = type_struct->props;
  TAO_Property_Evaluator_By_Name prop_eval (properties, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  // Perform property validation
  length = prop_types.length ();
  for (CORBA::ULong i = 0; i < length; i++)
    {
      const CosTradingRepos::ServiceTypeRepository::PropStruct&
        prop_struct = prop_types[i];
      const char* prop_name  = prop_struct.name;

      // Obtain the type of the exported property.
      CORBA::TypeCode* prop_type = prop_eval.property_type (prop_name);

      if (prop_type == CORBA::TypeCode::_nil ())
        {
          // Offer cannot have a missing mandatory property.
          if (prop_types[i].mode ==
              CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY)
            TAO_THROW (CosTrading::MissingMandatoryProperty (type, prop_name));
        }
      else
        {
          if (! prop_type->equal (prop_struct.value_type.in (), _env))
            {
              // Offer cannot redefine the type of an property.
              const CosTrading::Property* prop = prop_eval.get_property (prop_name);
              TAO_THROW (CosTrading::PropertyTypeMismatch (type, *prop));
            }
          else if (prop_struct.mode ==
                   CosTradingRepos::ServiceTypeRepository::PROP_READONLY &&
                   prop_eval.is_dynamic_property (prop_name))
            TAO_THROW (CosTrading::ReadonlyDynamicProperty (type, prop_name));
        }
    }
}

  // *************************************************************
  // TAO_Admin
  // *************************************************************

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
TAO_Admin (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader)
  : TAO_Trader_Components <POA_CosTrading::Admin> (trader.trading_components ()),
    TAO_Support_Attributes <POA_CosTrading::Admin> (trader.support_attributes ()),
    TAO_Import_Attributes <POA_CosTrading::Admin> (trader.import_attributes ()),
    TAO_Link_Attributes <POA_CosTrading::Admin> (trader.link_attributes ()),
    trader_ (trader),
    sequence_number_ (0)
{
  // A random 4-bytes will prefix the sequence number space for each
  // trader, making it extremely unlikely that the sequence spaces for
  // two traders will over lap.
  size_t time_value = ACE_OS::time ();
# if defined (ACE_HAS_BROKEN_RANDR)
  ACE_RANDR_TYPE seed = (ACE_RANDR_TYPE) time_value;
# else
  ACE_RANDR_TYPE seed = (ACE_RANDR_TYPE) &time_value;
# endif /* ACE_HAS_BROKEN_RANDR */

  this->stem_id_.length (8);
  this->stem_id_[0] = ACE_static_cast (CORBA::Octet, ACE_OS::rand_r (seed) %  256);
  this->stem_id_[1] = ACE_static_cast (CORBA::Octet, ACE_OS::rand_r (seed) %  256);
  this->stem_id_[2] = ACE_static_cast (CORBA::Octet, ACE_OS::rand_r (seed) %  256);
  this->stem_id_[3] = ACE_static_cast (CORBA::Octet, ACE_OS::rand_r (seed) %  256);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::~TAO_Admin (void)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Admin::OctetSeq *
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::request_id_stem (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TRADER_LOCK_TYPE, trader_mon, this->lock_, 0);

  // Add one to the sequence_number and concatenate it to the unique
  // prefix. The sequence number is four octets long, the unique
  // prefix, also 4 bytes long.

  this->stem_id_[4] = this->sequence_number_ & 0xff;
  this->stem_id_[5] = (this->sequence_number_ >> 8) & 0xff;
  this->stem_id_[6] = (this->sequence_number_ >> 16) & 0xff;
  this->stem_id_[7] = (this->sequence_number_ >> 24) & 0xff;

  // Increment the sequence number and return a copy of the stem_id.
  this->sequence_number_++;
  return new CosTrading::Admin::OctetSeq (this->stem_id_);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_search_card (CORBA::ULong value,
                     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_search_card ();

  this->trader_.import_attributes ().def_search_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_search_card (CORBA::ULong value,
                     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_search_card ();

  this->trader_.import_attributes ().max_search_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_match_card (CORBA::ULong value,
                    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_match_card ();

  this->trader_.import_attributes ().def_match_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_match_card (CORBA::ULong value,
                    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_match_card ();

  this->trader_.import_attributes ().max_match_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_return_card (CORBA::ULong value,
                     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_return_card ();

  this->trader_.import_attributes ().def_return_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_return_card (CORBA::ULong value,
                     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_return_card ();

  this->trader_.import_attributes ().max_return_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_list (CORBA::ULong value,
              CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_list ();

  this->trader_.import_attributes ().max_list (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_supports_modifiable_properties (CORBA::Boolean value,
                                    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_modifiable_properties ();

  this->trader_.support_attributes ().supports_modifiable_properties (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_supports_dynamic_properties (CORBA::Boolean value,
                                 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_dynamic_properties ();

  this->trader_.support_attributes ().supports_dynamic_properties (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_supports_proxy_offers (CORBA::Boolean value,
                           CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_proxy_offers ();

  this->trader_.support_attributes ().supports_proxy_offers (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_hop_count (CORBA::ULong value,
                   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_hop_count ();

  this->trader_.import_attributes ().def_hop_count (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_hop_count (CORBA::ULong value,
                   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_hop_count ();

  this->trader_.import_attributes ().max_hop_count (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::FollowOption
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_follow_policy (CosTrading::FollowOption policy,
                       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::FollowOption return_value =
    this->trader_.import_attributes ().def_follow_policy ();

  this->trader_.import_attributes ().def_follow_policy (policy);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::FollowOption
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_follow_policy (CosTrading::FollowOption policy,
                       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::FollowOption return_value =
    this->trader_.import_attributes ().max_follow_policy ();

  this->trader_.import_attributes ().max_follow_policy (policy);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::FollowOption
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_link_follow_policy (CosTrading::FollowOption policy,
                            CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::FollowOption return_value =
    this->trader_.link_attributes ().max_link_follow_policy ();

  this->trader_.link_attributes ().max_link_follow_policy (policy);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::TypeRepository_ptr
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_type_repos (CosTrading::TypeRepository_ptr repository,
                CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CosTrading::TypeRepository_ptr return_value =
    this->trader_.support_attributes ().type_repos ();

  this->trader_.support_attributes ().type_repos (repository);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Admin::OctetSeq*
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_request_id_stem (const CosTrading::Admin::OctetSeq& stem,
                       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TRADER_LOCK_TYPE, trader_mon, this->lock_, &this->stem_id_);
  this->stem_id_ = stem;
  return &this->stem_id_;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
list_offers (CORBA::ULong how_many,
             CosTrading::OfferIdSeq_out ids,
             CosTrading::OfferIdIterator_out id_itr,
             CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, CosTrading::NotImplemented))
{
  // This method only applies when the register interface is implemented
  if (CORBA::is_nil(this->trader_.trading_components().register_if()))
    TAO_THROW (CosTrading::NotImplemented());

  TAO_Offer_Database<MAP_LOCK_TYPE>& type_map = this->trader_.offer_database ();
  TAO_Offer_Id_Iterator* offer_id_iter = type_map.retrieve_all_offer_ids ();

  id_itr = CosTrading::OfferIdIterator::_nil ();
  if (how_many > 0)
    {
      if (offer_id_iter->next_n (how_many, ids, _env) == CORBA::B_TRUE)
        {
          id_itr = offer_id_iter->_this (_env);
          TAO_CHECK_ENV_RETURN_VOID (_env);
        }
      else
        delete offer_id_iter;
    }
  else
    ids = new CosTrading::OfferIdSeq (0);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
list_proxies (CORBA::ULong how_many,
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

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::TAO_Link (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader)
  : TAO_Trader_Components <POA_CosTrading::Link> (trader.trading_components ()),
    TAO_Support_Attributes <POA_CosTrading::Link> (trader.support_attributes ()),
    TAO_Link_Attributes <POA_CosTrading::Link> (trader.link_attributes ()),
    trader_ (trader)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::~TAO_Link (void)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Link<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::
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
  if (this->links_.find (link_name) == 0)
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

#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
  CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
  link_info.target = orb->object_to_string (target, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
#else
  link_info.target = CosTrading::Lookup::_duplicate (target);
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */

  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;
  TAO_CHECK_ENV_RETURN_VOID (_env);

  // Insert this link into the collection of links.
  this->links_.bind (link_name, link_info);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
remove_link (const char *name,
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

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Link::LinkInfo *
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::describe_link (const char *name,
                                               CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::Link::IllegalLinkName,
                   CosTrading::Link::UnknownLinkName))
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::Link::IllegalLinkName (name), 0);

  // Ensure this isn't a duplicate link name.
  Links::ENTRY* link_entry = 0;
  TAO_String_Hash_Key link_name (name);
  if (this->links_.find (link_name, link_entry) == -1)
    TAO_THROW_RETURN (CosTrading::Link::UnknownLinkName (name), 0);

  // Build a new Link Info structure.
  CosTrading::Link::LinkInfo* new_link_info = 0;
  CosTrading::Link::LinkInfo& old_link_info = link_entry->int_id_;

  ACE_NEW_RETURN (new_link_info, CosTrading::Link::LinkInfo, 0);

  new_link_info->def_pass_on_follow_rule = old_link_info.def_pass_on_follow_rule;
  new_link_info->limiting_follow_rule = old_link_info.limiting_follow_rule;

#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
  CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
  CORBA::Object_var obj =
    orb->string_to_object (old_link_info.target, _env);
  TAO_CHECK_ENV_RETURN (_env, new_link_info);
  CosTrading::Lookup_var remote_lookup =
    CosTrading::Lookup::_narrow (obj.in (), _env);
  TAO_CHECK_ENV_RETURN (_env, new_link_info);
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */

  new_link_info->target = old_link_info.target;

  // Delayed retrieval of register interface.
  // This avoids the nested upcall that would occur were we to invoke
  // this method in the add_link method.

#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
  CosTrading::Register_var remote_register = remote_lookup->register_if (_env);
  TAO_CHECK_ENV_RETURN (_env, new_link_info);
  new_link_info->target_reg =
    orb->object_to_string (remote_register.in (), _env);
  TAO_CHECK_ENV_RETURN (_env, new_link_info);
#else
  new_link_info->target_reg = old_link_info.target->register_if (_env);
  TAO_CHECK_ENV_RETURN (_env, new_link_info);
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */

  // return the link information for this link name.
  return new_link_info;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::LinkNameSeq*
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::list_links (CORBA::Environment& _env)
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

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
modify_link (const char *name,
             CosTrading::FollowOption def_pass_on_follow_rule,
             CosTrading::FollowOption limiting_follow_rule,
             CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Link::IllegalLinkName,
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

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
TAO_Proxy (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader)
  : TAO_Trader_Components <POA_CosTrading::Proxy> (trader.trading_components ()),
    TAO_Support_Attributes <POA_CosTrading::Proxy> (trader.support_attributes ()),
    trader_ (trader)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::~TAO_Proxy (void)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::OfferId
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
export_proxy (CosTrading::Lookup_ptr target,
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

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
withdraw_proxy (const char *id,
                CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
                  CosTrading::IllegalOfferId,
                  CosTrading::UnknownOfferId,
                  CosTrading::Proxy::NotProxyOfferId))
{
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Proxy::ProxyInfo *
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
describe_proxy (const char *id,
                CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
                  CosTrading::IllegalOfferId,
                  CosTrading::UnknownOfferId,
                  CosTrading::Proxy::NotProxyOfferId))
{
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), 0);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
list_proxies (CORBA::ULong how_many,
                CosTrading::OfferIdSeq*& ids,
                CosTrading::OfferIdIterator_ptr& id_itr,
                CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::NotImplemented))
{
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));
}

#endif /* TAO_TRADER_INTERFACES_C */
