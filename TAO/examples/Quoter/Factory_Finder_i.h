
//=============================================================================
/**
 *  @file    Factory_Finder_i.h
 *
 *  $Id$
 *
 *  Factory Finder Implementation for the Quoter example
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "QuoterS.h"

#ifndef QUOTER_FACTORY_FINDER_IMPL_H
#define QUOTER_FACTORY_FINDER_IMPL_H

/**
 * @class Quoter_Factory_Finder_i
 = TILE
 * A CosLifeCycle conforming Factory Finder for the Quoter
 * example. It uses the Naming Service to find a fitting factory.
 */
class Quoter_Factory_Finder_i : public POA_Stock::Quoter_Factory_Finder
{

public:
  Quoter_Factory_Finder_i (int debug_level = 1);
  ~Quoter_Factory_Finder_i (void);

  /**
   * Returns a sequence of Factories if factories matching the
   * factory_key were found. If no factory was found, then the
   * NoFactory exception, defined in CosLifeCycle, is raised.
   */
  virtual CosLifeCycle::Factories * find_factories (const CosLifeCycle::Key &factory_key);

private:
  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif /* QUOTER_FACTORY_FINDER_IMPL_H */
