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

  int run (CORBA_Environment &ACE_TRY_ENV);
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

  const char *load_balancer_file_;
  // The load balancing service IOR is stored in this file

  Round_Robin_Strategy round_robin_;

  LoadBalancer_Impl *balancer_;
  // The load balancer

  CORBA::ORB_var orb_;
  // The ORB

  PortableServer::POA_var root_poa_;
  // The root POA
};

#endif /* TAO_LOADBALANCER_H */
