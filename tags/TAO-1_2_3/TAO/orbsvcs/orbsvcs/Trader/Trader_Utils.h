/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Trader_Utils.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// Client Utils:
//   TAO_Dynamic_Property
//   TAO_Policy_Creator
//   TAO_Property_Evaluator
//   TAO_Property_Evaluator_By_Name
//
// Service Utils:
//   TAO_Policies
//   TAO_Offer_Filter
//   TAO_Offer_Modifier
//   TAO_Property_Filter
//   TAO_Property_Evaluator
//   TAO_Property_Evaluator_By_Name
//
// ========================================================================

#ifndef TAO_TRADER_UTILS_H
#define TAO_TRADER_UTILS_H
#include "ace/pre.h"

#include "Trader.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Trading_Export TAO_Property_Evaluator
{
  // = TITLE
  //   This class abstracts away the details of obtaining property
  //   values and property types. Since the procedure for obtaining the
  //   value or type of a dynamic property is disparate from the method
  //   for a static property, TAO_Property_Evaluator provides methods
  //   that will unify the two approaches under a single
  //   interface. Since dynamic properties aren't necessarily supported
  //   by a trader, this class accounts for that contingency. The use of
  //   indexed lookups allows them to occur in constant time on the
  //   CORBA sequences, but requires that the client know the layout of
  //   properties ahead of time.
public:

  TAO_Property_Evaluator(const CosTrading::PropertySeq& properties,
                         CORBA::Boolean supports_dp = 1);

  TAO_Property_Evaluator(CosTrading::Offer& offer,
                         CORBA::Boolean supports_dp = 1);
  // Construct an instance of TAO_Property_Evaluator that operates on
  // an <offer> where the support for dynamic properties is dictated
  // by <supports_dynamic_properties>.

  virtual ~TAO_Property_Evaluator (void);
  // Clean up dynamic properties.

  int is_dynamic_property(int index);
  // Returns 1 if the property at index <index> is dynamic. Returns a
  // 0 when the index is out of bounds.

  CORBA::Any* property_value(int index ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));

  // Returns value of the property whose index is <index>. If the
  // property at that index is dynamic and the trader supports dynamic
  // properties, then the property_value method will obtain the value
  // of the dynamic property using the evalDP method on the
  // CosTradingDynamic::DynamicPropEval interface, passing on a
  // CosTradingDynamic::DPEvalFailure exception on failure. If the
  // property index is undefined, the method returns a null pointer.

  CORBA::TypeCode_ptr property_type (int index);
  // Returns the type of the property whose index is <index>. If the
  // property is dynamic and the trader supports dynamic properties,
  // then the method returns the <returned_type> field of the
  // CosTradingDynamic::DynamicProp struct associated with the
  // property name. If the index is out of bounds, the method returns
  // a null pointer (that is, 0).

protected:

  typedef CosTradingDynamic::DynamicProp DP_Struct;
  typedef CosTradingDynamic::DynamicPropEval DP_Eval;

  const CosTrading::PropertySeq& props_;
  // The offer from which the TAO_Property_Evaluator extracts property
  // information.

  int supports_dp_;

  CORBA::Any** dp_cache_;
  // In order for the client to treat the results of property_value
  // uniformly, we need to collect the dynamically allocated anys
  // retrieved from dynamic properties and free them upon deletion. If
  // we didn't do this, then the property_value method would leak or
  // cause seg faults, since the client wouldn't be able to tell
  // whether or not the return value should be freed.

private:

  TAO_Property_Evaluator (const TAO_Property_Evaluator&);
  TAO_Property_Evaluator& operator= (const TAO_Property_Evaluator&);
};

class TAO_Trading_Export TAO_Property_Evaluator_By_Name : public TAO_Property_Evaluator
{
  // = TITLE
  //    This class extends the TAO_Property_Evaluator to allow lookups
  //    based on the property name of interest. Since the property
  //    information is contained within an integer indexed array,
  //    lookups may occur in O(n) time, where n is the length of the
  //    array. To make lookups by name more efficient,
  //    TAO_Property_Evaluator_By_Name creates a mapping of property
  //    names to integer indicies, upon which lookups are guaranteed to
  //    be O(lg n).
public:

  TAO_Property_Evaluator_By_Name (const CosTrading::PropertySeq& properties
                                  ACE_ENV_ARG_DECL ,
                                  CORBA::Boolean supports_dp = 1)
    ACE_THROW_SPEC ((CosTrading::DuplicatePropertyName,
                     CosTrading::IllegalPropertyName));

