// $Id$

#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/POAManagerFactory.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/poa_macros.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/IORInterceptor_Adapter.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/POAManager.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_POA_Manager::TAO_POA_Manager (
  TAO_Object_Adapter &object_adapter,
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  const char * id,
  const ::CORBA::PolicyList &policies,
  PortableServer::POAManagerFactory_ptr poa_manager_factory)
#else
  const char * id)
#endif
  : state_ (PortableServer::POAManager::HOLDING),
    lock_ (object_adapter.lock ()),
    poa_collection_ (),
    object_adapter_ (object_adapter),
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    id_ (id == 0 ? this->generate_manager_id () : CORBA::string_dup (id)),
    poa_manager_factory_ (* dynamic_cast <TAO_POAManager_Factory*> (poa_manager_factory)),
    policies_ (policies)
{
  poa_manager_factory_._add_ref ();
}
#else
    id_ (id == 0 ? this->generate_manager_id () : CORBA::string_dup (id))
{
}
#endif


TAO_POA_Manager::~TAO_POA_Manager (void)
{
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  poa_manager_factory_._remove_ref ();
#endif
}

char *
TAO_POA_Manager::get_id (void)
{
  return CORBA::string_dup (this->id_.in ());
}

void
TAO_POA_Manager::activate_i (void)
{
  // This operation changes the state of the POA manager to active. If
  // issued while the POA manager is in the inactive state, the
  // AdapterInactive exception is raised.  Entering the active state
  // enables the associated POAs to process requests.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      throw PortableServer::POAManager::AdapterInactive ();
    }
  else
    {
      this->state_ = PortableServer::POAManager::ACTIVE;
      // Find the poas that applied the custom servant dispatching
      // strategy to launch the dispatching threads.

      for (POA_COLLECTION::iterator iterator = this->poa_collection_.begin ();
       iterator != this->poa_collection_.end ();
       ++iterator)
        {
          (*iterator)->poa_activated_hook ();
        }
    }

  this->adapter_manager_state_changed (this->state_);
}

void
TAO_POA_Manager::deactivate_i (CORBA::Boolean etherealize_objects,
                               CORBA::Boolean wait_for_completion)
{
  // Is the @a wait_for_completion semantics for this thread correct?
  TAO_Root_POA::check_for_valid_wait_for_completions (this->object_adapter_.orb_core (),
                                                      wait_for_completion);

  // This operation changes the state of the POA manager to
  // inactive. If issued while the POA manager is in the inactive
  // state, this operation has not effect.  Entering the
  // inactive state causes the associated POAs to reject requests that
  // have not begun to be executed as well as any new requests.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      return;
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
      TAO_Root_POA *poa = *iterator;
      // Notify the poas that applied the custom servant dispatching
      // strategy to stop the dispatching threads.
      poa->poa_deactivated_hook ();

      poa->deactivate_all_objects_i (etherealize_objects, wait_for_completion);
    }

  // If the ORB::shutdown operation is called, it makes a call on
  // deactivate with a TRUE etherealize_objects parameter for each POA
  // manager known in the process; the wait_for_completion parameter
  // to deactivate will be the same as the similarly named parameter
  // of ORB::shutdown.
  this->adapter_manager_state_changed (this->state_);
}

void
TAO_POA_Manager::adapter_manager_state_changed (PortableServer::POAManager::State state)
{
  PortableInterceptor::AdapterState adapter_state =
    static_cast<PortableInterceptor::AdapterState> (state);

  TAO_IORInterceptor_Adapter *ior_adapter =
    this->object_adapter_.orb_core ().ior_interceptor_adapter ();

  if (ior_adapter)
    {
      ior_adapter->adapter_manager_state_changed (this->id_.in (),
                                                  adapter_state);
    }
}

#if (TAO_HAS_MINIMUM_POA == 0)

void
TAO_POA_Manager::hold_requests_i (CORBA::Boolean wait_for_completion)
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_Root_POA::check_for_valid_wait_for_completions (this->object_adapter_.orb_core (),
                                                      wait_for_completion);

  // This operation changes the state of the POA manager to
  // holding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // holding state causes the associated POAs to queue incoming
  // requests.  Any requests that have been queued but have not
  // started executing will continue to be queued while in the holding
  // state.

  if (this->state_ == PortableServer::POAManager::INACTIVE)
    {
      throw PortableServer::POAManager::AdapterInactive ();
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
          TAO_Root_POA *poa = *iterator;
          poa->wait_for_completions (wait_for_completion);
        }
    }

  this->adapter_manager_state_changed (this->state_);
}

