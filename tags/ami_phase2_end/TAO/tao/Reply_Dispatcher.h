// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Reply_Dispatcher.h
//
// = DESCRIPTION
//     Dispatch the reply appropriately.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_REPLY_DISPATCHER_H
#define TAO_REPLY_DISPATCHER_H

#include "tao/GIOP.h"

// Forward Declarations.

class TAO_Stub;
class TAO_InputCDR;

class TAO_Export TAO_Reply_Dispatcher
{
  // = TITLE
  //
  // = DESCRIPTION
  //

public:
  TAO_Reply_Dispatcher (void);
  // Constructor.

  virtual ~TAO_Reply_Dispatcher (void);
  // Destructor.

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              TAO_GIOP_ServiceContextList& reply_ctx,
                              TAO_InputCDR* cdr) = 0;
  // Dispatch the reply.

  virtual TAO_InputCDR *cdr (void) const;
  // Get the CDR stream (if any)
};

class TAO_Export TAO_Synch_Reply_Dispatcher : public TAO_Reply_Dispatcher
{
  // = TITLE
  //
  //     Reply dispatcher for Synchoronous Method Invocation (SMI)s.
  //
  // = DESCRIPTION
  //

public:
  TAO_Synch_Reply_Dispatcher (TAO_InputCDR* cdr);
  // Constructor.

  virtual ~TAO_Synch_Reply_Dispatcher (void);
  // Destructor.

  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  const TAO_GIOP_Version& version (void) const;
  // Get the GIOP version

  TAO_GIOP_ServiceContextList& reply_ctx (void);
  // Get the reply context

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              TAO_GIOP_ServiceContextList& reply_ctx,
                              TAO_InputCDR* cdr);
  virtual TAO_InputCDR *cdr (void) const;

private:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.

  TAO_GIOP_Version version_;
  // The version

  TAO_GIOP_ServiceContextList reply_ctx_;
  // The service context list

  TAO_InputCDR *cdr_;
  // CDR stream for reading the input.
};

#if defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_REPLY_DISPATCHER_H */
