// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Quoter
//
// = FILENAME
//    quoter_client.h
//
// = DESCRIPTION
//    Contains classes that do the work of the client side of the Quoter Test
//
// = AUTHORS
//    Darrell Brunsch
//
// ============================================================================

#ifndef QUOTER_CLIENT_H
#define QUOTER_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "tao/corba.h"
#include "QuoterC.h"
#include "orbsvcs/CosLifeCycleC.h"

class Quoter_Client
{
  // = TITLE
  //    Quoter_Client
  //
  // = DESCRIPTION
  //    Does all the work of making the calls on the Quoter server
public:
  // = Constructor and destructor.
  Quoter_Client (void);
  ~Quoter_Client (void);

  int run (void);
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  int init_naming_service (void);
  // Function to initialize the naming service.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *quoter_factory_key_;
  // Key of factory obj ref.

  char *quoter_key_;
  // Key of the obj ref to be retrieved via the factory.

  int shutdown_;
  // Flag to tell server to shutdown.

  Stock::Quoter_Factory_Finder_var factory_Finder_var_;
  // Pointer to a factory finder

  Stock::Quoter_Factory_var factory_var_;
  // Pointer to a factory

  CosLifeCycle::GenericFactory_var generic_Factory_var_;
  // Pointer to a generic factory

  Stock::Quoter_var quoter_var_;
  // Quoter obj ref.

  CORBA::ORB_var orb_;
  // Remember our orb.

  int useLifeCycleService_;
  // Flag to tell if the a Generic Factory is going to be used
  // or the LifeCycle Service (0 means Generic Factory, 1 means LCS)
};

class Quoter_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //    Quoter_Task
  //
  // = DESCRIPTION
  //    Task that creates a Quoter_Client and uses it
public:
  Quoter_Task (int argc, char **argv);

  virtual int svc (void);

private:
  Quoter_Client quoter_client;
  int argc_;
  char **argv_;

};

#endif /* QUOTER_CLIENT_H */
