// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Muxed_TMS.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_MUXED_TMS_H
#define TAO_MUXED_TMS_H
#include "ace/pre.h"

#include "tao/Transport_Mux_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager.h"

class TAO_Pluggable_Reply_Params;

/**
 * @class TAO_Muxed_TMS
 *
 * Using this strategy a single connection can have multiple
 * outstanding requests.
 * @@ Can the performance of the demuxer be made more predictable,
 * for example, using the request id as an active demux key?
 * NOTE: check the OMG resolutions about bidirectional
 * connections, it is possible that the request ids can only
 * assume even or odd values.
 */
class TAO_Export TAO_Muxed_TMS : public TAO_Transport_Mux_Strategy
{

public:
  /// Constructor.
  TAO_Muxed_TMS (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Muxed_TMS (void);

  /// Generate and return an unique request id for the current
  /// invocation.
  virtual CORBA::ULong request_id (void);

  // = Please read the documentation in the TAO_Transport_Mux_Strategy
  //   class.
  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rh);
  virtual int unbind_dispatcher (CORBA::ULong request_id);

  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  virtual int idle_after_send (void);
  virtual int idle_after_reply (void);
  virtual void connection_closed (void);

protected:
  /// Used to generate a different request_id on each call to
  /// request_id().
  CORBA::ULong request_id_generator_;

  /// Keep track of the orb core pointer. We need to this to create the
  /// Reply Dispatchers.
  TAO_ORB_Core *orb_core_;

  typedef ACE_Hash_Map_Manager_Ex <CORBA::ULong,
                                   TAO_Reply_Dispatcher *,
                                   ACE_Hash <CORBA::ULong>,
                                   ACE_Equal_To <CORBA::ULong>,
                                   ACE_Null_Mutex> REQUEST_DISPATCHER_TABLE;

  /// Table of <Request ID, Reply Dispatcher> pairs.
  REQUEST_DISPATCHER_TABLE dispatcher_table_;
};

#include "ace/post.h"
#endif /* MUXED_TMS_H */
