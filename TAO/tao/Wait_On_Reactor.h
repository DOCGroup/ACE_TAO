// -*- C++ -*-

//=============================================================================
/**
 *  @file    Wait_On_Reactor.h
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_WAIT_ON_REACTOR_H
#define TAO_WAIT_ON_REACTOR_H

#include /**/ "ace/pre.h"

#include "tao/Wait_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Wait_On_Reactor
 *
 * Wait on the Reactor. Happens in a single threaded client environment.
 */
class TAO_Wait_On_Reactor : public TAO_Wait_Strategy
{
public:
  /// Constructor.
  explicit TAO_Wait_On_Reactor (TAO_Transport *transport);

  /// Destructor.
  ~TAO_Wait_On_Reactor () override = default;

  /*! @copydoc TAO_Wait_Strategy::wait() */
  int wait (ACE_Time_Value *max_wait_time, TAO_Synch_Reply_Dispatcher &rd) override;

  /*! @copydoc TAO_Wait_Strategy::register_handler() */
  int register_handler () override;

  /*! @copydoc TAO_Wait_Strategy::non_blocking() */
  bool non_blocking () const override;

  /*! @copydoc TAO_Wait_Strategy::can_process_upcalls() */
  bool can_process_upcalls () const override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_WAIT_ON_REACTOR_H */
