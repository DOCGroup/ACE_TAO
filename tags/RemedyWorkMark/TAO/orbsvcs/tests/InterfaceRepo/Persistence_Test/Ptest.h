// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/InterfaceRepo
//
// = FILENAME
//    Ptest.h
//
// = DESCRIPTION
//    This code tests the persistence of the IFR by inserting
//    IR objects into the repository with one function, and querying the
//    repository with another, with a repository shutdown in between.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (PTEST_H)
#define PTEST_H

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

class Ptest
{
  // = TITLE
  //     IFR Persistence test Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for code which either populates or queries the
  //     Interface Repository. Designed to be used with a Perl script
  //     which can start and stop both this process and the repository
  //     process in the necessary order.
public:
  Ptest (void);
  // Constructor

  ~Ptest (void);
  // Destructor

  int init (int argc,
            ACE_TCHAR *argv[]);
  // Initialize the ORB and get the IFR object reference.

  int run (void);
  // Execute test code.

private:
  void populate (void);
  int query (void);
  // The two IFR tests.

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);
  // Process the command line arguments.

  CORBA::Boolean debug_;
  // Flag to output results of IFR queries.

  CORBA::Boolean query_;
  // Are we populating a new IFR or querying a persistent one?

  CORBA::ORB_var orb_;
  // Storage of the ORB reference.

  CORBA::Repository_var repo_;
  // Storage of the IFR reference.
};

#endif /* PTEST_H */
