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
  Quoter_Factory_Finder_i (int debug_level = 1);
  ~Quoter_Factory_Finder_i (void);

  virtual CosLifeCycle::Factories * find_factories (const CosLifeCycle::Key &factory_key
                                                    TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory));
  // Returns a sequence of Factories if factories matching the
  // factory_key were found. If no factory was found, then the
  // NoFactory exception, defined in CosLifeCycle, is raised.

private:
  int debug_level_;
  // debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
};

#endif /* QUOTER_FACTORY_FINDER_IMPL_H */
