// -*- C++ -*-

// $Id$


#include "orbsvcs/LoadBalancing/Round_Robin_Strategy.h"

ACE_RCSID(orbsvcs, Round_Robin_Strategy, "$Id$")


Round_Robin_Strategy::Round_Robin_Strategy (void)
  : proxies_ (),
    next_replica_ (this->proxies_.end ())
{
  // Nothing else
}

Round_Robin_Strategy::~Round_Robin_Strategy (void)
{
  ReplicaProxySetIterator begin = this->proxies_.begin ();
  ReplicaProxySetIterator end = this->proxies_.end ();

  for (ReplicaProxySetIterator i = begin;
       i != end;
       ++i)
    {
      // Decrease reference count on each proxy servant in the set.
      (*i)->_remove_ref ();
    }
}

CORBA::Object_ptr
Round_Robin_Strategy::replica (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->proxies_.is_empty ())
    {
      // @@ What do we do if the set is empty?
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CORBA::Object::_nil ());
    }

  // If we're at the end of the Replica set, then rewind to the
  // beginning of the set.
  if (this->next_replica_ == this->proxies_.end ())
    this->next_replica_ == this->proxies_.begin ();

  ReplicaProxy_Impl * proxy_servant = *(this->next_replica_);

  // Advance to the next Replica in the set in preparation for the
  // next call to this method.
  this->next_replica_++;

  return proxy_servant->_this (ACE_TRY_ENV);
}

int
Round_Robin_Strategy::insert (ReplicaProxy_Impl *proxy)
{
  return this->proxies_.insert (proxy)
}

int
Round_Robin_Strategy::remove (ReplicaProxy_Impl *proxy)
{
  return this->proxies_.remove (proxy);
}

