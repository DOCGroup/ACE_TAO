// -*- C++ -*-

//=============================================================================
/**
 * @file Trader.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@cs.wustl.edu>
 * @author Seth Widoff <sbw1@cs.wustl.edu>
 * @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TRADER_BASE_H
#define TAO_TRADER_BASE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Interpreter_Utils.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

#include "orbsvcs/CosTradingS.h"
#include "orbsvcs/CosTradingReposS.h"
#include "orbsvcs/CosTradingDynamicS.h"
#include "orbsvcs/Trader/trading_serv_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward Declaration.
class TAO_Trader_Base;

  // *************************************************************
  // TAO_Trader_Factory
  // *************************************************************

/**
 * @class TAO_Trader_Factory
 *
 * @brief Uses command line arguments to construct a trader instance with
 * the correct interface support, locking, and policy settings.
 */
class TAO_Trading_Serv_Export TAO_Trader_Factory
{
public:

  typedef TAO_Trader_Base TAO_TRADER;

  /**
   * Creates an instance of the trader according to parameters whose
   * default values can be overrided with the following command line
   * arguments:
   *
   * -TSthreadsafe, default is not thread-safe
   * -TSconformance {query, simple, stand-alone, linked}, default is linked
   * -TSsupports_dynamic_properties {true, false}, default is true
   * -TSsupports_modifiable_propertise {true, false}, default is true
   * -TSdef_search_card {integer}, default is 20
   * -TSmax_search_card {integer}, default is 50
   * -TSdef_match_card {integer}, default is 20
   * -TSmax_match_card {integer}, default is 50
   * -TSdef_return_card {integer}, default is 20
   * -TSmax_return_card {integer}, default is 50
   * -TSdef_hop_count {integer}, default is 5,
   * -TSmax_hop_count {integer}, default is 10
   * -TSdef_follow_policy {always,if_no_local,local_only}, default is if_no_local,
   * -TSmax_follow_policy {always,if_no_local,local_only}, default is always
   */
  static TAO_TRADER* create_trader (int& argc, ACE_TCHAR** argv);

protected:

  TAO_Trader_Factory (int& argc, ACE_TCHAR** argv);

private:

  enum Conformance { TAO_TRADER_QUERY,
                     TAO_TRADER_SIMPLE,
                     TAO_TRADER_STANDALONE,
                     TAO_TRADER_LINKED };

  TAO_TRADER* manufacture_trader (void);

  void parse_args (int& argc, ACE_TCHAR** argv);

  TAO_Trader_Factory& operator= (const TAO_Trader_Factory&);
  TAO_Trader_Factory (const TAO_Trader_Factory&);

  // = Command line configurable parameters.

  Conformance conformance_;
  CORBA::Boolean threadsafe_;
  CORBA::Boolean supports_dynamic_properties_;
  CORBA::Boolean supports_modifiable_properties_;
  CORBA::ULong def_search_card_;
  CORBA::ULong max_search_card_;
  CORBA::ULong def_match_card_;
  CORBA::ULong max_match_card_;
  CORBA::ULong def_return_card_;
  CORBA::ULong max_return_card_;
  CORBA::ULong def_hop_count_;
  CORBA::ULong max_hop_count_;
  CosTrading::FollowOption def_follow_policy_;
  CosTrading::FollowOption max_follow_policy_;
};

  // *************************************************************
  // Attribute Classes
  // *************************************************************

/**
 * @class TAO_Lockable
 *
 * Class used to remove the circular dependencies between the
 * Attribute classes and the Trader class.
 */
class TAO_Trading_Serv_Export TAO_Lockable
{
public:
  virtual ~TAO_Lockable (void);

  virtual ACE_Lock& lock (void) = 0;
};


/**
 * @class TAO_Support_Attributes_i
 *
 * @brief This class stores, allows access to and modification of
 * trader's support attributes.
 *
 * Contains a reference to the trader from which it is
 * used so it can use trader's lock.
 */
class TAO_Trading_Serv_Export TAO_Support_Attributes_i
{
public:
  // = Initialization and termination methods.
  TAO_Support_Attributes_i (TAO_Lockable &locker);
  ~TAO_Support_Attributes_i ();

  // = Accessor methods.
  CORBA::Boolean supports_modifiable_properties (void) const;
  void supports_modifiable_properties (CORBA::Boolean);

  CORBA::Boolean supports_dynamic_properties (void) const;
  void supports_dynamic_properties (CORBA::Boolean);

  CORBA::Boolean supports_proxy_offers (void) const;
  void supports_proxy_offers (CORBA::Boolean);

  CosTrading::TypeRepository_ptr type_repos (void) const;
  void type_repos (CosTrading::TypeRepository_ptr);

  CosTradingRepos::ServiceTypeRepository_ptr service_type_repos (void) const;

private:

  /// A reference to the trader (needed for obtaining the lock.)
  TAO_Lockable &locker_;

  /// Indicator of whether the trader supports property modification.
  CORBA::Boolean supports_modifiable_properties_;

  /// Indicator of whether the trader supports dynamic properties.
  CORBA::Boolean supports_dynamic_properties_;

