/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Lookup.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_LOOKUP_H
#define TAO_LOOKUP_H

#include "Policies.h"
#include "Offer_Filter.h"
#include "Offer_Iterator.h"
#include "Constraint_Interpreter.h"
#include "Preference_Interpreter.h"

#include "stl.h"

template<class TRADER>
class TAO_Lookup :
  public TAO_Trader_Components<POA_CosTrading::Lookup>,
  public TAO_Support_Attributes<POA_CosTrading::Lookup>,
  public TAO_Import_Attributes<POA_CosTrading::Lookup>
  //
  // = TITLE
  //     This class implements CosTrading::Lookup IDL interface.
{    
public:
  
  TAO_Lookup (TRADER &trader);

  ~TAO_Lookup (void);
  
  virtual void
    query (const char *type,
	   const char *constr,
	   const char *pref,
	   const CosTrading::PolicySeq& policies,
	   const CosTrading::Lookup::SpecifiedProps& desired_props,
	   CORBA::ULong how_many,
	   CosTrading::OfferSeq_out offers,
	   CosTrading::OfferIterator_out offer_itr,
	   CosTrading::PolicyNameSeq_out limits_applied,
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
		    CosTrading::DuplicatePolicyName));

  // BEGIN SPEC
  // The query operation is the means by which an object can obtain
  // references to other objects that provide services meeting its
  // requirements.
  
  // The "type" parameter conveys the required service type. It is key
  // to the central purpose of trading: to perform an introduction for
  // future type safe interactions between importer and exporter. By
  // stating a service type, the importer implies the desired interface
  // type and a domain of discourse for talking about properties of the
  // service.

  // The trader may return a service offer of a subtype of the "type"
  // requested. Sub-typing of service types is discussed in "Service
  // Types" on page 16-4. A service subtype can be described by the
  // properties of its supertypes. This ensures that a well-formed
  // query for the "type" is also a well-formed query with respect to
  // any subtypes. However, if the importer specifies the policy of
  // exact_type_match = TRUE, then only offers with the exact (no
  // subtype) service type requested are returned. 

  // The constraint "constr" is the means by which the importer states
  // those requirements of a service that are not captured in the
  // signature of the interface. These requirements deal with the
  // computational behavior of the desired service, non-functional
  // aspects, and non-computational aspects (such as the organization
  // owning the objects that provide the service). An importer is
  // always guaranteed that any returned offer satisfies the matching
  // constraint at the time of import. If the "constr" does not obey
  // the syntax rules for a legal constraint expression, then an
  // IllegalConstraint exception is raised.

  // The "pref" parameter is also used to order those offers that
  // match the "constr" so that the offers returned by the trader are
  // in the order of greatest interest to the importer. If "pref" does
  // not obey the syntax rules for a legal preference expression, then
  // an IllegalPreference exception is raised.

  // The "policies" parameter allows the importer to specify how the
  // search should be performed as opposed to what sort of services
  // should be found in the course of the search. This can be viewed
  // as parameterizing the algorithms within the trader
  // implementation. The "policies" are a sequence of name-value
  // pairs. The names available to an importer depend on the
  // implementation of the trader. However, some names are
  // standardized where they effect the interpretation of other
  // parameters or where they may impact linking and federation of
  // traders. 

  // The "desired_props" parameter defines the set of properties
  // describing returned offers that are to be returned with the
  // object reference. There are three possibilities, the importer
  // wants one of the properties, all of the properties (but without
  // having to name them), or some properties (the names of which are
  // provided).

  // The desired_props parameter does not affect whether or not a
  // service offer is returned. To avoid "missing" desired properties,
  // the importer should specify "exists prop_name" in the
  // constraint. 

  // The returned offers are passed back in one of two ways (or a
  // combination of both). °The "offers" return result conveys a list
  // of offers and the "offer_itr" is a reference to an interface at
  // which offers can be obtained. The "how_many" parameter states
  // how many offers are to be returned via the "offers" result, any
  // remaining offers are available via the iterator interface. If the
  // "how_many" exceeds the number of offers to be returned, then the
  // "offer_itr" will be nil. 

  // If any cardinality or other limits were applied by one or more
  // traders in responding to a particular query, then the
  // "limits_applied" parameter will contain the names of the policies
  // which limited the query. The sequence of names returned in
  // "limits_applied" from any federated or proxy queries must be
  // concatenated onto the names of limits applied locally and
  // returned.
  // END SPEC
  
  static const char* NAME;

  // Handy Typedefs
  typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;
  typedef CosTradingRepos::ServiceTypeRepository::TypeStruct_var TYPE_STRUCT;
  typedef CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq_var TYPE_NAME_SEQ;
  
