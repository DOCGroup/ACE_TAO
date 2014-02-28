// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Connect_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_LF_CONNECT_STRATEGY_H
#define TAO_LF_CONNECT_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/Connect_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
class ACE_Synch_Options;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LF_Event;

/**
 * @class TAO_LF_Connect_Strategy
 *
 * @brief Concrete implementation of a connect strategy that waits on
 *  the leader-follower during asynchronous connects
 */

class TAO_Export TAO_LF_Connect_Strategy : public TAO_Connect_Strategy
{
public:
  /// Constructor
  TAO_LF_Connect_Strategy (TAO_ORB_Core *orb);

  /// Destructor
  ~TAO_LF_Connect_Strategy (void);

  /**
   * Concrete implementation for this class. Please see
   * Connect_Strategy.h for details
   */
  virtual void synch_options (ACE_Time_Value *val, ACE_Synch_Options &opt);

protected:
  virtual int wait_i (TAO_LF_Event *ev, TAO_Transport *t, ACE_Time_Value *val);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_LF_CONNECT_STRATEGY_H*/
