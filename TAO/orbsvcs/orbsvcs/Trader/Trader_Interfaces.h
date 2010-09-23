// -*- C++ -*-

//=============================================================================
/**
 *  @file    Trader_Interfaces.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TRADER_INTERFACES_H
#define TAO_TRADER_INTERFACES_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Trader_Utils.h"
#include "orbsvcs/Trader/Constraint_Interpreter.h"
#include "orbsvcs/Trader/Offer_Iterators_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Hack because g++ forced this inane circular dependecy!

class TAO_Constraint_Interpreter;
class TAO_Constraint_Evaluator;
class TAO_Constraint_Validator;
class TAO_Preference_Interpreter;

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> class TAO_Lookup;
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> class TAO_Register;
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> class TAO_Admin;
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> class TAO_Proxy;
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> class TAO_Link;

TAO_END_VERSIONED_NAMESPACE_DECL

#include "orbsvcs/Trader/Trader_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Lookup
 *
 * @brief This class implements CosTrading::Lookup IDL interface.
 */
template<class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Lookup :
  public TAO_Trader_Components<POA_CosTrading::Lookup>,
  public TAO_Support_Attributes<POA_CosTrading::Lookup>,
  public TAO_Import_Attributes<POA_CosTrading::Lookup>
{
public:

  TAO_Lookup (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader);

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
           CosTrading::PolicyNameSeq_out limits_applied);

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
  // combination of both). The "offers" return result conveys a list
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

private:

  /// Factory method for creating an appropriate Offer Iterator based
  /// on the presence of the Register Interface.
  TAO_Offer_Iterator* create_offer_iterator (const TAO_Property_Filter&);

  /// Traverse the type hierarchy to pull the matching offers from all
  /// subtypes of the root type.
  void lookup_all_subtypes (const char* type,
                            CosTradingRepos::ServiceTypeRepository::IncarnationNumber& inc_num,
                            TAO_Offer_Database<MAP_LOCK_TYPE>& offer_database,
                            CosTradingRepos::ServiceTypeRepository_ptr rep,
                            TAO_Constraint_Interpreter& constr_inter,
                            TAO_Preference_Interpreter& pref_inter,
                            TAO_Offer_Filter& offer_filter);

  /// Check if offers of a type fit the constraints and order them
  /// according to the preferences submitted.
  void lookup_one_type (const char* type,
                        TAO_Offer_Database<MAP_LOCK_TYPE>& offer_database,
                        TAO_Constraint_Interpreter& constr_inter,
                        TAO_Preference_Interpreter& pref_inter,
                        TAO_Offer_Filter& offer_filter);

  /**
   * This method takes the list of ordered offers and places a number
   * of them in the sequence of returned offers and the rest into thr
   * iterator. In addition, fill_receptacles uses the
   * TAO_Property_Filter to ensure the returned offers contain the
   * properties specified in the desired_props in parameter.
   */
  int fill_receptacles (const char *,
                        CORBA::ULong how_many,
                        const CosTrading::Lookup::SpecifiedProps& desired_props,
                        TAO_Policies& policies,
                        TAO_Preference_Interpreter& pref_inter,
                        CosTrading::OfferSeq& offers,
                        CosTrading::OfferIterator_ptr& offer_itr);

  /// If a starting_trader policy was specfied, foward the query to the
  /// next link in the sequence.
  void forward_query (const char* next_hop,
                      const char *type,
                      const char *constr,
                      const char *pref,
                      const CosTrading::PolicySeq& policy_seq,
                      const CosTrading::Lookup::SpecifiedProps& desired_props,
                      CORBA::ULong how_many,
                      CosTrading::OfferSeq_out offers,
                      CosTrading::OfferIterator_out offer_itr,
                      CosTrading::PolicyNameSeq_out limits_applied);

  /**
   * Assemble a sequence of links that the federate_query method
   * should follow. Use the starting_trader policy, if one's provided,
   * otherwise use the Link interface to determine which of the
   * registered links should be followed in this query.
   */
  CORBA::Boolean retrieve_links (TAO_Policies& policies,
                                 CORBA::ULong offer_returned,
                                 CosTrading::LinkNameSeq_out links);

  /**
   * Perform and pass on a query over a set of links. Merge the
   * results of the federated queries into a single set of results
   * suitable for returning to the user.
   */
  void federated_query (const CosTrading::LinkNameSeq& links,
                        const TAO_Policies& policies,
                        const CosTrading::Admin::OctetSeq& request_id,
                        TAO_Preference_Interpreter& pref_inter,
                        const char *type,
                        const char *constr,
                        const char *pref,
                        const CosTrading::Lookup::SpecifiedProps& desired_props,
                        CORBA::ULong how_many,
                        CosTrading::OfferSeq& offers,
                        CosTrading::OfferIterator_ptr& offer_itr,
                        CosTrading::PolicyNameSeq& limits_applied);

  /// Merge the results from a federated query into the collected results.
  void order_merged_sequence (TAO_Preference_Interpreter& pref_inter,
                              CosTrading::OfferSeq& offers);

  CORBA::Boolean seen_request_id (TAO_Policies& policies,
                                  CosTrading::Admin::OctetSeq*& seq);

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Lookup (const TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))

  const unsigned int IDS_SAVED;

  /// A reference to the trader for obtaining offer maps.
  TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader_;

  typedef ACE_Unbounded_Queue<CosTrading::Admin::OctetSeq*> Request_Ids;

  /// A list of recent request_id_stems
  Request_Ids request_ids_;

  /// Lock to secure the set of request ids.
  TRADER_LOCK_TYPE lock_;
};

