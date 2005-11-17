/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Observer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_OBSERVER_H
#define EC_OBSERVER_H

#include "Driver.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_Observer;

/**
 * @class EC_Master
 *
 * @brief Run multiple events channels
 *
 * This test runs multiple event channels, all connected using
 * gateways.
 */
class EC_Master
{
public:
  EC_Master (void);

  virtual ~EC_Master (void);

  /// Execute the test.
  virtual int run (int argc, char* argv[]);

  /// Obtain the orb and the poa pointers
  virtual void initialize_orb_and_poa (int& argc, char* argv[]
                                       ACE_ENV_ARG_DECL);

  /// Accessors
  int channel_count (void) const;
  EC_Observer* channel (int i) const;

private:
  int parse_args (int &argc, char *argv []);

private:
  /// The seed
  ACE_RANDR_TYPE seed_;

  /// The driver programs
  int n_channels_;
  EC_Observer** channels_;

  /// The ORB
  CORBA::ORB_var orb_;

  /// The Root POA
  PortableServer::POA_var root_poa_;
};

/**
 * @class EC_Observer
 *
 * @brief Test the EC observers
 *
 */
class EC_Observer : public EC_Driver
{
public:
  /// Constructor
  EC_Observer (EC_Master *master,
               ACE_RANDR_TYPE seed,
               CORBA::ORB_ptr orb,
               PortableServer::POA_ptr root_poa,
               int id);

  /// Destructor
  ~EC_Observer (void);

  // = The EC_Driver methods
  /// add some command line args to enable/disable observerions
  virtual void initialize_orb_and_poa (int& argc, char* argv[]
                                       ACE_ENV_ARG_DECL);
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);

  /// Run the suppliers, using the <thread_manager> parameter
  void execute_test (ACE_ENV_SINGLE_ARG_DECL);
  void run_cleanup (ACE_ENV_SINGLE_ARG_DECL);

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

  /// lock internal state
  TAO_SYNCH_MUTEX lock_;
};

#endif /* EC_OBSERVER_H */