  TAO_Property_Evaluator_By_Name(CosTrading::Offer& offer,
                                 CORBA::Boolean supports_dp = 1);
  // Construct an instance of TAO_Property_Evaluator that operates on
  // an <offer> where the support for dynamic properties is dictated
  // by <supports_dynamic_properties>.

  int is_dynamic_property(const char* property_name);
  // Returns 1 if the property whose name is <property_name> is
  // defined and dynamic. If the property is undefined, this method
  // will throw a Property_Undefined exception with impunity.

  CORBA::Any* property_value(const char* property_name
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));

  // This method is identical to its counterpart in
  // TAO_Property_Evaluator, except property_value first discovers the
  // index through a string matching lookup.

  CORBA::TypeCode_ptr property_type(const char* property_name);
  // This method is identical to its counterpart in
  // TAO_Property_Evaluator, exception property_type first discovers
  // the index through a string matching lookup.

  const CosTrading::Property* get_property (const char* property_name);

private:

  TAO_Property_Evaluator_By_Name (const TAO_Property_Evaluator_By_Name&);
  TAO_Property_Evaluator_By_Name& operator= (const TAO_Property_Evaluator_By_Name&);

  TAO_Lookup_Table table_;
  // The instance of the above mapping for the offer provided in the
  // constructor.
};

class TAO_Trading_Export TAO_Dynamic_Property : public virtual POA_CosTradingDynamic::DynamicPropEval, public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   Little helper class that you can extend to have your dynamic
  //   property handler construct CosTradingDynamic::DynamicProp structs.
public:

  TAO_Dynamic_Property (void) {}
  TAO_Dynamic_Property (const TAO_Dynamic_Property &) {}
  void operator= (const TAO_Dynamic_Property &) {}
  virtual ~TAO_Dynamic_Property (void);

  void destroy (void);

  virtual CORBA::Any* evalDP(const char* name,
                             CORBA::TypeCode_ptr returned_type,
                             const CORBA::Any& extra_info
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTradingDynamic::DPEvalFailure)) = 0;
  // Dynamic property evaluation call-back method.

  CosTradingDynamic::DynamicProp*
  construct_dynamic_prop (const char* name,
                          CORBA::TypeCode_ptr returned_type,
                          const CORBA::Any& extra_info);
  // Method to construct a dynamic property structure suitable for
  // exporting in a CosTrading::PropertyStruct to the Trading Service.

private:

  CosTradingDynamic::DynamicPropEval_var prop_;
};

class TAO_Policies
{
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
public:

#define TAO_NUM_POLICIES  11

  enum POLICY_TYPE
  {
    STARTING_TRADER,
    EXACT_TYPE_MATCH,
    HOP_COUNT,
    LINK_FOLLOW_RULE,
    MATCH_CARD,
    RETURN_CARD,
    SEARCH_CARD,
    USE_DYNAMIC_PROPERTIES,
    USE_MODIFIABLE_PROPERTIES,
    USE_PROXY_OFFERS,
    REQUEST_ID
  };
  // This enum represents the relative order that properties are
  // passed from one trader to another. Hence, as recommended by the
  // spec, the starting_trader policies will be the first element in
  // the polcy sequence if it's set for a query.

  static const char * POLICY_NAMES[];

