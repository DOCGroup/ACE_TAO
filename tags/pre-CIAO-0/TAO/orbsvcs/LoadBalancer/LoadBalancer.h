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

  void init (int argc, char *argv[]
             ACE_ENV_ARG_DECL);
  // Initialize the service

  void run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the TAO_Loadbalancer.

protected:
  void parse_args (int argc, char *argv[]
                   ACE_ENV_ARG_DECL);
  // parses the arguments.

  void usage (const char *msg) const;
  // Print a usage message

private:
  const char * repository_id_;
  // The repository ID for the interface of the object being load
  // balanced.

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
