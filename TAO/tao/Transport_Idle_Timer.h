/**
 *  @file    Transport_Idle_Timer.h
 *
 * Periodic reactor timer scanning a thread lane's transport cache.
 *
 *  @author  Johnny Willemsen
 */

#ifndef TAO_TRANSPORT_IDLE_TIMER_H
#define TAO_TRANSPORT_IDLE_TIMER_H

#include /**/ "ace/pre.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Transport_Idle_Timer_Handler
  {
  public:
    virtual ~Transport_Idle_Timer_Handler () = default;
    virtual void scan_idle_transports () = 0;
  };

  /**
  * @class Transport_Idle_Timer
  *
  * @brief Periodic reactor timer scanning one thread lane's transport cache.
  *
  * Owned and cancelled by the transport cache manager.
  */
  class Transport_Idle_Timer : public ACE_Event_Handler
  {
  public:
    explicit Transport_Idle_Timer (Transport_Idle_Timer_Handler *handler);
    ~Transport_Idle_Timer () override = default;

    /// Reactor callback: scan the cache for eligible idle transports.
    int handle_timeout (const ACE_Time_Value &current_time,
                        const void *act = nullptr) override;

    Transport_Idle_Timer () = delete;
    Transport_Idle_Timer (const Transport_Idle_Timer &) = delete;
    Transport_Idle_Timer &operator= (const Transport_Idle_Timer &) = delete;
    Transport_Idle_Timer (Transport_Idle_Timer &&) = delete;
    Transport_Idle_Timer &operator= (Transport_Idle_Timer &&) = delete;

  private:
    Transport_Idle_Timer_Handler *handler_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_IDLE_TIMER_H */
