/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Trader.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
//   
// ========================================================================

#ifndef TAO_TRADER_H
#define TAO_TRADER_H

#include "Trader.h"
#include "Offer_Database.h"

  // *************************************************************
  // TAO_Trader
  // *************************************************************

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
class TAO_Trader : public TAO_Trader_Base
  //
  // = TITLE
  //     This class packages together all the various pieces that
  //     provide functionality specified in COS Trading specification.
  //
  // = DESCRIPTION
  //     TAO_Trader contains all the components that together represent
  //     a single trader.  Based on its constructor arguments, 
  //     TAO_Trader creates instances of appropriate interface 
  //     implementations as well as instances of objects common to
  //     more than one interface (offers, attributes, etc.).
  //     TAO_Trader also enforces the proper order on all
  //     initializations.  TAO_Trader acts like a "glue" class that
  //     creates appropriate components, holds everything together,
  //     and enforces order. TAO_Trader is parameterized by two types
  //     of locks: one for its service service offers, one for its
  //     state (configuration). 
{    
public:

  // The desired combination of interfaces to be passed to the 
  // TAO_Trader constructor.

  typedef TAO_Offer_Database<MAP_LOCK_TYPE> Offer_Database;
  // Offer Database Trait.

  TAO_Trader (Trader_Components components = LOOKUP);
  // Constructor which based on its arguments will create 
  // a particular type of trader (e.g. Query trader, Simple trader, etc.)
  // The argument is a bitwise OR of desired Trader_Components as listed
  // in enumerated type above.

  virtual ~TAO_Trader (void);
  // destructor.
  
  Offer_Database& offer_database (void);
  // Accessor for the structure with all the service offers.

  ACE_Lock &lock (void);
  // returns the trader
  
protected: 
  
  typedef TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> TRADER_SELF;
  
  Offer_Database offer_database_; 

  ACE_Lock_Adapter<TRADER_LOCK_TYPE> lock_;
  // lock that guards the state of the trader (its configuration).

  enum { LOOKUP_IF, REGISTER_IF, ADMIN_IF, PROXY_IF, LINK_IF };
  
  PortableServer::ServantBase* ifs_[5];
  
private:
  
  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> &))
};

  // *************************************************************
  // Template Attribute Classes
  // *************************************************************

template <class IF>
class TAO_Trader_Components : public virtual IF
{
public:

  TAO_Trader_Components (const TAO_Trading_Components_Impl& comps);
  
  // = CosTrading::TraderComponents methods. 
  virtual CosTrading::Lookup_ptr lookup_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns an object reference to the Lookup interface of the trader.
  // Returns nil if the trader does not support Lookup interface.
  
  virtual CosTrading::Register_ptr register_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns object reference for the Register interface of the trader.
  // Returns nil if the trader does not support Register interface.
  
  virtual CosTrading::Link_ptr link_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException)); 
  // Returns object reference for the Link interface of the trader.
  // Returns nil if the trader does not support Link interface.

  virtual CosTrading::Proxy_ptr proxy_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns object reference to the Proxy interface of the trader.
  // Returns nil if the trader does not support Proxy interface. 
   
  virtual CosTrading::Admin_ptr admin_if (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Returns object reference for the Admin interface of the trader.
  // Returns nil if the trader does not support Admin interface.

private:
  
  const TAO_Trading_Components_Impl& comps_;
};

template <class IF>
class TAO_Support_Attributes : public virtual IF
{
public:

  TAO_Support_Attributes (const TAO_Support_Attributes_Impl& attrs);

  // = CosTrading::SupportAttributes methods.
  
  virtual CORBA::Boolean supports_modifiable_properties (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Boolean supports_dynamic_properties (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Boolean supports_proxy_offers (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
  virtual CosTrading::TypeRepository_ptr type_repos (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  
private:

  const TAO_Support_Attributes_Impl& attrs_;
};

template <class IF>
class TAO_Import_Attributes : public virtual IF
{
public:

  TAO_Import_Attributes (const TAO_Import_Attributes_Impl& attrs);

  // = CosTrading::ImportAttributes methods.
  
  virtual CORBA::ULong def_search_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_search_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // Search cardinality determines the maximum number of offers searched
  // before not considering other offers. 
  
  virtual CORBA::ULong def_match_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_match_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // Match cardinality determines the maximum number of offers
  // matched to the constraints before not considering other offers..
  
  virtual CORBA::ULong def_return_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_return_card (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // Return cardinality determines the maximum number of offers marked 
  // to return before not considering other offers.

  
  virtual CORBA::ULong max_list (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong def_hop_count (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong max_hop_count (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CosTrading::FollowOption def_follow_policy (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  virtual CosTrading::FollowOption max_follow_policy (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  
private:

  const TAO_Import_Attributes_Impl& attrs_;
};

template <class IF>
class TAO_Link_Attributes : public virtual IF
{
public:

  TAO_Link_Attributes (const TAO_Link_Attributes_Impl& attrs);

  // = CosTrading::LinkAttributes methods
  virtual CosTrading::FollowOption max_link_follow_policy (CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
    
private:

  const TAO_Link_Attributes_Impl& attrs_;
};

  // *************************************************************
  // TAO_Sequence_Extracter
  // *************************************************************

template <class SEQ_TYPE>
class TAO_Sequence_Extracter : private TAO_Sequence_Extracter_Base
// = TITLE
//  Happy hack to extract sequence data out of user defined sequence
//  that may have x number of typedef aliases. 
{
 public:

  TAO_Sequence_Extracter (CORBA::TypeCode* type_code)
    : typecode_ (CORBA::TypeCode::_duplicate (type_code)) {}
  
  CORBA::Boolean extract (const CORBA::Any&, SEQ_TYPE*&);
  // Extract the underlying sequence value into a newly allocated
  // sequence of type SEQ_TYPE. The any assumes ownership of the
  // sequence, so don't release it.
  
 private:
  
  CORBA::TypeCode_var typecode_;
};


template <class SEQ, class OPERAND_TYPE>
CORBA::Boolean TAO_find (const SEQ& sequence, const OPERAND_TYPE operand);

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Trader_T.cpp"
#endif  /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_TRADER_H */
	
