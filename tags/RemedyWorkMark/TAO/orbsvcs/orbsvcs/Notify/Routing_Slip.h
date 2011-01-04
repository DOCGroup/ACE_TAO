// -*- C++ -*-

/**
 *  @file Routing_Slip.h
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */

#ifndef TAO_NOTIFY_ROUTING_SLIP_H
#define TAO_NOTIFY_ROUTING_SLIP_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/Delivery_Request.h"
#include "orbsvcs/Notify/Event_Persistence_Factory.h"

#include "orbsvcs/Notify/Persistent_File_Allocator.h"  // for Persistent_Callback

#include <ace/Vector_T.h>
#include <ace/Malloc_Base.h>  // necessary?

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations of classes/pointers/collections
// referenced from this header
class TAO_Notify_EventChannelFactory;
class TAO_Notify_Method_Request;
class TAO_Notify_ProxyConsumer;
class TAO_Notify_ProxySupplier;
class TAO_Notify_Method_Request_Queueable;

namespace TAO_Notify
{

class Routing_Slip_Persistence_Manager;

// Forward declarations of TAO_Notify classes/pointers/collections
// referenced from this header

/// A vector of Delivery Requests.  The body of a Routing_Slip.
typedef ACE_Vector <Delivery_Request_Ptr> Delivery_Request_Vec;

/// A vector of Methods_.  Used during recovery.
typedef ACE_Vector <TAO_Notify_Method_Request_Queueable *> Delivery_Method_Vec;

class Routing_Slip;
/// A reference-counted smart pointer to a Routing_Slip
typedef ACE_Strong_Bound_Ptr<Routing_Slip, TAO_SYNCH_MUTEX> Routing_Slip_Ptr;

class Routing_Slip_Persistence_Manager;

class Routing_Slip_Queue;

/**
 * \brief Class which manages the delivery of events to destination.
 *
 * Interacts with persistent storage to provide reliable delivery.
 */
class TAO_Notify_Serv_Export Routing_Slip : public Persistent_Callback
{
// FUZZ: disable check_for_ACE_Guard
  typedef ACE_Guard< TAO_SYNCH_MUTEX > Routing_Slip_Guard;
// FUZZ: enable check_for_ACE_Guard
public:
  /// "Factory" method for normal use.
  static Routing_Slip_Ptr create (const TAO_Notify_Event::Ptr& event);

  /// "Factory" method for use during reload from persistent storage.
  static Routing_Slip_Ptr create (
    TAO_Notify_EventChannelFactory & ecf,
    Routing_Slip_Persistence_Manager * rspm);

  void set_rspm (Routing_Slip_Persistence_Manager * rspm);

  void reconnect (void);

  /// Destructor (should be private but that inspires compiler wars)
  virtual ~Routing_Slip ();

  //////////////////
  // Action requests

  /// Route this event to destinations
  /// must be the Action request after
  /// the routing slip is created.
  void route (TAO_Notify_ProxyConsumer* pc, bool reliable_channel);

  /// \brief Schedule delivery to a consumer via a proxy supplier
  /// \param proxy_supplier the proxy supplier that will deliver the event
  /// \param filter should consumer-based filtering be applied?
  void dispatch (TAO_Notify_ProxySupplier * proxy_supplier, bool filter);


  /////////////////////////////////////////
  /// \brief Wait until the event/routing_slip has
  /// been saved at least once.
  void wait_persist ();

  /////////////////////////////////////
  // signals from the rest of the world

  /// \brief A delivery request has been satisfied.
  void delivery_request_complete (size_t request_id);

  /// \brief This Routing_Slip reached the front of the persistence queue
  void at_front_of_persist_queue ();

  /// \brief The persistent storage has completed the last request.
  virtual void persist_complete ();

  /////////////////////////////////////////////////////
  // \brief Access the event associated with this routing slip
  const TAO_Notify_Event::Ptr & event () const;

  /// \brief Provide an identifying number for this Routing Slip
  /// to use in debug messages.
  int sequence() const;

