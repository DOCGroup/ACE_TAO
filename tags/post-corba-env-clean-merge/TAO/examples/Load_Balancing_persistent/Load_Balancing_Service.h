// $Id$
// -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Load_Balancing
//
// = FILENAME
//    Load_Balancing_Service.h
//
// = DESCRIPTION
//    Driver for Load Balancing service.
//
// = AUTHORS
//    Marina Spivak <marina@cs.wustl.edu>
//    with modifications by Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================

#ifndef LOAD_BALANCING_SERVICE_H_
#define LOAD_BALANCING_SERVICE_H_

#include "tao/PortableServer/ORB_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Load_Balancing_Service
{
  // =TITLE
  //   Server, which creates and initializes a
  //   <Load_Balancer::Object_Group_Factory>
  //   object, and runs the orb loop.
public:

  Load_Balancing_Service (void);
  // Default constructor.

  ~Load_Balancing_Service (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialize the <Load_Balancing_Service>: initializes the ORB, parses
  // arguments, creates a servant ...

  int run (TAO_ENV_SINGLE_ARG_DECL);
  // Run the server.

private:
  int parse_args (int argc, char *argv[]);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File to output the <Object_Group_Factory> IOR.
};

#endif /* LOAD_BALANCING_SERVICE_H_ */
