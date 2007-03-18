// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Wait_On_Read.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_WAIT_ON_READ_H
#define TAO_WAIT_ON_READ_H

#include /**/ "ace/pre.h"

#include "tao/Wait_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Wait_On_Read
 *
 *
 * Simply block on read() to wait for the reply.
 */
class TAO_Wait_On_Read :  public TAO_Wait_Strategy
{

public:
  /// Constructor.
  TAO_Wait_On_Read (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Wait_On_Read (void);

  // = Documented in TAO_Wait_Strategy.

  virtual int wait (ACE_Time_Value *max_wait_time,
                    TAO_Synch_Reply_Dispatcher &rd);
  virtual int register_handler (void);
  virtual bool non_blocking (void) const;
  virtual bool can_process_upcalls (void) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_WAIT_ON_READ_H */
