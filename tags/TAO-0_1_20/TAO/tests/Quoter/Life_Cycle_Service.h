// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service.h
//
// = DESCRIPTION
//    A generic factory, customized for the use with the Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "Life_Cycle_Service_Impl.h"

#if !defined (QUOTER_LIFE_CYCLE_SERVICE_H)
#define QUOTER_LIFE_CYCLE_SERVICE_H

class Quoter_Life_Cycle_Service_Server
{
  // = TILE
  //   Server object for the Quoter Generic Factory

public:
  Quoter_Life_Cycle_Service_Server (void);
  // Default constructor

  ~Quoter_Life_Cycle_Service_Server (void);
  // Destructor

  int init (int argc,
            char *argv[],
            CORBA::Environment& env);
  // Initialize the Quoter_Server state - parsing arguments and ...

  int run (CORBA::Environment& env);
  // Run the orb.

  u_int parse_args (void);
  // Parse the passed parameters.

private:
  TAO_ORB_Manager orb_manager_;
  // instance of the ORB Manager

  Quoter_Life_Cycle_Service_Impl *quoter_Life_Cycle_Service_Impl_ptr_;
  // Instance of the Quoter Generic Factory.
  
  CosNaming::NamingContext_var quoterNamingContext_var_;
  // Reference to the Quoter naming context.

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* QUOTER_LIFE_CYCLE_SERVICE_H */

