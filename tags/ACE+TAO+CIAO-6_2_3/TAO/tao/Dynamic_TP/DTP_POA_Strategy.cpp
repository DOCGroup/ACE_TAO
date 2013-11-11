// $Id$

#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "tao/CSD_ThreadPool/CSD_TP_Remote_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Asynch_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Synch_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Asynch_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_With_Server_Request.h"
#include "tao/ORB_Core.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0


#if !defined (__ACE_INLINE__)
#include "tao/Dynamic_TP/DTP_POA_Strategy.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_DTP_POA_Strategy::~TAO_DTP_POA_Strategy()
{
}

TAO_DTP_POA_Strategy::CustomRequestOutcome
TAO_DTP_POA_Strategy::custom_synch_request(
  TAO::CSD::TP_Custom_Request_Operation* op)
{

  TAO::CSD::TP_Servant_State::HandleType servant_state =
    this->get_servant_state(op->servant());

  TAO::CSD::TP_Custom_Synch_Request *req_ptr;
  ACE_NEW_RETURN (req_ptr,
                  TAO::CSD::TP_Custom_Synch_Request(op, servant_state.in ()),
                  REQUEST_REJECTED);

  TAO::CSD::TP_Custom_Synch_Request_Handle request = req_ptr;

  if (!this->dtp_task_.add_request (request.in ()))
    {
      // The request was rejected by the task.
      return REQUEST_REJECTED;
    }

  // Now we wait until the request is handled (executed or cancelled).
  return (request->wait ()) ? REQUEST_EXECUTED : REQUEST_CANCELLED;
}

TAO_DTP_POA_Strategy::CustomRequestOutcome
TAO_DTP_POA_Strategy::custom_asynch_request (
  TAO::CSD::TP_Custom_Request_Operation* op)
{

  TAO::CSD::TP_Servant_State::HandleType servant_state =
    this->get_servant_state (op->servant ());

  TAO::CSD::TP_Custom_Asynch_Request *req_ptr;
  ACE_NEW_RETURN (req_ptr,
                  TAO::CSD::TP_Custom_Asynch_Request (op, servant_state.in ()),
                  REQUEST_REJECTED);

  TAO::CSD::TP_Custom_Asynch_Request_Handle request = req_ptr;

  return (this->dtp_task_.add_request (request.in ()))
            ? REQUEST_DISPATCHED : REQUEST_REJECTED;
}

bool
TAO_DTP_POA_Strategy::poa_activated_event_i (TAO_ORB_Core& orb_core)
{

  this->dtp_task_.thr_mgr (orb_core.thr_mgr ());

  // Activates the worker threads, and waits until all have been started.
  if (!this->config_initialized_)
    {
      TAO_DTP_Config_Registry * config_repo =
      ACE_Dynamic_Service<TAO_DTP_Config_Registry>::instance
        ("DTP_Config_Registry");

      if (config_repo == 0)
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy - ")
              ACE_TEXT ("cannot retrieve configuration repo\n")));
            }
          return false;
        }
      else
        {
          TAO_DTP_Definition config_entry;
          if (!config_repo->find (this->dynamic_tp_config_name_, config_entry))
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy - ")
                          ACE_TEXT ("warning: config not found...using ")
                          ACE_TEXT ("defaults!\n")));

            }
          this->set_dtp_config (config_entry);
          //this->dtp_task_.set_init_pool_threads(config_entry.init_threads_);
          //this->dtp_task_.set_min_pool_threads(config_entry.min_threads_);
          //this->dtp_task_.set_max_pool_threads(config_entry.max_threads_);
          //this->dtp_task_.set_thread_idle_time(config_entry.timeout_);
          //this->dtp_task_.set_thread_stack_size(config_entry.stack_size_);
          //this->dtp_task_.set_max_request_queue_depth(config_entry.queue_depth_);
        }



    }
  return (this->dtp_task_.open () == 0);

}

void
TAO_DTP_POA_Strategy::poa_deactivated_event_i ()
{
  // Passing in a value of 1 means that we want to shutdown the task, which
  // equates to causing all worker threads to shutdown.  The worker threads
  // themselves will also invoke the close() method, but the passed-in value
  // will be 0.  So, a 1 means "shutdown", and a 0 means "a single worker
  // thread is going away".
  this->dtp_task_.close(1);
}

