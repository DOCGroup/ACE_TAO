/* -*- C++ -*- */

// ============================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Trader_Base.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
//  Contents:
//
//  TAO_Trader_Factory
//  TAO_Trader_Base
//  TAO_Support_Attributes_i
//  TAO_Trading_Components_i
//  TAO_Link_Attributes_i
//  TAO_Import_Attributes_i
//  TAO_Sequence_Extracter_Base
//
// ============================================================================

#ifndef TAO_TRADER_BASE_H
#define TAO_TRADER_BASE_H

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

#include "orbsvcs/CosTradingS.h"
#include "orbsvcs/orbsvcs_export.h"

#if defined(_MSC_VER)
#pragma warning (disable:4250)
#endif /* _MSC_VER */

// Forward Declaration.
class TAO_Trader_Base;

  // *************************************************************
  // TAO_Trader_Factory
  // *************************************************************

class TAO_ORBSVCS_Export TAO_Trader_Factory
// = TITLE
//   Uses command line arguments to construct a trader instance with
//   the correct interface support, locking, and policy settings.
{
public:

  typedef TAO_Trader_Base TAO_TRADER;

  static TAO_TRADER* create_trader (int& argc, char** argv);
  // Creates an instance of the trader according to parameters whose
  // default values can be overrided with the following command line
  // arguments:
  //
  // -TSthreadsafe, default is not thread-safe
  // -TSconformance {query, simple, stand-alone, linked}, default is linked
  // -TSsupports_dynamic_properties {true, false}, default is true
  // -TSsupports_modifiable_propertise {true, false}, default is true
  // -TSdef_search_card {integer}, default is 20
  // -TSmax_search_card {integer}, default is 50
  // -TSdef_match_card {integer}, default is 20
  // -TSmax_match_card {integer}, default is 50
  // -TSdef_return_card {integer}, default is 20
  // -TSmax_return_card {integer}, default is 50
  // -TSdef_hop_count {integer}, default is 5,
  // -TSmax_hop_count {integer}, default is 10
  // -TSdef_follow_policy {always,if_no_local,local_only}, default is if_no_local,
  // -TSmax_follow_policy {always,if_no_local,local_only}, default is always

protected:

  TAO_Trader_Factory (int& argc, char** argv);

private:

  enum Conformance { TAO_TRADER_QUERY,
                     TAO_TRADER_SIMPLE,
                     TAO_TRADER_STANDALONE,
                     TAO_TRADER_LINKED };

  TAO_TRADER* manufacture_trader (void);

  void parse_args (int& argc, char** argv);

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

class TAO_Lockable
// = DESCRIPTION
//   Class used to remove the circular dependencies between the
//   Attribute classes and the Trader class.
{
public:

  virtual ACE_Lock& lock (void) = 0;
};


class TAO_ORBSVCS_Export TAO_Support_Attributes_i
  // = TITLE
  //     This class stores, allows access to and modification of
  //     trader's support attributes.
  //
  // = DESCRIPTION
  //     Contains a reference to the trader from which it is
  //     used so it can use trader's lock.
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

  TAO_Lockable &locker_;
  // A reference to the trader (needed for obtaining the lock.)

  CORBA::Boolean supports_modifiable_properties_;
  // Indicator of whether the trader supports property modification.

  CORBA::Boolean supports_dynamic_properties_;
  // Indicator of whether the trader supports dynamic properties.

  CORBA::Boolean supports_proxy_offers_;
  // Indicator of whether the trader supports proxy offers.

  CORBA::Object_var type_repos_;
  // A reference to the TypeRepostitory used by the trader.

  CosTradingRepos::ServiceTypeRepository_var service_type_repos_;
  // Already narrowed reference to the ServiceTypeRepository.
};

class TAO_ORBSVCS_Export TAO_Link_Attributes_i
  // = TITLE
  //     This class stores, allows access to and modification of
  //     trader's link attributes.
  //
  // = DESCRIPTION
  //     Contains a reference to the trader from which it is
  //     used so it can use trader's lock.
{
public:
  // = Initialization and termination methods.

  TAO_Link_Attributes_i (TAO_Lockable &locker);
  ~TAO_Link_Attributes_i ();

  // = Accessor methods.

  CosTrading::FollowOption max_link_follow_policy (void) const;
  void  max_link_follow_policy (CosTrading::FollowOption);

private:

  TAO_Lockable &locker_;
  // A reference to the trader (needed for obtaining the lock.)

  CosTrading::FollowOption max_link_follow_policy_;
  //
};