/**
 * @class TAO_Register
 *
 * @brief This class implements CosTrading::Register IDL interface.
 */
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Register :
  public TAO_Trader_Components<POA_CosTrading::Register>,
  public TAO_Support_Attributes<POA_CosTrading::Register>
{
public:

  TAO_Register (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader);

  virtual ~TAO_Register (void);

  virtual CosTrading::OfferId _cxx_export (CORBA::Object_ptr reference,
                                           const char *type,
                                           const CosTrading::PropertySeq& properties);

  // BEGIN SPEC
  // The export operation is the means by which a service is
  // advertised, via a trader, to a community of potential
  // importers. The OfferId returned is the handle with which the
  // exporter can identify the exported offer when attempting to
  // access it via other operations. The OfferId is only meaningful in
  // the context of the trader that generated it.

  //   The "reference" parameter is the information that enables a client
  // to interact with a remote server. If a trader implementation chooses
  // to consider certain types of object references (e.g., a nil object
  // reference) to be unexportable, then it may return the InvalidObjectRef
  // exception in such cases.

  // The "type" parameter
  // identifies the service type, which contains the interface type of
  // the "reference" and a set of named property types that may be
  // used in further describing this offer (i.e., it restricts what is
  // acceptable in the properties parameter).

  // If the string
  // representation of the "type" does not obey the rules for
  // identifiers, then an IllegalServiceType exception is raised. If
  // the "type" is correct syntactically but a trader is able to
  // unambiguously determine that it is not a recognized service type,
  // then an UnknownServiceType exception is raised.

  // If the trader
  // can determine that the interface type of the "reference"
  // parameter is not a subtype of the interface type specified in
  // "type," then an InterfaceTypeMismatch exception is raised. The
  // "properties" parameter is a list of named values that conform to
  // the property value types defined for those names. They describe
  // the service being offered. This description typically covers
  // behavioral, non-functional, and non-computational aspects of the
  // service.

  // If any of the property names do not obey the syntax
  // rules for PropertyNames, then an IllegalPropertyName exception is
  // raised.

  // If the type of any of the property values is not the
  // same as the declared type (declared in the service type), then a
  // PropertyTypeMismatch exception is raised.

  // If an attempt is made to assign a dynamic property value to a
  // readonly property, then the ReadonlyDynamicProperty exception is
  // raised. If the "properties" parameter omits any property
  // declared in the service type with a mode of mandatory, then a
  // MissingMandatoryProperty exception is raised. If two or more
  // properties with the same property name are included in this
  // parameter, the DuplicatePropertyName exception is raised.
  // END SPEC

  virtual void withdraw (const char *id);

  // BEGIN SPEC
  // The withdraw operation removes the service offer from the trader
  // (i.e., after withdraw the offer can no longer be returned as the
  // result of a query). The offer is identified by the "id" parameter
  // which was originally returned by export. If the string
  // representation of "id" does not obey the rules for offer
  // identifiers, then an IllegalOfferId exception is raised. If the
  // "id" is legal but there is no offer within the trader with that
  // "id," then an UnknownOfferId exception is raised. If the "id"
  // identifies a proxy offer rather than an ordinary offer, then a
  // ProxyOfferId exception is raised.
  // END SPEC

  virtual CosTrading::Register::OfferInfo*
    describe (const char * id);

  // BEGIN SPEC
  // The describe operation returns the information about an offered
  // service that is held by the trader. It comprises the "reference"
  // of the offered service, the "type" of the service offer, and the
  // "properties" that describe this offer of service. The offer is
  // identified by the "id" parameter which was originally returned by
  // export. If the string representation of "id" does not obey the
  // rules for object identifiers, then an IllegalOfferId exception is
  // raised. If the "id" is legal but there is no offer within the
  // trader with that "id," then an UnknownOfferId exception is
  // raised. If the "id" identifies a proxy offer rather than an
  // ordinary offer, then a ProxyOfferId exception is raised.
  // END SPEC

  virtual void modify (const char * id,
                       const CosTrading::PropertyNameSeq& del_list,
                       const CosTrading::PropertySeq& modify_list);

  // BEGIN SPEC
  // The modify operation is used to change the description of a
  // service as held within a service offer. The object reference and
  // the service type associated with the offer cannot be
  // changed. This operation may: add new (non-mandatory) properties
  // to describe an offer, change the values of some existing (not
  // readonly) properties, or delete existing (neither mandatory nor
  // readonly) properties.

  // The modify operation either succeeds completely or it fails
  // completely. The offer is identified by the "id" parameter which
  // was originally returned by export. If the string representation
  // of "id" does not obey the rules for offer identifiers, then an
  // IllegalOfferId exception is raised. If the "id" is legal but
  // there is no offer within the trader with that "id," then an
  // UnknownOfferId exception is raised. If the "id" identifies a
  // proxy offer rather than an ordinary offer, then a ProxyOfferId
  // exception is raised.

  // The "del_list" parameter gives the names of the properties that
  // are no longer to be recorded for the identified offer. Future
  // query and describe operations will not see these properties. If
  // any of the names within the "del_list" do not obey the rules for
  // PropertyName's, then an IllegalPropertyName exception is
  // raised. If a "name" is legal but there is no property for the
  // offer with that "name," then an UnknownPropertyName exception is
  // raised. If the list includes a property that has a mandatory
  // mode, then the MandatoryProperty exception is raised. If the
  // same property name is included two or more times in this
  // parameter, the DuplicatePropertyName exception is raised.

  // The "modify_list" parameter gives the names and values of
  // properties to be changed. If the property is not in the offer,
  // then the modify operation adds it. The modified (or added)
  // property values are returned in future query and describe
  // operations in place of the original values. If any of the names
  // within the "modify_list" do not obey the rules for
  // PropertyName's, then an IllegalPropertyName exception is
  // raised. If the list includes a property that has a readonly
  // mode, then the ReadonlyProperty exception is raised unless that
  // readonly property is not currently recorded for the offer. The
  // ReadonlyDynamicProperty exception is raised if an attempt is made
  // to assign a dynamic property value to a readonly property. If
  // the value of any modified property is of a type that is not the
  // same as the type expected, then the PropertyTypeMismatch
  // exception is raised. If two or more properties with the same
  // property name are included in this argument, the
  // DuplicatePropertyName exception is raised.

  // The NotImplemented exception shall be raised if and only if the
  // supports_modifiable_properties attribute yields FALSE.
  // END SPEC

  virtual void withdraw_using_constraint (const char *type,
                                          const char *constr);

  // BEGIN SPEC
  // The withdraw_using_constraint operation withdraws a set of offers
  // from within a single trader. This set is identified in the same
  // way that a query operation identifies a set of offers to be
  // returned to an importer.

  // The "type" parameter conveys the required service type. Each
  // offer of the specified type will have the constraint expression
  // applied to it. If it matches the constraint expression, then the
  // offer will be withdrawn. If "type" does not obey the rules for
  // service types, then an IllegalServiceType exception is raised.
  // If the "type" is correct syntactically but is not recognized as a
  // service type by the trader, then an UnknownServiceType exception
  // is raised.

  // The constraint "constr" is the means by which the client
  // restricts the set of offers to those that are intended for
  // withdrawal. If "constr" does not obey the syntax rules for a
  // constraint then an IllegalConstraint exception is raised. If
  // the constraint fails to match with any offer of the specified
  // service type, then a NoMatchingOffers exception is raised.
  // END SPEC

  virtual CosTrading::Register_ptr
    resolve (const CosTrading::TraderName &name);

  // BEGIN SPEC
  // This operation is used to resolve a context relative name for
  // another trader. In particular, it is used when exporting to a
  // trader that is known by a name rather than by an object
  // reference. The client provides the name, which will be a sequence
  // of name components. If the content of the parameter cannot
  // yield legal syntax for the first component, then the
  // IllegalTraderName exception is raised. Otherwise, the first name
  // component is compared against the name held in each link. If no
  // match is found, or the trader does not support links, the
  // UnknownTraderName exception is raised. Otherwise, the trader
  // obtains the register_if held as part of the matched link. If
  // the Register interface is not nil, then the trader binds to the
  // Register interface and invokes resolve but passes the TraderName
  // with the first component removed; if it is nil, then the
  // RegisterNotSupported exception is raised. When a trader is able
  // to match the first name component leaving no residual name, that
  // trader returns the reference for the Register interface for that
  // linked trader. In unwinding the recursion, intermediate traders
  // return the Register interface reference to their client (another
  // trader).
  // END SPEC

 protected:

  void validate_properties (const char* type,
                            const CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
                            const CosTrading::PropertySeq& properties);

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Register (const TAO_Register<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))

 private:

  TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader_;
};

