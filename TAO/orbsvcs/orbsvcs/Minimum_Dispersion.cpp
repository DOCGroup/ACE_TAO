// -*- C++ -*-

// $Id$


#include "Minimum_Dispersion.h"
#include "ReplicaProxy.h"

ACE_RCSID(orbsvcs, Minimum_Dispersion, "$Id$")


TAO_LB_Minimum_Dispersion_Strategy::TAO_LB_Minimum_Dispersion_Strategy (void)
  : proxies_ ()
{
}

TAO_LB_Minimum_Dispersion_Strategy::~TAO_LB_Minimum_Dispersion_Strategy (void)
{
  // @@ Ossama: more code that is not thread safe
  TAO_LB_ReplicaProxySetIterator begin = this->proxies_.begin ();
  TAO_LB_ReplicaProxySetIterator end = this->proxies_.end ();

  for (TAO_LB_ReplicaProxySetIterator i = begin;
       i != end;
       ++i)
    {
      // Decrease reference count on each proxy servant in the set.
      (*i)->_remove_ref ();
    }
}

CORBA::Object_ptr
TAO_LB_Minimum_Dispersion_Strategy::replica (CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Ossama: more code that is not thread safe
  while (!this->proxies_.is_empty ())
    {
      TAO_LB_ReplicaProxySetIterator begin = this->proxies_.begin ();
      TAO_LB_ReplicaProxySetIterator end = this->proxies_.end ();

      TAO_LB_ReplicaProxySetIterator i = begin;
      TAO_LB_ReplicaProxy * proxy = (*i);
      float d = (*i)->current_load ();

      for (++i ; i != end; ++i)
        {
          if (d > (*i)->current_load ())
            {
              proxy = *i;
              d = (*i)->current_load ();
            }
        }

      // @@ Ossama: we should setup a timeout policy here...
      ACE_TRY
        {
          // Before returning an object reference to the client
          // validate it first.
          CORBA::Object_var object =
            proxy->replica ();
          CORBA::Boolean non_existent =
            object->_non_existent (ACE_TRY_ENV);
          ACE_TRY_CHECK;
          if (!non_existent)
            {
              return object._retn ();
            }
        }
      ACE_CATCHANY
        {
        }
      ACE_ENDTRY;
      // @@ Ossama: a bit melodramatic, we remove the object if *any*
      // exception is thrown.  If the object really does not exist (we
      // get non_existent==1) then this is exactly what we want to do,
      // but if we get something like TRANSIENT we may want to do
      // something less drastic, or at least strategize it ;-)
      this->proxies_.remove (proxy);
    }
  // @@ What do we do if the set is empty?
  ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                    CORBA::Object::_nil ());
}

int
TAO_LB_Minimum_Dispersion_Strategy::insert (TAO_LB_ReplicaProxy *proxy)
{
  // @@ Ossama: more code that is not thread safe
  return this->proxies_.insert (proxy);
}

int
TAO_LB_Minimum_Dispersion_Strategy::remove (TAO_LB_ReplicaProxy *proxy)
{
  // @@ Ossama: more code that is not thread safe
  return this->proxies_.remove (proxy);
}

void
TAO_LB_Minimum_Dispersion_Strategy::load_changed (TAO_LB_ReplicaProxy *proxy,
                                               CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Ossama: more code that is not thread safe
  if (this->proxies_.is_empty ())
    return;

  TAO_LB_ReplicaProxySetIterator begin = this->proxies_.begin ();
  TAO_LB_ReplicaProxySetIterator end = this->proxies_.end ();

  float s = 0;
  CORBA::ULong n = 0;
  TAO_LB_ReplicaProxySetIterator i = begin;
  for (;i != end; ++i)
    {
      s += (*i)->current_load ();
      n++;
    }

  float avg = (n == 0 ? s : s / n);
  float cl = proxy->current_load ();

  if (avg == 0)
    return;

  float relative_load = cl / avg;

  // @@ Ossama: no debug messages in production code, my fault....
  ACE_DEBUG ((LM_DEBUG, "Load[%x] %f %f %f\n",
              proxy, cl, avg, relative_load));

  // @@ Ossama: Make the 1.5 factor adjustable, it is how much
  // dispersion we tolerate before starting to send advisories.
  if (relative_load > 1 + 1.5F / n)
    {
      proxy->has_high_load_ = 1;
      proxy->control_->high_load_advisory (ACE_TRY_ENV);
      ACE_CHECK;
      return;
    }

  // @@ Ossama: notice that we wait until the load is signifcantly
  // lower before sending the nominal load advisory, it does not
  // matter that much because the replicas automatically restart after
  // rejecting one client....
  // @@ Ossama: make the 0.9 factor adjustable, at least at
  // construction time...
  if (proxy->has_high_load_ && relative_load < 1 + 0.9F / n)
    {
      proxy->has_high_load_ = 0;
      proxy->control_->nominal_load_advisory (ACE_TRY_ENV);
      ACE_CHECK;
    }
}
