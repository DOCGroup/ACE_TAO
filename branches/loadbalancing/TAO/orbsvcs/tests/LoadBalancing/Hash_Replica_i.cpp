// -*- C++ -*-
// $Id$

#include "ace/ACE.h"

#include "Hash_Replica_i.h"


Hash_Replica_Impl::Hash_Replica_Impl (CORBA::Object_ptr load_balanced_group)
  : load_balanced_group_ (load_balanced_group),
    reject_requests_ (0),
    requests_ (0),
    start_ (0, 0)
{
  // Nothing else
}

CORBA::ULong
Hash_Replica_Impl::do_hash (const char *str,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  if (this->reject_requests_)
    ACE_THROW_RETURN (PortableServer::ForwardRequest (
                        CORBA::Object::_duplicate (
                          this->load_balanced_group_.in ())),
                      0);

  // Update the load (requests per second)

  if (this->requests_ == 0)
    this->start_ = ACE_OS::gettimeofday ();

  ACE_Time_Value elapsed_time =
    ACE_OS::gettimeofday () - this->start_;

  this->requests_++;
  CORBA::Float load = CORBA::Float(this->requests_) / elapsed_time.sec ();

  this->balancer_proxy_->current_load (load,
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // If 20 seconds have elapsed, then reset the load measurements.
  if (elapsed_time.sec () >= 20)
    {
      this->start_ = ACE_OS::gettimeofday ();
      this->requests_ = 0;
    }

  // Return the hash.
  return ACE::hash_pjw (str);
}

void
Hash_Replica_Impl::set_proxy (LoadBalancing::ReplicaProxy_ptr proxy)
{
  this->balancer_proxy_ = proxy;
}

void
Hash_Replica_Impl::reject_requests (int reject)
{
  this->reject_requests_ = reject;
}
