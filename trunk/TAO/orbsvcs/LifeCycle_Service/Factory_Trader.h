// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//   Factory_Trader.cpp
//
// = DESCRIPTION
//   Factory Trader for the Generic Factory.
//
// = AUTHOR
//   Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#if !defined (FACTORY_TRADER_H)
#define FACTORY_TRADER_H

#include "tao/corba.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"
#include "orbsvcs/CosLifeCycleC.h"


class Factory_Trader 
{
public:
  Factory_Trader ();
  ~Factory_Trader ();

  void add_type ();
  // Add a the Factory type to the repository
  
  void export (const char * name,
	       const char * location, 
	       const char * description,
	       const CORBA::Object_ptr object_ptr);
  // export a specific factory

  CORBA::Object_ptr query (const CORBA::String constraint);
  // query for a specific factory using a constraint

  static const char * GENERIC_FACTORY_INTERFACE_REPOSITORY_ID;
private:
  TAO_Service_Type_Repository repository_;
  TAO_Trader_Factory::TAO_TRADER *trader_ptr_;
  TAO_Trading_Components_Impl *trading_Components_ptr_;
  TAO_Support_Attributes_Impl *support_Attributes_ptr_;
};

#endif // FACTORY_TRADER_H
