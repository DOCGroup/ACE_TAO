// $Id$

// ============================================================================
//
// = FILENAME
//    QuoterFactoryFinder.cpp
//
// = DESCRIPTION
//   Server for the Quoter Factory Finder 
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/tao.h"
#include "FactoryFinderImpl.h"

#if !defined (QUOTER_FACTORY_FINDER_H)
#define QUOTER_FACTORY_FINDER_H

class QuoterFactoryFinder_Server
{
  // = TILE
  //   Server object for the Quoter Factory Finder

public:
  QuoterFactoryFinder_Server (void);
  // Default constructor

  ~QuoterFactoryFinder_Server (void);
  // Destructor

  int init (int argc, char *argv[], CORBA::Environment& env);
  // Initialize the Quoter_Server state - parsing arguments and ...

  int run (CORBA::Environment& env);
  // Run the orb 

  unsigned int parse_args ();
  // parse the passed parameters

private:
  TAO_ORB_Manager orb_manager_;
  // instance of the ORB Manager

  QuoterFactoryFinder_i *quoterFactoryFinder_Impl_ptr_;
  // instance of the Quoter Factory Finder

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* QUOTER_FACTORY_FINDER_H */
