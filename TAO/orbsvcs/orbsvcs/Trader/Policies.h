/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Policies.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ======================================================================= 

#ifndef TAO_POLICIES_H
#define TAO_POLICIES_H

// STL fun stuff.
#include "stl.h"
#include "Trader.h"

class TAO_Policies
//
// = TITLE
// This class ensures that policies submitted to Lookup make sense,
// have the correct value types, and don't exceed the maximums set
// through the Admin Interface. 
//
// = DESCRIPTION
// TAO_Policies does an admirable job of reconciling differences
// between the default parameter settings of the Trader and the import 
// and other policies set by the client. Unbeknownst to its client
// TAO_Policies hides this arbitration, and records whether the user
// policy was chosen, or the default. This information gets returned
// to the invoker of the query method.
{
public:

  enum POLICY_TYPE
  {
    EXACT_TYPE_MATCH,
    HOP_COUNT,
    LINK_FOLLOW_RULE,
    MATCH_CARD,
    RETURN_CARD,
    SEARCH_CARD,
    STARTING_TRADER,
    USE_DYNAMIC_PROPERTIES,
    USE_MODIFIABLE_PROPERTIES,
    USE_PROXY_OFFERS,
    REQUEST_ID
  };
  
  static const char * POLICY_NAMES[];
  
  TAO_Policies (TAO_Trader_Base& trader,
		const CosTrading::PolicySeq& policies,
		CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPolicyName,
		     CosTrading::DuplicatePolicyName));

  // BEGIN SPEC
  // The "policies" parameter allows the importer to specify how the
  // search should be performed as opposed to what sort of services
  // should be found in the course of the search. This can be viewed
  // as parameterizing the algorithms within the trader
  // implementation. The "policies" are a sequence of name-value
  // pairs. The names available to an importer depend on the
  // implementation of the trader. However, some names are
  // standardized where they effect the interpretation of other
  // parameters or where they may impact linking and federation of
  // traders. ° If a policy name in this parameter does not obey the
  // syntactic rules for legal PolicyName's, then an IllegalPolicyName
  // exception is raised. ° If the type of the value associated with a
  // policy differs from that specified in this specification, then a
  // PolicyTypeMismatch exception is raised. ° If subsequent
  // processing of a PolicyValue yields any errors (e.g., the
  // starting_trader policy value is malformed), then an
  // InvalidPolicyValue exception is raised. ° If the same policy name
  // is included two or more times in this parameter, then the
  // DuplicatePolicyName exception is raised. 
  // END SPEC
  
  CORBA::ULong search_card (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "search_card" policy indicates to the trader the maximum
  // number of offers it should consider when looking for type
  // conformance and constraint expression match. The lesser of this
  // value and the trader's max_search_card attribute is used by the
  // trader. If this policy is not specified, then the value of the
  // trader's def_search_card attribute is used.
  // END SPEC
  
  CORBA::ULong match_card (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "match_card" policy indicates to the trader the maximum
  // number of matching offers to which the preference specification
  // should be applied. The lesser of this value and the trader's
  // max_match_card attribute is used by the trader. If this policy is
  // not specified, then the value of the trader's def_match_card
  // attribute is used.
  // END SPEC
  
  CORBA::ULong return_card (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "return_card" policy indicates to the trader the maximum
  // number of matching offers to return as a result of this
  // query. The lesser of this value and the trader's max_return_card
  // attribute is used by the trader. If this policy is not specified,
  // then the value of the trader's def_return_card attribute is
  // used. 
  // END SPEC
  
  // = Offer consideration policies
  
  CORBA::Boolean use_modifiable_properties (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "use_modifiable_properties" policy indicates whether the
  // trader should consider offers which have modifiable properties
  // when constructing the set of offers to which type conformance and
  // constraint processing should be applied. If the value of this
  // policy is TRUE, then such offers will be included; if FALSE, they
  // will not. If this policy is not specified, such offers will be
  // included.
  // END SPEC
  
  CORBA::Boolean use_dynamic_properties (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "use_dynamic_properties" policy indicates whether the trader
  // should consider offers which have dynamic properties when
  // constructing the set of offers to which type conformance and
  // constraint processing should be applied. If the value of this
  // policy is TRUE, then such offers will be included; if FALSE, they
  // will not. If this policy is not specified, such offers will be
  // included.
  // END SPEC
  
  CORBA::Boolean use_proxy_offers (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "use_proxy_offers" policy indicates whether the trader should
  // consider proxy offers when constructing the set of offers to
  // which type conformance and constraint processing should be
  // applied. If the value of this policy is TRUE, then such offers
  // will be included; if FALSE, they will not. If this policy is not
  // specified, such offers will be included.
  // END SPEC
  
  CORBA::Boolean exact_type_match (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "exact_type_match" policy indicates to the trader whether the
  // importer's service type must exactly match an offer's service
  // type; if not (and by default), then any offer of a type
  // conformant to the importer's service type is considered.
  // END SPEC
  
  // = Federated trader policies (not implemented yet)
  
  CosTrading::TraderName* starting_trader (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
		    CosTrading::Lookup::InvalidPolicyValue));
  // BEGIN SPEC
  // The "starting_trader" policy facilitates the distribution of the
  // trading service itself. It allows an importer to scope a search
  // by choosing to explicitly navigate the links of the trading
  // graph. If the policy is used in a query invocation it is
  // recommended that it be the first policy-value pair; this
  // facilitates an optimal forwarding of the query operation. A
  // "policies" parameter need not include a value for the
  // "starting_trader" policy. Where this policy is present, the first
  // name component is compared against the name held in each link. If
  // no match is found, the InvalidPolicyValue exception is
  // raised. Otherwise, the trader invokes query() on the Lookup
  // interface held by the named link, but passing the
  // "starting_trader" policy with the first component removed.
  // END SPEC

  CosTrading::FollowOption link_follow_rule (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Determine the link follow policy for this query overall.
  
  CosTrading::FollowOption link_follow_rule (const char* link_name,
					     CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
		     CosTrading::Lookup::InvalidPolicyValue));
  // Determine the link follow policy for a given <link_name>.
  
  // BEGIN SPEC
  //The "link_follow_rule" policy indicates how the client wishes
  //links to be followed in the resolution of its query. See the
  //discussion in "Link Follow Behavior" on page 16-16 for details. 
  // END SPEC

  // This method returns the link_follow_rule for a link whose name is 
  // <link_name> using the following formula:
  // if the importer specified a link_follow_rule policy
  //      min(trader.max_follow_policy, link.limiting_follow_rule,
  //          query.link_follow_rule)
  // else min(trader.max_follow_policy, link.limiting_follow_rule,
  //          trader.def_follow_policy) 
  
  CORBA::ULong hop_count (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "hop_count" policy indicates to the trader the maximum number
  // of hops across federation links that should be tolerated in the
  // resolution of this query. The hop_count at the current trader is
  // determined by taking the minimum of the trader's max_hop_count
  // attribute and the importer's hop_count policy, if provided, or
  // the trader's def_hop_count attribute if it is not. If the
  // resulting value is zero, then no federated queries are
  // permitted. If it is greater than zero, then it must be
  // decremented before passing on to a federated trader. 
  // END SPEC

  CosTrading::Admin::OctetSeq* request_id (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Return the request_id passed to the query method across a link to
  // another trader. 
  
  CosTrading::PolicySeq* policies_to_forward (void);
  // Policies to forward to the next trader in a directed federated query.
  
  CosTrading::PolicySeq*
    policies_to_pass (CosTrading::FollowOption def_pass_on_follow_rule,
		      CORBA::ULong offers_returned,
		      CosTrading::Admin_ptr admin_if);
  // Policies to pass on to the next generation of queries. Decrements 
  // the hop counter,adds a link_follow_rule if none exists, adjusts
  // the return_card, and adds the stem_id if none exists.
  
private:

  typedef vector <CosTrading::Policy*> POL_VECTOR;
    
  CORBA::ULong ulong_prop (POLICY_TYPE pol,
			   CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Reconclile a ULong property with its default.
  
  CORBA::Boolean boolean_prop (POLICY_TYPE pol,
			       CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Reconcile a Boolean property with its debault.
   
  POL_VECTOR policies_;
  // The policies indexable from the enumerated type.
  
  TAO_Trader_Base& trader_;
  // For the validating identifier names.
};


#endif /* TAO_POLICIES_H */
