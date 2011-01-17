// $Id$

#include "tao/Thread_Lane_Resources.h"
#include "tao/Acceptor_Registry.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Connection_Handler.h"
#include "tao/Transport.h"
#include "tao/Connector_Registry.h"
#include "tao/SystemException.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Descriptor_Interface.h"

#include "ace/Reactor.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Thread_Lane_Resources::TAO_Thread_Lane_Resources (
    TAO_ORB_Core &orb_core,
    TAO_New_Leader_Generator *new_leader_generator
  )
  : orb_core_ (orb_core),
    acceptor_registry_ (0),
    connector_registry_ (0),
    transport_cache_ (0),
    leader_follower_ (0),
    new_leader_generator_ (new_leader_generator),
    input_cdr_dblock_allocator_ (0),
    input_cdr_buffer_allocator_ (0),
    input_cdr_msgblock_allocator_ (0),
    transport_message_buffer_allocator_ (0),
    output_cdr_dblock_allocator_ (0),
    output_cdr_buffer_allocator_ (0),
    output_cdr_msgblock_allocator_ (0),
    amh_response_handler_allocator_ (0),
    ami_response_handler_allocator_ (0)
{
  // Create the transport cache.
  ACE_NEW (this->transport_cache_,
           TAO::Transport_Cache_Manager (
            orb_core.resource_factory ()->purge_percentage (),
            orb_core.resource_factory ()->create_purging_strategy (),
            orb_core.resource_factory ()->cache_maximum (),
            orb_core.resource_factory ()->locked_transport_cache (),
            orb_core.orbid ()));
}

TAO_Thread_Lane_Resources::~TAO_Thread_Lane_Resources (void)
{
}

TAO::Transport_Cache_Manager &
TAO_Thread_Lane_Resources::transport_cache (void)
{
  return *this->transport_cache_;
}

int
TAO_Thread_Lane_Resources::has_acceptor_registry_been_created (void) const
{
  return this->acceptor_registry_ != 0;
}

int
TAO_Thread_Lane_Resources::is_collocated (const TAO_MProfile& mprofile)
{
  if (!this->has_acceptor_registry_been_created ())
    {
      return 0;
    }

  return this->acceptor_registry ().is_collocated (mprofile);
}

TAO_Acceptor_Registry &
TAO_Thread_Lane_Resources::acceptor_registry (void)
{
  // Double check.
  if (this->acceptor_registry_ == 0)
    {
      // @todo: Wouldnt this crash big time if you happen to
      // dereference a null-pointer? Needs fixing.
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        *this->acceptor_registry_);

      if (this->acceptor_registry_ == 0)
        {
          // @@ Not exception safe code
          // Get the resource factory.
          TAO_Resource_Factory &resource_factory =
            *this->orb_core_.resource_factory ();

          // Ask it to create a new acceptor registry.
          this->acceptor_registry_ =
            resource_factory.get_acceptor_registry ();
        }
    }

  return *this->acceptor_registry_;
}

TAO_Connector_Registry *
TAO_Thread_Lane_Resources::connector_registry (void)
{
  // Double check.
  if (this->connector_registry_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        0);

      if (this->connector_registry_ == 0)
        {
          // Ask it to create a new acceptor registry.
          TAO_Connector_Registry *connector_registry =
            this->orb_core_.resource_factory ()->get_connector_registry ();

          if (connector_registry == 0)
            {
              throw ::CORBA::INITIALIZE (
                CORBA::SystemException::_tao_minor_code (
                  TAO_CONNECTOR_REGISTRY_INIT_LOCATION_CODE,
                  0),
                CORBA::COMPLETED_NO);
            }

          if (connector_registry->open (&this->orb_core_) != 0)
            {
              throw ::CORBA::INITIALIZE (
                CORBA::SystemException::_tao_minor_code (
                  TAO_CONNECTOR_REGISTRY_INIT_LOCATION_CODE,
                  0),
                CORBA::COMPLETED_NO);
            }

          // Finally, everything is created and opened successfully:
          // now we can assign to the member.  Otherwise, the
          // assignment would be premature.
          this->connector_registry_ = connector_registry;
        }
    }

  return this->connector_registry_;
}


TAO_Leader_Follower &
TAO_Thread_Lane_Resources::leader_follower (void)
{
  // Double check.
  if (this->leader_follower_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        *this->leader_follower_);

      if (this->leader_follower_ == 0)
        {
          // Create a new Leader Follower object.
          ACE_NEW_RETURN (this->leader_follower_,
                          TAO_Leader_Follower (&this->orb_core_,
                                               this->new_leader_generator_),
                          *this->leader_follower_);
        }
    }

  return *this->leader_follower_;
}


ACE_Allocator*
TAO_Thread_Lane_Resources::input_cdr_dblock_allocator (void)
{
  if (this->input_cdr_dblock_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->input_cdr_dblock_allocator_ == 0)
        {
          this->input_cdr_dblock_allocator_ =
            this->resource_factory ()->input_cdr_dblock_allocator ();
        }
    }

  return this->input_cdr_dblock_allocator_;
}


