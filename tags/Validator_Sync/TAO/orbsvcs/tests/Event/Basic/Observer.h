/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Observer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_OBSERVER_H
#define EC_OBSERVER_H

#include "Driver.h"
#include "orbsvcs/Event/EC_Gateway.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_Observer;

class EC_Master
{
  // = TITLE
  //   Run multiple events channels
  //
  // = DESCRIPTION
  //   This test runs multiple event channels, all connected using
  //   gateways.
  //
public:
  EC_Master (void);

  virtual ~EC_Master (void);

  virtual int run (int argc, char* argv[]);
  // Execute the test.

  virtual void initialize_orb_and_poa (int& argc, char* argv[]
                                       ACE_ENV_ARG_DECL);
  // Obtain the orb and the poa pointers

  int channel_count (void) const;
  EC_Observer* channel (int i) const;
  // Accessors

private:
  int parse_args (int &argc, char *argv []);

private:
  ACE_RANDR_TYPE seed_;
  // The seed

  int n_channels_;
  EC_Observer** channels_;
  // The driver programs

  CORBA::ORB_var orb_;
  // The ORB

  PortableServer::POA_var root_poa_;
  // The Root POA
};

class EC_Observer : public EC_Driver
{
  //
  // = TITLE
  //   Test the EC observers
  //
  // = DESCRIPTION
  //
public:
  EC_Observer (EC_Master *master,
               ACE_RANDR_TYPE seed,
               CORBA::ORB_ptr orb,
               PortableServer::POA_ptr root_poa,
               int id);
  // Constructor

  ~EC_Observer (void);
  // Destructor

  // = The EC_Driver methods
  virtual void initialize_orb_and_poa (int& argc, char* argv[]
                                       ACE_ENV_ARG_DECL);
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);
  // add some command line args to enable/disable observerions

  void execute_test (ACE_ENV_SINGLE_ARG_DECL);
  void run_cleanup (ACE_ENV_SINGLE_ARG_DECL);
  // Run the suppliers, using the <thread_manager> parameter

  void dump_results (void);
  void connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i
    ACE_ENV_ARG_DECL);
  void consumer_push (void*,
                      const RtecEventComm::EventSet&
                      ACE_ENV_ARG_DECL);

private:
  EC_Master *master_;
  ACE_RANDR_TYPE seed_;
  int id_;

  TAO_EC_Gateway_IIOP *gwys_;

  TAO_SYNCH_MUTEX lock_;
  // lock internal state
};

#endif /* EC_OBSERVER_H */
