// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Wait_On_Reactor.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_WAIT_ON_REACTOR_H
#define TAO_WAIT_ON_REACTOR_H
#include "ace/pre.h"

#include "tao/Wait_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Wait_On_Reactor
 *
 * @brief Wait on the Reactor. Happens in s Single Threaded client
 * environment.
 *
 */
class TAO_Export TAO_Wait_On_Reactor : public TAO_Wait_Strategy
{

public:
  /// Constructor.
  TAO_Wait_On_Reactor (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Wait_On_Reactor (void);

  // = Documented in TAO_Wait_Strategy.

  virtual int wait (ACE_Time_Value *max_wait_time,
                    TAO_Synch_Reply_Dispatcher &rd);
  virtual int register_handler (void);
  virtual int non_blocking (void);
};

#include "ace/post.h"
#endif /* TAO_WAIT_ON_REACTOR_H */
