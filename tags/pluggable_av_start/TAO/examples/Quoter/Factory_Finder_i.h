// $Id$

// ============================================================================
//
// = FILENAME
//    FactoryFinder_i.h
//
// = DESCRIPTION
//    Factory Finder Implementation for the Quoter example
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "QuoterS.h"

#ifndef QUOTER_FACTORY_FINDER_IMPL_H
#define QUOTER_FACTORY_FINDER_IMPL_H

class Quoter_Factory_Finder_i : public POA_Stock::Quoter_Factory_Finder
{
  // = TILE
  //   A CosLifeCycle conforming Factory Finder for the Quoter
  //   example. It uses the Naming Service to find a fitting factory.

public:
  Quoter_Factory_Finder_i (void);
  ~Quoter_Factory_Finder_i (void);

  virtual CosLifeCycle::Factories * find_factories (const CosLifeCycle::Key &factory_key,
                                                    CORBA::Environment &_tao_environment);
  // Returns a squence of Factories if factories matching the
  // factory_key were found. If no factory was found, then the
  // NoFactory exception, defined in CosLifeCycle, is raised.
};

#endif /* QUOTER_FACTORY_FINDER_IMPL_H */