  /// Indicator of whether the trader supports proxy offers.
  CORBA::Boolean supports_proxy_offers_;

  /// A reference to the TypeRepostitory used by the trader.
  CORBA::Object_var type_repos_;

  /// Already narrowed reference to the ServiceTypeRepository.
  CosTradingRepos::ServiceTypeRepository_var service_type_repos_;
};

/**
 * @class TAO_Link_Attributes_i
 *
 * @brief This class stores, allows access to and modification of
 * trader's link attributes.
 *
 * Contains a reference to the trader from which it is
 * used so it can use trader's lock.
 */
class TAO_Trading_Serv_Export TAO_Link_Attributes_i
{
public:
  // = Initialization and termination methods.

  TAO_Link_Attributes_i (TAO_Lockable &locker);
  ~TAO_Link_Attributes_i ();

  // = Accessor methods.

  CosTrading::FollowOption max_link_follow_policy (void) const;
  void  max_link_follow_policy (CosTrading::FollowOption);

private:

  /// A reference to the trader (needed for obtaining the lock.)
  TAO_Lockable &locker_;

  ///
  CosTrading::FollowOption max_link_follow_policy_;
};

/**
 * @class TAO_Import_Attributes_i
 *
 * @brief This class stores, allows access to and modification of
 * trader's import attributes.
 *
 * Contains a reference to the trader from which it is
 * used so it can use trader's lock.
 */
class TAO_Trading_Serv_Export TAO_Import_Attributes_i
{
public:
  // = Initialization and termination methods.

  TAO_Import_Attributes_i (TAO_Lockable &locker);

  ~TAO_Import_Attributes_i (void);

  // = Accessor methods.

  CORBA::ULong def_search_card (void) const;
  void def_search_card (CORBA::ULong);

  CORBA::ULong max_search_card (void) const;
  void max_search_card (CORBA::ULong);

  CORBA::ULong def_match_card (void) const;
  void def_match_card (CORBA::ULong);

  CORBA::ULong max_match_card (void) const;
  void max_match_card (CORBA::ULong);

  CORBA::ULong def_return_card (void) const;
  void def_return_card (CORBA::ULong);

  CORBA::ULong max_return_card (void) const;
  void max_return_card (CORBA::ULong);

  CORBA::ULong max_list (void) const;
  void max_list (CORBA::ULong);

  CORBA::ULong def_hop_count (void) const;
  void def_hop_count (CORBA::ULong);

  CORBA::ULong max_hop_count (void) const;
  void max_hop_count (CORBA::ULong);

  CosTrading::FollowOption def_follow_policy (void) const;
  void def_follow_policy (CosTrading::FollowOption);

  CosTrading::FollowOption max_follow_policy (void) const;
  void max_follow_policy (CosTrading::FollowOption);

private:

  TAO_Lockable &locker_;

  /// Upper bound of offers to be searched if <search_card>
  /// is not specified.
  CORBA::ULong def_search_card_;

  /// Maximum upper bound of offers to be searched.
  CORBA::ULong max_search_card_;

  /// Upper bound of matched offers to be ordered if
  /// <match_card> is not specified).
  CORBA::ULong def_match_card_;

  /// Maximum upper bound of matched offers to be ordered.
  CORBA::ULong max_match_card_;

  /// Upper bound of ordered offers to be returned if
  /// <returned_card> is not specified.
  CORBA::ULong def_return_card_;

  /// Maximum upper bound of ordered offers to be returned.
  CORBA::ULong max_return_card_;

  /// Upper bound of depth
  CORBA::ULong max_list_;

  /// Upper bound of depth of links to be traversed if <hop_count> is
  /// not specified.
  CORBA::ULong def_hop_count_;

  /// Maximum upper bound of depth of links to be traversed.
  CORBA::ULong max_hop_count_;

  /// Default link follow policy for a particular trader.
  CosTrading::FollowOption def_follow_policy_;

  /// Limiting link follow policy for all links of the trader -
  /// overrides both link and importer policies.
  CosTrading::FollowOption max_follow_policy_;
};

/**
 * @class TAO_Trading_Components_i
 *
 * @brief Set/get methods for references to various
 * interfaces of the trader.
 *
 * Note, this class is for use in local address space only and is NOT a
 * direct implementation of IDL methods. (Implementation of
 * IDL methods in CosTrading::Trader_Components would need
 * add a _duplicate call for each get method).
 */
class TAO_Trading_Serv_Export TAO_Trading_Components_i
{
public:

  // = Initialization and termination methods.
  TAO_Trading_Components_i (TAO_Lockable &locker);
  ~TAO_Trading_Components_i (void);

  // = CosTrading::TraderComponents methods.

  /// Returns an object reference to the Lookup interface of the trader.
  /// Returns nil if the trader does not support Lookup interface.
  CosTrading::Lookup_ptr lookup_if (void) const;

  /// Set the reference to the Lookup interface.
  void lookup_if (CosTrading::Lookup_ptr);

