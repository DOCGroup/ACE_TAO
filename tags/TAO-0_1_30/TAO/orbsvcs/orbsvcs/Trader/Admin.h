/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Admin.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_ADMIN_H
#define TAO_ADMIN_H

#include "Attributes_T.h"

template <class TRADER>
class TAO_Admin : 
  public TAO_Trader_Components <POA_CosTrading::Admin>,
  public TAO_Support_Attributes <POA_CosTrading::Admin>,
  public TAO_Import_Attributes <POA_CosTrading::Admin>,
  public TAO_Link_Attributes <POA_CosTrading::Admin>
//
// = TITLE
//     This class implements CosTrading::Admin IDL interface.
// 
// = DESCRIPTION (FROM SPEC)
// 
// The admin interface enables the values of the trader attributes to
// be read and written. All attributes are defined as readonly in
// either SupportAttributes, ImportAttributes, LinkAttributes, or
// Admin. To set the trader "attribute" to a new value,
// set_<attribute_name> operations are defined in Admin. Each of these
// set operations returns the previous value of the attribute as its
// function value. If the admin interface operation
// set_support_proxy_offers is invoked with a value set to FALSE in a
// trader which supports the proxy interface, the
// set_support_proxy_offer value does not affect the function of
// operations in the proxy interface. However, in this case, it does
// have the effect of making any proxy offers exported via the proxy
// interface for that trader unavailable to satisfy queries on that
// trader's lookup interface. 
{
public:

  TAO_Admin (TRADER &trader);

  ~TAO_Admin (void);

  // = Importing Parameters (used by the Lookup Interface)

  virtual CORBA::ULong set_def_search_card (CORBA::ULong value,
					    CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::ULong set_max_search_card (CORBA::ULong value,
					    CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // search card is the cardinality of the offers searched for
  // constraint compliance. 


  virtual CORBA::ULong set_def_match_card (CORBA::ULong value,
					    CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::ULong set_max_match_card (CORBA::ULong value,
					    CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // match card is the cardinality of offers found compliant with the
  // constraints. 

  virtual CORBA::ULong set_def_return_card (CORBA::ULong value,
					    CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::ULong set_max_return_card (CORBA::ULong value,
					    CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // return card is the cardinality of the offers returned from
  // Lookup.

  virtual CORBA::ULong set_max_list (CORBA::ULong value,
				     CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Boolean
    set_supports_modifiable_properties (CORBA::Boolean value,
					CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Boolean
    set_supports_dynamic_properties (CORBA::Boolean value,
				     CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Boolean
    set_supports_proxy_offers (CORBA::Boolean value,
			       CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Types of offers available for consideration. Ween out those
  // offers with modifiable properties
  
  // = Link Interface parameters

  virtual CORBA::ULong set_def_hop_count (CORBA::ULong value,
					  CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::ULong set_max_hop_count (CORBA::ULong value,
					  CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CosTrading::FollowOption
    set_def_follow_policy (CosTrading::FollowOption policy,
			   CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CosTrading::FollowOption
    set_max_follow_policy (CosTrading::FollowOption policy,
			   CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  virtual CosTrading::FollowOption
    set_max_link_follow_policy (CosTrading::FollowOption policy,
				CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // = Set Type Repository
  
  virtual CosTrading::TypeRepository_ptr
    set_type_repos (CosTrading::TypeRepository_ptr repository,
		    CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CosTrading::Admin::OctetSeq*
    request_id_stem (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CosTrading::Admin::OctetSeq*
    set_request_id_stem (const CosTrading::Admin::OctetSeq& stem,
			 CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual void list_offers (CORBA::ULong how_many,
			    CosTrading::OfferIdSeq_out ids,
			    CosTrading::OfferIdIterator_out id_itr,
			    CORBA::Environment& env) 
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::NotImplemented));


  // BEGIN SPEC
  // The list_offers operation allows the administrator of a trader to
  // perform housekeeping by obtaining a handle on each of the offers
  // within a trader (e.g., for garbage collection etc.). Only the
  // identifiers of ordinary offers are returned, identifiers of proxy
  // offers are not returned via this operation. If the trader does
  // not support the Register interface, the NotImplemented exception
  // is raised. The returned identifiers are passed back in one of two
  // ways (or a combination of both). ° The "ids" return result
  // conveys a list of offer identifiers and the "id_itr" is a
  // reference to an interface at which additional offer identities
  // can be obtained. ° The "how_many" parameter states how many
  // identifiers are to be returned via the "ids" result; any
  // remaining are available via the iterator interface. If the
  // "how_many" exceeds the number of offers held in the trader, then
  // the "id_itr" is nil. 
  // END SPEC

  virtual void list_proxies (CORBA::ULong how_many,
			     CosTrading::OfferIdSeq_out ids,
			     CosTrading::OfferIdIterator_out id_itr,
			     CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::NotImplemented));

  
  static const char* NAME;
  
private:

  TRADER &trader_;

  CosTrading::Admin::OctetSeq stem_id_;
  // Unique prefix to create a sequence number space.
  
  CORBA::ULong sequence_number_;
  // Current sequence number.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Admin.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_ADMIN_H */
