// $Id$

#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Default_Servant_Dispatcher.h"
#include "tao/PortableServer/Collocated_Object_Proxy_Broker.h"
#include "tao/PortableServer/Active_Object_Map_Entry.h"
#include "tao/PortableServer/ForwardRequestC.h"

// -- TAO Include --
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_Upcall.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_string.h"

ACE_RCSID (PortableServer,
           Servant_Upcall,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    Servant_Upcall::Servant_Upcall (TAO_ORB_Core *oc)
      : object_adapter_ (0),
        poa_ (0),
        servant_ (0),
        state_ (INITIAL_STAGE),
        system_id_ (TAO_POA_OBJECT_ID_BUF_SIZE, 0, system_id_buf_),
        user_id_ (0),
        current_context_ (),
#if (TAO_HAS_MINIMUM_POA == 0)
        cookie_ (0),
        operation_ (0),
#endif /* TAO_HAS_MINIMUM_POA == 0 */
        active_object_map_entry_ (0)
    {
      TAO_Object_Adapter *object_adapter =
        dynamic_cast<TAO_Object_Adapter *>(oc->poa_adapter ());
      this->object_adapter_ = object_adapter;
    }

    int
    Servant_Upcall::prepare_for_upcall (
      const TAO::ObjectKey &key,
      const char *operation,
      CORBA::Object_out forward_to
      ACE_ENV_ARG_DECL)
    {
      while (1)
        {
          int wait_occurred_restart_call = 0;

          int result =
            this->prepare_for_upcall_i (key,
                                        operation,
                                        forward_to,
                                        wait_occurred_restart_call
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

          if (result == TAO_Adapter::DS_FAILED &&
              wait_occurred_restart_call)
            {
              // We ended up waiting on a condition variable.  The POA
              // state may have changed while we are waiting.  Therefore,
              // we need to call prepare_for_upcall_i() again.  We also
              // need to cleanup the state of the upcall object before
              // continuing.
              this->upcall_cleanup ();
              continue;
            }
          else
            {
              return result;
            }
        }
    }

    int
    Servant_Upcall::prepare_for_upcall_i (
      const TAO::ObjectKey &key,
      const char *operation,
      CORBA::Object_out forward_to,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
    {
      // Acquire the object adapter lock first.
      int result = this->object_adapter_->lock ().acquire ();
      if (result == -1)
        // Locking error.
        ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                          TAO_Adapter::DS_FAILED);

      // We have acquired the object adapter lock.  Record this for later
      // use.
      this->state_ = OBJECT_ADAPTER_LOCK_ACQUIRED;

      // Check if a non-servant upcall is in progress.  If a non-servant
      // upcall is in progress, wait for it to complete.  Unless of
      // course, the thread making the non-servant upcall is this thread.
      this->object_adapter_->wait_for_non_servant_upcalls_to_complete (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

      // Locate the POA.
      this->object_adapter_->locate_poa (key,
                                         this->system_id_,
                                         this->poa_
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

      // Check the state of the POA.
      this->poa_->check_state (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

      // Setup current for this request.
      this->current_context_.setup (this->poa_,
                                    key);

      // Increase <poa->outstanding_requests_> for the duration of finding
      // the POA, finding the servant, and making the upcall.
      this->poa_->increment_outstanding_requests ();

      // We have setup the POA Current.  Record this for later use.
      this->state_ = POA_CURRENT_SETUP;

      ACE_TRY
        {
          // Lookup the servant.
          this->servant_ =
            this->poa_->locate_servant_i (operation,
                                          this->system_id_,
                                          *this,
                                          this->current_context_,
                                          wait_occurred_restart_call
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (wait_occurred_restart_call)
            return TAO_Adapter::DS_FAILED;
        }
#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
      ACE_CATCH (PortableServer::ForwardRequest, forward_request)
        {
          forward_to =
            CORBA::Object::_duplicate (forward_request.forward_reference.in ());
          return TAO_Adapter::DS_FORWARD;
        }
#else
      ACE_CATCHANY
        {
          ACE_UNUSED_ARG (forward_to);
          ACE_RE_THROW;
        }
#endif /* TAO_HAS_MINIMUM_CORBA */
      ACE_ENDTRY;

      // Now that we know the servant.
      this->current_context_.servant (this->servant_);

      // For servants from Servant Locators, there is no active object map
      // entry.
      if (this->active_object_map_entry ())
        this->current_context_.priority (this->active_object_map_entry ()->priority_);

      if (this->state_ != OBJECT_ADAPTER_LOCK_RELEASED)
        {
          // Release the object adapter lock.
          this->object_adapter_->lock ().release ();

          // We have release the object adapter lock.  Record this for
          // later use.
          this->state_ = OBJECT_ADAPTER_LOCK_RELEASED;
        }

      // Serialize servants (if appropriate).
      this->single_threaded_poa_setup (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_Adapter::DS_FAILED);

      // We have acquired the servant lock.  Record this for later use.
      this->state_ = SERVANT_LOCK_ACQUIRED;

      // After this point, <this->servant_> is ready for dispatching.
      return TAO_Adapter::DS_OK;
    }

    void
    Servant_Upcall::pre_invoke_remote_request (
      TAO_ServerRequest &req
      ACE_ENV_ARG_DECL)
    {
      this->object_adapter_->servant_dispatcher_->pre_invoke_remote_request (
        this->poa (),
        this->priority (),
        req,
        this->pre_invoke_state_
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    void
    Servant_Upcall::pre_invoke_collocated_request (
      ACE_ENV_SINGLE_ARG_DECL)
    {
      this->object_adapter_->servant_dispatcher_->pre_invoke_collocated_request (
        this->poa (),
        this->priority (),
        this->pre_invoke_state_
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    void
    Servant_Upcall::post_invoke (void)
    {
      this->object_adapter_->servant_dispatcher_->post_invoke (
        this->poa (),
        this->pre_invoke_state_);
    }

    Servant_Upcall::Pre_Invoke_State::Pre_Invoke_State (void)
      : state_ (NO_ACTION_REQUIRED),
        original_native_priority_ (0),
        original_CORBA_priority_ (0)
    {
    }

    ::TAO_Root_POA *
    Servant_Upcall::lookup_POA (const TAO::ObjectKey &key
                                ACE_ENV_ARG_DECL)
    {
      // Acquire the object adapter lock first.
      int result = this->object_adapter_->lock ().acquire ();
      if (result == -1)
        // Locking error.
        ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                          0);

      // We have acquired the object adapter lock.  Record this for later
      // use.
      this->state_ = OBJECT_ADAPTER_LOCK_ACQUIRED;

      // Check if a non-servant upcall is in progress.  If a non-servant
      // upcall is in progress, wait for it to complete.  Unless of
      // course, the thread making the non-servant upcall is this thread.
      this->object_adapter_->wait_for_non_servant_upcalls_to_complete (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Locate the POA.
      this->object_adapter_->locate_poa (key,
                                         this->system_id_,
                                         this->poa_
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return this->poa_;
    }

    Servant_Upcall::~Servant_Upcall (void)
    {
      this->upcall_cleanup ();
    }

    void
    Servant_Upcall::upcall_cleanup (void)
    {
      this->post_invoke ();

      switch (this->state_)
        {
        case SERVANT_LOCK_ACQUIRED:
          // Unlock servant (if appropriate).
          this->single_threaded_poa_cleanup ();

          /* FALLTHRU */

        case OBJECT_ADAPTER_LOCK_RELEASED:
          // Cleanup servant locator related state.  Note that because
          // this operation does not change any Object Adapter related
          // state, it is ok to call it outside the lock.
          this->post_invoke_servant_cleanup ();

          // Since the object adapter lock was released, we must acquire
          // it.
          //
          // Note that errors are ignored here since we cannot do much
          // with it.
          this->object_adapter_->lock ().acquire ();

          // Check if a non-servant upcall is in progress.  If a
          // non-servant upcall is in progress, wait for it to complete.
          // Unless of course, the thread making the non-servant upcall is
          // this thread.
          this->object_adapter_->wait_for_non_servant_upcalls_to_complete_no_throw ();

          // Cleanup servant related state.
          this->servant_cleanup ();

          /* FALLTHRU */

        case POA_CURRENT_SETUP:
          // Cleanup POA related state.
          this->poa_cleanup ();

          // Teardown current for this request.
          this->current_context_.teardown ();

          /* FALLTHRU */

        case OBJECT_ADAPTER_LOCK_ACQUIRED:
          // Finally, since the object adapter lock was acquired, we must
          // release it.
          this->object_adapter_->lock ().release ();

          /* FALLTHRU */

        case INITIAL_STAGE:
        default:
          // @@ Keep compiler happy, the states above are the only
          //    possible ones.
          break;
        }
    }

    void
    Servant_Upcall::post_invoke_servant_cleanup (void)
    {
      this->poa_->post_invoke_servant_cleanup (this->current_context_.object_id (),
                                               *this);
    }

    void
    Servant_Upcall::single_threaded_poa_setup (ACE_ENV_SINGLE_ARG_DECL)
    {
#if (TAO_HAS_MINIMUM_POA == 0)
      // Serialize servants (if necessary).
      //
      // Note that this lock must be acquired *after* the object adapter
      // lock has been released.  This is necessary since we cannot block
      // waiting for the servant lock while holding the object adapter
      // lock.  Otherwise, the thread that wants to release this lock will
      // not be able to do so since it can't acquire the object adapterx
      // lock.
        int result = this->poa_->enter();

        if (result == -1)
          // Locking error.
          ACE_THROW (CORBA::OBJ_ADAPTER ());
#else
        ACE_ENV_ARG_NOT_USED; // FUZZ: ignore check_for_ace_check
#endif /* !TAO_HAS_MINIMUM_POA == 0 */
    }

    void
    Servant_Upcall::single_threaded_poa_cleanup (void)
    {
#if (TAO_HAS_MINIMUM_POA == 0)
      int result = 0;

      // Since the servant lock was acquired, we must release it.
      result = this->poa_->exit ();

      ACE_UNUSED_ARG (result);
#endif /* TAO_HAS_MINIMUM_POA == 0 */
    }

    void
    Servant_Upcall::increment_servant_refcount (void)
    {
      // Cleanup servant related stuff.
      if (this->active_object_map_entry_ != 0)
        ++this->active_object_map_entry_->reference_count_;
    }

    void
    Servant_Upcall::servant_cleanup (void)
    {
      // Cleanup servant related stuff.
      if (this->active_object_map_entry_ != 0)
        {
          // Decrement the reference count.
          CORBA::UShort new_count =
            --this->active_object_map_entry_->reference_count_;

          if (new_count == 0)
            {
              ACE_DECLARE_NEW_CORBA_ENV;
              ACE_TRY
                {
                  this->poa_->cleanup_servant (
                    this->active_object_map_entry_->servant_,
                    this->active_object_map_entry_->user_id_
                    ACE_ENV_ARG_PARAMETER);

                  ACE_TRY_CHECK;
                }
              ACE_CATCHALL
                {
                  // Ignore errors from servant cleanup ....
                }
              ACE_ENDTRY;

              if (this->poa_->waiting_servant_deactivation() > 0 &&
                  this->object_adapter_->enable_locking_)
                {
                  // Wakeup all waiting threads.
                  this->poa_->servant_deactivation_condition_.broadcast ();
                }
            }
        }
    }

    void
    Servant_Upcall::poa_cleanup (void)
    {
      // Decrease <poa->outstanding_requests_> now that the upcall
      // is complete.
      //
      // Note that the object adapter lock is acquired before
      // <POA::outstanding_requests_> is decreased.
      CORBA::ULong outstanding_requests =
        this->poa_->decrement_outstanding_requests ();

      // Check if all pending requests are over.
      if (outstanding_requests == 0)
        {
          // If locking is enabled and some thread is waiting in POA::destroy.
          if (this->object_adapter_->enable_locking_ &&
              this->poa_->wait_for_completion_pending_)
            {
              // Wakeup all waiting threads.
              this->poa_->outstanding_requests_condition_.broadcast ();
            }

          // Note that there is no need to check for
          // <non_servant_upcall_in_progress> since it is not possible for
          // non-servant upcalls to be in progress at this point.
          if (this->poa_->waiting_destruction_)
            {
              ACE_DECLARE_NEW_CORBA_ENV;

              ACE_TRY
                {
                  this->poa_->complete_destruction_i (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
              ACE_CATCHANY
                {
                  // Ignore exceptions
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "TAO_POA::~complete_destruction_i");
                }
              ACE_ENDTRY;
              ACE_CHECK;

              this->poa_ = 0;
            }
        }
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
