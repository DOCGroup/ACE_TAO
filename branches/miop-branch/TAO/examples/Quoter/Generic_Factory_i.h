// $Id$

// ============================================================================
//
// = FILENAME
//    Generic_Factory_i.h
//
// = DESCRIPTION
//    A LifeCycle Service Generic Factory for the Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "QuoterS.h"


#ifndef QUOTER_GENERIC_FACTORY_IMPL_H
#define QUOTER_GENERIC_FACTORY_IMPL_H

class Quoter_Generic_Factory_i : public POA_Stock::Quoter_Generic_Factory
{
  // = TILE
  //   A CosLifeCycle conforming Generic Factory for the Quoter
  //   example. It uses the Naming Service to find a fitting factory.

public:
  Quoter_Generic_Factory_i (int debug_level = 1);
  ~Quoter_Generic_Factory_i (void);

  CORBA::Boolean supports (const CosLifeCycle::Key &factory_key
                           TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns true if the Generic Factory is able to forward a request
  // for creating an object described by the <factory_key>.

  CORBA::Object_ptr create_object (const CosLifeCycle::Key &factory_key,
                                   const CosLifeCycle::Criteria &the_criteria
                                   TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory,
                       CosLifeCycle::InvalidCriteria,
                       CosLifeCycle::CannotMeetCriteria));
  // Returns an object reference to a newly created object, though the
  // Generic Factory itself cannot create objects, it will forward the
  // request to a more concrete Factory.

private:
  CosNaming::NamingContext_ptr get_naming_context (const CosLifeCycle::Key &factory_key
                                                   TAO_ENV_ARG_DECL_NOT_USED);
  // a helper to get the proper naming context.

  int debug_level_;
  // debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
};

#endif /* QUOTER_GENERIC_FACTORY_IMPL_H */