class TAO_ORBSVCS_Export TAO_Import_Attributes_i
  // = TITLE
  //     This class stores, allows access to and modification of
  //     trader's import attributes.
  //
  // = DESCRIPTION
  //     Contains a reference to the trader from which it is
  //     used so it can use trader's lock.
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

  CORBA::ULong def_search_card_;
  // Upper bound of offers to be searched if <search_card>
  // is not specified.

  CORBA::ULong max_search_card_;
  // Maximum upper bound of offers to be searched.

  CORBA::ULong def_match_card_;
  // Upper bound of matched offers to be ordered if
  // <match_card> is not specified).

  CORBA::ULong max_match_card_;
  // Maximum upper bound of matched offers to be ordered.

  CORBA::ULong def_return_card_;
  // Upper bound of ordered offers to be returned if
  // <returned_card> is not specified.

  CORBA::ULong max_return_card_;
  // Maximum upper bound of ordered offers to be returned.

  CORBA::ULong max_list_;
  // Upper bound of depth

  CORBA::ULong def_hop_count_;
  // Upper bound of depth of links to be traversed if <hop_count> is
  // not specified.

  CORBA::ULong max_hop_count_;
  // Maximum upper bound of depth of links to be traversed.

  CosTrading::FollowOption def_follow_policy_;
  // Default link follow policy for a particular trader.

  CosTrading::FollowOption max_follow_policy_;
  // Limiting link follow policy for all links of the trader -
  // overrides both link and importer policies.
};

class TAO_ORBSVCS_Export TAO_Trading_Components_i
  //
  // = TITLE
  //     Set/get methods for references to various
  //     interfaces of the trader.
  //
  // = DESCRIPTION
  //     Note, this class is for use in local address space only and is NOT a
  //     direct implementation of IDL methods. (Implementation of
  //     IDL methods in CosTrading::Trader_Components would need
  //     add a _duplicate call for each get method).
{
public:

  // = Initialization and termination methods.
  TAO_Trading_Components_i (TAO_Lockable &locker);
  ~TAO_Trading_Components_i (void);

  // = CosTrading::TraderComponents methods.

  CosTrading::Lookup_ptr lookup_if (void) const;
  // Returns an object reference to the Lookup interface of the trader.
  // Returns nil if the trader does not support Lookup interface.

  void lookup_if (CosTrading::Lookup_ptr);
  // Set the reference to the Lookup interface.

  CosTrading::Register_ptr register_if (void) const;
  // Returns object reference for the Register interface of the trader.
  // Returns nil if the trader does not support Register interface.

  void register_if (CosTrading::Register_ptr);
  // Set the reference to the Register interface of the trader.

  CosTrading::Link_ptr link_if (void) const;
  // Returns object reference for the Link interface of the trader.
  // Returns nil if the trader does not support Link interface.

  void link_if (CosTrading::Link_ptr);
  // Set the reference to the Link interface of the trader.

  CosTrading::Proxy_ptr proxy_if (void) const;
  // Returns object reference to the Proxy interface of the trader.
  // Returns nil if the trader does not support Proxy interface.

  void proxy_if (CosTrading::Proxy_ptr);
  // Set the reference to the Proxy interface of the trader.

  CosTrading::Admin_ptr admin_if (void) const;
  // Returns object reference for the Admin interface of the trader.
  // Returns nil if the trader does not support Admin interface.

  void admin_if (CosTrading::Admin_ptr);
  // Set the reference to the Admin interface of the trader.

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

class TAO_ORBSVCS_Export TAO_Trader_Base : public TAO_Lockable
  //
  // = TITLE
  //     TAO_Trader inherits from this "helper" class.
  //     The sole purpose of this class is to factor some of TAO_Trader's
  //     data members out, so that they would not have to be templatized
  //     and be be aware of the type of lock they use.
  //
  // = DESCRIPTION
  //     TAO_Trader is a template class.  And while we want
  //     <import_attributes_>, <trader_components_>, and
  //     <support_attributes> use a lock contained in TAO_Trader, we do not
  //     want all these classes to be templatized.  TAO_Trader_Base class solves
  //     this problem.
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

  static CORBA::Boolean is_valid_identifier_name (const char* ident);
  // Determine whether the identifier is a valid one (i.e., if the
  // first character is a letter, and the subsequent ones letter,
  // numbers, or underscores.)

protected:
  // = Objects determining current configuration of a trader.

  TAO_Trading_Components_i trading_components_;
  // Stores and allows lookup of trader's components.

  TAO_Import_Attributes_i import_attributes_;
  // Stores and allows access/modification of trader's import attributes.

  TAO_Support_Attributes_i support_attributes_;
  // Stores and allows access/modification of trader's support attributes.

  TAO_Link_Attributes_i link_attributes_;
  // Stores and allows access/modification of trader's link attributes.

 protected:

  TAO_Trader_Base (void);
  // Implemented.

 private:

  TAO_Trader_Base (const TAO_Trader_Base& TAO_Trader_Base);
  TAO_Trader_Base& operator= (const TAO_Trader_Base&);
  // Unimplemented.
};