  /// \brief Should delivery of this event be retried if it fails?
  bool should_retry () const;

private:
  ////////////////////
  // state transitions
  void enter_state_transient (Routing_Slip_Guard & guard);
  void continue_state_transient (Routing_Slip_Guard & guard);
  void enter_state_reloaded (Routing_Slip_Guard & guard);
  void enter_state_new (Routing_Slip_Guard & guard);
  void continue_state_new (Routing_Slip_Guard & guard);
  void enter_state_complete_while_new (Routing_Slip_Guard & guard);
  void enter_state_saving (Routing_Slip_Guard & guard);
  void enter_state_saved (Routing_Slip_Guard & guard);
  void enter_state_updating (Routing_Slip_Guard & guard);
  void enter_state_changed_while_saving (Routing_Slip_Guard & guard);
  void continue_state_changed_while_saving (Routing_Slip_Guard & guard);
  void enter_state_changed (Routing_Slip_Guard & guard);
  void continue_state_changed (Routing_Slip_Guard & guard);
  void enter_state_complete (Routing_Slip_Guard & guard);
  void enter_state_deleting (Routing_Slip_Guard & guard);
  void enter_state_terminal (Routing_Slip_Guard & guard);

private:
  bool create_persistence_manager();

  /// Private constructor for use by create method
  Routing_Slip(const TAO_Notify_Event::Ptr& event);

  /// Test to see if all deliveries are complete.
  bool all_deliveries_complete () const;

  /// This routing_slip needs to be saved.
  void add_to_persist_queue(Routing_Slip_Guard & guard);

  /// Marshal into a CDR
  void marshal (TAO_OutputCDR & cdr);

  /// Marshal from CDR
  bool unmarshal (TAO_Notify_EventChannelFactory &ecf, TAO_InputCDR & rscdr);

private:
  /// Protection for internal information
  TAO_SYNCH_MUTEX internals_;
  /// true when event persistence qos is guaranteed
  bool is_safe_;
  /// signalled when is_safe_ goes true
  ACE_SYNCH_CONDITION until_safe_;

  /// Smart pointer to this object
  /// Provides continuity between smart pointers and "Routing_Slip::this"
  /// Also lets the Routing_Slip manage its own minimum lifetime.
  Routing_Slip_Ptr this_ptr_;

  // The event being delivered.
  TAO_Notify_Event::Ptr event_;

  /// A  mini-state machine to control persistence
  /// See external doc for circles and arrows.
  enum State
  {
    rssCREATING,
    rssTRANSIENT,
    rssRELOADED,
    rssNEW,
    rssCOMPLETE_WHILE_NEW,
    rssSAVING,
    rssSAVED,
    rssUPDATING,
    rssCHANGED_WHILE_SAVING,
    rssCHANGED,
    rssCOMPLETE,
    rssDELETING,
    rssTERMINAL
  } state_;

  /// A collection of delivery requests
  Delivery_Request_Vec delivery_requests_;

  /// Methods that should be restarted during event recovery
  Delivery_Method_Vec delivery_methods_;

  /// How many delivery requests are complete
  size_t complete_requests_;

  /// Pointer to a Routing_Slip_Persistence_Manager
  Routing_Slip_Persistence_Manager * rspm_;

  int sequence_;

  static TAO_SYNCH_MUTEX sequence_lock_;
  static int routing_slip_sequence_;
  static size_t count_enter_transient_;
  static size_t count_continue_transient_;
  static size_t count_enter_reloaded_;
  static size_t count_enter_new_;
  static size_t count_continue_new_;
  static size_t count_enter_complete_while_new_;
  static size_t count_enter_saving_;
  static size_t count_enter_saved_;
  static size_t count_enter_updating_;
  static size_t count_enter_changed_while_saving_;
  static size_t count_continue_changed_while_saving_;
  static size_t count_enter_changed_;
  static size_t count_continue_changed_;
  static size_t count_enter_complete_;
  static size_t count_enter_deleting_;
  static size_t count_enter_terminal_;

  static Routing_Slip_Queue persistent_queue_;
};

} // namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_ROUTING_SLIP_H */
