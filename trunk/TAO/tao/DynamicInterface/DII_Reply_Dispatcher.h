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
#include "tao/Asynch_Reply_Dispatcher.h"

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
  TAO_DII_Deferred_Reply_Dispatcher (const CORBA::Request_ptr req);
 // Constructor.

  virtual ~TAO_DII_Deferred_Reply_Dispatcher (void);
  // Destructor.

  // = The Reply_Dispatcher methods
  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State *message_state);

  virtual void connection_closed (void);

private:
  const CORBA::Request_ptr req_;
  // Where the reply needs to go.
};

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
