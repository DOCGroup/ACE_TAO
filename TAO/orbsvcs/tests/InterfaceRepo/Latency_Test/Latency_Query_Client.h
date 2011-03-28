// -*- C++ -*-

//=============================================================================
/**
 *  @file    Latency_Query_Client.h
 *
 *  $Id$
 *
 *  This class tests the latency of queries made on the IFR.
 *
 *
 *  @author Jeff Parsons <parsons@isis-server.isis.vanderbilt.edu>
 */
//=============================================================================


#if !defined (LATENCY_QUERY_CLIENT_H)
#define LATENCY_QUERY_CLIENT_H

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

/**
 * @class Latency_Query_Client
 *
 * @brief Querying IFR Client Implementation
 *
 * Class wrapper for a client which queries the Interface Repository.
 */
class Latency_Query_Client
{
public:
  /// Constructor
  Latency_Query_Client (void);

  /// Destructor
  ~Latency_Query_Client (void);

  /// Initialize the ORB and get the IFR object reference.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Execute test code.
  int run (void);
private:
  /// Process the command line arguments.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Put in something to query about.
  int populate_ifr (void);

private:
  /// Toggle debugging output.
  bool debug_;

  /// Toggle saving of dump history.
  bool do_dump_history_;

  /// Number of queries in a run.
  CORBA::ULong iterations_;

  /// Storage of the ORB reference.
  CORBA::ORB_var orb_;

  /// Storage of the IFR reference.
  CORBA::Repository_var repo_;

//  CORBA::AliasDef_var tdef_;
  // Storage of the typedef definition that we will query.

  /// Storage of the attribute definition we will query.
  CORBA::AttributeDef_var attr_;
};

#endif /* LATENCY_QUERY_CLIENT_H */
