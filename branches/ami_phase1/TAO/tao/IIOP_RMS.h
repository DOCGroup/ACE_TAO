// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IIOP_RMS.h
//
// = DESCRIPTION
//     IIOP Request Multiplexing Strategies.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IIOP_RMS_H
#define TAO_IIOP_RMS_H

#include "tao/CDR.h"

// Forward declarations.

class TAO_IIOP_Reply_Dispatcher;


class TAO_Export TAO_IIOP_Request_Multiplexing_Strategy
{
  // = TITLE
  // 
  //     Strategy to determine whether the connection should be
  //     multiplexed for multiple requests or it is exclusive for a
  //     single request at a time. 
  // 
  // = DESCRIPTION
  // 
  
public:
  TAO_IIOP_Request_Multiplexing_Strategy (void);
  // Base class constructor.

  virtual ~TAO_IIOP_Request_Multiplexing_Strategy (void);
  // Base class destructor.

  virtual CORBA::ULong request_id (void)  = 0;
  // Generate and return an unique request id for the current
  // invocation.  

  // = Bind and Find methods for the <Request ID, ReplyDispatcher>
  //   pairs. The ReplyDispatcher is not the CORBA ReplyDispatcher of
  //   the AMI's.  
  
  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_IIOP_Reply_Dispatcher *rh) = 0; 
  // Bind the dispatcher with the request id.
  
  virtual TAO_IIOP_Reply_Dispatcher* find_dispatcher (CORBA::ULong request_id) = 0;
  // Find the Reply Dispatcher.

  // = "Factory methods" to obtain the CDR stream, in the Muxed case
  //    the factory simply allocates a new one, in the Exclusive case
  //    the factory returns a pointer to the pre-allocated CDR. 

  virtual void set_cdr_stream (TAO_InputCDR *cdr) = 0;
  // Set the CDR stream.

  virtual TAO_InputCDR *get_cdr_stream (void);
  // Get the CDR stream.
  
  virtual void destroy_cdr_stream (void) = 0;
  // Destroy the CDR stream.

protected:
  TAO_InputCDR *cdr_;
  // Pointer to the CDR stream used to read the incoming message. This
  // is obtained from the Invocation object. This is preallocated in
  // the SMI and dynamically allocated in AMI.
};
  
class TAO_Export TAO_IIOP_Muxed_RMS : public  TAO_IIOP_Request_Multiplexing_Strategy
{
  // = TITLE
  //
  //    Connection is multiplexed for many requests.
  //
  // = DESCRIPTION
  // 

public:
  TAO_IIOP_Muxed_RMS (void);
  // Constructor.
  
  virtual ~TAO_IIOP_Muxed_RMS (void);
  // Destructor.

  virtual CORBA::ULong request_id (void);
  // Generate and return an unique request id for the current
  // invocation.  

  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_IIOP_Reply_Dispatcher *rh);
  // Bind the dispatcher with the request id.
  
  virtual TAO_IIOP_Reply_Dispatcher* find_dispatcher (CORBA::ULong request_id);
  // Find the Reply Dispatcher.
  
  // virtual TAO_InputCDR *cdr_stream (void);
  // Create a new CDR stream and return. 

  virtual void set_cdr_stream (TAO_InputCDR *cdr);
  // Set the CDR stream.

  // virtual TAO_InputCDR *cdr_stream (void);
  // Get the CDR stream.

  virtual void destroy_cdr_stream (void);
  // Delete the cdr stream.

protected:
  // @@ HASH TABLE???
};  

class TAO_Export TAO_IIOP_Exclusive_RMS : public TAO_IIOP_Request_Multiplexing_Strategy
{
  // = TITLE
  //   
  //    Connection exclusive for the request.
  //
  // = DESCRIPTION
  // 

public:
  TAO_IIOP_Exclusive_RMS (void);
  // Constructor.
  
  virtual ~TAO_IIOP_Exclusive_RMS (void);
  // Destructor.

  virtual CORBA::ULong request_id (void);
  // Generate and return an unique request id for the current
  // invocation. We can actually return a predecided ULong, since we
  // allow only one invocation over this connection at a time.

  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_IIOP_Reply_Dispatcher *rh);
  // Bind the dispatcher with the request id.
  
  virtual TAO_IIOP_Reply_Dispatcher* find_dispatcher (CORBA::ULong request_id);
  // Find the Reply Dispatcher.
  
  // virtual TAO_InputCDR *get_cdr_stream (void);
  // Return the preallocated CDR stream.
  
  virtual void set_cdr_stream (TAO_InputCDR *cdr);
  // Set the CDR stream.

  // virtual TAO_InputCDR *cdr_stream (void);
  // Get the CDR stream.

  virtual void destroy_cdr_stream (void);
  // NO-OP function.

protected:
  CORBA::ULong request_id_;
  // Request id for the current request.
  
  TAO_IIOP_Reply_Dispatcher *rd_;
  // Reply Dispatcher corresponding to the request. 
};  
#endif /* IIOP_RMS_H */
