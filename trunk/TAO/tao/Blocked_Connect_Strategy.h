// -*- C++ -*-

//=============================================================================
/**
 *  @file Blocked_Connect_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_BLOCKED_CONNECT_STRATEGY_H
#define TAO_BLOCKED_CONNECT_STRATEGY_H
#include /**/ "ace/pre.h"

#include "Connect_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Blocked_Connect_Strategy
 *
 * @brief Concrete implementation of a connect strategy that blocks
 *  during connects
 *
 */

class TAO_Export TAO_Blocked_Connect_Strategy : public TAO_Connect_Strategy
{
public:
  /// Contructor
  TAO_Blocked_Connect_Strategy (TAO_ORB_Core *orb);

  /// Destructor
  ~TAO_Blocked_Connect_Strategy (void);

  /*
   * Concrete implementation for this class. Please see
   * Connect_Strategy.h for details
   */
  virtual void synch_options (ACE_Time_Value *val,
                              ACE_Synch_Options &opt);

  virtual int wait (TAO_Connection_Handler *ch,
                     ACE_Time_Value *val);

  virtual int wait (TAO_Transport *t,
                     ACE_Time_Value *val);
};


#include /**/ "ace/post.h"
#endif /*TAO_BLOCKED_CONNECT_STRATEGY_H*/
