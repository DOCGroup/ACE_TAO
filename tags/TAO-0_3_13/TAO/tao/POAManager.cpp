// $Id$

#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/poa_macros.h"
#include "tao/POAManager.h"

#if !defined (__ACE_INLINE__)
# include "tao/POAManager.i"
#endif /* ! __ACE_INLINE__ */

TAO_POA_Manager::TAO_POA_Manager (void)
  : state_ (TAO_POA_Manager::HOLDING),
    closing_down_ (0),
    lock_ (0),
    poa_collection_ ()
{
  this->lock_ = TAO_ORB_Core_instance ()->server_factory ()->create_poa_mgr_lock ();
}

TAO_POA_Manager::~TAO_POA_Manager (void)
{
  delete this->lock_;
}

void
TAO_POA_Manager::activate_i (CORBA::Environment &ACE_TRY_ENV)
{
  // This operation changes the state of the POA manager to active. If
  // issued while the POA manager is in the inactive state, the
  // AdapterInactive exception is raised.  Entering the active state
  // enables the associated POAs to process requests.

  if (this->state_ == TAO_POA_Manager::INACTIVE)
    ACE_THROW (PortableServer::POAManager::AdapterInactive ());
  else
    this->state_ = TAO_POA_Manager::ACTIVE;
}

void
TAO_POA_Manager::hold_requests_i (CORBA::Boolean wait_for_completion,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // This operation changes the state of the POA manager to
  // holding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // holding state causes the associated POAs to queue incoming
  // requests.  Any requests that have been queued but have not
  // started executing will continue to be queued while in the holding
  // state.

  if (this->state_ == TAO_POA_Manager::INACTIVE)
    ACE_THROW (PortableServer::POAManager::AdapterInactive ());
  else
    this->state_ = TAO_POA_Manager::HOLDING;

  // If the wait_for_completion parameter is FALSE, this operation
  // returns immediately after changing the state. If the parameter is
  // TRUE, this operation does not return until either there are no
  // actively executing requests in any of the POAs associated with
  // this POA manager (that is, all requests that were started prior
  // to the state change have completed) or the state of the POA
  // manager is changed to a state other than holding.

  ACE_UNUSED_ARG (wait_for_completion);
}

void
TAO_POA_Manager::discard_requests_i (CORBA::Boolean wait_for_completion,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  // This operation changes the state of the POA manager to
  // discarding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // discarding state causes the associated POAs to discard incoming
  // requests.  In addition, any requests that have been queued but
  // have not started executing are discarded. When a request is
  // discarded, a TRANSIENT system exception is returned to the
  // client.

  if (this->state_ == TAO_POA_Manager::INACTIVE)
    ACE_THROW (PortableServer::POAManager::AdapterInactive ());
  else
    this->state_ = TAO_POA_Manager::DISCARDING;

  // If the wait_for_completion parameter is FALSE, this operation
  // returns immediately after changing the state. If the parameter is
  // TRUE, this operation does not return until either there are no
  // actively executing requests in any of the POAs associated with
  // this POA manager (that is, all requests that were started prior
  // to the state change have completed) or the state of the POA
  // manager is changed to a state other than discarding.

  ACE_UNUSED_ARG (wait_for_completion);
}

void
TAO_POA_Manager::deactivate_i (CORBA::Boolean etherealize_objects,
                               CORBA::Boolean wait_for_completion,
                               CORBA::Environment &ACE_TRY_ENV)
{
  this->closing_down_ = 1;

  // This operation changes the state of the POA manager to
  // inactive. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // inactive state causes the associated POAs to reject requests that
  // have not begun to be executed as well as any new requests.

  if (this->state_ == TAO_POA_Manager::INACTIVE)
    ACE_THROW (PortableServer::POAManager::AdapterInactive ());
  else
    this->state_ = TAO_POA_Manager::INACTIVE;

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

  for (POA_COLLECTION::iterator iterator = this->poa_collection_.begin ();
       iterator != this->poa_collection_.end ();
       ++iterator)
    {
      TAO_POA *poa = *iterator;
      poa->destroy (etherealize_objects,
                    wait_for_completion,
                    ACE_TRY_ENV);
//       poa->etherealize_servants (etherealize_objects,
//                                  wait_for_completion,
//                                  ACE_TRY_ENV);
      ACE_CHECK;
    }

  // If the wait_for_completion parameter is FALSE, this operation
  // will return immediately after changing the state. If the
  // parameter is TRUE, this operation does not return until there are
  // no actively executing requests in any of the POAs associated with
  // this POA manager (that is, all requests that were started prior
  // to the state change have completed) and, in the case of a TRUE
  // etherealize_objects, all invocations of etherealize have
  // completed for POAs having the RETAIN and USE_SERVANT_MANAGER
  // policies.

  // If the ORB::shutdown operation is called, it makes a call on
  // deactivate with a TRUE etherealize_objects parameter for each POA
  // manager known in the process; the wait_for_completion parameter
  // to deactivate will be the same as the similarly named parameter
  // of ORB::shutdown.
}

TAO_POA_Manager::Processing_State
TAO_POA_Manager::state (CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), TAO_POA_Manager::UNKNOWN, ACE_TRY_ENV);

  return this->state_;
}

void
TAO_POA_Manager::remove_poa (TAO_POA *poa,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // If we are not closing down, we must remove this poa from our
  // collection.
  if (!this->closing_down_)
    {
      // Lock access to the POAManager for the duration of this transaction
      TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), ACE_TRY_ENV);

      this->remove_poa_i (poa,
                          ACE_TRY_ENV);
    }

  // If we are closing down, we are currently iterating over our poa
  // collection and there is not need to remove this poa from our
  // collection.

  // @@ This may cause segfault if another thread gets a hold at this
  // POAManager but gets blocked on register POA waiting for
  // remove_poa to complete.  I think we need to use the client side mapping
  // to refcount this.
  if (this->poa_collection_.is_empty ())
    this->destroy ();
}

void
TAO_POA_Manager::remove_poa_i (TAO_POA *poa,
                               CORBA::Environment &ACE_TRY_ENV)
{
  int result = this->poa_collection_.remove (poa);

  if (result != 0)
    ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO));
}

void
TAO_POA_Manager::register_poa (TAO_POA *poa,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), ACE_TRY_ENV);

  this->register_poa_i (poa,
                        ACE_TRY_ENV);
}

void
TAO_POA_Manager::register_poa_i (TAO_POA *poa,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  int result = this->poa_collection_.insert (poa);

  if (result != 0)
    ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO));
}
