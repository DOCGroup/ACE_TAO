// -*- C++ -*-

//=============================================================================
/**
 *  @file    Ptest.h
 *
 *  $Id$
 *
 *  This code tests the persistence of the IFR by inserting
 *  IR objects into the repository with one function, and querying the
 *  repository with another, with a repository shutdown in between.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (PTEST_H)
#define PTEST_H

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

/**
 * @class Ptest
 *
 * @brief IFR Persistence test Implementation
 *
 * Class wrapper for code which either populates or queries the
 * Interface Repository. Designed to be used with a Perl script
 * which can start and stop both this process and the repository
 * process in the necessary order.
 */
class Ptest
{
public:
  /// Constructor
  Ptest (void);

  /// Destructor
  ~Ptest (void);

  /// Initialize the ORB and get the IFR object reference.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Execute test code.
  int run (void);

private:
  /// The two IFR tests.
  void populate (void);
  int query (void);

  /// Process the command line arguments.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Flag to output results of IFR queries.
  CORBA::Boolean debug_;

  /// Are we populating a new IFR or querying a persistent one?
  CORBA::Boolean query_;

  /// Storage of the ORB reference.
  CORBA::ORB_var orb_;

  /// Storage of the IFR reference.
  CORBA::Repository_var repo_;
};

#endif /* PTEST_H */
