// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/tests/LoadBalancing
//
// = FILENAME
//    Hash_Replica_i.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#include "orbsvcs/LoadBalancingS.h"

#include "Hash_ReplicaS.h"

#ifndef TAO_HASH_REPLICA_I_H
#define TAO_HASH_REPLICA_I_H


class Hash_Replica_Impl : public virtual POA_Hash_Replica
{
public:
  Hash_Replica_Impl (CORBA::Object_ptr load_balanced_group);
  // Constructor

  virtual CORBA::ULong do_hash (const char * str,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));
  // Perform a simple hashing operation on the input string <str> and
  // return the hashed string <hash_str>.

  void reject_requests (int reject);
  // Inform the Replica that it should or should not accept client
  // requests.  0 == accept requests, ~0 == reject requests

private:
  CORBA::Object_var load_balanced_group_;
  // Reference that represents the collective identity of replicas
  // being load balanced.

  LoadBalancing::ReplicaProxy_var balancer_proxy_;
  // Object reference to the load balancer proxy.

  int reject_requests_;
  // Flag that indicates whether the servant should reject all client
  // requests.

  CORBA::ULong requests_;
  // The number of requests since load measurements started.

  ACE_Time_Value start_;
  // Time when load measurements started.
};


#endif  /* TAO_HASH_REPLICA_I_H */

