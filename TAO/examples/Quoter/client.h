// -*- C++ -*-

//=============================================================================
/**
 *  @file    client.h
 *
 *  Contains classes that do the work of the client side of the Quoter Test
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
  Quoter_Client ();
  ~Quoter_Client ();

  Quoter_Client (Quoter_Client const &) = delete;
  Quoter_Client &operator= (Quoter_Client const &) = delete;
  Quoter_Client (Quoter_Client &&) = delete;
  Quoter_Client &operator= (Quoter_Client &&) = delete;

  /// Execute client example code.
  int run ();

  /// Initialize the client communication endpoint with server.
  int init (int argc, ACE_TCHAR **argv);

private:
  /// Remember our orb.
  CORBA::ORB_var orb_ {};

  /// Function to initialize the naming service.
  int init_naming_service ();

  /// Parses the arguments passed on the command line.
  int parse_args ();

  /// # of arguments on the command line.
  int argc_ {};

  /// arguments from command line.
  ACE_TCHAR **argv_ {};

  /// Key of factory obj ref.
  char *quoter_factory_key_ {};

  /// Key of the obj ref to be retrieved via the factory.
  char *quoter_key_ {};

  /// Flag to tell server to shutdown.
  bool shutdown_ {};

  /// Pointer to a factory finder
  Stock::Quoter_Factory_Finder_var factory_Finder_var_ {};

  /// Pointer to a factory
  Stock::Quoter_Factory_var factory_var_ {};

  /// Pointer to a generic factory
  CosLifeCycle::GenericFactory_var generic_Factory_var_ {};

  /// Quoter obj ref.
  Stock::Quoter_var quoter_var_ {};

  /// Flag to tell if the a Generic Factory is going to be used
  /// or the LifeCycle Service (false means Generic Factory, true means LCS)
  bool useLifeCycleService_ {};

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_ {1};
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

  Quoter_Task (Quoter_Task const &) = delete;
  Quoter_Task &operator= (Quoter_Task const &) = delete;
  Quoter_Task (Quoter_Task &&) = delete;
  Quoter_Task &operator= (Quoter_Task &&) = delete;

  int svc () override;

private:
  Quoter_Client quoter_client;
  int argc_ {};
  ACE_TCHAR **argv_ {};
};

#endif /* QUOTER_CLIENT_H */
