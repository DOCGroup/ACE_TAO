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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO.h"
#include "Factory_Finder_i.h"

#ifndef QUOTER_FACTORY_FINDER_H
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

  u_int parse_args (void);
  // parse the passed parameters

private:
  TAO_ORB_Manager orb_manager_;
  // instance of the ORB Manager

  Quoter_Factory_Finder_i *quoter_Factory_Finder_i_ptr_;
  // instance of the Quoter Factory Finder

  CosNaming::NamingContext_var quoterNamingContext_var_;
  // reference to the Quoter naming context

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* QUOTER_FACTORY_FINDER_H */