ACE_Allocator*
TAO_Thread_Lane_Resources::input_cdr_buffer_allocator (void)
{
  if (this->input_cdr_buffer_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->input_cdr_buffer_allocator_ == 0)
        {
          this->input_cdr_buffer_allocator_ =
            this->resource_factory ()->input_cdr_buffer_allocator ();
        }
    }

  return this->input_cdr_buffer_allocator_;
}


ACE_Allocator*
TAO_Thread_Lane_Resources::input_cdr_msgblock_allocator (void)
{
  if (this->input_cdr_msgblock_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->input_cdr_msgblock_allocator_ == 0)
        {
          this->input_cdr_msgblock_allocator_ =
            this->resource_factory ()->input_cdr_msgblock_allocator ();
        }
    }

  return this->input_cdr_msgblock_allocator_;
}

ACE_Allocator*
TAO_Thread_Lane_Resources::transport_message_buffer_allocator (void)
{
  if (this->transport_message_buffer_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->transport_message_buffer_allocator_ == 0)
        {
          this->transport_message_buffer_allocator_ =
            this->resource_factory ()->input_cdr_dblock_allocator ();
        }
    }

  return this->transport_message_buffer_allocator_;
}


ACE_Allocator*
TAO_Thread_Lane_Resources::output_cdr_dblock_allocator (void)
{
  if (this->output_cdr_dblock_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->output_cdr_dblock_allocator_ == 0)
        {
          this->output_cdr_dblock_allocator_ =
            this->resource_factory ()->output_cdr_dblock_allocator ();
        }
    }

  return this->output_cdr_dblock_allocator_;
}


ACE_Allocator*
TAO_Thread_Lane_Resources::output_cdr_buffer_allocator (void)
{
  if (this->output_cdr_buffer_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->output_cdr_buffer_allocator_ == 0)
        {
          this->output_cdr_buffer_allocator_ =
            this->resource_factory ()->output_cdr_buffer_allocator ();
        }
    }

  return this->output_cdr_buffer_allocator_;
}


ACE_Allocator*
TAO_Thread_Lane_Resources::output_cdr_msgblock_allocator (void)
{
  if (this->output_cdr_msgblock_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->output_cdr_msgblock_allocator_ == 0)
        {
          this->output_cdr_msgblock_allocator_ =
            this->resource_factory ()->output_cdr_msgblock_allocator ();
        }
    }

  return this->output_cdr_msgblock_allocator_;
}

ACE_Allocator*
TAO_Thread_Lane_Resources::amh_response_handler_allocator (void)
{
  if (this->amh_response_handler_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->amh_response_handler_allocator_ == 0)
        {
          this->amh_response_handler_allocator_ =
            this->resource_factory ()->amh_response_handler_allocator ();
        }
    }

  return this->amh_response_handler_allocator_;
}

ACE_Allocator*
TAO_Thread_Lane_Resources::ami_response_handler_allocator (void)
{
  if (this->ami_response_handler_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->ami_response_handler_allocator_ == 0)
        {
          this->ami_response_handler_allocator_ =
            this->resource_factory ()->ami_response_handler_allocator ();
        }
    }

  return this->ami_response_handler_allocator_;
}

int
TAO_Thread_Lane_Resources::open_acceptor_registry (const TAO_EndpointSet &endpoint_set,
                                                   bool ignore_address)
{
  // Access the acceptor registry.
  TAO_Acceptor_Registry &ar = this->acceptor_registry ();

  // Open it.
  return ar.open (&this->orb_core_,
                  this->leader_follower ().reactor (),
                  endpoint_set,
                  ignore_address);
}

TAO_Resource_Factory *
TAO_Thread_Lane_Resources::resource_factory (void)
{
  return this->orb_core_.resource_factory ();
}

