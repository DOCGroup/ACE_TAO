// $Id$
// ============================================================================
//
// = LIBRARY
//    orbsvcs/Concurrency_Service/Concurency_Service
//
// = FILENAME
//    Concurrency_Service.h
//
// = DESCRIPTION
//      This class implements the functionality of the Concurrency_Service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#if !defined (_CONCURRENCY_SERVICE_H)
#define _CONCURRENCY_SERVICE_H

//#include "ace/streams.h"
//#include "tao/tao_util.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/TAO.h"
#include "orbsvcs/Concurrency/Concurrency_Utils.h"
#include "orbsvcs/Concurrency/CC_LockSetFactory.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Concurrency_Service:public TAO_ORB_Manager
{
  // =TITLE
  //   Defines a class that encapsulates the implementation of the
  //   concurrency service.
  // =DESCRIPTION
  //   
  //   

public:
  Concurrency_Service (void);
  // Default Constructor.

  Concurrency_Service (int argc,
		       char** argv,
                       CORBA::Environment& _env);
  // Constructor taking the command-line arguments.

  ~Concurrency_Service (void);
  // Destructor.
	
  int init (int argc,
            char** argv,
            CORBA::Environment& _env);
  // Initialize the Concurrency Service with the arguments.

  int run (CORBA_Environment& _env);
  // Run the Concurrency_Service.

private:
  int parse_args(void);
  // Parses the commandline arguments.

  int init_naming_service (CORBA::Environment& _env);
  // Initialize the name server and register the concurency server
  // factory with it. Maybe later it will be convinient to register
  // the individual lock sets with the naming service. At present the
  // clients must keep track of the reference and pass 'by hand'

  int use_naming_service_;
  // Flag to tell wheter the naming service will be used.

  FILE *ior_output_file_;
  // File to output the concurrency server IOR.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager

  TAO_Naming_Server my_name_server_;
  // An instance of the name server used for registering the factory
  // objects.

  TAO_Concurrency_Server my_concurrency_server_;
  // An instance of the concurrency server

  CosConcurrencyControl::LockSetFactory_var lockset_factory_;
  // Factory var to register with the naming service

  CosNaming::NamingContext_var concurrency_context_;
  // Naming context for the concurrency service

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the naming service

  int argc_;
  // Number of commandline arguments

  char **argv_;
  // Commandline arguments
};

#endif /* _CONCURRENCY_SERVICE_H */
