// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IIOP_Transport.h
//
// = DESCRIPTION
//     IIOP Transport specific processing
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IIOP_TRANSPORT_H
#define TAO_IIOP_TRANSPORT_H

#include "tao/Pluggable.h"

// Forward decls.
class TAO_IIOP_Handler_Base;
class TAO_Client_Connection_Handler;
class TAO_Server_Connection_Handler;
class TAO_ORB_Core;

class TAO_Export TAO_IIOP_Transport : public TAO_Transport
{
  // = TITLE
  //   This class acts as a bridge class to the transport specific 
  //   connection handler (handler_).  
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  TAO_IIOP_Transport (TAO_IIOP_Handler_Base *handler,
                      TAO_IIOP_Request_Multiplexing_Strategy *rms = 0,
                      TAO_IIOP_Wait_Strategy *ws = 0);
  // Base object's creator method. 

  ~TAO_IIOP_Transport (void);
  // Default destructor.

  CORBA::ULong tag (void);
  // Returns the specific IOP instance, in this case IIOP.

  void close_connection (void);
  // Call the corresponding connection handlers handle_close method.

  void resume_connection (ACE_Reactor *reactor);
  // Calls the reactors resume_handler on behalf of the corresponding
  // connection handler.

  int idle (void);
  // Idles the corresponding connection handler.

  TAO_IIOP_Handler_Base *&handler (void);
  // Return a reference to the corresponding connection handler.

  int is_nil (TAO_Transport *obj);
  // Returns 0 if the obj is 0, else 1.

  TAO_Transport *_nil (void);
  // Return a NULL pointer of type TAO_Transport *.

  ACE_HANDLE handle (void);
  // Return the underlying connection handle.

  ssize_t send (const ACE_Message_Block *mblk,
                ACE_Time_Value *s = 0);
  // Write the contents of the Message_Block to the connection.

  ssize_t send (const u_char *buf,
                size_t len,
                ACE_Time_Value *s = 0);
  // Write the contents of the buffer of length len to the connection.

  ssize_t send (const iovec *iov,
                int iovcnt,
                ACE_Time_Value *s = 0);
  // Write the contents of iovcnt iovec's to the connection.

  ssize_t recv (char *buf,
                size_t len,
                ACE_Time_Value *s = 0);
  // Read len bytes from into buf.

  ssize_t recv (char *buf,
                size_t len,
                int flags,
                ACE_Time_Value *s = 0);
  // Read len bytes from into buf using flags.

  ssize_t recv (iovec *iov,
                int iovcnt,
                ACE_Time_Value *s = 0);
  //  Read received data into the iovec buffers.

  // @@ Fred, why is this method defined inline?  It should be defined
  // in the *.cpp file!
  virtual int send_request (TAO_ORB_Core *  /* orb_core */,
                            TAO_OutputCDR & /* stream   */,
                            int             /* twoway   */) { return -1; };
  // Default action to be taken for send request.

  virtual int wait_for_reply (CORBA::ULong request_id);
  // Wait for the reply depending on the strategy.

protected:
  TAO_IIOP_Handler_Base *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.

  CORBA::ULong tag_;
  // IIOP tag.
  
  TAO_IIOP_Request_Multiplexing_Strategy *request_reply_strategy_;
  // Strategy to decide whether multiple requests can be sent over the
  // same connection or the connection is exclusive for a request.

  TAO_IIOP_Wait_Strategy *wait_strategy_;
  // Strategy for waiting for the reply after sending the request. 

  // = States for the input message.
  
  size_t len_;
  // Total length of the whole message. This does not include the
  // header length.
  
  size_t offset_;
  // Current offset of the input message.
};

class TAO_Export TAO_IIOP_Client_Transport : public TAO_IIOP_Transport
{
  // = TITLE
  //   The Transport class used for Client side communication with a
  //   server.
  // 
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler,
                             TAO_IIOP_Request_Multiplexing_Strategy *rms = 0,
                             TAO_IIOP_Wait_Strategy *ws = 0););
  // Constructor.  Note, TAO_IIOP_Handler_Base is the base class for
  // both TAO_Client_Connection_Handler and
  // TAO_Server_Connection_Handler.

  ~TAO_IIOP_Client_Transport (void);
  // destructor

  TAO_Client_Connection_Handler *client_handler (void);
  // return a pointer to the client's connection handler.

  int send_request (TAO_ORB_Core *orb_core,
                    TAO_OutputCDR &stream,
                    int twoway);
  // This is a bridge method for the connection handlers
  // <send_request> method.  The connection handler is responsible for
  // concurrency strategies, typically using the leader-follower
  // pattern.

  int handle_client_input (void);
  // Read and handle the reply.

private:
  TAO_Client_Connection_Handler *client_handler_;
  // pointer to the corresponding client side connection handler.
};

class TAO_Export TAO_IIOP_Server_Transport : public TAO_IIOP_Transport
{
  // = TITLE
  //   The Transport class used for server communication with a
  //   connected client.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:

  TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler);
  //  Default creator method.

  ~TAO_IIOP_Server_Transport (void);
  // Default destructor

  TAO_Server_Connection_Handler *server_handler (void);
  //  Return a pointer to the underlying connection handler. 

//  virtual int send_response (TAO_OutputCDR &response);
  // @@ Fred, should this method be removed?

private:
  TAO_Server_Connection_Handler *server_handler_;
  // Pointer to the corresponding connection handler.
};


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

  // = Bind and Find methods for the <Request ID, ReplyHandler>
  //   pairs. The ReplyHandler is not the CORBA ReplyHandler of the 
  //   AMI's.  
  
  virtual int bind_handler (CORBA::ULong request_id,
                            TAO_Reply_Handlern *rh) = 0; 
  // Bind the handler with the request id.
  
  virtual TAO_Reply_Handler* find_handler (CORBA::ULong request_id) = 0;
  // Find the Reply Handler.

  // = "Factory methods" to obtain the CDR stream, in the Muxed case
  //    the factory simply allocates a new one, in the Exclusive case
  //    the factory returns a pointer to the pre-allocated CDR. 

  virtual TAO_InputCDR *get_cdr_stream (void) = 0;
  // Get the CDR stream.

  virtual void destroy_cdr_stream (TAO_InputCDR*) = 0;
  // Destroy the CDR stream.
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

  virtual int bind_handler (CORBA::ULong request_id,
                            TAO_Reply_Handlern *rh);
  // Bind the handler with the request id.
  
  virtual TAO_Reply_Handler* find_handler (CORBA::ULong request_id);
  // Find the Reply Handler.
  
  virtual TAO_InputCDR *get_cdr_stream (void);
  // Create a new CDR stream and return. 

  virtual void destroy_cdr_stream (TAO_InoutCDR *ccdr);
  // Delete the cdr stream.
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

  virtual int bind_handler (CORBA::ULong request_id,
                            TAO_Reply_Handlern *rh);
  // Bind the handler with the request id.
  
  virtual TAO_Reply_Handler* find_handler (CORBA::ULong request_id);
  // Find the Reply Handler.
  
  virtual TAO_InputCDR *get_cdr_stream (void);
  // Return the preallocated CDR stream.
  
  virtual void destroy_cdr_stream (TAO_InputCDR *cdr);
  // NOOP function.

protected:
  CORBA::ULong request_id_;
  // Request id for the current request.
  
  TAO_Reply_Handler *rh_;
  // Reply Handler corresponding to the request. 
  
  TAO_InputCDR cdr_;
  // @@ Preallocated CDR stream.
};  

class TAO_Export TAO_IIOP_Wait_Strategy
{
  // = TITLE
  //
  //    Strategy for waiting for the reply.
  // 
  // = DESCRIPTION
  // 
  
public:
  TAO_IIOP_Wait_Strategy (void);
  // Constructor.
  
  virtual ~TAO_IIOP_Wait_Strategy (void);
  // Destructor.
  
  virtual TAO_GIOP_ReplyStatusType wait (CORBA::ULong request_id,
                                         TAO_IIOP_Request_Multiplexing_Strategy *rms) = 0;
  // Base class virtual method
};

class TAO_Export TAO_Wait_On_Reactor : public TAO_IIOP_Wait_Strategy
{
  // = TITLE
  // 
  //    Wait on the Reactor. Happens in s Single Threaded client
  //    environment. 
  //
  // = DESCRIPTION
  //

public:
  TAO_Wait_On_Reactor (void);
  // Constructor.
  
  virtual ~TAO_Wait_On_Reactor (void);
  // Destructor.
  
  virtual TAO_GIOP_ReplyStatusType wait (CORBA::ULong request_id,
                                         TAO_IIOP_Request_Multiplexing_Strategy *rms);
  // Do the event loop of the Reactor.
};

class TAO_Export TAO_Wait_On_Leader_Follower : public TAO_IIOP_Wait_Strategy
{
  // = TITLE
  //
  //    Wait according to the Leader-Follower model. Leader does the
  //    event loop of the Reactor and the Followers wait on the
  //    condition variable.
  //    
  // = DESCRIPTION
  // 

  TAO_Wait_On_Leader_Follower (void);
  // Constructor.
  
  virtual ~TAO_Wait_On_Leader_Follower (void);
  // Destructor.
  
  virtual TAO_GIOP_ReplyStatusType wait (CORBA::ULong request_id,
                                         TAO_IIOP_Request_Multiplexing_Strategy *rms);
  // Wait according to the L-F model.
};

class TAO_Export TAO_Wait_On_Read :  public TAO_IIOP_Wait_Strategy
{
  // = TITLE
  // 
  //    Wait on receiving the reply.
  //
  // = DESCRIPTION
  //

  TAO_Wait_On_Read (void);
  // Constructor.

  virtual ~TAO_Wait_On_Read (void);
  // Destructor.

  virtual TAO_GIOP_ReplyStatusType wait (CORBA::ULong request_id,
                                         TAO_IIOP_Request_Multiplexing_Strategy *rms);
  // Wait on the read operation.
};
#endif  /* TAO_IIOP_TRANSPORT_H */
