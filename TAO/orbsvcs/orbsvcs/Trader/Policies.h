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

#include "CosTradingC.hh"
#include "Trader_Base.h"

// STL fun stuff.
#include <vector>
#include <deque>

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
    USE_PROXY_OFFERS
  };
  
  static const char * POLICY_NAMES[];
  
  TAO_Policies (TAO_Trader_Base& trader,
		CosTrading::PolicySeq& policies,
		CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::IllegalPolicyName,
		    CosTrading::DuplicatePolicyName);

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
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

  // BEGIN SPEC
  // The "search_card" policy indicates to the trader the maximum
  // number of offers it should consider when looking for type
  // conformance and constraint expression match. The lesser of this
  // value and the trader's max_search_card attribute is used by the
  // trader. If this policy is not specified, then the value of the
  // trader's def_search_card attribute is used.
  // END SPEC
  
  CORBA::ULong match_card (CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

  // BEGIN SPEC
  // The "match_card" policy indicates to the trader the maximum
  // number of matching offers to which the preference specification
  // should be applied. The lesser of this value and the trader's
  // max_match_card attribute is used by the trader. If this policy is
  // not specified, then the value of the trader's def_match_card
  // attribute is used.
  // END SPEC
  
  CORBA::ULong return_card (CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

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
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

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
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

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
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

  // BEGIN SPEC
  // The "use_proxy_offers" policy indicates whether the trader should
  // consider proxy offers when constructing the set of offers to
  // which type conformance and constraint processing should be
  // applied. If the value of this policy is TRUE, then such offers
  // will be included; if FALSE, they will not. If this policy is not
  // specified, such offers will be included.
  // END SPEC
  
  CORBA::Boolean exact_type_match (CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

  // BEGIN SPEC
  // The "exact_type_match" policy indicates to the trader whether the
  // importer's service type must exactly match an offer's service
  // type; if not (and by default), then any offer of a type
  // conformant to the importer's service type is considered.
  // END SPEC
  
  // = Federated trader policies (not implemented yet)
  
  CosTrading::TraderName* starting_trader (CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch,
		    CosTrading::Lookup::InvalidPolicyValue);
  
  CosTrading::FollowOption link_follow_rule (CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch,
		    CosTrading::Lookup::InvalidPolicyValue);
  
  CORBA::ULong hop_count (CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);

  // = Return the limits applied.
  
  CosTrading::PolicyNameSeq* limits_applied (void);
  // BEGIN SPEC
  // If any cardinality or other limits were applied by one or more
  // traders in responding to a particular query, then the
  // "limits_applied" parameter will contain the names of the policies
  // which limited the query. The sequence of names returned in
  // "limits_applied" from any federated or proxy queries must be
  // concatenated onto the names of limits applied locally and
  // returned. 
  // END SPEC
private:

  typedef vector <CosTrading::Policy*> POL_VECTOR;
  typedef deque <char*> POL_QUEUE;
    
  CORBA::ULong ulong_prop (POLICY_TYPE pol,
			   CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);
  // Reconclile a ULong property with its default.
  
  CORBA::Boolean boolean_prop (POLICY_TYPE pol,
			       CORBA::Environment& _env)
    TAO_THROW_SPEC (CosTrading::Lookup::PolicyTypeMismatch);
  // Reconcile a Boolean property with its debault.
 
  POL_QUEUE limits_;
  // The policies employed to date.
  
  POL_VECTOR policies_;
  // The policies indexable from the enumerated type.
  
  TAO_Trader_Base& trader_;
  // For the validating identifier names.
};


#endif /* TAO_POLICIES_H */
