// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service_i.h
//
// = DESCRIPTION
//    A Life Cycle Service for the Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "orbsvcs/LifeCycleServiceS.h"
#include "Factory_Trader.h"
#include "Criteria_Evaluator.h"

#ifndef LIFE_CYCLE_SERVICE_IMPL_H
#define LIFE_CYCLE_SERVICE_IMPL_H

class Life_Cycle_Service_i : public POA_LifeCycleService::Life_Cycle_Service
{
  // = TILE
  //   A CosLifeCycle conforming Generic Factory.

public:
  Life_Cycle_Service_i (void);
  ~Life_Cycle_Service_i (void);

  CORBA::Boolean supports (const CosLifeCycle::Key &factory_key,
                           CORBA::Environment &_env_there);
  // Returns true if the Generic Factory is able to forward a request
  // for creating an object described by the <factory_key>.

  CORBA::Object_ptr create_object (const CosLifeCycle::Key &factory_key,
                                   const CosLifeCycle::Criteria &the_criteria,
                                   CORBA::Environment &_env_there);
  // Returns an object reference to a newly created object, though the
  // Generic Factory itself cannot create objects, it will forward the
  // request to a more concrete Factory.

  void register_factory (const char * name,
                         const char * location,
                         const char * description,
                         CORBA::Object_ptr object,
                         CORBA::Environment &_env_there);
  // Registers a factory with specified properties

private:
  Factory_Trader *factory_trader_ptr_;
};

#endif /* LIFE_CYCLE_SERVICE_IMPL_H */








