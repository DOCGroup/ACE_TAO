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
  
  void request_id (CORBA::ULong request_id);
  // Set the request id.
  
  CORBA::ULong request_id (void) const;
  // Return the request id.

  void reply_status (CORBA::ULong reply_status);
  // Set the reply status. Reply status is stored as read from the
  // incoming message. Readers of this data should see it whether this
  // number fits into the TAO_GIOP_ReplyStatusType type. 
  
  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  void cdr (TAO_InputCDR *cdr);
  // Set the CDR which the has the reply message.

  TAO_InputCDR *cdr (void) const;
  // Get the CDR stream.
  
  virtual int dispatch_reply (void) = 0;
  // Dispatch the reply.

protected:
  CORBA::ULong request_id_;
  // Request ID for this request.
  
  TAO_InputCDR *cdr_;
  // CDR stream for reading the input.
  
  CORBA::ULong reply_status_;
  // Replt status.
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
  TAO_Synch_Reply_Dispatcher (void);
  // Constructor.

  virtual ~TAO_Synch_Reply_Dispatcher (void);
  // Destructor.
  
  virtual int dispatch_reply (void);
  // NO OP.
};

#endif /* TAO_REPLY_DISPATCHER_H */
