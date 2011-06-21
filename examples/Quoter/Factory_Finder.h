
//=============================================================================
/**
 *  @file    Factory_Finder.h
 *
 *  $Id$
 *
 *  Server for the Quoter Factory Finder
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Utils/ORB_Manager.h"
#include "Factory_Finder_i.h"

#ifndef QUOTER_FACTORY_FINDER_H
#define QUOTER_FACTORY_FINDER_H

/**
 * @class Quoter_Factory_Finder_Server
 = TILE
 * Server object for the Quoter Factory Finder
 */
class Quoter_Factory_Finder_Server
{

public:
  /// Default constructor
  Quoter_Factory_Finder_Server (void);

  /// Destructor
  ~Quoter_Factory_Finder_Server (void);

  /// Initialize the Quoter_Server state - parsing arguments and ...
  int init (int argc, ACE_TCHAR *argv[]);

  /// Run the orb
  int run (void);

  /// parse the passed parameters
  u_int parse_args (void);

private:
  /// instance of the ORB Manager
  TAO_ORB_Manager orb_manager_;

  /// instance of the Quoter Factory Finder
  Quoter_Factory_Finder_i *quoter_Factory_Finder_i_ptr_;

  /// reference to the Quoter naming context
  CosNaming::NamingContext_var quoterNamingContext_var_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif /* QUOTER_FACTORY_FINDER_H */
