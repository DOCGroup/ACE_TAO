// $Id$

// ============================================================================
//
// = FILENAME
//    Generic_Factory.h
//
// = DESCRIPTION
//    A generic factory, customized for the use with the Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Utils/ORB_Manager.h"
#include "Generic_Factory_i.h"

#ifndef GENERIC_FACTORY_H
#define GENERIC_FACTORY_H

class Quoter_Generic_Factory_Server
{
  // = TILE
  //   Server object for the Quoter Generic Factory

public:
  Quoter_Generic_Factory_Server (void);
  // Default constructor

  ~Quoter_Generic_Factory_Server (void);
  // Destructor

  int init (int argc,
            ACE_TCHAR *argv[]);
  // Initialize the Quoter_Server state - parsing arguments and ...

  int run (void);
  // Run the orb.

  u_int parse_args (void);
  // Parse the passed parameters.

private:
  TAO_ORB_Manager orb_manager_;
  // instance of the ORB Manager

  Quoter_Generic_Factory_i *quoter_Generic_Factory_i_ptr_;
  // Instance of the Quoter Generic Factory.

  CosNaming::NamingContext_var quoterNamingContext_var_;
  // Reference to the Quoter naming context.

  int argc_;
  // Number of commandline arguments.

  ACE_TCHAR **argv_;
  // commandline arguments.

  unsigned char use_LifeCycle_Service_;
  // flag for whether or not to use the lifecycle service

  int debug_level_;
  // debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
};

#endif /* GENERIC_FACTORY_H */
