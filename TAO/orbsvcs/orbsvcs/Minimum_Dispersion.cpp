// -*- C++ -*-

// $Id$


#include "Minimum_Dispersion.h"
#include "ReplicaProxy.h"

ACE_RCSID(orbsvcs, Minimum_Dispersion, "$Id$")


TAO_LB_Minimum_Dispersion_Strategy::TAO_LB_Minimum_Dispersion_Strategy (void)
  : proxies_ (),
    lock_ ()
{
}

TAO_LB_Minimum_Dispersion_Strategy::~TAO_LB_Minimum_Dispersion_Strategy (void)
{
  ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                     guard,
                     this->lock_));

  TAO_LB_ReplicaProxySetIterator begin = this->proxies_.begin ();
  TAO_LB_ReplicaProxySetIterator end = this->proxies_.end ();

  // @@ Bad deactivation!

  for (TAO_LB_ReplicaProxySetIterator i = begin;
       i != end;
       ++i)
    {
      // Decrease reference count on each proxy servant in the set.
      (*i)->_remove_ref ();
    }
}

CORBA::Object_ptr
TAO_LB_Minimum_Dispersion_Strategy::replica (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for ( ; ; )
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->lock_,
                        CORBA::Object::_nil ());

      if (this->proxies_.is_empty ())
        // @@ What do we do if the set is empty?
        ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                          CORBA::Object::_nil ());

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

      // Before returning an object reference to the client
      // validate it first.
      CORBA::Object_var object =
        proxy->replica ();

      {
        ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (this->lock_);

        ACE_GUARD_RETURN (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                          reverse_guard,
                          reverse_lock,
                          CORBA::Object::_nil ());

        // @@ Ossama: we should setup a timeout policy here...
        ACE_TRY
          {
            CORBA::Boolean non_existent =
              object->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      }

      // @@ Ossama: a bit melodramatic, we remove the object if *any*
      // exception is thrown.  If the object really does not exist (we
      // get non_existent==1) then this is exactly what we want to do,
      // but if we get something like TRANSIENT we may want to do
      // something less drastic, or at least strategize it ;-)
      this->proxies_.remove (proxy);
    }
}

int
TAO_LB_Minimum_Dispersion_Strategy::insert (TAO_LB_ReplicaProxy *proxy)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            -1));

  return this->proxies_.insert (proxy);
}

int
TAO_LB_Minimum_Dispersion_Strategy::remove (TAO_LB_ReplicaProxy *proxy)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            -1));

  return this->proxies_.remove (proxy);
}

void
TAO_LB_Minimum_Dispersion_Strategy::load_changed (TAO_LB_ReplicaProxy *proxy
                                               ACE_ENV_ARG_DECL)
{
  int send_load_advisory = 0;

  {
    ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                       guard,
                       this->lock_));

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

    // @@ Ossama: Make the 1.5 factor adjustable, it is how much
    // dispersion we tolerate before starting to send advisories.
    if (relative_load > 1 + 1.5F / n)
      {
        proxy->has_high_load_ = 1;
        send_load_advisory = 2;  // 2 == Send high load advisory
      }

    if (send_load_advisory == 1 && relative_load < 1 + 0.9F / n)
      {
        proxy->has_high_load_ = 0;
        send_load_advisory = 1;  // 1 == Send nominal load advisory
      }
  }

  // @@ Ossama: no debug messages in production code, my fault....
  // ACE_DEBUG ((LM_DEBUG, "Load[%x] %f %f %f\n",
  //             proxy, cl, avg, relative_load));

  // @@ Ossama: Make the 1.5 factor adjustable, it is how much
  // dispersion we tolerate before starting to send advisories.
  if (send_load_advisory == 2)
    {
      proxy->control_->high_load_advisory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;  // We may not throw an exception, so explicitly return.
    }

  // @@ Ossama: notice that we wait until the load is signifcantly
  // lower before sending the nominal load advisory, it does not
  // matter that much because the replicas automatically restart after
  // rejecting one client....
  // @@ Ossama: make the 0.9 factor adjustable, at least at
  // construction time...
  if (send_load_advisory == 1)
    {
      proxy->control_->nominal_load_advisory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}
