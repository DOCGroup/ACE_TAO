#ifndef TAO_TRADER_INTERFACES_CPP
#define TAO_TRADER_INTERFACES_CPP

#include "orbsvcs/Trader/Trader_Interfaces.h"
#include "orbsvcs/Trader/Trader_T.h"
#include "ace/INET_Addr.h"
#include "orbsvcs/Trader/Trader_Constraint_Visitors.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
      const CosTrading::Admin::OctetSeq** old_seq = 0;
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
       CosTrading::PolicyNameSeq_out returned_limits_applied)
{
  // Instantiate a class to help interpret query policies.
  TAO_Policies policies (this->trader_, in_policies);

  // If a federated query returns to us, ignore it to prevent
  // redundant results and infinite loops.
  const CosTrading::Admin::OctetSeq* request_id = 0;
  int check = this->seen_request_id (policies, request_id);

  if (check)
    {
      returned_offers = new CosTrading::OfferSeq;
      returned_limits_applied = new CosTrading::PolicyNameSeq;
      return;
    }

  // The presence of a link interface determines whether we should
  // attempt to forward or propagate queries.
  CosTrading::Link_ptr link_if =
    this->trader_.trading_components ().link_if ();

  // If the importer has specified a starting trader, foward the
  // query.
  const CosTrading::TraderName* trader_name =
    policies.starting_trader ();

  if (! CORBA::is_nil (link_if) && trader_name != 0)
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
                           returned_limits_applied);
      return;
    }

  // Retrieve the type description struct from the Service Type Repos.
  const TAO_Support_Attributes_i& support_attrs =
    this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
    rep->fully_describe_type (type);

  // @@ Should throw a NO_MEMORY exception here...
  ACE_NEW (returned_offers,
           CosTrading::OfferSeq);

  // Obtain a reference to the offer database.
  TAO_Offer_Database<MAP_LOCK_TYPE>& offer_database =
    this->trader_.offer_database ();

  // TAO_Offer_Filter -- ensures that we don't consider offers with
  // modifiable or dynamic properties if the Trader doesn't support
  // them, or the importer has turned them off using policies.
  // TAO_Constraint_Validator -- validates the constraint with the
  // property types in the supplied type.  TAO_Constraint_Interpreter
  // -- parses the constraint string, and determines whether an offer
  // meets those constraints.  TAO_Preference_Interpreter -- parses
  // the preference string and orders offers according to those
  // constraints.
  TAO_Offer_Filter offer_filter (policies);
  TAO_Trader_Constraint_Validator validator (type_struct.in ());
  TAO_Constraint_Interpreter constr_inter (validator,
                                           constraint);
  TAO_Preference_Interpreter pref_inter (validator,
                                         preferences);

  // Try to find the map of offers of desired service type.
  offer_filter.configure_type (type_struct.ptr ());
  this->lookup_one_type (type,
                         offer_database,
                         constr_inter,
                         pref_inter,
                         offer_filter);

  CORBA::Boolean result = policies.exact_type_match ();

  if (!result)
    {
      // If the importer hasn't demanded an exact match search, we
      // search all the subtypes of the supplied type. NOTE: Only the
      // properties belonging to the provided type are considered on
      // subtypes. Additional properties on the subtype are generally
      // ignored. This is as it should be, consistent with the notions
      // of type inheritence.
      this->lookup_all_subtypes (type,
                                 type_struct->incarnation,
                                 offer_database,
                                 rep,
                                 constr_inter,
                                 pref_inter,
                                 offer_filter);
    }

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
                            returned_offer_iterator);

  // The following steps are only appropriate for a linked trader.
  if (! CORBA::is_nil (link_if))
    {
      // Determine if we should perform a federated query, and if so
      // construct a sequence of links to follow.
      CosTrading::LinkNameSeq_var links;
      CORBA::Boolean should_follow =
        this->retrieve_links (policies,
                              offers_returned,
                              CosTrading::LinkNameSeq_out (links.out ()));

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
                                 *returned_limits_applied.ptr ());
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
#if defined(_MSC_VER)
  TAO_Offer_Database<MAP_LOCK_TYPE>::offer_iterator
    offer_iter (type, offer_database);
#else
  // MSVC won't grok this for some reason, but it's necessary for the
  // HP compiler, which seriously requires the typename keyword
  // here. I apologize if this ifdef offends some ACE users'
  // sensibilities --- it certainly offends mine.
  typename TAO_Offer_Database<MAP_LOCK_TYPE>::offer_iterator
    offer_iter (type, offer_database);
