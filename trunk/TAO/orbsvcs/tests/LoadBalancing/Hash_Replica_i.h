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

#ifndef TAO_HASH_REPLICA_I_H
#define TAO_HASH_REPLICA_I_H

#include "Hash_ReplicaS.h"

class Hash_ReplicaControl;

class Hash_Replica_Impl : public virtual POA_Hash_Replica
{
public:
  Hash_Replica_Impl (Hash_ReplicaControl *control);
  // Constructor

  virtual CORBA::ULong do_hash (const char * str
                                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Perform a simple hashing operation on the input string <str> and
  // return the hashed string <hash_str>.

  void reject_requests (int reject);
  // Inform the Replica that it should or should not accept client
  // requests.  0 == accept requests, ~0 == reject requests

private:
  Hash_ReplicaControl *control_;
  // The control

  int reject_requests_;
  // Flag that indicates whether the servant should reject all client
  // requests.
};

#endif  /* TAO_HASH_REPLICA_I_H */
