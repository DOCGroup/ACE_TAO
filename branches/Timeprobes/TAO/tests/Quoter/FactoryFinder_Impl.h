// $Id$

// ============================================================================
//
// = FILENAME
//    FactoryFinder_Impl.h
//
// = DESCRIPTION
//    Factory Finder Implementation for the Quoter example
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "quoterS.h"

#if !defined (QUOTER_FACTORY_FINDER_IMPL_H)
#define QUOTER_FACTORY_FINDER_IMPL_H

class QuoterFactoryFinderImpl : public POA_Stock::QuoterFactoryFinder 
{
  // = TILE
  //   A CosLifeCycle conforming Factory Finder for the Quoter
  //   example. It uses the Naming Service to find a fitting factory.

public:
  QuoterFactoryFinderImpl (void);
  ~QuoterFactoryFinderImpl (void);

  virtual CosLifeCycle::Factories * find_factories (const CosLifeCycle::Key & factory_key,
                                                    CORBA::Environment &_tao_environment);
  // Returns a squence of Factories if factories matching the
  // factory_key were found. If no factory was found, then the
  // NoFactory exception, defined in CosLifeCycle, is raised.

private:
  // CosNaming::NamingContext_var quoterNamingContext_var_;    
  // Hold a reference to the Quoter example naming context.
};

#endif /* QUOTER_FACTORY_FINDER_IMPL_H */
