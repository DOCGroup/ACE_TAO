// $Id$

// ============================================================================
//
// = FILENAME
//    FactoryFinder.h
//
// = DESCRIPTION
//    Server for the Quoter Factory Finder 
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/tao.h"
#include "Factory_Finder_Impl.h"

#if !defined (QUOTER_FACTORY_FINDER_H)
#define QUOTER_FACTORY_FINDER_H

class Quoter_Factory_Finder_Server
{
  // = TILE
  //   Server object for the Quoter Factory Finder

public:
  Quoter_Factory_Finder_Server (void);
  // Default constructor

  ~Quoter_Factory_Finder_Server (void);
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

  Quoter_Factory_Finder_Impl *quoter_Factory_Finder_Impl_ptr_;
  // instance of the Quoter Factory Finder
  
  CosNaming::NamingContext_var quoterNamingContext_var_;
  // reference to the Quoter naming context

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* QUOTER_FACTORY_FINDER_H */
