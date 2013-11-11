// -*- C++ -*-
//=============================================================================
/**
 *  @file   Validate_Client_Task.h
 *
 *  $Id$
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================

#ifndef NOTIFY_VALIDATE_CLIENT_TASK_H
#define NOTIFY_VALIDATE_CLIENT_TASK_H

#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#include "tao/ORB.h"
#include "tao/Condition.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_EventChannelFactory;


/**
 * @class TAO_Notify_validate_client_Task
 *
 * @brief ClientControl
 *
 * Defines the interface for the consumer control strategy.
 * This strategy handles misbehaving or failing consumers.
 * = MEMORY MANAGMENT
 * = LOCKING
 * = TODO
 */
class TAO_Notify_Serv_Export TAO_Notify_validate_client_Task
  : public ACE_Task<ACE_NULL_SYNCH>
{
public:

  typedef TAO_Condition<TAO_SYNCH_MUTEX> CONDITION;

  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_Notify_validate_client_Task (const ACE_Time_Value &delay,
                                     const ACE_Time_Value &interval,
                                     TAO_Notify_EventChannelFactory *ecf);

  /// destructor...
  virtual ~TAO_Notify_validate_client_Task (void);

  virtual int svc (void);

  void shutdown (void);

private:
  /// The delay of first time validate.
  ACE_Time_Value delay_;

  /// The validate interval
  ACE_Time_Value interval_;

  /// The event channel
  TAO_Notify_EventChannelFactory *ecf_;

  CONDITION condition_;

  bool shutdown_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* NOTIFY_VALIDATE_CLIENT_TASK_H */

