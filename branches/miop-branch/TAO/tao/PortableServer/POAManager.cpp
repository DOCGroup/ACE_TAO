// $Id$

#include "POAManager.h"
#include "POA.h"
#include "poa_macros.h"
#include "tao/Server_Strategy_Factory.h"

#if !defined (__ACE_INLINE__)
# include "POAManager.i"
#endif /* ! __ACE_INLINE__ */

TAO_POA_Manager::TAO_POA_Manager (TAO_Object_Adapter &object_adapter)
  : state_ (PortableServer::POAManager::HOLDING),
    lock_ (object_adapter.lock ()),
    poa_collection_ (),
    object_adapter_ (object_adapter)
{
}

TAO_POA_Manager::~TAO_POA_Manager (void)
{
}

void
TAO_POA_Manager::activate_i (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POAManager::AdapterInactive))
{
  // This operation changes the state of the POA manager to active. If
  // issued while the POA manager is in the inactive state, the
  // AdapterInactive exception is raised.  Entering the active state
  // enables the associated POAs to process requests.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      ACE_THROW (PortableServer::POAManager::AdapterInactive ());
    }
  else
    {
      this->state_ = PortableServer::POAManager::ACTIVE;
    }
}

void
TAO_POA_Manager::deactivate_i (CORBA::Boolean etherealize_objects,
                               CORBA::Boolean wait_for_completion
                               TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POAManager::AdapterInactive))
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_POA::check_for_valid_wait_for_completions (this->object_adapter_.orb_core (),
                                                 wait_for_completion
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // This operation changes the state of the POA manager to
  // inactive. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // inactive state causes the associated POAs to reject requests that
  // have not begun to be executed as well as any new requests.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      ACE_THROW (PortableServer::POAManager::AdapterInactive ());
    }
  else
    {
      this->state_ = PortableServer::POAManager::INACTIVE;
    }

  // After changing the state, if the etherealize_objects parameter is:
  //
  // a) TRUE - the POA manager will cause all associated POAs that
  // have the RETAIN and USE_SERVANT_MANAGER policies to perform the
  // etherealize operation on the associated servant manager for all
  // active objects.
  //
  // b) FALSE - the etherealize operation is not called. The purpose
  // is to provide developers with a means to shut down POAs in a
  // crisis (for example, unrecoverable error) situation.

  // If the wait_for_completion parameter is FALSE, this operation
  // will return immediately after changing the state. If the
  // parameter is TRUE and the current thread is not in an invocation
  // context dispatched by some POA belonging to the same ORB as this
  // POA, this operation does not return until there are no actively
  // executing requests in any of the POAs associated with this POA
  // manager (that is, all requests that were started prior to the
  // state change have completed) and, in the case of a TRUE
  // etherealize_objects, all invocations of etherealize have
  // completed for POAs having the RETAIN and USE_SERVANT_MANAGER
  // policies. If the parameter is TRUE and the current thread is in
  // an invocation context dispatched by some POA belonging to the
  // same ORB as this POA the BAD_INV_ORDER exception is raised and
  // the state is not changed.

  for (POA_COLLECTION::iterator iterator = this->poa_collection_.begin ();
       iterator != this->poa_collection_.end ();
       ++iterator)
    {
      TAO_POA *poa = *iterator;
      poa->deactivate_all_objects_i (etherealize_objects,
                                     wait_for_completion
                                     TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // If the ORB::shutdown operation is called, it makes a call on
  // deactivate with a TRUE etherealize_objects parameter for each POA
  // manager known in the process; the wait_for_completion parameter
  // to deactivate will be the same as the similarly named parameter
  // of ORB::shutdown.
}

#if (TAO_HAS_MINIMUM_POA == 0)

void
TAO_POA_Manager::hold_requests_i (CORBA::Boolean wait_for_completion
                                  TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POAManager::AdapterInactive))
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_POA::check_for_valid_wait_for_completions (this->object_adapter_.orb_core (),
                                                 wait_for_completion
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // This operation changes the state of the POA manager to
  // holding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // holding state causes the associated POAs to queue incoming
  // requests.  Any requests that have been queued but have not
  // started executing will continue to be queued while in the holding
  // state.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      ACE_THROW (PortableServer::POAManager::AdapterInactive ());
    }
  else
    {
      this->state_ = PortableServer::POAManager::HOLDING;
    }

  // If the wait_for_completion parameter is FALSE, this operation
  // returns immediately after changing the state. If the parameter is
  // TRUE and the current thread is not in an invocation context
  // dispatched by some POA belonging to the same ORB as this POA,
  // this operation does not return until either there are no actively
  // executing requests in any of the POAs associated with this POA
  // manager (that is, all requests that were started prior to the
  // state change have completed) or the state of the POA manager is
  // changed to a state other than holding. If the parameter is TRUE
  // and the current thread is in an invocation context dispatched by
  // some POA belonging to the same ORB as this POA the BAD_INV_ORDER
  // exception is raised and the state is not changed.

  if (wait_for_completion)
    {
      for (POA_COLLECTION::iterator iterator = this->poa_collection_.begin ();
           iterator != this->poa_collection_.end ();
           ++iterator)
        {
          TAO_POA *poa = *iterator;
          poa->wait_for_completions (wait_for_completion
                                     TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
TAO_POA_Manager::discard_requests_i (CORBA::Boolean wait_for_completion
                                     TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POAManager::AdapterInactive))
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_POA::check_for_valid_wait_for_completions (this->object_adapter_.orb_core (),
                                                 wait_for_completion
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // This operation changes the state of the POA manager to
  // discarding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // discarding state causes the associated POAs to discard incoming
  // requests.  In addition, any requests that have been queued but
  // have not started executing are discarded. When a request is
  // discarded, a TRANSIENT system exception is returned to the
  // client.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      ACE_THROW (PortableServer::POAManager::AdapterInactive ());
    }
  else
    {
      this->state_ = PortableServer::POAManager::DISCARDING;
    }

  // If the wait_for_completion parameter is FALSE, this operation
  // returns immediately after changing the state. If the
  // parameter is TRUE and the current thread is not in an
  // invocation context dispatched by some POA belonging to the
  // same ORB as this POA, this operation does not return until
  // either there are no actively executing requests in any of the
  // POAs associated with this POA manager (that is, all requests
  // that were started prior to the state change have completed)
  // or the state of the POA manager is changed to a state other
  // than discarding. If the parameter is TRUE and the current
  // thread is in an invocation context dispatched by some POA
  // belonging to the same ORB as this POA the BAD_INV_ORDER
  // exception is raised and the state is not changed.

  if (wait_for_completion)
    {
      for (POA_COLLECTION::iterator iterator = this->poa_collection_.begin ();
           iterator != this->poa_collection_.end ();
           ++iterator)
        {
          TAO_POA *poa = *iterator;
          poa->wait_for_completions (wait_for_completion
                                     TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

int
TAO_POA_Manager::remove_poa (TAO_POA *poa)
{
  int result = this->poa_collection_.remove (poa);

  if (result == 0)
    {
      if (this->poa_collection_.is_empty ())
        {
          CORBA::release (this);
        }
    }

  return result;
}

int
TAO_POA_Manager::register_poa (TAO_POA *poa)
{
  return this->poa_collection_.insert (poa);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set<TAO_POA *>;
template class ACE_Unbounded_Set_Iterator<TAO_POA *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Set<TAO_POA *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_POA *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