  /// Returns object reference for the Register interface of the trader.
  /// Returns nil if the trader does not support Register interface.
  CosTrading::Register_ptr register_if (void) const;

  /// Set the reference to the Register interface of the trader.
  void register_if (CosTrading::Register_ptr);

  /// Returns object reference for the Link interface of the trader.
  /// Returns nil if the trader does not support Link interface.
  CosTrading::Link_ptr link_if (void) const;

  /// Set the reference to the Link interface of the trader.
  void link_if (CosTrading::Link_ptr);

  /// Returns object reference to the Proxy interface of the trader.
  /// Returns nil if the trader does not support Proxy interface.
  CosTrading::Proxy_ptr proxy_if (void) const;

  /// Set the reference to the Proxy interface of the trader.
  void proxy_if (CosTrading::Proxy_ptr);

  /// Returns object reference for the Admin interface of the trader.
  /// Returns nil if the trader does not support Admin interface.
  CosTrading::Admin_ptr admin_if (void) const;

  /// Set the reference to the Admin interface of the trader.
  void admin_if (CosTrading::Admin_ptr);

private:

  TAO_Lockable &locker_;

  CosTrading::Lookup_var lookup_;
  CosTrading::Register_var register_;
  CosTrading::Link_var link_;
  CosTrading::Proxy_var proxy_;
  CosTrading::Admin_var admin_;
};

  // *************************************************************
  // TAO_Trader_Base
  // *************************************************************

/**
 * @class TAO_Trader_Base
 *
 * @brief TAO_Trader inherits from this "helper" class.
 * The sole purpose of this class is to factor some of TAO_Trader's
 * data members out, so that they would not have to be templatized
 * and be be aware of the type of lock they use.
 *
 * TAO_Trader is a template class.  And while we want
 * <import_attributes_>, <trader_components_>, and
 * <support_attributes> use a lock contained in TAO_Trader, we do not
 * want all these classes to be templatized.  TAO_Trader_Base class solves
 * this problem.
 */
class TAO_Trading_Serv_Export TAO_Trader_Base : public TAO_Lockable
{
public:

  enum Trader_Components
  {
    LOOKUP = 0x001,
    REGISTER = 0x002,
    LINK = 0x004,
    PROXY = 0x008,
    ADMIN = 0x010
  };

  virtual ~TAO_Trader_Base (void);

  // = Accessors for objects that manage trader's configuration.

  TAO_Trading_Components_i &trading_components (void);

  const TAO_Trading_Components_i &trading_components (void) const;

  TAO_Import_Attributes_i &import_attributes (void);

  const TAO_Import_Attributes_i &import_attributes (void) const;

  TAO_Support_Attributes_i &support_attributes (void);

  const TAO_Support_Attributes_i &support_attributes (void) const;

  TAO_Link_Attributes_i &link_attributes (void);

  const TAO_Link_Attributes_i &link_attributes (void) const;

  // = Accessor for trader's lock.

  /**
   * Determine whether the identifier is a valid one (i.e., if the
   * first character is a letter, and the subsequent ones letter,
   * numbers, or underscores.)
   *
   * IDL identifier scoping and escaping rules apply.
   */
  static CORBA::Boolean is_valid_identifier_name (const char* ident);

  /**
   * Determine whether the identifier is a valid one (i.e., if the
   * first character is a letter, and the subsequent ones letter,
   * numbers, or underscores.)
   */
  static CORBA::Boolean is_valid_property_name (const char* ident);

  /**
   * Determine whether the link name is a valid one
   * currently defined the same as property name.
   */
  static CORBA::Boolean is_valid_link_name (const char* ident)
  {
    return is_valid_property_name (ident);
  }

protected:
  // = Objects determining current configuration of a trader.

  /// Stores and allows lookup of trader's components.
  TAO_Trading_Components_i trading_components_;

  /// Stores and allows access/modification of trader's import attributes.
  TAO_Import_Attributes_i import_attributes_;

  /// Stores and allows access/modification of trader's support attributes.
  TAO_Support_Attributes_i support_attributes_;

  /// Stores and allows access/modification of trader's link attributes.
  TAO_Link_Attributes_i link_attributes_;

 protected:

  /// Implemented.
  TAO_Trader_Base (void);

 private:

  /// Unimplemented.
  TAO_Trader_Base (const TAO_Trader_Base& TAO_Trader_Base);
  TAO_Trader_Base& operator= (const TAO_Trader_Base&);
};


// Helpful typedefs
// Should probably be private to TAO_Offer_Database, but g++ has a
// hard time with it like that when compiling TAO_Service_Offer_Iterator.
typedef ACE_Hash_Map_Manager_Ex<CORBA::ULong, CosTrading::Offer*, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex> TAO_Offer_Map;

typedef ACE_Unbounded_Set<CORBA::String_var> TAO_String_Set;
typedef ACE_Unbounded_Queue<char*> TAO_String_Queue;

// = Helpful operators.

bool
operator> (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r);

bool
operator< (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r);

bool
operator== (const CosTrading::Admin::OctetSeq& left,
            const CosTrading::Admin::OctetSeq& right);

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TRADER_BASE_H */
