// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/Naming_Service/Naming_Service
//
// = FILENAME
//    Naming_Service.cpp
//
// = DESCRIPTION
//      This class implements a Naming_Service object.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#include "Naming_Service.h"

// Default Constructor.

Naming_Service::Naming_Service (void)
{
}

// Constructor taking command-line arguments

Naming_Service::Naming_Service (int argc,
				char** argv)
{
   this->init (argc,argv);
}

// Initialize the state of the Naming_Service object

int
Naming_Service::init (int argc,
		      char** argv)
{
  TAO_TRY
    {
      this->init_child_poa (argc,
			    argv,
			    "child_poa",
			    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->my_naming_server_.init (this->orb_,
				    this->child_poa_);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Naming_Service::init");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Run the ORB event loop

int
Naming_Service::run (CORBA_Environment& env)
{
  return TAO_ORB_Manager::run (env);
}

// Destructor.

Naming_Service::~Naming_Service (void)
{
}


int
main (int argc, char ** argv)
{
  int init_result;

  Naming_Service naming_service;

  init_result = naming_service.init (argc,argv);
  if (init_result < 0)
    return init_result;

  TAO_TRY
    {
      naming_service.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NamingService");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