// =  Classes to deal with the ACE_Hash_Map_Manager.

class TAO_String_Hash_Key : public CORBA::String_var
{
  // = TITLE
  //     Key for the Hash Table. The EXT_ID of the
  //     ACE_Hash_Map_Manager.
public:
  // = Initialization and termination methods.
  TAO_String_Hash_Key (void);
  // Default constructor.

  TAO_String_Hash_Key (char * name);
  // Constructor from a const string.

  TAO_String_Hash_Key (const char * name);
  // Constructor from a const string.

  TAO_String_Hash_Key (const CORBA::String_var &hash_key);
  // Copy constructor.

  ~TAO_String_Hash_Key (void);
  // Destructor.

  int operator == (const TAO_String_Hash_Key &hash_key) const;
  // The operator for hash binding and "find"ing.

  friend int operator < (const TAO_String_Hash_Key &left,
                         const TAO_String_Hash_Key &right);
  // The operator for hash binding and "find"ing.

  u_long hash (void) const;
  // The function that computes a hash value.
};

class TAO_Hashable_ULong
// = TITLE
// Helper class using hashable integers in the
// ACE_Hash_Map_Manager. The right way to do this would probably be
// template specialization of the Hash Map Manager, but compilers have
// a difficult enough time with the Trading Service as it is.
{
public:

  TAO_Hashable_ULong (void)
    : number_ (0) {}

  TAO_Hashable_ULong (CORBA::ULong number)
    : number_ (number) {}

  u_long hash (void) const { return number_; }

  TAO_Hashable_ULong& operator= (const TAO_Hashable_ULong& number)
    {
      this->number_ = number.number_;
      return *this;
    }

  operator CORBA::ULong (void) const { return number_; }

  friend int operator== (const TAO_Hashable_ULong& left,
                         const TAO_Hashable_ULong& right)
    { return left.number_ == right.number_; }

private:

  CORBA::ULong number_;
};

// Helpful typedefs
// Should probably be private to TAO_Offer_Database, but g++ has a
// hard time with it like that when compiling TAO_Service_Offer_Iterator.
typedef ACE_Hash_Map_Manager<TAO_Hashable_ULong,CosTrading::Offer*,ACE_Null_Mutex> TAO_Offer_Map;
typedef ACE_Hash_Map_Manager<TAO_String_Hash_Key, int, ACE_Null_Mutex> TAO_Lookup_Table;
typedef ACE_Unbounded_Set<TAO_String_Hash_Key> TAO_String_Set;
typedef ACE_Hash_Map_Manager<TAO_String_Hash_Key, CORBA::TypeCode_ptr, ACE_Null_Mutex> TAO_Typecode_Table;
typedef ACE_Unbounded_Queue<char*> TAO_String_Queue;

  // *************************************************************
  // TAO_Sequence_Extracter_Base
  // *************************************************************

class TAO_Sequence_Extracter_Base
// = TITLE
//   Static methods used by the TAO_Sequence_Extracter class.
{
public:

  static CORBA::TCKind sequence_type (CORBA::TypeCode* code,
                                      CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Return the sequence element type for the sequence whose typecode
  // is <code>.
};

// = Helpful operators.

int
operator> (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r);

int
operator< (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
           const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r);

int operator== (const CosTrading::Admin::OctetSeq& left,
                const CosTrading::Admin::OctetSeq& right);

#endif /* TAO_TRADER_BASE_H */
