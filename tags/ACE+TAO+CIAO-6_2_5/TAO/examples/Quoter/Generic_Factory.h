
//=============================================================================
/**
 *  @file    Generic_Factory.h
 *
 *  $Id$
 *
 *  A generic factory, customized for the use with the Quoter example.
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
#include "Generic_Factory_i.h"

#ifndef GENERIC_FACTORY_H
#define GENERIC_FACTORY_H

/**
 * @class Quoter_Generic_Factory_Server
 = TILE
 * Server object for the Quoter Generic Factory
 */
class Quoter_Generic_Factory_Server
{

public:
  /// Default constructor
  Quoter_Generic_Factory_Server (void);

  /// Destructor
  ~Quoter_Generic_Factory_Server (void);

  /// Initialize the Quoter_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the orb.
  int run (void);

  /// Parse the passed parameters.
  u_int parse_args (void);

private:
  /// instance of the ORB Manager
  TAO_ORB_Manager orb_manager_;

  /// Instance of the Quoter Generic Factory.
  Quoter_Generic_Factory_i *quoter_Generic_Factory_i_ptr_;

  /// Reference to the Quoter naming context.
  CosNaming::NamingContext_var quoterNamingContext_var_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;

  /// flag for whether or not to use the lifecycle service
  unsigned char use_LifeCycle_Service_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif /* GENERIC_FACTORY_H */
