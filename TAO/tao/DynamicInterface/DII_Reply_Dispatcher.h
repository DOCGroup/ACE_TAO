// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_Reply_Dispatcher.h
 *
 *  $Id$
 *
 *  @brief Dispatch the reply appropriately.
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */

#ifndef TAO_DII_REPLY_DISPATCHER_H
#define TAO_DII_REPLY_DISPATCHER_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Asynch_Reply_Dispatcher_Base.h"

#if defined (TAO_HAS_AMI)
#include "tao/Messaging/Messaging.h"
#endif /* TAO_HAS_AMI */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Request;
  typedef Request *Request_ptr;
}

class TAO_ORB_Core;

/**
 * @class TAO_DII_Deferred_Reply_Dispatcher
 *
 * @brief Reply dispatcher for DII deferred requests.
 */
class TAO_DynamicInterface_Export TAO_DII_Deferred_Reply_Dispatcher
  : public TAO_Asynch_Reply_Dispatcher_Base
{
public:
  TAO_DII_Deferred_Reply_Dispatcher (const CORBA::Request_ptr req,
                                     TAO_ORB_Core *orb_core);



  /// The Reply_Dispatcher methods
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &param);

  virtual void connection_closed (void);

  /// The following methods are not needed for this class..
  virtual void reply_timed_out (void)  {}
  virtual long schedule_timer (CORBA::ULong , const ACE_Time_Value &)
  {
    return 0;
  }

protected:
  /// Destructor.
  ~TAO_DII_Deferred_Reply_Dispatcher (void);

private:
  /// Where the reply needs to go.
  const CORBA::Request_ptr req_;
};

#if defined (TAO_HAS_AMI)
/**
 * @class TAO_DII_Asynch_Reply_Dispatcher
 *
 * @brief Reply dispatcher for DII asynch requests.
 */
class TAO_DynamicInterface_Export TAO_DII_Asynch_Reply_Dispatcher
  : public TAO_Asynch_Reply_Dispatcher_Base
{
public:
  TAO_DII_Asynch_Reply_Dispatcher (const Messaging::ReplyHandler_ptr callback,
                                     TAO_ORB_Core *orb_core);
  virtual ~TAO_DII_Asynch_Reply_Dispatcher (void);

  /// The Reply_Dispatcher methods
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &param);

  virtual void connection_closed (void);

private:

  /// The buffer that is used to initialise the data block
  char buf_[ACE_CDR::DEFAULT_BUFSIZE];

  /// Datablock that is created on the stack to initialise the CDR
  /// stream underneath.
  ACE_Data_Block db_;

  /// CDR stream for reading the input.
  TAO_InputCDR reply_cdr_;

  /// Where the reply needs to go.
  const Messaging::ReplyHandler_ptr callback_;
};
#endif /* TAO_HAS_AMI */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_REPLY_DISPATCHER_H */
