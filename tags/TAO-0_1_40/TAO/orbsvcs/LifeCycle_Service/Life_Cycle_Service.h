// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service.h
//
// = DESCRIPTION
//    A creation service for objects using the CosLifeCycle GenericFactory.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "Life_Cycle_Service_Impl.h"

#if !defined (LIFE_CYCLE_SERVICE_H)
#define IFE_CYCLE_SERVICE_H

class Life_Cycle_Service_Server
{
  // = TILE
  //   Server object for the LifeCycle Service

public:
  Life_Cycle_Service_Server (void);
  // Default constructor

  ~Life_Cycle_Service_Server (void);
  // Destructor

  int init (int argc,
            char *argv[],
            CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and ...

  int run (CORBA::Environment& env);
  // Run the orb.

  u_int parse_args (void);
  // Parse the passed parameters.

private:
  TAO_ORB_Manager orb_manager_;
  // instance of the ORB Manager

  Life_Cycle_Service_Impl *life_Cycle_Service_Impl_ptr_;
  // Instance of the creation service 

  CosNaming::NamingContext_var namingContext_var_;
  // reference to the naming service

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* LIFE_CYCLE_SERVICE_H */