/**
 * @class TAO_Admin
 *
 * @brief This class implements CosTrading::Admin IDL interface.
 *
 * DESCRIPTION (FROM SPEC)
 * The admin interface enables the values of the trader attributes to
 * be read and written. All attributes are defined as readonly in
 * either SupportAttributes, ImportAttributes, LinkAttributes, or
 * Admin. To set the trader "attribute" to a new value,
 * set_<attribute_name> operations are defined in Admin. Each of these
 * set operations returns the previous value of the attribute as its
 * function value. If the admin interface operation
 * set_support_proxy_offers is invoked with a value set to FALSE in a
 * trader which supports the proxy interface, the
 * set_support_proxy_offer value does not affect the function of
 * operations in the proxy interface. However, in this case, it does
 * have the effect of making any proxy offers exported via the proxy
 * interface for that trader unavailable to satisfy queries on that
 * trader's lookup interface.
 */
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Admin :
  public TAO_Trader_Components <POA_CosTrading::Admin>,
  public TAO_Support_Attributes <POA_CosTrading::Admin>,
  public TAO_Import_Attributes <POA_CosTrading::Admin>,
  public TAO_Link_Attributes <POA_CosTrading::Admin>
{
public:

  TAO_Admin (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader);

  ~TAO_Admin (void);

  // = Importing Parameters (used by the Lookup Interface)

  /// Search card is the cardinality of the offers searched for
  /// constraint compliance.
  virtual CORBA::ULong set_def_search_card (CORBA::ULong value);
  virtual CORBA::ULong set_max_search_card (CORBA::ULong value);


  /// Match card is the cardinality of offers found compliant with the
  /// constraints.
  virtual CORBA::ULong set_def_match_card (CORBA::ULong value);
  virtual CORBA::ULong set_max_match_card (CORBA::ULong value);

  /// Return card is the cardinality of the offers returned from
  /// Lookup.
  virtual CORBA::ULong set_def_return_card (CORBA::ULong value);
  virtual CORBA::ULong set_max_return_card (CORBA::ULong value);

  /// Types of offers available for consideration. Ween out those
  /// offers with modifiable properties
  virtual CORBA::ULong set_max_list (CORBA::ULong value);
  virtual CORBA::Boolean
    set_supports_modifiable_properties (CORBA::Boolean value);
  virtual CORBA::Boolean
    set_supports_dynamic_properties (CORBA::Boolean value);
  virtual CORBA::Boolean
    set_supports_proxy_offers (CORBA::Boolean value);

  // = Link Interface parameters

  virtual CORBA::ULong set_def_hop_count (CORBA::ULong value);
  virtual CORBA::ULong set_max_hop_count (CORBA::ULong value);

  virtual CosTrading::FollowOption
    set_def_follow_policy (CosTrading::FollowOption policy);
  virtual CosTrading::FollowOption
    set_max_follow_policy (CosTrading::FollowOption policy);
  virtual CosTrading::FollowOption
    set_max_link_follow_policy (CosTrading::FollowOption policy);

  // = Set Type Repository

  virtual CosTrading::TypeRepository_ptr
    set_type_repos (CosTrading::TypeRepository_ptr repository);

  virtual CosTrading::Admin::OctetSeq*
    request_id_stem (void);

  virtual CosTrading::Admin::OctetSeq*
    set_request_id_stem (const CosTrading::Admin::OctetSeq& stem);

  virtual void list_offers (CORBA::ULong how_many,
                            CosTrading::OfferIdSeq_out ids,
                            CosTrading::OfferIdIterator_out id_itr);


  // BEGIN SPEC
  // The list_offers operation allows the administrator of a trader to
  // perform housekeeping by obtaining a handle on each of the offers
  // within a trader (e.g., for garbage collection etc.). Only the
  // identifiers of ordinary offers are returned, identifiers of proxy
  // offers are not returned via this operation. If the trader does
  // not support the Register interface, the NotImplemented exception
  // is raised. The returned identifiers are passed back in one of two
  // ways (or a combination of both). The "ids" return result
  // conveys a list of offer identifiers and the "id_itr" is a
  // reference to an interface at which additional offer identities
  // can be obtained. The "how_many" parameter states how many
  // identifiers are to be returned via the "ids" result; any
  // remaining are available via the iterator interface. If the
  // "how_many" exceeds the number of offers held in the trader, then
  // the "id_itr" is nil.
  // END SPEC

  virtual void list_proxies (CORBA::ULong,
                             CosTrading::OfferIdSeq_out,
                             CosTrading::OfferIdIterator_out);

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Admin (const TAO_Admin<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))

  TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader_;

  /// Unique prefix to create a sequence number space.
  CosTrading::Admin::OctetSeq stem_id_;

  /// Current sequence number.
  CORBA::ULong sequence_number_;

  TRADER_LOCK_TYPE lock_;
};


