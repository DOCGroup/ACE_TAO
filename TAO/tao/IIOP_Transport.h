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
//     Fred Kuhns
//
// ============================================================================

#ifndef TAO_IIOP_TRANSPORT_H
#define TAO_IIOP_TRANSPORT_H

#include "tao/Pluggable.h"

class TAO_IIOP_Handler_Base;
class TAO_Client_Connection_Handler;
class TAO_Server_Connection_Handler;
class TAO_ORB_Core;

class TAO_IIOP_Transport : public TAO_Transport
{
public:

  TAO_IIOP_Transport (TAO_IIOP_Handler_Base* handler);
  ~TAO_IIOP_Transport ();

  CORBA::ULong tag (void);

  void close_conn (void);
  void resume_conn (ACE_Reactor *reactor);

  int idle (void);

  TAO_IIOP_Handler_Base*& handler (void);
  virtual int is_nil (TAO_Transport *obj);
  virtual TAO_Transport *_nil (void);

  ACE_HANDLE handle(void);

  virtual ssize_t send (const ACE_Message_Block *mblk);
  virtual ssize_t send (const u_char *buf, size_t len);
  virtual ssize_t send (const iovec *iov, int iovcnt);

  virtual ssize_t receive (char *buf, size_t len);
  virtual ssize_t receive (char *buf, size_t len, int flags);
  virtual ssize_t receive (iovec *iov, int iovcnt);

  int send_request (TAO_ORB_Core * /* orb_core */,
                    TAO_OutputCDR & /* stream */,
                    int /* twoway */) { return -1; }

protected:

  TAO_IIOP_Handler_Base *handler_;
  // the connection service handler used for accessing
  // lower layer communication protocols.

  CORBA::ULong tag_;
  // IIOP tag.

  ssize_t do_sendv (const iovec *iov, int iovcnt, int total_bytes);

};

class TAO_IIOP_Client_Transport : public TAO_IIOP_Transport
{
public:

  TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler);
  // constructor.  Note, TAO_IIOP_Handler_Base is the base
  // class for both TAO_Client_Connection_Handler and
  // TAO_Server_Connection_Handler.

  ~TAO_IIOP_Client_Transport();
  // destructor

  TAO_Client_Connection_Handler* client_handler (void);

  int send_request (TAO_ORB_Core *orb_core,
                    TAO_OutputCDR &stream,
                    int twoway);
private:
  TAO_Client_Connection_Handler *client_handler_;
};

class TAO_IIOP_Server_Transport : public TAO_IIOP_Transport
{
public:

  TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler);

  ~TAO_IIOP_Server_Transport ();
  // destructor

  TAO_Server_Connection_Handler* server_handler (void);

//  virtual int send_response (TAO_OutputCDR &response);

private:
  TAO_Server_Connection_Handler *server_handler_;
};

#endif  /* TAO_IIOP_TRANSPORT_H */
