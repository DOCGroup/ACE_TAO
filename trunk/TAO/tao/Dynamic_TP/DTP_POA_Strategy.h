// -*- C++ -*-

//=============================================================================
/**
 *  @file    DTP_POA_Strategy.h
 *
 *  $Id$
 *
 *  @author  Marc Neeley <neeleym@ociweb.com>
 */
//=============================================================================

#ifndef TAO_DYNAMIC_TP_POA_STRATEGY_H
#define TAO_DYNAMIC_TP_POA_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "tao/Dynamic_TP/DTP_Config.h"
#include "tao/Dynamic_TP/DTP_Task.h"
#include "tao/CSD_ThreadPool/CSD_TP_Servant_State_Map.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_Framework/CSD_Strategy_Base.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "ace/OS_NS_strings.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


class TAO_DTP_POA_Strategy;
typedef TAO_Intrusive_Ref_Count_Handle<TAO_DTP_POA_Strategy>
        TAO_DTP_POA_Strategy_Handle;

namespace TAO
{
  namespace CSD
  {
    class TP_Custom_Request_Operation;
  }
}


/**
 * @class TAO_DTP_POA_Strategy
 *
 * @brief
 *
 */
class TAO_Dynamic_TP_Export TAO_DTP_POA_Strategy:
  public TAO::CSD::Strategy_Base
{
 public:

  /// Constructors.
  /// This constructor supports situations where dynamic instantiation
  /// of a thread pool for POAs is initiated from a svc.conf file.
  /// A configuration name is passed in such that the object can
  /// lookup a configuration in a pre-populated repository.
  TAO_DTP_POA_Strategy(ACE_CString tp_config_name,
                              bool     serialize_servants = true);

  /// This constructor supports situations where explicit instantiation
  /// of a thread pool for POAs is initiated from an application, such as
  /// the use of the apply_to() method.
  TAO_DTP_POA_Strategy(TAO_DTP_Definition * tp_config,
                              bool     serialize_servants = true);

  /// Virtual Destructor.
  virtual ~TAO_DTP_POA_Strategy();

  /// Turn on/off serialization of servants.
  void set_servant_serialization(bool serialize_servants);

  /// Return codes for the custom dispatch_request() methods.
  enum CustomRequestOutcome
  {
    /// The request was successfully put on the request queue.
    REQUEST_DISPATCHED,
    /// The request has been executed/completed by a worker thread.
    REQUEST_EXECUTED,
    /// The request was removed from the queue and cancelled.
    REQUEST_CANCELLED,
    /// The request queue rejected the request
    REQUEST_REJECTED
  };

  /// Inject a synchronous, custom request into the request queue.
  /// This will block the calling thread until the request is handled
  /// (dispatched or cancelled) or rejected.
  /// Will return REQUEST_EXECUTED, REQUEST_CANCELLED, or REQUEST_REJECTED.
  CustomRequestOutcome custom_synch_request
    (TAO::CSD::TP_Custom_Request_Operation* op);

  /// Inject an asynchronous, custom request into the request queue.
  /// This will return control to the calling thread once the request
  /// has been placed into the queue (or rejected).
  /// Will return REQUEST_DISPATCHED or REQUEST_REJECTED.
  CustomRequestOutcome custom_asynch_request
    (TAO::CSD::TP_Custom_Request_Operation* op);

  /// Cancel all requests that are targeted for the provided servant.
  /// This is requested on the user application level.
  void cancel_requests(PortableServer::Servant servant);


 protected:

  /// Handle the dispatching of a remote request.
  ///
  /// This will cause a new "request" object to be created and pushed
  /// on to a "request queue".  The worker threads are responsible for
  /// servicing the queue, and performing the actual dispatch logic.
  virtual Strategy_Base::DispatchResult dispatch_remote_request_i
    (TAO_ServerRequest&              server_request,
     const PortableServer::ObjectId& object_id,
     PortableServer::POA_ptr         poa,
     const char*                     operation,
     PortableServer::Servant         servant);

  /// Handle the dispatching of a collocated request.
  ///
  /// This will cause a new "request" object to be created and pushed
  /// on to a "request queue".  The worker threads are responsible for
  /// servicing the queue, and performing the actual dispatch logic.
  virtual Strategy_Base::DispatchResult dispatch_collocated_request_i
    (TAO_ServerRequest&              server_request,
     const PortableServer::ObjectId& object_id,
     PortableServer::POA_ptr         poa,
     const char*                     operation,
     PortableServer::Servant         servant);

  /// Event - The POA has been activated.
  /// This will activate the worker thread(s).
  /// Returns true if the worker threads were activated successfully.
  /// Otherwise, returns false.
  virtual bool poa_activated_event_i(TAO_ORB_Core& orb_core);

  /// Event - The POA has been deactivated.
  /// This will shutdown the worker thread(s).
  virtual void poa_deactivated_event_i();

  /// Event - A servant has been activated
  virtual void servant_activated_event_i
    (PortableServer::Servant servant,
     const PortableServer::ObjectId& oid);

  /// Event - A servant has been deactivated
  virtual void servant_deactivated_event_i
    (PortableServer::Servant servant,
     const PortableServer::ObjectId& oid);

 private:


  /**
   * Helper method that is responsible for looking up the servant
   * state object in the servant state map *if* the "serialize
   * servants" flag is set to true.  In the case where the
   * "serialize servants" flag is set to false, then a "nil"
   * servant state handle object is returned.
   *
   * @param servant - input - a pointer to the servant object.
   *
   * @returns a handle to a servant state object.
   *
   * @throw PortableServer::POA::ServantNotActive if the servant
   *        state cannot be determined.
   */
  TAO::CSD::TP_Servant_State::HandleType get_servant_state
    (PortableServer::Servant servant);

  /// The "serialize servants" flag.
  bool serialize_servants_;

  /// The map of servant state objects - only used when the
  /// "serialize servants" flag is set to true.
  TAO::CSD::TP_Servant_State_Map servant_state_map_;

  /// This holds the name of a configuration that have been loaded
  /// into a DTP_Config_Registry.  This is the key to the RB
  /// Tree entry.

  ACE_CString dynamic_tp_config_name_;

  /// This is the active object used by the worker threads.
  /// The request queue is owned/managed by the task object.
  /// The strategy object puts requests into the task's request
  /// queue, and the worker threads service the queued requests
  /// by performing the actual servant request dispatching logic.
  TAO_DTP_Task dtp_task_;


  /// This boolean is used to determine if the configuration has
  /// been sent in from an application or, if false, needs to be
  /// looked up from a Dynamic TP Config repo. It works in conjunction with
  /// dynamic_tp_config_name_ above.

  bool config_initialized_;


  void set_dtp_config(TAO_DTP_Definition &tp_config);
};



TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Dynamic_TP/DTP_POA_Strategy.inl"
#endif /* __ACE_INLINE__ */

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_DYNAMIC_TP_POA_STRATEGY_H */
