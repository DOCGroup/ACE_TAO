// -*- C++ -*-

//=============================================================================
/**
 * @file Trader_T.h
 *
 * @author Marina Spivak <marina@cs.wustl.edu>
 * @author Seth Widoff <sbw1@cs.wustl.edu>
 * @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TRADER_H
#define TAO_TRADER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Offer_Database.h"
#include "ace/Containers.h"
#include "ace/Lock_Adapter_T.h"

// *************************************************************
// TAO_Trader
// *************************************************************

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DynSequence_i;

/**
 * @class TAO_Trader
 *
 * @brief This class packages together all the various pieces that
 * provide functionality specified in COS Trading specification.
 *
 * TAO_Trader contains all the components that together represent
 * a single trader.  Based on its constructor arguments,
 * TAO_Trader creates instances of appropriate interface
 * implementations as well as instances of objects common to
 * more than one interface (offers, attributes, etc.).
 * TAO_Trader also enforces the proper order on all
 * initializations.  TAO_Trader acts like a "glue" class that
 * creates appropriate components, holds everything together,
 * and enforces order. TAO_Trader is parameterized by two types
 * of locks: one for its service service offers, one for its
 * state (configuration).
 */
template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Trader : public TAO_Trader_Base
{
public:
  // The desired combination of interfaces to be passed to the
  // TAO_Trader constructor.

  /// Offer Database Trait.
  typedef TAO_Offer_Database<MAP_LOCK_TYPE> Offer_Database;

  /**
   * Constructor which based on its arguments will create
   * a particular type of trader (e.g. Query trader, Simple trader, etc.)
   * The argument is a bitwise OR of desired Trader_Components as listed
   * in enumerated type above.
   */
  TAO_Trader (Trader_Components components = LOOKUP);

  /// Destructor.
  ~TAO_Trader () override;

  /// Accessor for the structure with all the service offers.
  Offer_Database& offer_database ();

  /// Returns the trader
  ACE_Lock &lock () override;

protected:
  typedef TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> TRADER_SELF;

  Offer_Database offer_database_;

  /// Lock that guards the state of the trader (its configuration).
  ACE_Lock_Adapter<TRADER_LOCK_TYPE> lock_;

  enum { LOOKUP_IF, REGISTER_IF, ADMIN_IF, PROXY_IF, LINK_IF };

  PortableServer::ServantBase* ifs_[5];

private:
  // = Disallow these operations.
  void operator= (const TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> &) = delete;
};

  // *************************************************************
  // Template Attribute Classes
  // *************************************************************

template <class IF>
class TAO_Trader_Components :
  public virtual IF
{
public:
  TAO_Trader_Components (const TAO_Trading_Components_i& comps);

  // = CosTrading::TraderComponents methods.
  /// Returns an object reference to the Lookup interface of the trader.
  /// Returns nil if the trader does not support Lookup interface.
  CosTrading::Lookup_ptr lookup_if () override;

  /// Returns object reference for the Register interface of the trader.
  /// Returns nil if the trader does not support Register interface.
  CosTrading::Register_ptr register_if () override;

  /// Returns object reference for the Link interface of the trader.
  /// Returns nil if the trader does not support Link interface.
  CosTrading::Link_ptr link_if () override;

  /// Returns object reference to the Proxy interface of the trader.
  /// Returns nil if the trader does not support Proxy interface.
  CosTrading::Proxy_ptr proxy_if () override;

  /// Returns object reference for the Admin interface of the trader.
  /// Returns nil if the trader does not support Admin interface.
  CosTrading::Admin_ptr admin_if () override;

private:
  const TAO_Trading_Components_i& comps_;
};

template <class IF>
class TAO_Support_Attributes : public virtual IF
{
public:
  TAO_Support_Attributes (const TAO_Support_Attributes_i& attrs);

  // = CosTrading::SupportAttributes methods.

  CORBA::Boolean supports_modifiable_properties () override;

  CORBA::Boolean supports_dynamic_properties () override;

  CORBA::Boolean supports_proxy_offers () override;

  CosTrading::TypeRepository_ptr type_repos () override;

private:
  const TAO_Support_Attributes_i& attrs_;
};

template <class IF>
class TAO_Import_Attributes : public virtual IF
{
public:
  TAO_Import_Attributes (const TAO_Import_Attributes_i& attrs);

  // = CosTrading::ImportAttributes methods.

  CORBA::ULong def_search_card () override;

  CORBA::ULong max_search_card () override;

  // Search cardinality determines the maximum number of offers searched
  // before not considering other offers.

  CORBA::ULong def_match_card () override;

  CORBA::ULong max_match_card () override;

  // Match cardinality determines the maximum number of offers
  // matched to the constraints before not considering other offers..

  CORBA::ULong def_return_card () override;

  CORBA::ULong max_return_card () override;

  // Return cardinality determines the maximum number of offers marked
  // to return before not considering other offers.


  CORBA::ULong max_list () override;

  CORBA::ULong def_hop_count () override;

  CORBA::ULong max_hop_count () override;

  CosTrading::FollowOption def_follow_policy () override;

  CosTrading::FollowOption max_follow_policy () override;

private:
  const TAO_Import_Attributes_i& attrs_;
};

template <class IF>
class TAO_Link_Attributes : public virtual IF
{
public:
  TAO_Link_Attributes (const TAO_Link_Attributes_i& attrs);

  // = CosTrading::LinkAttributes methods
  CosTrading::FollowOption max_link_follow_policy () override;

private:
  const TAO_Link_Attributes_i& attrs_;
};

  // *************************************************************
  // TAO_Sequence_Extracter
  // *************************************************************

/**
 * @class TAO_Element_Equal
 *
 * @brief Function object for determining if the sequence element at the
 * current position of the dynamic sequence any parameter is equal to
 * the element parameter.
 */
template <class ELEMENT_TYPE>
class TAO_Element_Equal
{
public:
  /// Calls the correct method on dyn_seq to extract the element type, then
  /// uses the appropriate form of equals comparison.
  int operator () (TAO_DynSequence_i& dyn_any,
                   const ELEMENT_TYPE& element);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Trader/Trader_T.cpp"
#endif  /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ACE_TRADER_H */
