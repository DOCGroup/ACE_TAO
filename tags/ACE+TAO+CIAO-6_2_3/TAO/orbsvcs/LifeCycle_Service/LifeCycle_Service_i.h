
//=============================================================================
/**
 *  @file    LifeCycle_Service_i.h
 *
 *  $Id$
 *
 *  A Life Cycle Service for the Quoter example.
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "orbsvcs/LifeCycleServiceS.h"
#include "Factory_Trader.h"
#include "Criteria_Evaluator.h"

#ifndef LIFECYCLE_SERVICE_IMPL_H
#define LIFECYCLE_SERVICE_IMPL_H

/**
 * @class Life_Cycle_Service_i
 = TILE
 * A CosLifeCycle conforming Generic Factory.
 */
class Life_Cycle_Service_i : public POA_LifeCycleService::Life_Cycle_Service
{

public:
  Life_Cycle_Service_i (int debug_level = 1);
  ~Life_Cycle_Service_i (void);

  /// Returns true if the Generic Factory is able to forward a request
  /// for creating an object described by the <factory_key>.
  CORBA::Boolean supports (const CosLifeCycle::Key &factory_key);

  /**
   * Returns an object reference to a newly created object, though the
   * Generic Factory itself cannot create objects, it will forward the
   * request to a more concrete Factory.
   */
  CORBA::Object_ptr create_object (const CosLifeCycle::Key &factory_key,
                                   const CosLifeCycle::Criteria &the_criteria);

  void register_factory (const char * name,
                         const char * location,
                         const char * description,
                         CORBA::Object_ptr object);

  // Registers a factory with specified properties

private:
  Factory_Trader *factory_trader_ptr_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif /* LIFECYCLE_SERVICE_IMPL_H */








