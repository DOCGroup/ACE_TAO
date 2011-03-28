//=============================================================================
/**
 *  @file   Factory_Trader.cpp
 *
 *  $Id$
 *
 * Factory Trader for the Generic Factory.
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#ifndef FACTORY_TRADER_H
#define FACTORY_TRADER_H

#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"
#include "orbsvcs/CosLifeCycleC.h"


class Factory_Trader
{
public:
  Factory_Trader (int debug_level = 1);
  ~Factory_Trader ();

  /// Add a the Factory type to the repository
  void add_type ();

  /// export a specific factory
  void _cxx_export (const char * name,
                    const char * location,
                    const char * description,
                    const CORBA::Object_ptr object_ptr);

  /// query for a specific factory using a constraint
  CORBA::Object_ptr query (const char* constraint);

  static const char * GENERIC_FACTORY_INTERFACE_REPOSITORY_ID;
private:
  TAO_Service_Type_Repository repository_;
  TAO_Trader_Factory::TAO_TRADER *trader_ptr_;
  TAO_Trading_Components_i *trading_Components_ptr_;
  TAO_Support_Attributes_i *support_Attributes_ptr_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif // FACTORY_TRADER_H
