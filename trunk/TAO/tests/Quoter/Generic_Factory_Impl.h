// $Id$

// ============================================================================
//
// = FILENAME
//    Generic_Factory_Impl.h
//
// = DESCRIPTION
//    A LifeCycle Service Generic Factory for the Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "quoterS.h"

#if !defined (QUOTER_GENERIC_FACTORY_IMPL_H)
#define QUOTER_GENERIC_FACTORY_IMPL_H

class Quoter_Generic_Factory_Impl : public POA_Stock::Quoter_Generic_Factory 
{
  // = TILE
  //   A CosLifeCycle conforming Generic Factory for the Quoter
  //   example. It uses the Naming Service to find a fitting factory.

public:
  Quoter_Generic_Factory_Impl (void);
  ~Quoter_Generic_Factory_Impl (void);

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
};

#endif /* QUOTER_GENERIC_FACTORY_IMPL_H */
