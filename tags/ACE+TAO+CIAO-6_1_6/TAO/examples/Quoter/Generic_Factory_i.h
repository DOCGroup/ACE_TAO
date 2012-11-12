
//=============================================================================
/**
 *  @file    Generic_Factory_i.h
 *
 *  $Id$
 *
 *  A LifeCycle Service Generic Factory for the Quoter example.
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "QuoterS.h"


#ifndef QUOTER_GENERIC_FACTORY_IMPL_H
#define QUOTER_GENERIC_FACTORY_IMPL_H

/**
 * @class Quoter_Generic_Factory_i
 = TILE
 * A CosLifeCycle conforming Generic Factory for the Quoter
 * example. It uses the Naming Service to find a fitting factory.
 */
class Quoter_Generic_Factory_i : public POA_Stock::Quoter_Generic_Factory
{

public:
  Quoter_Generic_Factory_i (int debug_level = 1);
  ~Quoter_Generic_Factory_i (void);

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

private:
  /// a helper to get the proper naming context.
  CosNaming::NamingContext_ptr get_naming_context (const CosLifeCycle::Key &factory_key);

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif /* QUOTER_GENERIC_FACTORY_IMPL_H */
