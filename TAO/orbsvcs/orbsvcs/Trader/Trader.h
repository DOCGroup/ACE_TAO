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
// ============================================================================

#ifndef TAO_TRADER_BASE_H
#define TAO_TRADER_BASE_H

#include <string>
#include <map>

#include "Attributes.h"

typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;

// Forward Declaration.
class TAO_Trader_Base;

// Cludge to avoid template instantiation in the executable.
class TAO_Trader_Factory
{
public:

  typedef TAO_Trader_Base TAO_TRADER;

  static TAO_TRADER* create_linked_trader (void);
  
#ifdef ACE_HAS_THREADS
  
  //  static TAO_TRADER* create_MT_linked_trader (void);
  
#endif /* ACE_HAS_THREADS */
};

class TAO_Export TAO_Trader_Base
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
  typedef int HUGE_NUMBER;

  TAO_Trader_Base (void);

  // = Accessors for objects that manage trader's configuration.

  TAO_Trading_Components_Impl &trading_components (void);

  const TAO_Trading_Components_Impl &trading_components (void) const;

  TAO_Import_Attributes_Impl &import_attributes (void);

  const TAO_Import_Attributes_Impl &import_attributes (void) const;

  TAO_Support_Attributes_Impl &support_attributes (void);

  const TAO_Support_Attributes_Impl &support_attributes (void) const;

  TAO_Link_Attributes_Impl &link_attributes (void);

  const TAO_Link_Attributes_Impl &link_attributes (void) const;

  // = Accessor for trader's lock.

  virtual ACE_Lock &lock (void) = 0;
  // return lock that guards trader's current configuration.

  static CORBA::Boolean is_valid_identifier_name (const char* ident);
  // Determine whether the identifier is a valid one (i.e., if the
  // first character is a letter, and the subsequent ones letter,
  // numbers, or underscores.)

protected:
  // = Objects determining current configuration of a trader.

  TAO_Trading_Components_Impl trading_components_;
  // Stores and allows lookup of trader's components.

  TAO_Import_Attributes_Impl import_attributes_;
  // Stores and allows access/modification of trader's import attributes.

  TAO_Support_Attributes_Impl support_attributes_;
  // Stores and allows access/modification of trader's support attributes.

  TAO_Link_Attributes_Impl link_attributes_;
  // Stores and allows access/modification of trader's link attributes.
};

int
operator> (const SERVICE_TYPE_REPOS::IncarnationNumber &l,
	   const SERVICE_TYPE_REPOS::IncarnationNumber &r);

int
operator< (const SERVICE_TYPE_REPOS::IncarnationNumber &l,
	   const SERVICE_TYPE_REPOS::IncarnationNumber &r);


#endif /* TAO_TRADER_BASE_H */
