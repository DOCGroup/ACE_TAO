// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = DESCRIPTION
//     Dispatch the reply appropriately.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_DII_REPLY_DISPATCHER_H
#define TAO_DII_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dynamicinterface_export.h"
#include "tao/Messaging/Asynch_Reply_Dispatcher.h"
#include "tao/Asynch_Reply_Dispatcher_Base.h"

class TAO_ORB_Core;

class TAO_DynamicInterface_Export TAO_DII_Deferred_Reply_Dispatcher
  : public TAO_Asynch_Reply_Dispatcher_Base
{
  // = TITLE
  //    TAO_DII_Deferred_Reply_Dispatcher
  //
  // = DESCRIPTION
  //    Reply dispatcher for DII deferred requests.
  //
public:
  TAO_DII_Deferred_Reply_Dispatcher (const CORBA::Request_ptr req,
                                     TAO_ORB_Core *orb_core);
 // Constructor.

  virtual ~TAO_DII_Deferred_Reply_Dispatcher (void);
  // Destructor.

  // = The Reply_Dispatcher methods
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &param);

  virtual void connection_closed (void);

private:

  /// The buffer that is used to initialise the data block
  char buf_[ACE_CDR::DEFAULT_BUFSIZE];

  /// datablock that is created on teh stack to initialise the CDR
  /// stream underneath.
  ACE_Data_Block db_;

  /// CDR stream for reading the input.
  TAO_InputCDR reply_cdr_;

  /// Where the reply needs to go.
  const CORBA::Request_ptr req_;
};

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