private:

  typedef TRADER::LOCAL_OFFER_ITER LOCAL_OFFER_ITER;
  typedef TRADER::SERVICE_TYPE_MAP SERVICE_TYPE_MAP;
  typedef pair<CosTrading::OfferId, CosTrading::Offer*> OFFER;
  typedef deque<OFFER> LOOKUP_OFFER_LIST;
  
  TAO_Offer_Iterator*
    create_offer_iterator (const char *type,
			   const TAO_Property_Filter& filter);
  // Factory method for creating an appropriate Offer Iterator based
  // on the presence of the Register Interface. 
  
  void perform_lookup (const char* type,
		       const char* constraint,
		       const char* preferences,
		       SERVICE_TYPE_MAP& service_type_map,
		       CosTradingRepos::ServiceTypeRepository_ptr rep,
		       TAO_Policies& policies,
		       LOOKUP_OFFER_LIST& ordered_offers,
           CosTrading::PolicyNameSeq_out returned_limits_applied,
		       CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::IllegalConstraint,
		    CosTrading::Lookup::IllegalPreference,
		    CosTrading::Lookup::PolicyTypeMismatch,
		    CosTrading::Lookup::InvalidPolicyValue,
		    CosTrading::IllegalServiceType,
		    CosTrading::UnknownServiceType));
  // This method has three phases. The first phase passes the
  // offer_map through the constraint interpreter, winnowing away
  // those offers that don't match the validated constraint. The
  // second phase orders the offers using the specified
  // preferences. The last phase places the ordered offers into a list 
  // for returning. At each phase, the cardinality policies specified
  // in the policies structure determine the number of offers
  // submitted to each phase. 

  void lookup_all_subtypes (const char* type,
			    SERVICE_TYPE_REPOS::IncarnationNumber& inc_num,
			    SERVICE_TYPE_MAP& service_type_map,
			    CosTradingRepos::ServiceTypeRepository_ptr rep,
			    TAO_Constraint_Interpreter& constr_inter,
			    TAO_Preference_Interpreter& pref_inter,
			    TAO_Offer_Filter& offer_filter);
  // Traverse the type hierarchy to pull the matching offers from all
  // subtypes of the root type.
  
  void lookup_one_type (const char* type,
			SERVICE_TYPE_MAP& service_type_map,
			TAO_Constraint_Interpreter& constr_inter,
			TAO_Preference_Interpreter& pref_inter,
			TAO_Offer_Filter& offer_filter);
  // Check if offers of a type fit the constraints and order them
  // according to the preferences submitted.
    
  int fill_receptacles (const char* type,
			 LOOKUP_OFFER_LIST& ordered_offers,
			 CORBA::ULong how_many,
			 const CosTrading::Lookup::SpecifiedProps& desired_props,			 
			 CosTrading::OfferSeq*& offers,
			 CosTrading::OfferIterator_ptr& offer_itr,
			 CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		    CosTrading::DuplicatePropertyName));
  // This method takes the list of ordered offers and places a number
  // of them in the sequence of returned offers and the rest into thr
  // iterator. In addition, fill_receptacles uses the
  // TAO_Property_Filter to ensure the returned offers contain the
  // properties specified in the desired_props in parameter.

  void forward_query (const CosTrading::TraderName& starting_trader,
		      const char *type,
		      const char *constr,
		      const char *pref,
		      TAO_Policies& policies,
		      const CosTrading::Lookup::SpecifiedProps& desired_props,
		      CORBA::ULong how_many,
		      CosTrading::OfferSeq_out offers,
		      CosTrading::OfferIterator_out offer_itr,
		      CosTrading::PolicyNameSeq_out limits_applied,
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
		     CosTrading::DuplicatePolicyName));
  // If a starting_trader policy was specfied, foward the query to the 
  // next link in the sequence.
		      
  CORBA::Boolean duplicate_stem_id (TAO_Policies& policies,
				    CORBA::Environment& _env);
  // Determine if the stem id provided to the query is one we've
  // already seen.

  CORBA::Boolean retrieve_links (TAO_Policies& policies,
				 CORBA::ULong offer_returned,
				 CosTrading::LinkNameSeq_out links,
				 CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::Lookup::PolicyTypeMismatch));
  // Assemble a sequence of links that the federate_query method
  // should follow. Use the starting_trader policy, if one's provided, 
  // otherwise use the Link interface to determine which of the
  // registered links should be followed in this query.
  
  void federated_query (const CosTrading::LinkNameSeq& links,
			const char *type,
			const char *constr,
			const char *pref,
			TAO_Policies& policies,
			const CosTrading::Lookup::SpecifiedProps& desired_props,
			CORBA::ULong how_many,
			CosTrading::OfferSeq_out offers,
			CosTrading::OfferIterator_out offer_itr,
			CosTrading::PolicyNameSeq_out limits_applied,
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
		     CosTrading::DuplicatePolicyName));
  // Perform and pass on a query over a set of links. Merge the
  // results of the federated queries into a single set of results
  // suitable for returning to the user.  
  
  TRADER &trader_;
  // A reference to the trader for obtaining offer maps.

  typedef set
    <
    CosTrading::Admin::OctetSeq_var,
    less <CosTrading::Admin::OctetSeq_var>
    >
    REQUEST_IDS;
  
  REQUEST_IDS request_ids_;
  // A list of recent request_id_stems 
};

int operator< (const CosTrading::Admin::OctetSeq_var& left,
	       const CosTrading::Admin::OctetSeq_var& right);

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Lookup.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_LOOKUP_H */