  TAO_Policies (TAO_Trader_Base& trader,
                const CosTrading::PolicySeq& policies
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::Lookup::IllegalPolicyName,
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

  ~TAO_Policies (void);

  CORBA::ULong search_card (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "search_card" policy indicates to the trader the maximum
  // number of offers it should consider when looking for type
  // conformance and constraint expression match. The lesser of this
  // value and the trader's max_search_card attribute is used by the
  // trader. If this policy is not specified, then the value of the
  // trader's def_search_card attribute is used.
  // END SPEC

  CORBA::ULong match_card (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "match_card" policy indicates to the trader the maximum
  // number of matching offers to which the preference specification
  // should be applied. The lesser of this value and the trader's
  // max_match_card attribute is used by the trader. If this policy is
  // not specified, then the value of the trader's def_match_card
  // attribute is used.
  // END SPEC

  CORBA::ULong return_card (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "return_card" policy indicates to the trader the maximum
  // number of matching offers to return as a result of this
  // query. The lesser of this value and the trader's max_return_card
  // attribute is used by the trader. If this policy is not specified,
  // then the value of the trader's def_return_card attribute is
  // used.
  // END SPEC

  // = Offer consideration policies

  CORBA::Boolean use_modifiable_properties (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "use_modifiable_properties" policy indicates whether the
  // trader should consider offers which have modifiable properties
  // when constructing the set of offers to which type conformance and
  // constraint processing should be applied. If the value of this
  // policy is TRUE, then such offers will be included; if FALSE, they
  // will not. If this policy is not specified, such offers will be
  // included.
  // END SPEC

  CORBA::Boolean use_dynamic_properties (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "use_dynamic_properties" policy indicates whether the trader
  // should consider offers which have dynamic properties when
  // constructing the set of offers to which type conformance and
  // constraint processing should be applied. If the value of this
  // policy is TRUE, then such offers will be included; if FALSE, they
  // will not. If this policy is not specified, such offers will be
  // included.
  // END SPEC

  CORBA::Boolean use_proxy_offers (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "use_proxy_offers" policy indicates whether the trader should
  // consider proxy offers when constructing the set of offers to
  // which type conformance and constraint processing should be
  // applied. If the value of this policy is TRUE, then such offers
  // will be included; if FALSE, they will not. If this policy is not
  // specified, such offers will be included.
  // END SPEC

  CORBA::Boolean exact_type_match (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

  // BEGIN SPEC
  // The "exact_type_match" policy indicates to the trader whether the
  // importer's service type must exactly match an offer's service
  // type; if not (and by default), then any offer of a type
  // conformant to the importer's service type is considered.
  // END SPEC

  // = Federated trader policies (not implemented yet)

  CosTrading::TraderName* starting_trader (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
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

  CosTrading::FollowOption link_follow_rule (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Determine the link follow policy for this query overall.

  // BEGIN SPEC
  //The "link_follow_rule" policy indicates how the client wishes
  //links to be followed in the resolution of its query. See the
  //discussion in "Link Follow Behavior" on page 16-16 for details.
  // END SPEC


  CosTrading::FollowOption link_follow_rule (const CosTrading::Link::LinkInfo& link_info
                                             ACE_ENV_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
                     CosTrading::Lookup::InvalidPolicyValue,
                     CosTrading::Link::IllegalLinkName,
                     CosTrading::Link::UnknownLinkName));
  // Determine the link follow policy for a given <link_name>.
  // This method returns the link_follow_rule for a link whose name is
  // <link_name> using the following formula:
  // if the importer specified a link_follow_rule policy
  //      min(trader.max_follow_policy, link.limiting_follow_rule,
  //          query.link_follow_rule)
  // else min(trader.max_follow_policy, link.limiting_follow_rule,
  //          trader.def_follow_policy)

  CORBA::ULong hop_count (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));

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

  CosTrading::Admin::OctetSeq* request_id (ACE_ENV_SINGLE_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Return the request_id passed to the query method across a link to
  // another trader.

  void copy_to_pass (CosTrading::PolicySeq& policy_seq,
                     const CosTrading::Admin::OctetSeq& request_id) const;
  // Policies to forward to the next trader in a federated query.

  void copy_to_forward (CosTrading::PolicySeq& policy_seq,
                        const CosTrading::TraderName& name) const;
  // Policies to forward to the next trader in a directed query.

  void copy_in_follow_option (CosTrading::PolicySeq& policy_seq,
                              const CosTrading::Link::LinkInfo& link_info
                              ACE_ENV_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch,
                     CosTrading::Lookup::InvalidPolicyValue));
  // Determine the link follow policy to pass down the link with <link_name>.
  // This method returns the link_follow_rule for a link whose name is
  // <link_name> using the following formula:
  // If the importer specified a link_follow_rule, policy
  //      pass on min(query.link_follow_rule, link.limiting_follow_rule,
  //                  trader.max_follow_policy)
  // else pass on min(link.def_pass_on_follow_rule,
  //                  trader.max_follow_policy)

private:

  CORBA::ULong ulong_prop (POLICY_TYPE pol
                           ACE_ENV_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Reconclile a ULong property with its default.

  CORBA::Boolean boolean_prop (POLICY_TYPE pol
                               ACE_ENV_ARG_DECL) const
    ACE_THROW_SPEC ((CosTrading::Lookup::PolicyTypeMismatch));
  // Reconcile a Boolean property with its debault.

  TAO_Policies (const TAO_Policies&);
  TAO_Policies& operator= (const TAO_Policies&);

  CosTrading::Policy* policies_[TAO_NUM_POLICIES];
  // The policies indexable from the enumerated type.

  TAO_Trader_Base& trader_;
  // For the validating identifier names.
};

class TAO_Trading_Export TAO_Policy_Creator
{
  // = TITLE
  //
  // This class is a utility for clients using the CosTrading::Lookup
  // interface that helps them build a policy sequence without violating
  // syntax rules and having to mess with typecodes.
public:

  TAO_Policy_Creator (int num_policies = 0);

  // = Routines to set policies.

  void search_card (CORBA::ULong scard);
  // Set the maximum number of offers searched for the query.

  void match_card (CORBA::ULong mcard);
  // Set the maximum number of offers searched for the query.

  void return_card (CORBA::ULong rcard);
  // Set the maximum number of offers rerturned for the query.

  // A note about cardinalities: The spec implies that these
  // cardinalities apply to the global office space, that is, all
  // offers on all linked traders. However, there's no mechanism for
  // one trader to return to the calling trader the number of offers
  // searched or matched. Thus, these cardinalities are applied on a
  // per-trader basis.

  void use_modifiable_properties (CORBA::Boolean mod_props);
  // Consider offers with modifiable properties.

  void use_dynamic_properties (CORBA::Boolean dyn_props);
  // Consider offers with dynamic properties.

  void use_proxy_offers (CORBA::Boolean prox_offs);
  // Consider proxy offers (NOT SUPPORTED).

  void starting_trader (const CosTrading::TraderName& name); // Copy
  void starting_trader (CosTrading::TraderName* name); // Own
  // Designate a trader at which to begin the query.

  void link_follow_rule (CosTrading::FollowOption follow_option);
  // Specify under what conditions a federated query is appropriate.

  void hop_count (CORBA::ULong hop_count);
  // Limit the breadth of a federated query.

  void request_id (const CosTrading::Admin::OctetSeq& request_id);
  // Set the identifier for this query (clients shouldn't use this).

  void exact_type_match (CORBA::Boolean exact_type);
  // Search only the designated type --- not it's subtypes.

  operator const CosTrading::PolicySeq& (void) const;
  // Return the constructed policy sequence.

  const CosTrading::PolicySeq& policy_seq (void) const;
  // Return a PolicySeq suitable for passing to the query method of
  // the Lookup interface.

private:

  TAO_Policy_Creator (const TAO_Policy_Creator&);
  TAO_Policy_Creator& operator= (const TAO_Policy_Creator&);

  CosTrading::Policy& fetch_next_policy (TAO_Policies::POLICY_TYPE pol_type);
  // Method to prepare the next slot in the policies_ sequence for
  // policy insertion.

  int poltable_[TAO_Policies::REQUEST_ID + 1];
  // Table mapping policy enum value to the index in the policies sequence.

  CosTrading::PolicySeq policies_;
  // The sequence being prepared for submittal to the query method.

  CORBA::ULong num_policies_;
  // The number of policies so far in the sequence.
};

class TAO_Offer_Modifier
{
  // = TITLE
  //   This class deletes, modifies, and adds properties to a given
  //   offer according to the rules of the modify method on the Register
  //   interface.
public:

  TAO_Offer_Modifier (const char* type,
                      const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct,
                      CosTrading::Offer* offer);
  // Modify an <offer> of type <type>, whose properties are described
  // by <type_struct>

  ~TAO_Offer_Modifier (void);

  void delete_properties (const CosTrading::PropertyNameSeq& deletes
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::Register::UnknownPropertyName,
                     CosTrading::Register::MandatoryProperty,
                     CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName));
  // Delete the properties whose names were given to the
  // constructor. Ensure we don't delete mandatory properties.

  void merge_properties (const CosTrading::PropertySeq& modifies
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName,
                     CosTrading::PropertyTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::Register::ReadonlyProperty));
  // Copy to the destination the union of the source and destination
  // properties. In the case of duplicate properties, update the
  // destination with the source's value. This class claims the memory
  // in the modifies sequence.

  void affect_change (const CosTrading::PropertySeq& modifies);
  // Return a reference to the Offer with the changes affected.

private:

  TAO_Offer_Modifier (const TAO_Offer_Modifier&);
  TAO_Offer_Modifier& operator= (const TAO_Offer_Modifier&);

  typedef ACE_Hash_Map_Manager_Ex <TAO_String_Hash_Key,
                                   CosTrading::Property *,
                                   ACE_Hash<TAO_String_Hash_Key>,
                                   ACE_Equal_To<TAO_String_Hash_Key>,
                                   ACE_Null_Mutex>
          Property_Table;

  const char *type_;
  // The type of the offer.

  Property_Table props_;
  // The map of properties in the offer.

  TAO_Typecode_Table prop_types_;
  // Table of property types.

  TAO_String_Set readonly_, mandatory_;
  // The set of readonly and mandatory property names in the offer's
  // type.

  CosTrading::Offer* offer_;
  // A reference to the offer undergoing change.
};

class TAO_Offer_Filter
{
  // = TITLE
  //    The purpose of this class is to ensure that offers that
  //    shouldn't be considered by the TAO_Constraint_Interpreter
  //    aren't.
  //
  // = DESCRIPTION
  //    There two classes of reasons why an offer for a correct
  //    type shouldn't be considered: 1) The default parameters of the
  //    Trader or policies passed to the Lookup::query method deem it
  //    inappropriate to consider offers with modifiable (i.e., not
  //    readonly) or dynamic properties. 2) We've exceeded the
  //    default or provided cardinality constraints. TAO_Offer_Filter
  //    ensures that violation of policies doesn't occur. It's the
  //    enforcer.
public:

  TAO_Offer_Filter (TAO_Policies& policies
                    ACE_ENV_ARG_DECL);
  // Glean from the TypeStruct and Policy setting the appropriate way
  // to screen unsuitable offers from consideration.

  void configure_type (CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct);
  // Set the offer filter to screen for offers containing properties
  // that aren't marked as readonly in this TypeStruct.

  CORBA::Boolean ok_to_consider (CosTrading::Offer* offer);
  // Determine whether the poicies contained in the given policy
  // object allow the Lookup interface to consider the offer. That is,
  // if use_modifiable_properties is false, and the offer contains
  // modifiable properties as designated in the type struct, return
  // false. If use_dynamic_properties is false, and the offer contains
  // dynamic properties, then return false. If the lookup interface is
  // safe in considering this offer, return true and subtract from the
  // search card value. When the search card value falls to zero,
  // ok_to_consider always returns false.

  CORBA::Boolean ok_to_consider_more (void);
  // It's ok to consider more offers when lookup hasn't exceeded the
  // cardinality values for searching and matching offers.

  void matched_offer (void);
  // Signal that the Lookup method has matched an offer; decrement the
  // match_card.

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

  CORBA::ULong search_card_remaining (void) const;
  CORBA::ULong match_card_remaining (void) const;
  // Accessors to retrieve the adjusted cardinalities.

private:

  TAO_Offer_Filter (const TAO_Offer_Filter&);
  TAO_Offer_Filter& operator= (const TAO_Offer_Filter&);

  TAO_String_Set not_mod_props_;
  // The set of the name of modifiable properties.

  TAO_String_Set limits_;
  // Cardinality and property limitations applied.

  CORBA::ULong search_card_, match_card_, return_card_;
  // Keep track of the cardinalities.

  CORBA::Boolean dp_;
  CORBA::Boolean mod_;
  // Keep track of property limitations: modifiable or dynamic ones
  // may be bad.
};

class TAO_Property_Filter
{
  // = TITLE
  //
  //   The Ace_Property_Filter copies those properties specified in a
  //   CosTrading::Lookup::SpecifiedProps from a source
  //   CosTrading::Offer to a destination CosTrading::Offer.
public:

  typedef CosTrading::Lookup::SpecifiedProps SPECIFIED_PROPS;

  TAO_Property_Filter (void) : policy_ (CosTrading::Lookup::all) {}
  // An accomplice to g++'s insane lust for copy constructors.

  TAO_Property_Filter (const SPECIFIED_PROPS& desired_props
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName));
  // Verify that the specified properties are correct.

  TAO_Property_Filter (const TAO_Property_Filter& prop_filter);
  TAO_Property_Filter& operator= (const TAO_Property_Filter& prop_filter);

  void filter_offer (CosTrading::Offer* source,
                     CosTrading::Offer& destination);
  // Copy the desired properties from the source offer to the
  // destination offer.

private:

  typedef ACE_Unbounded_Queue< CosTrading::Property* > Prop_Queue;

  TAO_String_Set props_;
  CosTrading::Lookup::HowManyProps policy_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_TRADER_UTILS_H */