void
TAO_POA_Manager::discard_requests_i (CORBA::Boolean wait_for_completion)
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_Root_POA::check_for_valid_wait_for_completions (this->object_adapter_.orb_core (),
                                                      wait_for_completion);

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
      throw PortableServer::POAManager::AdapterInactive ();
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
          TAO_Root_POA *poa = *iterator;
          poa->wait_for_completions (wait_for_completion);
        }
    }

  this->adapter_manager_state_changed (this->state_);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

int
TAO_POA_Manager::remove_poa (TAO_Root_POA *poa)
{
  int const result = this->poa_collection_.remove (poa);

  // The #if really only needs to go around the
  // "this->poa_manager_factory_.remove_poamanager (this);" line, but it's
  // moved out as an optimization for now.  If additional non-CORBA/e and
  // non-minimum POA code needs to go in that clause the #if would have to
  // move back in.

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  if (result == 0)
    {
      if (this->poa_collection_.is_empty ())
        {
          this->poa_manager_factory_.remove_poamanager (this);
        }
    }
#endif
  return result;
}

int
TAO_POA_Manager::register_poa (TAO_Root_POA *poa)
{
  return this->poa_collection_.insert (poa);
}

void
TAO_POA_Manager::check_state (void)
{
  if (state_ == PortableServer::POAManager::ACTIVE)
    {
      // When a POA manager is in the active state, the associated
      // POAs will receive and start processing requests (assuming
      // that appropriate thread resources are available).
      return;
    }

  if (state_ == PortableServer::POAManager::DISCARDING)
    {
      // When a POA manager is in the discarding state, the associated
      // POAs will discard all incoming requests (whose processing has
      // not yet begun). When a request is discarded, the TRANSIENT
      // system exception, with standard minor code 1, must be
      // returned to the client-side to indicate that the request
      // should be re-issued. (Of course, an ORB may always reject a
      // request for other reasons and raise some other system
      // exception.)
      throw
        CORBA::TRANSIENT (
          CORBA::SystemException::_tao_minor_code (
            TAO_POA_DISCARDING,
            1),
          CORBA::COMPLETED_NO);
    }

  if (state_ == PortableServer::POAManager::HOLDING)
    {
      // When a POA manager is in the holding state, the associated
      // POAs will queue incoming requests. The number of requests
      // that can be queued is an implementation limit. If this limit
      // is reached, the POAs may discard requests and return the
      // TRANSIENT system exception, with standard minor code 1, to
      // the client to indicate that the client should reissue the
      // request. (Of course, an ORB may always reject a request for
      // other reasons and raise some other system exception.)

      // Since there is no queuing in TAO, we immediately raise a
      // TRANSIENT exception.
      throw ::CORBA::TRANSIENT (
                CORBA::SystemException::_tao_minor_code (TAO_POA_HOLDING, 1),
                CORBA::COMPLETED_NO);
    }

  if (state_ == PortableServer::POAManager::INACTIVE)
    {
      // The inactive state is entered when the associated POAs are to
      // be shut down. Unlike the discarding state, the inactive state
      // is not a temporary state. When a POA manager is in the
      // inactive state, the associated POAs will reject new
      // requests. The rejection mechanism used is specific to the
      // vendor. The GIOP location forwarding mechanism and
      // CloseConnection message are examples of mechanisms that could
      // be used to indicate the rejection. If the client is
      // co-resident in the same process, the ORB could raise the
      // OBJ_ADAPTER system exception, with standard minor code 1, to
      // indicate that the object implementation is unavailable.
      throw ::CORBA::OBJ_ADAPTER (
                CORBA::SystemException::_tao_minor_code (TAO_POA_INACTIVE, 1),
                CORBA::COMPLETED_NO);
    }
}

CORBA::ORB_ptr
TAO_POA_Manager::_get_orb (void)
{
  return CORBA::ORB::_duplicate (this->object_adapter_.orb_core ().orb ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
