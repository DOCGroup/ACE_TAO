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

class TAO_IIOP_Transport : public TAO_Transport
{
  // = TITLE
  //   @@ Fred, please fill in here.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  TAO_IIOP_Transport (TAO_IIOP_Handler_Base *handler);
  // @@ Fred, please fill in here.

  ~TAO_IIOP_Transport (void);
  // @@ Fred, please fill in here.

  CORBA::ULong tag (void);
  // @@ Fred, please fill in here.

  // @@ Fred, please rename this to "close_connection".
  void close_conn (void);
  // @@ Fred, please fill in here.

  void resume_conn (ACE_Reactor *reactor);
  // @@ Fred, please fill in here.

  int idle (void);
  // @@ Fred, please fill in here.

  TAO_IIOP_Handler_Base *&handler (void);
  // @@ Fred, please fill in here.

  virtual int is_nil (TAO_Transport *obj);
  // @@ Fred, please fill in here.

  virtual TAO_Transport *_nil (void);
  // @@ Fred, please fill in here.

  ACE_HANDLE handle (void);
  // @@ Fred, please fill in here.

  virtual ssize_t send (const ACE_Message_Block *mblk);
  // @@ Fred, please fill in here.

  virtual ssize_t send (const u_char *buf, size_t len);
  // @@ Fred, please fill in here.

  virtual ssize_t send (const iovec *iov, int iovcnt);
  // @@ Fred, please fill in here.

  virtual ssize_t receive (char *buf, size_t len);
  // @@ Fred, please fill in here.

  // @@ Fred, the following method isn't defined in Pluggable.h.
  // Please check to make sure that this is correct...
  virtual ssize_t receive (char *buf, size_t len, int flags);
  // @@ Fred, please fill in here.

  virtual ssize_t receive (iovec *iov, int iovcnt);
  // @@ Fred, please fill in here.

  int send_request (TAO_ORB_Core * /* orb_core */,
                    TAO_OutputCDR & /* stream */,
                    int /* twoway */) { return -1; }
  // @@ Fred, please fill in here.

protected:
  TAO_IIOP_Handler_Base *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.

  CORBA::ULong tag_;
  // IIOP tag.

  ssize_t do_sendv (const iovec *iov,
                    int iovcnt,
                    int total_bytes);
  // @@ Fred, please fill in here.
};

class TAO_IIOP_Client_Transport : public TAO_IIOP_Transport
{
  // = TITLE
  //   @@ Fred, please fill in here.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.

public:
  TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler);
  // constructor.  Note, TAO_IIOP_Handler_Base is the base class for
  // both TAO_Client_Connection_Handler and
  // TAO_Server_Connection_Handler.

  ~TAO_IIOP_Client_Transport (void);
  // destructor

  TAO_Client_Connection_Handler *client_handler (void);
  //   @@ Fred, please fill in here.

  int send_request (TAO_ORB_Core *orb_core,
                    TAO_OutputCDR &stream,
                    int twoway);
  //   @@ Fred, please fill in here.

private:
  TAO_Client_Connection_Handler *client_handler_;
  //   @@ Fred, please fill in here.
};

class TAO_IIOP_Server_Transport : public TAO_IIOP_Transport
{
  // = TITLE
  //   @@ Fred, please fill in here.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:

  TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler);
  //   @@ Fred, please fill in here.

  ~TAO_IIOP_Server_Transport (void);
  // destructor

  TAO_Server_Connection_Handler *server_handler (void);
  //   @@ Fred, please fill in here.

//  virtual int send_response (TAO_OutputCDR &response);

private:
  TAO_Server_Connection_Handler *server_handler_;
  //   @@ Fred, please fill in here.
};

#endif  /* TAO_IIOP_TRANSPORT_H */
