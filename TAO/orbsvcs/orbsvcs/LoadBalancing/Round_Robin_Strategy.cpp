// -*- C++ -*-

// $Id$


#include "Round_Robin_Strategy.h"
#include "ReplicaProxy.h"

ACE_RCSID(orbsvcs, Round_Robin_Strategy, "$Id$")


Round_Robin_Strategy::Round_Robin_Strategy (void)
  : proxies_ (),
    next_replica_ (this->proxies_.begin ())
{
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
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::Object::_nil ());
    }

  // If we're at the end of the Replica set, then rewind to the
  // beginning of the set.
  if (this->next_replica_ == this->proxies_.end ())
    this->next_replica_ = this->proxies_.begin ();

  ReplicaProxy_Impl * proxy_servant = *(this->next_replica_);

  // Advance to the next Replica in the set in preparation for the
  // next call to this method.
  this->next_replica_++;

  return proxy_servant->replica ();
}

int
Round_Robin_Strategy::insert (ReplicaProxy_Impl *proxy)
{
  int r = this->proxies_.insert (proxy);
  this->next_replica_ = this->proxies_.begin ();
  return r;
}

int
Round_Robin_Strategy::remove (ReplicaProxy_Impl *proxy)
{
  int r = this->proxies_.remove (proxy);
  this->next_replica_ = this->proxies_.begin ();
  return r;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ReplicaProxy_Impl*>;
template class ACE_Unbounded_Set<ReplicaProxy_Impl*>;
template class ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl*>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ReplicaProxy_Impl*>
#pragma instantiate ACE_Unbounded_Set<ReplicaProxy_Impl*>
#pragma instantiate ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
