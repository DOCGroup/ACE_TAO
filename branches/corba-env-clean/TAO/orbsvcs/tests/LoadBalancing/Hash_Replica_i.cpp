// -*- C++ -*-
// $Id$

#include "ace/ACE.h"

#include "Hash_Replica_i.h"
#include "Hash_ReplicaControl.h"

Hash_Replica_Impl::Hash_Replica_Impl (Hash_ReplicaControl *control)
  : control_ (control),
    reject_requests_ (0)
{
  // Nothing else
}

CORBA::ULong
Hash_Replica_Impl::do_hash (const char *str
                            TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reject_requests_)
    {
      this->control_->request_rejected (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      return 0;
    }

  this->control_->request_received ();

  // Return the hash.
  return ACE::hash_pjw (str);
}

void
Hash_Replica_Impl::reject_requests (int reject)
{
  this->reject_requests_ = reject;
}