/**
 * @class TAO_Link
 */
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Link :
  public TAO_Trader_Components <POA_CosTrading::Link>,
  public TAO_Support_Attributes <POA_CosTrading::Link>,
  public TAO_Link_Attributes <POA_CosTrading::Link>
{
public:

  TAO_Link (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader);

  ~TAO_Link (void);

  /**
   * BEGIN SPEC
   * The add_link operation allows a trader subsequently to use the
   * service of another trader in the performance of its own trading
   * service operations.
   */
  virtual void add_link (const char *name,
                         CosTrading::Lookup_ptr target,
                         CosTrading::FollowOption def_pass_on_follow_rule,
                         CosTrading::FollowOption limiting_follow_rule);

  // The "name" parameter is used in subsequent link management
  // operations to identify the intended link. If the parameter is not
  // legally formed, then the IllegalLinkName exception is raised. An
  // exception of DuplicateLinkName is raised if the link name already
  // exists. The link name is also used as a component in a sequence
  // of name components in naming a trader for resolving or forwarding
  // operations. The sequence of context relative link names provides
  // a path to a trader.

  // The "target" parameter identifies the Lookup interface at which
  // the trading service provided by the target trader can be
  // accessed. Should the Lookup interface parameter be nil, then an
  // exception of InvalidLookupRef is raised. The target interface is
  // used to obtain the associated Register interface, which will be
  // subsequently returned as part of a describe_link operation and
  // invoked as part of a resolve operation.

  // The "def_pass_on_follow_rule" parameter specifies the default
  // link behavior for the link if no link behavior is specified on an
  // importer's query request. If the "def_pass_on_follow_rule"
  // exceeds the "limiting_follow_rule" specified in the next
  // parameter, then a DefaultFollowTooPermissive exception is
  // raised.

  // The "limiting_follow_rule" parameter specifies the most
  // permissive link follow behavior that the link is willing to
  // tolerate. The exception LimitingFollowTooPermissive is raised if
  // this parameter exceeds the trader's attribute of
  // "max_link_follow_policy" at the time of the link's creation. Note
  // it is possible for a link's "limiting_follow_rule" to exceed the
  // trader's "max_link_follow_policy" later in the life of a link, as
  // it is possible that the trader could set its
  // "max_link_follow_policy" to a more restrictive value after the
  // creation of the link.
  // END SPEC

  virtual void remove_link (const char *name);

  // BEGIN SPEC
  // The remove_link operation removes all knowledge of the target
  // trader. The target trader cannot be used subsequently to resolve,
  // forward, or propagate trading operations from this trader.

  // The "name" parameter identifies the link to be removed. The
  // exception IllegalLinkName is raised if the link is formed poorly
  // and the UnknownLinkName exception is raised if the named link is
  // not in the trader.
  // END SPEC

  virtual CosTrading::Link::LinkInfo* describe_link (const char *name);

  // BEGIN SPEC
  // The describe_link operation returns information on a link held in
  // the trader.

  // The "name" parameter identifies the link whose description is
  // required. For a malformed link name, the exception
  // IllegalLinkName is raised. An UnknownLinkName exception is raised
  // if the named link is not found in the trader.

  // The operation returns a LinkInfo structure comprising: the
  // Lookup interface of the target trading service, the Register
  // interface of the target trading service, and the default, as
  // well as the limiting follow behavior of the named link.

  // If the target service does not support the Register interface,
  // then that field of the LinkInfo structure is nil. Given the
  // description of the Register::resolve() operation in "Resolve
  // Operation" on page 16-45, most implementations will opt for
  // determining the Register interface when add_link is called and
  // storing that information statically with the rest of the link
  // state.
  // END SPEC

  virtual CosTrading::LinkNameSeq* list_links (void);

  // BEGIN SPEC
  // The list_links operation returns a list of the names of all
  // trading links within the trader. The names can be used
  // subsequently for other management operations, such as
  // describe_link or remove_link.
  // END SPEC

  virtual void modify_link (const char *name,
                            CosTrading::FollowOption def_pass_on_follow_rule,
                            CosTrading::FollowOption limiting_follow_rule);

  // BEGIN SPEC
  // The modify_link operation is used to change the existing link
  // follow behaviors of an identified link. The Lookup interface
  // reference of the target trader and the name of the link cannot be
  // changed.

  // The "name" parameter identifies the link whose follow behaviors
  // are to be changed. A poorly formed "name" raises the
  // IllegalLinkName exception. An UnknownLinkName exception is raised
  // if the link name is not known to the trader.

  // The "def_pass_on_follow_rule" parameter specifies the new default
  // link behavior for this link. If the "def_pass_on_follow_rule"
  // exceeds the "limiting_follow_rule" specified in the next
  // parameter, then a DefaultFollowTooPermissive exception is
  // raised.

  // The "limiting_follow_rule" parameter specifies the new limit for
  // the follow behavior of this link. The exception
  // LimitingFollowTooPermissive is raised if the value exceeds the
  // current "max_link_follow_policy" of the trader.
  // END SPEC

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Link (const TAO_Link<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))

  typedef ACE_Hash_Map_Manager_Ex
  <
  CORBA::String_var,
  CosTrading::Link::LinkInfo,
  ACE_Hash<CORBA::String_var>,
  ACE_Equal_To<CORBA::String_var>,
  MAP_LOCK_TYPE
  >
  Links;

  /// The collection of link connecting this trader to others in the
  /// federation.
  Links links_;

  TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader_;
};

  // *************************************************************
  // TAO_Proxy
  // *************************************************************

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Proxy :
  public TAO_Trader_Components <POA_CosTrading::Proxy>,
  public TAO_Support_Attributes <POA_CosTrading::Proxy>
{
public:

  TAO_Proxy (TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &trader);

  ~TAO_Proxy (void);

  virtual CosTrading::OfferId
    export_proxy (CosTrading::Lookup_ptr,
                  const char *,
                  const CosTrading::PropertySeq &,
                  CORBA::Boolean if_match_all,
                  const char *,
                  const CosTrading::PolicySeq &);

  virtual void withdraw_proxy (const char *);

  virtual CosTrading::Proxy::ProxyInfo *
  describe_proxy (const char *);

   // = CosTrading::TraderComponents methods.

  virtual void list_proxies (CORBA::ULong,
                             CosTrading::OfferIdSeq *&,
                             CosTrading::OfferIdIterator_ptr &);

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Proxy (const TAO_Proxy<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> &))


  TAO_Trader<TRADER_LOCK_TYPE,MAP_LOCK_TYPE>& trader_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Trader/Trader_Interfaces.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TRADER_INTERFACES_H */