TAO::CSD::Strategy_Base::DispatchResult
TAO_DTP_POA_Strategy::dispatch_remote_request_i
                             (TAO_ServerRequest&              server_request,
                              const PortableServer::ObjectId& object_id,
                              PortableServer::POA_ptr         poa,
                              const char*                     operation,
                              PortableServer::Servant         servant)
{
  TAO::CSD::TP_Servant_State::HandleType servant_state =
                        this->get_servant_state (servant);

  // Now we can create the TP_Remote_Request object, and then add it to our
  // task_'s "request queue".
  //
  // TBD-CSD: Need to use a Cached Allocator to "create" the
  //          TP_Remote_Request objects.  For now, use the heap.
  TAO::CSD::TP_Remote_Request *req_ptr;
  ACE_NEW_RETURN (req_ptr,
                  TAO::CSD::TP_Remote_Request (server_request,
                                               object_id,
                                               poa,
                                               operation,
                                               servant,
                                               servant_state.in ()),
                  TAO::CSD::Strategy_Base::DISPATCH_REJECTED);

  TAO::CSD::TP_Remote_Request_Handle request = req_ptr;

  // Hand the request object to our task so that it can add the request
  // to its "request queue".
  if (!this->dtp_task_.add_request (request.in ()))
    {
      // Return the DISPATCH_REJECTED return code so that the caller (our
      // base class' dispatch_request() method) knows that we did
      // not handle the request, and that it should be rejected.
      return TAO::CSD::Strategy_Base::DISPATCH_REJECTED;
    }

    return TAO::CSD::Strategy_Base::DISPATCH_HANDLED;
}

TAO::CSD::Strategy_Base::DispatchResult
TAO_DTP_POA_Strategy::dispatch_collocated_request_i
                             (TAO_ServerRequest&              server_request,
                              const PortableServer::ObjectId& object_id,
                              PortableServer::POA_ptr         poa,
                              const char*                     operation,
                              PortableServer::Servant         servant)
{


  TAO::CSD::TP_Servant_State::HandleType servant_state =
                        this->get_servant_state (servant);

  bool is_sync_with_server = server_request.sync_with_server ();
  bool is_synchronous      = server_request.response_expected ();

  TAO::CSD::TP_Collocated_Synch_Request_Handle
    synch_request;
  TAO::CSD::TP_Collocated_Synch_With_Server_Request_Handle
    synch_with_server_request;
  TAO::CSD::TP_Request_Handle
    request;

  // Create the request object using the appropriate concrete type.
  if (is_sync_with_server)
    {
      TAO::CSD::TP_Collocated_Synch_With_Server_Request *req_ptr;
      ACE_NEW_RETURN (req_ptr,
                      TAO::CSD::TP_Collocated_Synch_With_Server_Request
                        (server_request,
                         object_id,
                         poa,
                         operation,
                         servant,
                         servant_state.in ()),
                      DISPATCH_REJECTED);

      synch_with_server_request = req_ptr;

      // Give the request handle its own "copy".
      synch_with_server_request->_add_ref ();
      request = synch_with_server_request.in ();
    }
  else if (is_synchronous)
    {

      TAO::CSD::TP_Collocated_Synch_Request *req_ptr;
      ACE_NEW_RETURN (req_ptr,
                      TAO::CSD::TP_Collocated_Synch_Request (
                                   server_request,
                                   object_id,
                                   poa,
                                   operation,
                                   servant,
                                   servant_state.in ()),
                      DISPATCH_REJECTED);

      synch_request = req_ptr;

      // Give the request handle its own "copy".
      synch_request->_add_ref ();
      request = synch_request.in ();
    }
  else
    {
      TAO::CSD::TP_Collocated_Asynch_Request *req_ptr;
      ACE_NEW_RETURN (req_ptr,
                      TAO::CSD::TP_Collocated_Asynch_Request (server_request,
                                                              object_id,
                                                              poa,
                                                              operation,
                                                              servant,
                                                              servant_state.in ()),
                      DISPATCH_REJECTED);

      // Just use the (base) request handle to hold the request object.
      request = req_ptr;
    }

  // Hand the request object to our task so that it can add the request
  // to its "request queue".
  if (!this->dtp_task_.add_request (request.in ()))
    {
      // Return the DISPATCH_REJECTED return code so that the caller (our
      // base class' dispatch_request() method) knows that we did
      // not handle the request, and that it should be rejected.
      return DISPATCH_REJECTED;
    }

  // We need to wait on the request object if the request type is a
  // synchronous request.
  if (!synch_request.is_nil ())
    {
      int srw = synch_request->wait ();
      if (srw == false)
        {
          // Raise exception when request was cancelled.
          throw ::CORBA::NO_IMPLEMENT ();
        }
    }
  else if (!synch_with_server_request.is_nil())
    {
      bool swsr = synch_with_server_request->wait();
      if (swsr == false)
        {
          // Raise exception when request was cancelled.
          throw ::CORBA::NO_IMPLEMENT ();
        }
    }

  return DISPATCH_HANDLED;
}

