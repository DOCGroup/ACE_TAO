// -*- C++ -*-

// $Id$


#include "Minimum_Dispersion.h"
#include "ReplicaProxy.h"

ACE_RCSID(orbsvcs, Minimum_Dispersion, "$Id$")


Minimum_Dispersion_Strategy::Minimum_Dispersion_Strategy (void)
  : proxies_ ()
{
}

Minimum_Dispersion_Strategy::~Minimum_Dispersion_Strategy (void)
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
Minimum_Dispersion_Strategy::replica (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->proxies_.is_empty ())
    {
      // @@ What do we do if the set is empty?
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::Object::_nil ());
    }

  ReplicaProxySetIterator begin = this->proxies_.begin ();
  ReplicaProxySetIterator end = this->proxies_.end ();

  float s = 0;
  CORBA::ULong n = 0;
  ReplicaProxySetIterator i = begin;
  for (;i != end; ++i)
    {
      s += (*i)->current_load ();
      n++;
    }

  float avg = s / n;

  i = begin;
  ReplicaProxy_Impl * proxy_servant = (*i);
  float d = 0;
  if (avg > (*i)->current_load ())
    d = avg - (*i)->current_load ();

  for (++i ; i != end; ++i)
    {
      if (avg <= (*i)->current_load ())
        continue;
      if (d < (avg - (*i)->current_load ()))
        {
          proxy_servant = *i;
          d = avg - (*i)->current_load ();
        }
    }

  return proxy_servant->replica ();
}

int
Minimum_Dispersion_Strategy::insert (ReplicaProxy_Impl *proxy)
{
  return this->proxies_.insert (proxy);
}

int
Minimum_Dispersion_Strategy::remove (ReplicaProxy_Impl *proxy)
{
  return this->proxies_.remove (proxy);
}

void
Minimum_Dispersion_Strategy::load_changed (ReplicaProxy_Impl *proxy,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->proxies_.is_empty ())
    return;

  ReplicaProxySetIterator begin = this->proxies_.begin ();
  ReplicaProxySetIterator end = this->proxies_.end ();

  float s = 0;
  CORBA::ULong n = 0;
  ReplicaProxySetIterator i = begin;
  for (;i != end; ++i)
    {
      s += (*i)->current_load ();
      n++;
    }

  float avg = s / n;
  float cl = proxy->current_load ();

  if (avg == 0)
    return;

  float relative_load = cl / avg;

  ACE_DEBUG ((LM_DEBUG, "Load %f %f %f\n", cl, avg, relative_load));

  if (relative_load < 0.80)
    {
      proxy->control_->nominal_load_advisory (ACE_TRY_ENV);
      ACE_CHECK;
    }

  if (relative_load > 1.05)
    {
      proxy->control_->high_load_advisory (ACE_TRY_ENV);
      ACE_CHECK;
    }
}
