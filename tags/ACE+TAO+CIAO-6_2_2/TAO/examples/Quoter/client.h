// -*- C++ -*-

//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *  Contains classes that do the work of the client side of the Quoter Test
 *
 *
 *  @author Darrell Brunsch
 */
//=============================================================================

#ifndef QUOTER_CLIENT_H
#define QUOTER_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "QuoterC.h"

#include "orbsvcs/CosLifeCycleC.h"

#include "ace/Task.h"
#include "ace/Thread_Manager.h"

/**
 * @class Quoter_Client
 *
 * @brief Quoter_Client
 *
 * Does all the work of making the calls on the Quoter server
 */
class Quoter_Client
{
public:
  // = Constructor and destructor.
  Quoter_Client (void);
  ~Quoter_Client (void);

  /// Execute client example code.
  int run (void);

  /// Initialize the client communication endpoint with server.
  int init (int argc, ACE_TCHAR **argv);

private:
  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Function to initialize the naming service.
  int init_naming_service (void);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// Key of factory obj ref.
  char *quoter_factory_key_;

  /// Key of the obj ref to be retrieved via the factory.
  char *quoter_key_;

  /// Flag to tell server to shutdown.
  int shutdown_;

  /// Pointer to a factory finder
  Stock::Quoter_Factory_Finder_var factory_Finder_var_;

  /// Pointer to a factory
  Stock::Quoter_Factory_var factory_var_;

  /// Pointer to a generic factory
  CosLifeCycle::GenericFactory_var generic_Factory_var_;

  /// Quoter obj ref.
  Stock::Quoter_var quoter_var_;

  /// Flag to tell if the a Generic Factory is going to be used
  /// or the LifeCycle Service (0 means Generic Factory, 1 means LCS)
  int useLifeCycleService_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

/**
 * @class Quoter_Task
 *
 * @brief Quoter_Task
 *
 * Task that creates a Quoter_Client and uses it
 */
class Quoter_Task : public ACE_Task<ACE_SYNCH>
{
public:
  Quoter_Task (int argc, ACE_TCHAR **argv);

  virtual int svc (void);

private:
  Quoter_Client quoter_client;
  int argc_;
  ACE_TCHAR **argv_;

};

#endif /* QUOTER_CLIENT_H */
