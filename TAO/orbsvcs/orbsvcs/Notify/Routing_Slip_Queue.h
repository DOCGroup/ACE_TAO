// -*- C++ -*-

#ifndef TAO_NOTIFY_ROUTING_SLIP_QUEUE_H
#define TAO_NOTIFY_ROUTING_SLIP_QUEUE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/Routing_Slip.h"

#include <deque>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  /**
   * \brief A queue of Routing_Slips waiting to be persisted.
   *
   * The Routing_Slip_Queue keeps a queue of Routing_Slips waiting
   * to be written to persistent storage.  The "allowed" parameter
   * determines how many Routing_Slips can be handled simultaneously
   * by the persistent storage.  Until this threshold is reached,
   * Routing_Slips are not held in the queue, but pass straight through.
   *
   * Once the allowe number of Routing_Slips are being handled, any
   * additional requests are held in the queue until persistence is
   * complete for another Routing_Slips.
   *
   * Having Routing_Slips waiting in the queue is "a good thing" [TM]
   * because it allows delivery completions to be applied to the
   * routing slip before it is written -- thereby reducing or completely
   * eliminating the number of actual writes to persistent storage.
   *
   * Experimentation indicates that a good value for "allowed" is 1.
   *
   * Allowed == 0 is treated as a special case meaning pass all Routing_Slips
   * through the queue immediately.  Setting it a good way to test how well
   * your storage device withstands continuous beating.
   */
  class TAO_Notify_Serv_Export Routing_Slip_Queue
  {
    using Queue = std::deque<Routing_Slip_Ptr>;
// FUZZ: disable check_for_ACE_Guard
    using Guard = ACE_Guard< TAO_SYNCH_MUTEX >;
// FUZZ: enable check_for_ACE_Guard
  public:
    /**
     * \brief Construct setting "allowed".
     * \param allowed the number of Routing_Slips that can be handled
     *  simultaneously by the persistent store.
     */
    explicit Routing_Slip_Queue (size_t allowed = 1);

    /// Destructor.
    ~Routing_Slip_Queue () = default;

    Routing_Slip_Queue (Routing_Slip_Queue const &) = delete;
    Routing_Slip_Queue (Routing_Slip_Queue &&) = delete;
    Routing_Slip_Queue &operator= (Routing_Slip_Queue const &) = delete;
    Routing_Slip_Queue &operator= (Routing_Slip_Queue &&) = delete;

    /**
     * \brief Add a routing slip to the tail of the queue and dispatch if necessary.
     */
    void add (const Routing_Slip_Ptr & routing_slip);
    /**
     * \brief A call back to indicate that processing is complete for a previously-queued
     * Routing_Slip.
     */
    void complete ();

    /**
     * /brief Adjust the "allowed" value on-the-fly (not recommended, but it works.)
     */
    void set_allowed (size_t allowed);

  private:
    void dispatch (Guard & guard);
    bool dispatch_one (Guard & guard);

  private:
    // configuration setting
    size_t allowed_;
    /// Protection for internal information
    TAO_SYNCH_MUTEX internals_;
    size_t active_ {};
    Queue queue_;
  };
} // namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_ROUTING_SLIP_QUEUE_H */