void
TAO_DTP_POA_Strategy::servant_activated_event_i
                                (PortableServer::Servant servant,
                                 const PortableServer::ObjectId&)
{
  if (this->serialize_servants_)
    {
      // Add the servant to the servant state map.
      this->servant_state_map_.insert (servant);
    }
}

void
TAO_DTP_POA_Strategy::servant_deactivated_event_i
                                (PortableServer::Servant servant,
                                 const PortableServer::ObjectId&)
{
  // Cancel all requests stuck in the queue for the specified servant.
  this->dtp_task_.cancel_servant (servant);

  if (this->serialize_servants_)
    {
      // Remove the servant from the servant state map.
      this->servant_state_map_.remove (servant);
    }
}

void
TAO_DTP_POA_Strategy::cancel_requests (PortableServer::Servant servant)
{
  // Cancel all requests stuck in the queue for the specified servant.
  this->dtp_task_.cancel_servant (servant);
}

TAO::CSD::TP_Servant_State::HandleType
TAO_DTP_POA_Strategy::get_servant_state (PortableServer::Servant servant)
{
  TAO::CSD::TP_Servant_State::HandleType servant_state;

  if (this->serialize_servants_)
    {
      servant_state = this->servant_state_map_.find (servant);
    }

  return servant_state;
}

void
TAO_DTP_POA_Strategy::set_dtp_config (TAO_DTP_Definition &tp_config)
{

  if (tp_config.min_threads_ <= 0)
    {
      this->dtp_task_.set_min_pool_threads (1);
      this->dtp_task_.set_thread_idle_time (ACE_Time_Value (0,0));
    }
  else
    {
      this->dtp_task_.set_min_pool_threads (tp_config.min_threads_);
      this->dtp_task_.set_thread_idle_time (tp_config.timeout_);
    }

  // initial_pool_threads_
  if ((tp_config.init_threads_ <= 0) ||
      (tp_config.init_threads_ < tp_config.min_threads_))
    {
     this->dtp_task_.set_init_pool_threads (this->dtp_task_.get_min_pool_threads());
    }
  else
    {
      this->dtp_task_.set_init_pool_threads (tp_config.init_threads_);
    }

  // max_pool_threads_

  if (tp_config.max_threads_ <= 0)
    {
      // Set to 0 so that max is unbounded.
      this->dtp_task_.set_max_pool_threads(0);
    }
  else
    if (tp_config.max_threads_ < tp_config.init_threads_)
      {
        this->dtp_task_.set_max_pool_threads(
          this->dtp_task_.get_init_pool_threads ());
      }
    else
      {
        this->dtp_task_.set_max_pool_threads (tp_config.max_threads_);
      }

      // thread_stack_size_

    if (tp_config.stack_size_ <= 0)
      {
        this->dtp_task_.set_thread_stack_size (ACE_DEFAULT_THREAD_STACKSIZE);
      }
    else
      {
        this->dtp_task_.set_thread_stack_size (tp_config.stack_size_);
      }

    // max_request_queue_depth_
    if (tp_config.queue_depth_ < 0)
      {
        this->dtp_task_.set_max_request_queue_depth (0);
      }
    else
      {
        this->dtp_task_.set_max_request_queue_depth (tp_config.queue_depth_);
      }


    if (TAO_debug_level > 4)
    {
          TAOLIB_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy: ")
          ACE_TEXT ("Initialized with:\n")
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy initial_pool_threads_=")
          ACE_TEXT ("[%d]\n")
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy min_pool_threads_=[%d]\n")
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy max_pool_threads_=[%d]\n")
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy max_request_queue_depth_=")
          ACE_TEXT ("[%d]\n")
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy thread_stack_size_=[%d]\n")
          ACE_TEXT ("TAO (%P|%t) - DTP_POA_Strategy thread_idle_time_=[%d]\n"),
          this->dtp_task_.get_init_pool_threads(),
          this->dtp_task_.get_min_pool_threads(),
          this->dtp_task_.get_max_pool_threads(),
          this->dtp_task_.get_max_request_queue_depth(),
          this->dtp_task_.get_thread_stack_size(),
          this->dtp_task_.get_thread_idle_time()));
    }
}
TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