void
TAO_Thread_Lane_Resources::finalize (void)
{
  // Close connectors before acceptors!
  // Ask the registry to close all registered connectors.
  if (this->connector_registry_ != 0)
    {
      this->connector_registry_->close_all ();
      delete this->connector_registry_;
      this->connector_registry_ = 0;
    }

  // Ask the registry to close all registered acceptors.
  if (this->acceptor_registry_ != 0)
    {
      this->acceptor_registry_->close_all ();
      delete this->acceptor_registry_;
      this->acceptor_registry_ = 0;
    }

  // Set of handlers still in the connection cache.
  TAO::Connection_Handler_Set handlers;

  // Close the transport cache and return the handlers that were still
  // registered.  The cache will decrease the #REFCOUNT# on the
  // handler when it removes the handler from cache.  However,
  // #REFCOUNT# is increased when the handler is placed in the handler
  // set.
  this->transport_cache_->close (handlers);

  // Go through the handler set, closing the connections and removing
  // the references.
  TAO_Connection_Handler **handler = 0;

  for (TAO::Connection_Handler_Set::iterator iter (handlers);
       iter.next (handler);
       iter.advance ())
    {
      // Connection is closed.  Potential removal from the Reactor.
      (*handler)->close_connection ();

      // #REFCOUNT# related to the handler set decreases.
      (*handler)->transport ()->remove_reference ();
    }

  delete this->transport_cache_;
  this->transport_cache_ = 0;

  delete this->leader_follower_;
  this->leader_follower_ = 0;

  // Delete all the allocators here.. They shouldnt be done earlier,
  // lest some of the contents in the above, say reactor or acceptor
  // may use memory from the pool..
  if (this->input_cdr_dblock_allocator_ != 0)
    {
      this->input_cdr_dblock_allocator_->remove ();
      delete this->input_cdr_dblock_allocator_;
      this->input_cdr_dblock_allocator_ = 0;
    }

  if (this->input_cdr_buffer_allocator_ != 0)
    {
      this->input_cdr_buffer_allocator_->remove ();
      delete this->input_cdr_buffer_allocator_;
      this->input_cdr_buffer_allocator_ = 0;
    }

  if (this->input_cdr_msgblock_allocator_ != 0)
    {
      this->input_cdr_msgblock_allocator_->remove ();
      delete this->input_cdr_msgblock_allocator_;
      this->input_cdr_msgblock_allocator_ = 0;
    }

  if (this->transport_message_buffer_allocator_ != 0)
    {
      this->transport_message_buffer_allocator_->remove ();
      delete this->transport_message_buffer_allocator_;
      this->transport_message_buffer_allocator_ = 0;
    }

  if (this->output_cdr_dblock_allocator_ != 0)
    {
      this->output_cdr_dblock_allocator_->remove ();
      delete this->output_cdr_dblock_allocator_;
      this->output_cdr_dblock_allocator_ = 0;
    }

  if (this->output_cdr_buffer_allocator_ != 0)
    {
      this->output_cdr_buffer_allocator_->remove ();
      delete this->output_cdr_buffer_allocator_;
      this->output_cdr_buffer_allocator_ = 0;
    }

  if (this->output_cdr_msgblock_allocator_ != 0)
    {
      this->output_cdr_msgblock_allocator_->remove ();
      delete this->output_cdr_msgblock_allocator_;
      this->output_cdr_msgblock_allocator_ = 0;
    }

  if (this->amh_response_handler_allocator_ != 0)
    {
      this->amh_response_handler_allocator_->remove ();
      delete this->amh_response_handler_allocator_;
      this->amh_response_handler_allocator_ = 0;
    }

  if (this->ami_response_handler_allocator_ != 0)
    {
      this->ami_response_handler_allocator_->remove ();
      delete this->ami_response_handler_allocator_;
      this->ami_response_handler_allocator_ = 0;
    }
}

void
TAO_Thread_Lane_Resources::shutdown_reactor (void)
{
  TAO_Leader_Follower &leader_follower = this->leader_follower ();

  ACE_GUARD (TAO_SYNCH_MUTEX,
             ace_mon,
             leader_follower.lock ());

  ACE_Reactor *reactor = leader_follower.reactor ();

  // Wakeup all the threads waiting blocked in the event loop, this
  // does not guarantee that they will all go away, but reduces the
  // load on the POA....

  // If there are some client threads running we have to wait until
  // they finish, when the last one does it will shutdown the reactor
  // for us.  Meanwhile no new requests will be accepted because the
  // POA will not process them.
  if (!this->orb_core_.resource_factory ()->drop_replies_during_shutdown () &&
      leader_follower.has_clients ())
    {
      reactor->wakeup_all_threads ();
      return;
    }

  // End the reactor if we want shutdown dropping replies along the
  // way.
  reactor->end_reactor_event_loop ();
}

void
TAO_Thread_Lane_Resources::close_all_transports (void)
{
  // If we have no-drop-reply strategy or already fininalized simply return.
  if (!this->orb_core_.resource_factory ()->drop_replies_during_shutdown () ||
       this->transport_cache_ == 0)
    return;

  // Set of handlers still in the connection cache.
  TAO::Connection_Handler_Set handlers;

  // Close the transport cache and return the handlers that were still
  // registered.  The cache will decrease the #REFCOUNT# on the
  // handler when it removes the handler from cache.  However,
  // #REFCOUNT# is increased when the handler is placed in the handler
  // set.
  this->transport_cache_->close (handlers);

  // Go through the handler set, closing the connections and removing
  // the references.
  TAO_Connection_Handler **handler = 0;

  for (TAO::Connection_Handler_Set::iterator iter (handlers);
       iter.next (handler);
       iter.advance ())
    {
      // Connection is closed.  Potential removal from the Reactor.
      (*handler)->close_connection ();

      // #REFCOUNT# related to the handler set decreases.
      (*handler)->transport ()->remove_reference ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