#endif

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

      TAO_Trader_Constraint_Evaluator evaluator (offer);
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

  all_types = rep->list_types (sst);

  // Scan all types inserted after the super types. If the transitive
  // closure of a type's super type relation includes the super type
  // being considered, then perform a search on that type.
  CORBA::ULong num_types = all_types->length ();
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct;
  for (CORBA::ULong i = 0;
       i < num_types && offer_filter.ok_to_consider_more ();
       i++)
    {
      // Obtain a description of the prospective type.
      type_struct = rep->fully_describe_type (all_types[i]);

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
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
int
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
fill_receptacles (const char* /* type */,
                  CORBA::ULong how_many,
                  const CosTrading::Lookup::SpecifiedProps& desired_props,
                  TAO_Policies& policies,
                  TAO_Preference_Interpreter& pref_inter,
                  CosTrading::OfferSeq& offers,
                  CosTrading::OfferIterator_ptr& offer_itr)
{
  // BEGIN SPEC
  // The returned offers are passed back in one of two ways (or a
  // combination of both). ?The "offers" return result conveys a list
  // of offers and the "offer_itr" is a reference to an interface at
  // which offers can be obtained. ?The "how_many" parameter states
  // how many offers are to be returned via the "offers" result, any
  // remaining offers are available via the iterator interface. If the
  // "how_many" exceeds the number of offers to be returned, then the
  // "offer_itr" will be nil.
  // END SPEC

  TAO_Property_Filter prop_filter (desired_props);

  // RETURNING: Calculate how many offers go into the sequence
  //  Calculate how many go into the iterator

  CORBA::ULong return_card = policies.return_card ();

  CORBA::ULong i = 0;
  CORBA::ULong size = static_cast<CORBA::ULong> (pref_inter.num_offers ());
  CORBA::ULong offers_in_sequence = (how_many < size) ? how_many : size;
  CORBA::ULong offers_in_iterator = size - offers_in_sequence;

  // Ensure the total number of offers returned doesn't exceed return_card.
  offers_in_sequence = offers_in_sequence > return_card
    ? return_card
    : offers_in_sequence;

  return_card -= offers_in_sequence;

  offers_in_iterator = offers_in_iterator > return_card
    ? return_card
    : offers_in_iterator;

  CORBA::ULong total_offers = offers_in_sequence + offers_in_iterator;
  offers.length (offers_in_sequence);

  // Add to the sequence, filtering out the undesired properties.
  for (i = 0; i < offers_in_sequence; i++)
    {
      CosTrading::Offer* offer = 0;
      CosTrading::OfferId offer_id = 0;

      // Pull the next ordered offer out of the preference
      // interpreter.
      pref_inter.remove_offer (offer, offer_id);

      // Filter out the undesired properties.
      prop_filter.filter_offer (offer, offers[i]);
      CORBA::string_free (offer_id);
    }

  // Any remaining offers under the return_card go into iterator
  if (offers_in_iterator > 0)
    {
      // Create an iterator implementation
      TAO_Offer_Iterator *oi =
        this->create_offer_iterator (prop_filter);

      // Register it with the POA.
      offer_itr = oi->_this ();

      oi->_remove_ref ();

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
  size_t num_offers = pref_inter.num_offers ();
  for (size_t j = 0; j < num_offers; j++)
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
    ACE_NEW_RETURN (iterator,
                    TAO_Query_Only_Offer_Iterator (pfilter),
                    0);
  else
    ACE_NEW_RETURN (iterator,
                    TAO_Register_Offer_Iterator<MAP_LOCK_TYPE> (this->trader_.offer_database (),
                                                                pfilter),
                    0);
  return iterator;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
retrieve_links (TAO_Policies& policies,
                CORBA::ULong offers_returned,
                CosTrading::LinkNameSeq_out links)
{
  CORBA::Boolean should_follow = 0;
  CosTrading::FollowOption follow_rule = policies.link_follow_rule ();

  // Determine whether or not a federated query is warranted. A query
  // is waranted if the follow_rule governing this query is 'always'
  // or if_no_local and the local query returned nothing.
  if ((follow_rule == CosTrading::always ||
       (follow_rule == CosTrading::if_no_local && offers_returned == 0)))
    {
      CORBA::ULong hc = policies.hop_count ();

      if (hc > 0)
        should_follow = 1;
    }

  if (should_follow)
    {
      // Grab the names of all the links in the trader, and push
      // the suitable ones onto <valid_links>.
      CosTrading::Link_ptr link_if =
        this->trader_.trading_components ().link_if ();

      links = link_if->list_links ();

      // Determine which of the links registered with the Link
      // interface are suitable to follow.
      CORBA::ULong i = 0, j = 0,
        length = links->length ();

      for (i = 0; i < length; i++)
        {
          // Grab the link information.
          CosTrading::Link::LinkInfo_var
            link_info (link_if->describe_link (links[i]));

          // Compute the link follow rule.
          CosTrading::FollowOption link_rule =
            policies.link_follow_rule (link_info.in ());

          // Determine if the link follow rule applies.
          if (link_rule == CosTrading::always
              || (link_rule == CosTrading::if_no_local
                  && offers_returned == 0))
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
                 CosTrading::PolicyNameSeq& limits)
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
  TAO_Offer_Iterator_Collection* offer_iter_collection = 0;
  ACE_NEW (offer_iter_collection,
           TAO_Offer_Iterator_Collection);
  offer_iter_collection->add_offer_iterator (offer_iter);

  CosTrading::PolicySeq policies_to_pass;
  policies.copy_to_pass (policies_to_pass, request_id);

  for (int i = links.length () - 1; i >= 0; i--)
    {
      CosTrading::OfferSeq *out_offers = 0;
      CosTrading::OfferIterator *out_offer_iter = 0;
      CosTrading::PolicyNameSeq *out_limits = 0;

      try
        {
          // Obtain information about the link we're traversing.
          CosTrading::Link::LinkInfo_var link_info =
            link_interface->describe_link (links[i]);

          // Set the link follow policy for the query over the link.
          policies.copy_in_follow_option (policies_to_pass,
                                          link_info.in ());

          CosTrading::Lookup_var remote_lookup =
            CosTrading::Lookup::_duplicate (link_info->target.in ());

          // Perform the federated query.
          remote_lookup->query (type,
                                constr,
                                pref,
                                policies_to_pass,
                                desired_props,
                                how_many - offers.length (),
                                CosTrading::OfferSeq_out (out_offers),
                                CosTrading::OfferIterator_out (out_offer_iter),
                                CosTrading::PolicyNameSeq_out (out_limits));

          CORBA::ULong j = 0;
          CosTrading::OfferSeq_var out_offers_var (out_offers);
          CosTrading::PolicyNameSeq_var out_limits_var (out_limits);

          // Add another iterator to the collection.
          if (! CORBA::is_nil (out_offer_iter))
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
      catch (const CORBA::Exception&)
        {
          // Ah, well, this query failed, move on to the next one.
        }
    }

  // Sort the sequence in preference order.
  this->order_merged_sequence (pref_inter, offers);

  // Return the collection of offer iterators.
  offer_iter = offer_iter_collection->_this ();

  offer_iter_collection->_remove_ref ();
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
  CosTrading::Offer* target_buf = offers.get_buffer (1);

  // Order the sequence.
  for (j = 0; j < length; j++)
    pref_inter.order_offer (&target_buf[j]);
  //pref_inter.order_offer (&offers[j]);

  // Reallocate the sequence.
  offers.length (length);

  //  CosTrading::OfferSeq copy;
  //  copy.length (length);

  // Copy in the ordered offers.
  for (j = 0; j < length; j++)
    {
      CosTrading::Offer* offer = 0;
      pref_inter.remove_offer (offer);
      //copy[j] = *offer;
      offers[j] = *offer;
    }

  // Release the orphaned memory.
  CosTrading::OfferSeq::freebuf (target_buf);
  //  offers = copy
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
               CosTrading::PolicyNameSeq_out limits_applied)
{
  // Forward this query to the next link in the starting_trader sequence.
  CosTrading::Link_ptr link_interface
    = this->trader_.trading_components ().link_if ();

  try
    {
      CosTrading::Link::LinkInfo_var link_info =
        link_interface->describe_link (next_hop);

      CosTrading::Lookup_var remote_lookup =
        CosTrading::Lookup::_duplicate (link_info->target.in ());

      CORBA::Object_var us = this->_this ();

      CORBA::Boolean self_loop =
        remote_lookup->_is_equivalent (us.in ());

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
                                limits_applied);
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
                       limits_applied);
        }
    }
  catch (const CORBA::Exception&)
    {
      CosTrading::Policy policy;
      policy.name = TAO_Policies::POLICY_NAMES[TAO_Policies::STARTING_TRADER];
      policy.value <<= next_hop;
      throw CosTrading::Lookup::InvalidPolicyValue (policy);
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
seen_request_id (TAO_Policies& policies,
                 const CosTrading::Admin::OctetSeq*& seq)
{
  CORBA::Boolean return_value = 0;

  seq = policies.request_id ();

  if (seq == 0)
    {
      CosTrading::Admin_ptr admin_if =
        this->trader_.trading_components ().admin_if ();
      seq = admin_if->request_id_stem ();
    }
  else
    {
      // Allocate memory so memory mangement is the same for both
      // cases.
      ACE_NEW_THROW_EX (seq,
                        CosTrading::Admin::OctetSeq (*seq),
                        CORBA::NO_MEMORY ());
    }

  ACE_GUARD_RETURN (TRADER_LOCK_TYPE, trader_mon, this->lock_, 1);

  for (Request_Ids::ITERATOR riter (this->request_ids_);
       ! riter.done ();
       riter.advance ())
    {
      const CosTrading::Admin::OctetSeq** old_seq = 0;
      riter.next (old_seq);

      if (**old_seq == *seq)
        {
          return_value = 1;
          break;
        }
    }

  if (return_value == 0)
    {
      if (this->request_ids_.size () == IDS_SAVED)
        {
          const CosTrading::Admin::OctetSeq* octet_seq = 0;
          this->request_ids_.dequeue_head (octet_seq);
          delete octet_seq;
        }

      this->request_ids_.enqueue_tail (seq);
    }

  return return_value;
}

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
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
_cxx_export (CORBA::Object_ptr reference,
             const char *type,
             const CosTrading::PropertySeq &properties)
{
  // For robustness purposes --
  if (CORBA::is_nil (reference))
    throw CosTrading::Register::InvalidObjectRef ();

  // Get service type map
  TAO_Offer_Database<MAP_LOCK_TYPE> &offer_database = this->trader_.offer_database ();

  CosTrading::Offer* offer = 0;
  TAO_Support_Attributes_i& support_attrs =
    this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();

  // Yank our friend, the type struct, and confirm that the given
  // properties match the type definition.
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
    rep->fully_describe_type (type);

  // Oops the type is masked, we shouldn't let exporters know the type
  // exists.
  if (type_struct->masked)
    throw CosTrading::UnknownServiceType (type);

  // TAO-specific way to determine if an object is derived from or is
  // an interface type.
  int check = (! reference->_is_a (type_struct->if_name));
  if (check)
    throw CosTrading::Register::InterfaceTypeMismatch (type, reference);

  // Validate that the properties defined for this offer are correct
  // to their types and strength.
  this->validate_properties (type, type_struct.ptr (), properties);

  // CORBA::ULong plength = properties.length ();
  ACE_NEW_THROW_EX (offer, CosTrading::Offer, CORBA::NO_MEMORY ());

  // No copying, no memory leaks. Violates the "in" parameter semantics
  // when this object is colocated with the client, however.
  //  CosTrading::PropertySeq* hack_seq =
  //    const_cast<CosTrading::PropertySeq*> (&properties);
  //  CosTrading::Property* pbuf = hack_seq->get_buffer (1);

  //  CosTrading::PropertySeq* hack_seq =
  //    const_cast<CosTrading::PropertySeq*> (&properties);
  //  CosTrading::Property* pbuf = hack_seq->get_buffer (0);
  //  offer->properties.replace (plength, plength, pbuf, 0);
  //  offer->properties._allocate_buffer (plength);
  offer->properties = properties;
  offer->reference = reference->_duplicate (reference);

  // Insert the offer into the underlying type map.
  CosTrading::OfferId id = offer_database.insert_offer (type, offer);

  return id;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
withdraw (const char *id)
{
  // Get service type map.
  TAO_Offer_Database<MAP_LOCK_TYPE> &offer_database = this->trader_.offer_database ();
  offer_database.remove_offer ((CosTrading::OfferId) id);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Register::OfferInfo *
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
describe (const char *id)
{
  // Get service type map.
  char* type = 0;
  TAO_Offer_Database<MAP_LOCK_TYPE> &offer_database = this->trader_.offer_database ();

  // Perform a lookup to find the offer.
  CosTrading::Offer* offer =
    offer_database.lookup_offer ((CosTrading::OfferId) id, type);

  CosTrading::Register::OfferInfo *offer_info = 0;
  ACE_NEW_THROW_EX (offer_info, CosTrading::Register::OfferInfo, CORBA::NO_MEMORY ());

  offer_info->reference = CORBA::Object::_duplicate (offer->reference.in ());
  offer_info->type = CORBA::string_dup (type);

  // Let the offer_info prop_seq "borrow" the sequence of properties.
  //CORBA::ULong length = offer->properties.length ();
  //  CosTrading::Property* prop_buf = offer->properties.get_buffer ();
  offer_info->properties = offer->properties;

  return offer_info;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
modify (const char *id,
        const CosTrading::PropertyNameSeq& del_list,
        const CosTrading::PropertySeq& modify_list)
{
  // Throw an exception if the trader is not configured
  // to support properties modification.
  int check =  (! this->supports_modifiable_properties ());

  if (check)
    throw CosTrading::NotImplemented ();

  char* type = 0;
  TAO_Support_Attributes_i& support_attrs =
    this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();
  TAO_Offer_Database<MAP_LOCK_TYPE> &offer_database = this->trader_.offer_database ();

  CosTrading::Offer* offer = offer_database.
    lookup_offer (const_cast<CosTrading::OfferId> (id), type);

  if (offer != 0)
    {
      // Yank our friend, the type struct.
      CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
        rep->fully_describe_type (type);
      TAO_Offer_Modifier offer_mod (type, type_struct.in (), offer);

      // Delete, add, and change properties of the offer.
      offer_mod.delete_properties (del_list);

      offer_mod.merge_properties (modify_list);

      // Alter our reference to the offer. We do this last, since the
      // spec says: modify either suceeds completely or fails
      // completely.
      offer_mod.affect_change (modify_list);
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
withdraw_using_constraint (const char *type,
                           const char *constr)
{
  TAO_Support_Attributes_i&
    support_attrs = this->trader_.support_attributes ();
  CosTradingRepos::ServiceTypeRepository_ptr rep =
    support_attrs.service_type_repos ();
  TAO_Offer_Database<MAP_LOCK_TYPE> &offer_database =  this->trader_.offer_database ();
  CORBA::Boolean dp_support = support_attrs.supports_dynamic_properties ();
  TAO_String_Queue ids;

  // Retrieve the type struct
  CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
    rep->fully_describe_type (type);

  // Try to find the map of offers of desired service type.
  // @@ Again, should be Offer_Database::offer_iterator
  {
#if defined (_MSC_VER)
    TAO_Offer_Database<MAP_LOCK_TYPE>::offer_iterator
      offer_iter (type, offer_database);
#else
    // MSVC won't grok this for some reason, but it's necessary for
    // the HP compiler, which seriously requires the typename keyword
    // here. I apologize if this ifdef offends some ACE users'
    // sensibilities --- it certainly offends mine.
    typename TAO_Offer_Database<MAP_LOCK_TYPE>::offer_iterator
      offer_iter (type, offer_database);
#endif /* _MSC_VER */

    TAO_Trader_Constraint_Validator validator (type_struct.in ());
    TAO_Constraint_Interpreter constr_inter (validator, constr);

    while (offer_iter.has_more_offers ())
      {
        CosTrading::Offer* offer = offer_iter.get_offer ();
        // Add offer if it matches the constraints

        TAO_Trader_Constraint_Evaluator evaluator (offer, dp_support);
        if (constr_inter.evaluate (evaluator))
          ids.enqueue_tail (offer_iter.get_id ());

        offer_iter.next_offer ();
      }
  }

  if (ids.size () != 0)
    {
      while (! ids.is_empty ())
        {
          char* offer_id = 0;

          ids.dequeue_head (offer_id);
          offer_database.remove_offer (offer_id);
          CORBA::string_free (offer_id);
        }
    }
  else
    throw CosTrading::Register::NoMatchingOffers (constr);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Register_ptr
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
resolve (const CosTrading::TraderName &name)
{
  // Determine if the first link is a legal link name.
  if (! TAO_Trader_Base::is_valid_link_name (name[0]))
    throw CosTrading::Register::IllegalTraderName (name);

  // Grab a reference to the link interface, and get a link description.
  CosTrading::Link_ptr link_if =
    this->trader_.trading_components ().link_if ();

  // Ensure that the link interface is supported.
  if (! CORBA::is_nil (link_if))
    return CosTrading::Register::_nil ();

  CosTrading::Link::LinkInfo_var link_info;
  CosTrading::Register_var remote_reg;

  try
    {
      // Ensure that the link to the next trader exists.
      link_info = link_if->describe_link (name[0]);

      remote_reg =
        CosTrading::Register::_narrow (link_info->target_reg.in ());
    }
  catch (const CORBA::Exception&)
    {
      throw CosTrading::Register::UnknownTraderName (name);
    }

  // Ensure that the register pointer isn't nil.
  if (! CORBA::is_nil (remote_reg.in ()))
    throw CosTrading::Register::RegisterNotSupported (name);

  CosTrading::Register_ptr return_value = remote_reg.in ();

  if (name.length () > 1)
    {
      // Create a new Trader Name with the first link removed.
      CosTrading::TraderName trader_name (name.length () - 1);
      for (int i = trader_name.length () - 1; i >= 0; i--)
        trader_name[i] = name[i + 1];

      return_value = remote_reg->resolve (trader_name);
    }

  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
validate_properties (const char* type,
                     const CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
                     const CosTrading::PropertySeq& properties)
{
  CORBA::ULong length = properties.length ();
  const CosTradingRepos::ServiceTypeRepository::PropStructSeq&
    prop_types = type_struct->props;
  TAO_Property_Evaluator_By_Name prop_eval (properties);

  // Perform property validation
  length = prop_types.length ();

  for (CORBA::ULong i = 0; i < length; i++)
    {
      const CosTradingRepos::ServiceTypeRepository::PropStruct&
        prop_struct = prop_types[i];
      const char* prop_name  = prop_struct.name;

      // Obtain the type of the exported property.
      CORBA::TypeCode_var prop_type = prop_eval.property_type (prop_name);

      if (CORBA::is_nil (prop_type.in ()))
        {
          // Offer cannot have a missing mandatory property.
          if (prop_types[i].mode ==
              CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY)
            throw CosTrading::MissingMandatoryProperty (type, prop_name);
        }
      else
        {
          int check =
            (! prop_type->equal (prop_struct.value_type.in ()));
          if (check)
            {
              // Offer cannot redefine the type of an property.
              const CosTrading::Property* prop =
                prop_eval.get_property (prop_name);
              throw CosTrading::PropertyTypeMismatch (type, *prop);
            }
          else if (prop_struct.mode ==
                   CosTradingRepos::ServiceTypeRepository::PROP_READONLY &&
                   prop_eval.is_dynamic_property (prop_name))
            throw CosTrading::ReadonlyDynamicProperty (type, prop_name);
        }
    }
}

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
  // two traders will over lap.  @@ TODO: This is a bad way to
  // generate pseudo random numbers.

  // Ok, then, Carlos, we'll do it a different way: ip addr + pid.
  ACE_UINT32 ip_addr = 0;
  ACE_TCHAR host_name[BUFSIZ];

  if (ACE_OS::hostname (host_name,
                        BUFSIZ) != -1)
    {
      ACE_INET_Addr addr ((u_short) 0, host_name);
      ip_addr = addr.get_ip_address ();
    }
  // The better way to do unique stem identifiers.
  this->stem_id_.length (12);

  if (ip_addr != 0)
    {
      pid_t pid = ACE_OS::getpid ();
      this->stem_id_[0] = static_cast<CORBA::Octet> ((ip_addr >> 24) & 0xff);
      this->stem_id_[1] = static_cast<CORBA::Octet> ((ip_addr >> 16) & 0xff);
      this->stem_id_[2] = static_cast<CORBA::Octet> ((ip_addr >> 8) & 0xff);
      this->stem_id_[3] = static_cast<CORBA::Octet> (ip_addr & 0xff);
      this->stem_id_[4] = static_cast<CORBA::Octet> ((pid >> 24) & 0xff);
      this->stem_id_[5] = static_cast<CORBA::Octet> ((pid >> 16) & 0xff);
      this->stem_id_[6] = static_cast<CORBA::Octet> ((pid >> 8) & 0xff);
      this->stem_id_[7] = static_cast<CORBA::Octet> (pid & 0xff);
    }

  // The default way -- eight random integers.
  else
    {
      time_t time_value = ACE_OS::time ();
      ACE_OS::srand (static_cast<u_int> (time_value));

      this->stem_id_[0] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[1] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[2] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[3] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[4] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[5] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[6] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
      this->stem_id_[7] = static_cast<CORBA::Octet> (ACE_OS::rand () %  256);
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::~TAO_Admin (void)
{
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Admin::OctetSeq *
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::request_id_stem (void)
{
  ACE_GUARD_RETURN (TRADER_LOCK_TYPE, trader_mon, this->lock_, 0);

  // Add one to the sequence_number and concatenate it to the unique
  // prefix. The sequence number is four octets long, the unique
  // prefix, also 4 bytes long.

  this->stem_id_[8] = static_cast<CORBA::Octet> (this->sequence_number_ & 0xff);
  this->stem_id_[9] = static_cast<CORBA::Octet> ((this->sequence_number_ >> 8) & 0xff);
  this->stem_id_[10] = static_cast<CORBA::Octet> ((this->sequence_number_ >> 16) & 0xff);
  this->stem_id_[11] = static_cast<CORBA::Octet> ((this->sequence_number_ >> 24) & 0xff);

  // Increment the sequence number and return a copy of the stem_id.
  this->sequence_number_++;
  return new CosTrading::Admin::OctetSeq (this->stem_id_);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_search_card (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_search_card ();

  this->trader_.import_attributes ().def_search_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_search_card (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_search_card ();

  this->trader_.import_attributes ().max_search_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_match_card (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_match_card ();

  this->trader_.import_attributes ().def_match_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_match_card (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_match_card ();

  this->trader_.import_attributes ().max_match_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_return_card (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_return_card ();

  this->trader_.import_attributes ().def_return_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_return_card (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_return_card ();

  this->trader_.import_attributes ().max_return_card (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_list (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_list ();

  this->trader_.import_attributes ().max_list (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_supports_modifiable_properties (CORBA::Boolean value
 )
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_modifiable_properties ();

  this->trader_.support_attributes ().supports_modifiable_properties (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_supports_dynamic_properties (CORBA::Boolean value
 )
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_dynamic_properties ();

  this->trader_.support_attributes ().supports_dynamic_properties (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::Boolean
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_supports_proxy_offers (CORBA::Boolean value
 )
{
  CORBA::Boolean return_value =
    this->trader_.support_attributes ().supports_proxy_offers ();

  this->trader_.support_attributes ().supports_proxy_offers (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_hop_count (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().def_hop_count ();

  this->trader_.import_attributes ().def_hop_count (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CORBA::ULong
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_hop_count (CORBA::ULong value
 )
{
  CORBA::ULong return_value =
    this->trader_.import_attributes ().max_hop_count ();

  this->trader_.import_attributes ().max_hop_count (value);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::FollowOption
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_def_follow_policy (CosTrading::FollowOption policy
 )
{
  CosTrading::FollowOption return_value =
    this->trader_.import_attributes ().def_follow_policy ();

  this->trader_.import_attributes ().def_follow_policy (policy);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::FollowOption
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_follow_policy (CosTrading::FollowOption policy
 )
{
  CosTrading::FollowOption return_value =
    this->trader_.import_attributes ().max_follow_policy ();

  this->trader_.import_attributes ().max_follow_policy (policy);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::FollowOption
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_max_link_follow_policy (CosTrading::FollowOption policy)
{
  CosTrading::FollowOption return_value =
    this->trader_.link_attributes ().max_link_follow_policy ();

  this->trader_.link_attributes ().max_link_follow_policy (policy);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::TypeRepository_ptr
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_type_repos (CosTrading::TypeRepository_ptr repository)
{
  CosTrading::TypeRepository_ptr return_value =
    this->trader_.support_attributes ().type_repos ();

  this->trader_.support_attributes ().type_repos (repository);
  return return_value;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Admin::OctetSeq*
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
set_request_id_stem (const CosTrading::Admin::OctetSeq& stem)
{
  ACE_GUARD_RETURN (TRADER_LOCK_TYPE, trader_mon, this->lock_,
                    &this->stem_id_);
  this->stem_id_ = stem;
  return &this->stem_id_;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
list_offers (CORBA::ULong how_many,
             CosTrading::OfferIdSeq_out ids,
             CosTrading::OfferIdIterator_out id_itr)
{
  // This method only applies when the register interface is implemented
  if (CORBA::is_nil (this->trader_.trading_components().register_if()))
    throw CosTrading::NotImplemented();

  TAO_Offer_Database<MAP_LOCK_TYPE>& type_map = this->trader_.offer_database ();
  TAO_Offer_Id_Iterator* offer_id_iter = type_map.retrieve_all_offer_ids ();

  id_itr = CosTrading::OfferIdIterator::_nil ();
  if (how_many > 0)
    {
      int check = offer_id_iter->next_n (how_many, ids);

      if (check == 1)
        {
          id_itr = offer_id_iter->_this ();
          offer_id_iter->_remove_ref ();
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
list_proxies (CORBA::ULong,
              CosTrading::OfferIdSeq_out,
              CosTrading::OfferIdIterator_out)
{
  throw CosTrading::NotImplemented ();
}

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
          CosTrading::FollowOption limiting_follow_rule)
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_link_name (name))
    throw CosTrading::Link::IllegalLinkName (name);

  // Ensure this isn't a duplicate link name.
  CORBA::String_var link_name (name);
  if (this->links_.find (link_name) == 0)
    throw CosTrading::Link::DuplicateLinkName (name);

  // Ensure the lookup_ptr isn't nil.
  if (CORBA::is_nil (target))
    throw CosTrading::InvalidLookupRef (target);

  // Ensure that the default link behavior isn't stronger than the
  // limiting link behavior.
  if (def_pass_on_follow_rule > limiting_follow_rule)
    throw CosTrading::Link::DefaultFollowTooPermissive(
      def_pass_on_follow_rule,
      limiting_follow_rule);

  // Ensure that the limiting link behavior for this link doesn't
  // exceed the maximum allowed for a link.
  CosTrading::FollowOption follow_policy =
    this->max_link_follow_policy ();
  if (limiting_follow_rule < follow_policy)
    throw CosTrading::Link::LimitingFollowTooPermissive(
      limiting_follow_rule,
      follow_policy);

  // Create a link info structure for this link of the federation.
  CosTrading::Link::LinkInfo link_info;

  link_info.target = CosTrading::Lookup::_duplicate (target);

  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;

  // Insert this link into the collection of links.
  this->links_.bind (link_name, link_info);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
remove_link (const char *name)
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_link_name (name))
    throw CosTrading::Link::IllegalLinkName (name);

  // Ensure this isn't a duplicate link name.
  CORBA::String_var link_name (name);
  if (this->links_.find (link_name) == -1)
    throw CosTrading::Link::UnknownLinkName (name);

  // Erase the link state from the map.
  this->links_.unbind (link_name);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Link::LinkInfo *
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::describe_link (const char *name)
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_link_name (name))
    throw CosTrading::Link::IllegalLinkName (name);

  // Ensure this isn't a duplicate link name.
  typename Links::ENTRY* link_entry = 0;
  CORBA::String_var link_name (name);
  if (this->links_.find (link_name, link_entry) == -1)
    throw CosTrading::Link::UnknownLinkName (name);

  // Build a new Link Info structure.
  CosTrading::Link::LinkInfo* new_link_info = 0;
  CosTrading::Link::LinkInfo& old_link_info = link_entry->int_id_;

  ACE_NEW_THROW_EX (new_link_info,
                    CosTrading::Link::LinkInfo,
                    CORBA::NO_MEMORY ());

  new_link_info->def_pass_on_follow_rule = old_link_info.def_pass_on_follow_rule;
  new_link_info->limiting_follow_rule = old_link_info.limiting_follow_rule;

  new_link_info->target = old_link_info.target;

  // Delayed retrieval of register interface.
  // This avoids the nested upcall that would occur were we to invoke
  // this method in the add_link method.

  new_link_info->target_reg = old_link_info.target->register_if ();

  // return the link information for this link name.
  return new_link_info;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::LinkNameSeq*
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::list_links (void)
{
  // Allocate space for the link names.
  size_t size = this->links_.current_size ();
  CORBA::ULong i = 0;
  CosTrading::LinkName* link_seq =
    CosTrading::LinkNameSeq::allocbuf (static_cast<CORBA::ULong> (size));

  // Copy the link names into the buffer.
  for (typename Links::iterator links_iter (this->links_);
       ! links_iter.done ();
       links_iter++)
    link_seq[i++] = CORBA::string_dup ((*links_iter).ext_id_.in ());

  // Return a sequence of the buf names.
  return new CosTrading::LinkNameSeq (i, i, link_seq, 1);
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
modify_link (const char *name,
             CosTrading::FollowOption def_pass_on_follow_rule,
             CosTrading::FollowOption limiting_follow_rule)
{
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_link_name (name))
    throw CosTrading::Link::IllegalLinkName (name);

  // Ensure this isn't a duplicate link name.
  typename Links::ENTRY* link_entry = 0;
  CORBA::String_var link_name (name);
  if (this->links_.find (link_name, link_entry) == -1)
    throw CosTrading::Link::UnknownLinkName (name);

  // Ensure that the default link behavior isn't stronger than the
  // limiting link behavior.
  if (def_pass_on_follow_rule > limiting_follow_rule)
    throw CosTrading::Link::DefaultFollowTooPermissive(
      def_pass_on_follow_rule,
      limiting_follow_rule);

  // Ensure that the limiting link behavior for this link doesn't
  // exceed the maximum allowed for a link.
  CosTrading::FollowOption follow_policy =
    this->max_link_follow_policy ();

  if (limiting_follow_rule < follow_policy)
    throw CosTrading::Link::LimitingFollowTooPermissive(
      limiting_follow_rule,
      follow_policy);

  // Adjust the link settings
  CosTrading::Link::LinkInfo& link_info = link_entry->int_id_;
  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;
}

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
export_proxy (CosTrading::Lookup_ptr,
              const char *,
              const CosTrading::PropertySeq&,
              CORBA::Boolean,
              const char *,
              const CosTrading::PolicySeq&)
{
  throw CORBA::UNKNOWN ();

  ACE_NOTREACHED (return 0;)
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
withdraw_proxy (const char *)
{
  throw CORBA::UNKNOWN ();
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
CosTrading::Proxy::ProxyInfo *
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
describe_proxy (const char *)
{
  throw CORBA::UNKNOWN ();

  ACE_NOTREACHED (return 0;)
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
void
TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>::
list_proxies (CORBA::ULong,
              CosTrading::OfferIdSeq*&,
              CosTrading::OfferIdIterator_ptr&)
{
  throw CORBA::UNKNOWN ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_TRADER_INTERFACES_CPP */
