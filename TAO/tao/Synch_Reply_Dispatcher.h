// This may look like C, but it's really -*- C++ -*-


//=============================================================================
/**
 *  @file    Synch_Reply_Dispatcher.h
 *
 *  $Id$
 *
 *   Dispatch the reply appropriately.
 *
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SYNCH_REPLY_DISPATCHER_H
#define TAO_SYNCH_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/Reply_Dispatcher.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LF_Invocation_Event.h"
#include "tao/GIOP_Message_Version.h"
#include "tao/CDR.h"

class TAO_Pluggable_Reply_Params;

namespace IOP
{
  class ServiceContextList;
}

/**
 * @class TAO_Synch_Reply_Dispatcher
 *
 * @brief Reply dispatcher for Synchoronous Method Invocation (SMI)s.
 *
 */
class TAO_Export TAO_Synch_Reply_Dispatcher
  : public TAO_Reply_Dispatcher
  , public TAO_LF_Invocation_Event
{

public:
  /// Constructor.
  TAO_Synch_Reply_Dispatcher (TAO_ORB_Core *orb_core,
                              IOP::ServiceContextList &sc);

  /// Destructor.
  virtual ~TAO_Synch_Reply_Dispatcher (void);

  /// Return the reply CDR.
  TAO_InputCDR &reply_cdr (void);

  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  virtual void connection_closed (void);

protected:
  /// The service context list
  IOP::ServiceContextList &reply_service_info_;

private:
  /// Cache the ORB Core pointer.
  TAO_ORB_Core *orb_core_;

  /* @@todo: At some point of time we are going to get to a situation
     where TAO has huge stack sizes. Need to think on how we would
     deal with that. One idea would be to push these things on TSS as
     this is created by the thread on a per invocation basis. Post 1.2
     would be a nice time for that I guess
  */

  /// The buffer that is used to initialise the data block
  char buf_[ACE_CDR::DEFAULT_BUFSIZE];

  /// datablock that is created on teh stack to initialise the CDR
  /// stream underneath.
  ACE_Data_Block db_;

  /// CDR stream which has the reply information that needs to be
  /// demarshalled by the stubs
  TAO_InputCDR reply_cdr_;
};

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
