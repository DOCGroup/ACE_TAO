// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Retention_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "Servant_Retention_Strategy.h"
#include "Non_Servant_Upcall.h"
#include "POA.h"

ACE_RCSID (PortableServer,
           Servant_Retention_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
      void
      Servant_Retention_Strategy::strategy_init (
        TAO_POA *poa)
      {
        poa_ = poa;
        // dependent on type create the correct strategy.
      }

    void
    Retain_Servant_Retention_Strategy::deactivate_object (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
    {
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry = 0;
      int result = this->poa_->active_object_map ().
        find_servant_and_system_id_using_user_id (id,
                                                  active_object_map_entry);

      // If there is no active object associated with the specified Object
      // Id, the operation raises an ObjectNotActive exception.
      if (result != 0)
        {
          ACE_THROW (PortableServer::POA::ObjectNotActive ());
        }

      this->deactivate_map_entry (active_object_map_entry
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    void
    Retain_Servant_Retention_Strategy::deactivate_map_entry (
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry
      ACE_ENV_ARG_DECL)
    {
      // Decrement the reference count.
      CORBA::UShort new_count = --active_object_map_entry->reference_count_;

      if (new_count == 0)
        {
          this->cleanup_servant (active_object_map_entry
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          // It should be noted that there may be a period of time between
          // an object's deactivation and the etherealization (during
          // which outstanding requests are being processed) in which
          // arriving requests on that object should not be passed to its
          // servant. During this period, requests targeted for such an
          // object act as if the POA were in holding state until
          // etherealize completes. If etherealize is called as a
          // consequence of a deactivate call with a etherealize_objects
          // parameter of TRUE, incoming requests are rejected.

          // Else mark entry as closed...
          active_object_map_entry->deactivated_ = 1;
        }
    }

    void
    Retain_Servant_Retention_Strategy::cleanup_servant (
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry
      ACE_ENV_ARG_DECL)
    {
      // If a servant manager is associated with the POA,
      // ServantLocator::etherealize will be invoked with the oid and the
      // servant. (The deactivate_object operation does not wait for the
      // etherealize operation to complete before deactivate_object
      // returns.)
      //
      // Note: If the servant associated with the oid is serving multiple
      // Object Ids, ServantLocator::etherealize may be invoked multiple
      // times with the same servant when the other objects are
      // deactivated. It is the responsibility of the object
      // implementation to refrain from destroying the servant while it is
      // active with any Id.

      // If the POA has no ServantActivator associated with it, the POA
      // implementation calls _remove_ref when all operation invocations
      // have completed. If there is a ServantActivator, the Servant is
      // consumed by the call to ServantActivator::etherealize instead.
/*
      // First check for a non-zero servant.
      if (active_object_map_entry->servant_)
        {

    #if (TAO_HAS_MINIMUM_POA == 0)

          if (this->poa_->etherealize_objects_ &&
              this->poa_->cached_policies_.request_processing () == PortableServer::USE_SERVANT_MANAGER &&
              !CORBA::is_nil (this->poa_->servant_activator_.in ()))
            {
              CORBA::Boolean remaining_activations =
                this->poa_->active_object_map ().
                remaining_activations (active_object_map_entry->servant_);

              // A recursive thread lock without using a recursive thread
              // lock.  Non_Servant_Upcall has a magic constructor and
              // destructor.  We unlock the Object_Adapter lock for the
              // duration of the servant activator upcalls; reacquiring
              // once the upcalls complete.  Even though we are releasing
              // the lock, other threads will not be able to make progress
              // since <Object_Adapter::non_servant_upcall_in_progress_>
              // has been set.
              TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this);
              ACE_UNUSED_ARG (non_servant_upcall);

              // If the cleanup_in_progress parameter is TRUE, the reason
              // for the etherealize operation is that either the
              // deactivate or destroy operation was called with an
              // etherealize_objects parameter of TRUE. If the parameter
              // is FALSE, the etherealize operation is called for other
              // reasons.
              this->servant_activator_->etherealize (active_object_map_entry->user_id_,
                                                     this,
                                                     active_object_map_entry->servant_,
                                                     this->cleanup_in_progress_,
                                                     remaining_activations
                                                     ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else

    #endif

            {
              // A recursive thread lock without using a recursive thread
              // lock.  Non_Servant_Upcall has a magic constructor and
              // destructor.  We unlock the Object_Adapter lock for the
              // duration of the servant activator upcalls; reacquiring
              // once the upcalls complete.  Even though we are releasing
              // the lock, other threads will not be able to make progress
              // since <Object_Adapter::non_servant_upcall_in_progress_>
              // has been set.
              TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this);
              ACE_UNUSED_ARG (non_servant_upcall);

              active_object_map_entry->servant_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
  */
      // This operation causes the association of the Object Id specified
      // by the oid parameter and its servant to be removed from the
      // Active Object Map.
      int result = this->poa_->active_object_map ().
        unbind_using_user_id (active_object_map_entry->user_id_);

      if (result != 0)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }
  }
}

namespace TAO
{
  namespace Portable_Server
  {
    void
    Non_Retain_Servant_Retention_Strategy::deactivate_object (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

