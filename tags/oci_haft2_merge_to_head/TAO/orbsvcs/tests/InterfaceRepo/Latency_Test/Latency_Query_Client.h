// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/InterfaceRepo
//
// = FILENAME
//    Latency_Query_Client.h
//
// = DESCRIPTION
//    This class tests the latency of queries made on the IFR.
//
// = AUTHOR
//    Jeff Parsons <parsons@isis-server.isis.vanderbilt.edu>
//
// ============================================================================

#if !defined (LATENCY_QUERY_CLIENT_H)
#define LATENCY_QUERY_CLIENT_H

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/corba.h"

class Latency_Query_Client
{
  // = TITLE
  //     Querying IFR Client Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for a client which queries the Interface Repository.
public:
  Latency_Query_Client (void);
  // Constructor

  ~Latency_Query_Client (void);
  // Destructor

  int init (int argc,
            char *argv[]);
  // Initialize the ORB and get the IFR object reference.

  int run (void);
  // Execute test code.
private:
  int parse_args (int argc,
                  char *argv[]);
  // Process the command line arguments.

  int populate_ifr (ACE_ENV_SINGLE_ARG_DECL);
  // Put in something to query about.

private:
  CORBA::Boolean debug_;
  // Toggle debugging output.

  CORBA::Boolean do_dump_history_;
  // Toggle saving of dump history.

  CORBA::ULong iterations_;
  // Number of queries in a run.

  CORBA::ORB_var orb_;
  // Storage of the ORB reference.

  CORBA::Repository_var repo_;
  // Storage of the IFR reference.

//  CORBA::AliasDef_var tdef_;
  // Storage of the typedef definition that we will query.

  CORBA::AttributeDef_var attr_;
  // Storage of the attribute definition we will query.
};

#endif /* LATENCY_QUERY_CLIENT_H */
