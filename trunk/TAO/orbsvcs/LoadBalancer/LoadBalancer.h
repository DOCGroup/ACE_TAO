// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   orbsvcs/LoadBalancer
//
// = FILENAME
//   LoadBalancer.h
//
// = DESCRIPTION
//   Implement the LoadBalancing service as a standalone process.
//
// = AUTHORS
//   Ossama Othman (ossama@uci.edu)
//
// ============================================================================

#ifndef TAO_LOADBALANCER_H
#define TAO_LOADBALANCER_H

#include "orbsvcs/LoadBalancing/LoadBalancer_i.h"
#include "orbsvcs/LoadBalancing/Round_Robin_Strategy.h"
#include "orbsvcs/LoadBalancing/Minimum_Dispersion.h"

class TAO_LoadBalancer
{
  // = TITLE
  //   Defines a class that encapsulates the implementation of the
  //   LoadBalancing Service
  //
  // = DESCRIPTION
public:
  TAO_LoadBalancer (void);
  // Default Constructor.

  ~TAO_LoadBalancer (void);
  // Destructor.

  void init (int argc, char *argv[],
             CORBA::Environment &ACE_TRY_ENV);
  // Initialize the service

  void run (CORBA_Environment &ACE_TRY_ENV);
  // Run the TAO_Loadbalancer.

protected:
  void parse_args (int argc, char *argv[],
                   CORBA::Environment &ACE_TRY_ENV);
  // parses the arguments.

  void usage (const char *msg) const;
  // Print a usage message

private:
  const char *interface_repository_id_;
  // The interface repository ID for the object we load balance.

  // @@ Ossama: the current version just dumps the IOR to a file, we
  // may want to integrate it with the naming service too.
  const char *load_balancer_file_;
  // The load balancing service IOR is stored in this file

  int strategy_;
  // Select the right strategy

  // @@ Ossama: the strategies should be created by a factory, the
  // factory should be a Service Object that can be dynamically
  // loaded.
  // @@ Ossama: here is more food for thought, can we provide a
  // generic load balancing service that can implement static,
  // per-request and dynamic load balancing?  If not, can we share the
  // current strategies among them?
  TAO_LB_Round_Robin_Strategy round_robin_;
  TAO_LB_Minimum_Dispersion_Strategy minimum_dispersion_;

  TAO_LB_LoadBalancer *balancer_;
  // The load balancer

  CORBA::ORB_var orb_;
  // The ORB

  PortableServer::POA_var root_poa_;
  // The root POA
};

#endif /* TAO_LOADBALANCER_H */
