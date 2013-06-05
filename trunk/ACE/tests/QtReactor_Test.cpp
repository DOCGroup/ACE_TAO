/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    QtReactor_Test.cpp
 *
 *  $Id$
 *
 *  Simple test of QtReactor. Test is intended to verify if QtReactor
 *  correctly cooperates with Qt event loop in typical application. Test
 *  creates a number of timers which send datagrams. These datagrams are
 *  expected by datagram_handlers registered in reactor. Test asynchronously
 *  establishes also a number of loopback tcp connections using ACE_Acceptors
 *  and ACE_Connectors. Socket activities are handled asynchronously to ensure
 *  that reactor does not lose events in traffic transmission. Moreover, test
 *  registers and removes handlers frequently to cover register/remove_handler
 *  method of QtReactor which are known (10/07/2004) to be buggy.
 *
 *  Classes:
 *    QTestApplication     - main qt application running event loop for a
 *                           finite time
 *    Dgram_Handler        - responsible for sending and receiving datagrams as
 *                           well as handling timeouts. Datagrams are sent in
 *                           handle_timeout method.
 *    TCPConnectionHandler - connection handler responsible for sending and
 *                           receiving data using tcp streams.
 *    TCPAcceptorHandler   - acceptor responsible for acceptance and
 *                           registration of connections in HandlersRegister
 *                           class.
 *    HandlersRegister     - register of event_handlers, responsible also for
 *                           the analysis of test results.
 *
 *
 *  @author Marek Brudka <mbrudka@elka.pw.edu.pl>
 */
//=============================================================================


#include "test_config.h"

#include <assert.h>
#include <QtGui/qapplication.h>
#include <QtCore/qtimer.h>

#include "ace/OS_NS_time.h"
#include "ace/Time_Value.h"
#include "ace/QtReactor/QtReactor.h"
#include "ace/Event_Handler.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Dgram.h"

// Qt specific code
#include "QtReactor_Test.h"

/*
   QTestApplication class implementation
*/

QTestApplication::QTestApplication (int argc, char *argv[]):
  QApplication (argc, argv, FALSE) /* do not enable GUI */
{
  //FUZZ: disable check_for_lack_ACE_OS
  connect (&finishTimer_, SIGNAL (timeout ()), this, SLOT (finishTest ()));
  //FUZZ: enable check_for_lack_ACE_OS
}

void QTestApplication::finishTest ()
{
  ACE_OS::exit ();
}

void QTestApplication::exec (int msec)
{
  finishTimer_.stop ();
  if (0 < msec )
  {
    finishTimer_.setSingleShot (TRUE);
    finishTimer_.start (msec);
  }
  inherited::exec ();
}


// maximum time for testing QtReactor (msec)
const int TotalTestTime = 8000;

// how many handlers for each event handler class should be started ?
#ifndef __QNXNTO__
const int HandlersNo = 8 ;
#else /* __QNXNTO__ */
// it seems that Qt 3.1 for NTO 6.2 is compiled with rather small FD_SETSIZE
// Nevertheless, test works fine with native select reactor
const int HandlersNo = 4;
#endif
// base port for sending datagrams
const u_short BaseDgramPort = 5931;

// port for TCP connections
const u_short BaseTCPPort = 5931;

// how many datagrams should be sent for a single handler
const int DgramsToSend = 64;

// how many bytes to send vie TCP
const int TCPBytesToSend = 1024;

// how many times to reply tcp recv
const int TCPClientPings = 16;

// total number of bytes in TCP transmission
const int TCPTotalBytesToSend = TCPClientPings * TCPBytesToSend;

/**
  \class DgramHandler
  \brief Simple event handler that receives and counts datagrams
  as well as sends dgrams using timeouts.
*/
class DgramHandler: public ACE_Event_Handler
{
public:
  DgramHandler (ACE_Reactor *p_reactor = 0);
  virtual ~DgramHandler ();

  //FUZZ: disable check_for_lack_ACE_OS
  int  open (const ACE_INET_Addr &local,
             int protocol_family=ACE_PROTOCOL_FAMILY_INET,
             int protocol=0,
             int reuse_addr=0);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual ACE_HANDLE get_handle () const;
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE handle,ACE_Reactor_Mask close_mask);
  virtual int handle_timeout (const ACE_Time_Value &current_time, const void *act=0);
  int         dgramsSent () const;
  int         dgramsReceived () const;
  int         timeoutsTriggered () const;
  int         expectedTriggers () const;
  void        expectedTriggers (int);
private:
  int dgramsSent_;      //!< the number of sent datagrams
  int dgramsReceived_;  //!< the number of received datagrams
  int timeoutsTriggered_;
  int expectedTriggers_;
  ACE_SOCK_Dgram peer_; //!< datagram socket we listen to
};

/**
  \class TCPConnectionHandler
  \brief TCP stream handler for both sides of connection.
*/
class TCPConnectionHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;
public:
  TCPConnectionHandler (bool p_serverSide = false);
  virtual ~TCPConnectionHandler ();

  virtual int handle_output (ACE_HANDLE handle);
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE handle,ACE_Reactor_Mask close_mask);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  virtual int open (void * = 0);

  int scheduleSend (ACE_Message_Block *);
  int sendBuffers ();
  int totalReceived () const;
  int totalSent () const;
private:
  ACE_Message_Block *buffers_;
  int totalReceived_; //!< total number of received bytes
  int totalSent_; //!< total number of send bytes
  bool serverSide_; //!< if true, echo received data
  int pingsNo_; //!< number of pings client should make
};

typedef ACE_Connector< TCPConnectionHandler, ACE_SOCK_CONNECTOR > TCPConnectorHandler;
class TCPAcceptorHandler; // forward declaration

/* \class HandlersRegister
   \brief The collection of test tools. Here ACE classes bind to Qt application
*/
class HandlersRegister
{
public:
  HandlersRegister (ACE_Reactor *p_reactor);
  virtual ~HandlersRegister ();
  int scheduleTimers (const ACE_Time_Value &p_TestTime); //!< schedule ace timers
  int registerDgramHandlers (); //!< open dgrams socket and register in reactor
  int registerTCPHandlers ();  //!< creates TCP acceptors and connector
  int analyze () const; //!< analyze results
  int analyzeTimeouts () const; //!< analyze triggered timeouts
  int analyzeDgrams () const; //!< analyze collected dgrams
  int analyzeTCP () const; //!< analyze TCP transmission
  int registerTCPServer (TCPConnectionHandler *);
  int TCPServersNo () const; //!< return the number of accepted connections

private:
  ACE_Reactor          *reactor_; //!< reactor for this application
  DgramHandler         *DgramHandlers_[ HandlersNo] ;//!< dgram input handlers
  int                  TCPServersNo_; //!< number of accepted connections
  TCPConnectionHandler *TCPServers_[ HandlersNo ];
  TCPConnectionHandler *TCPClients_[ HandlersNo ];
  TCPAcceptorHandler   *acceptor_;
  TCPConnectorHandler  *connectors_[ HandlersNo ];
};

class TCPAcceptorHandler : public ACE_Acceptor< TCPConnectionHandler, ACE_SOCK_ACCEPTOR >
{
public:
  typedef ACE_Acceptor< TCPConnectionHandler, ACE_SOCK_ACCEPTOR > inherited;
public:
  TCPAcceptorHandler (HandlersRegister *p_handlersRegister);
  virtual ~TCPAcceptorHandler ();
  virtual int make_svc_handler (TCPConnectionHandler *& sh);
  virtual int activate_svc_handler (TCPConnectionHandler * sh);
private:
  HandlersRegister *handlersRegister_;
};

/*
   DgramHandler class implementation
*/

DgramHandler::DgramHandler (ACE_Reactor *p_reactor):
  ACE_Event_Handler (p_reactor),
  dgramsSent_ (0),
  dgramsReceived_ (0),
  timeoutsTriggered_ (0),
  expectedTriggers_ (0)
{
  reference_counting_policy ().value (Reference_Counting_Policy::ENABLED);
}

DgramHandler::~DgramHandler ()
{
  ACE_TRACE ("DgramHandler::~DgramHandler");
}

int  DgramHandler::open (const ACE_INET_Addr &local,
  int protocol_family,
  int protocol,
  int reuse_addr)
{
  if (0 > peer_.open (local,
                      protocol_family,
                      protocol,
                      reuse_addr))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P) %p\n"),
                       ACE_TEXT ("Cannot oper dgram socket")),
                      -1);

  return 0;
}

ACE_HANDLE DgramHandler::get_handle () const
{
  return peer_.get_handle ();
}

int DgramHandler::handle_input (ACE_HANDLE)
{
  int recvBuffer;
  ACE_INET_Addr peerAddress;

  int result = peer_.recv (&recvBuffer, sizeof (recvBuffer) , peerAddress);

  if (0 >= result)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P) %p\n"),
                       ACE_TEXT ("While reading datagram from socket"))
                      , -1);
  else
    ++dgramsReceived_;

  return 0;
}

int DgramHandler::handle_timeout (const ACE_Time_Value &current_time, const void *act)
{
  ACE_UNUSED_ARG (current_time);
  ACE_UNUSED_ARG (act);
  int sendBuffer = 0;

  if (++timeoutsTriggered_ >= expectedTriggers_)
    reactor ()->cancel_timer (this, 1);

  ACE_SOCK_Dgram socket;
  if (-1 == socket.open (ACE_INET_Addr (static_cast< u_short > (0),
                                        static_cast< ACE_UINT32 > (INADDR_ANY)),
                                        ACE_PROTOCOL_FAMILY_INET, 0, 1))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT (" (%P) %p\n"),
                ACE_TEXT ("Cannot open socket for sending Qt dgrams")));

  ACE_INET_Addr peerAddr;
  peer_.get_local_addr (peerAddr);

  if (sizeof (sendBuffer) != socket.send (&sendBuffer,
                                          sizeof (sendBuffer), peerAddr))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT (" (%P) %p\n"),
                ACE_TEXT ("Cannot send dgram")));
  else
    ++dgramsSent_;

  socket.close ();

  return 0;
}

int DgramHandler::handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
  if (peer_.get_handle () != handle )
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Unknown handle %d DgramHandler::handle_close "
                          "with mask %x. My handle is %d\n"),
                handle,
                close_mask,
                peer_.get_handle ()));
  else
    peer_.close ();

  return 0;
}

int DgramHandler::dgramsSent () const
{
  return dgramsSent_;
}

int DgramHandler::dgramsReceived () const
{
  return dgramsReceived_;
}

int DgramHandler::timeoutsTriggered () const
{
  return timeoutsTriggered_;
}

void DgramHandler::expectedTriggers (int triggers)
{
  expectedTriggers_ = triggers;
}

int DgramHandler::expectedTriggers () const
{
  return expectedTriggers_;
}

/*
   TCPConnectionHandler class implementation
*/

TCPConnectionHandler::TCPConnectionHandler (bool p_serverSide):
  buffers_ (0),
  totalReceived_ (0),
  totalSent_ (0),
  serverSide_ (p_serverSide),
  pingsNo_ (TCPClientPings)
{
  reference_counting_policy ().value (Reference_Counting_Policy::ENABLED);
}

TCPConnectionHandler::~TCPConnectionHandler ()
{
  ACE_TRACE ("TCPConnectionHandler::~TCPConnectionHandler");
}

int TCPConnectionHandler::handle_input (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);
  ACE_Message_Block *buffer = new ACE_Message_Block (TCPBytesToSend);
  int bytesReceived = peer_.recv (buffer->wr_ptr (), buffer->space ());

  if (bytesReceived > 0)
    {
      totalReceived_ += bytesReceived;
      if (serverSide_ || --pingsNo_ > 0) // echo received buffer
        {
          buffer->wr_ptr (bytesReceived);
          int result = scheduleSend (buffer);
          if (0 > result)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT (" (%P) %p\n"),
                               ACE_TEXT ("Cannot schedule TCP reply")),
                              -1);
        }
      else
        buffer->release ();

      return 0;
    }

  if (errno != EWOULDBLOCK)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P:%p (%d)\n"),
                       ACE_TEXT ("TCPConnectionHandler::handle_input call with no data on handle "), handle),
                      -1);

  ACE_ERROR ((LM_WARNING,
              ACE_TEXT (" (%P:%p (%d)\n"),
              ACE_TEXT ("TCPConnectionHandler::handle_input call with no data on handle "), handle));

  return 0;
}

int TCPConnectionHandler::handle_output (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);
  if (!buffers_)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TCPConnectionHandler::handle_output call for empty buffers (%d)\n"), handle));
  if (0 > sendBuffers ()) // socket broken, kill yourself
    return -1;

  if (!buffers_) // everything already send, unregister
    {
      reactor ()->cancel_wakeup (this,
                                 ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
      reactor ()->remove_handler (this,
                                  ACE_Event_Handler::WRITE_MASK  | ACE_Event_Handler::DONT_CALL);
    }

  return 0;
}

int TCPConnectionHandler::open (void * )
{
  int result = inherited::open ();

  if (!serverSide_)
    {
      ACE_Message_Block *buffer = new ACE_Message_Block (TCPBytesToSend);
      char *bufferData = buffer->wr_ptr ();
      int i;

      for (i = buffer->size () - 1; i >= 0; --i)
        bufferData[ i ] = static_cast< char > (i);
      buffer->wr_ptr (buffer->size ());
      if (0 != (scheduleSend (buffer)))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot schedule initial send\n")),
                          -1);
    }

  return result;
}

int TCPConnectionHandler::scheduleSend (ACE_Message_Block * buffer)
{
  // link buffer to the end of buffers list
  if (buffers_)
    {
      ACE_Message_Block *lastBuffer = buffers_;
      while (lastBuffer->cont ())
        lastBuffer = lastBuffer->cont () ;
      lastBuffer->cont (buffer);
    }
  else
    buffers_ = buffer;

  if (0 > sendBuffers ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P) %p\n"),
                       ACE_TEXT ("Cannot schedule TCP send.")),
                      -1);
  return 0;
}

int TCPConnectionHandler::sendBuffers ()
{
  int result = 0;

  if (buffers_)
    if (0 < (result = peer_.send_n (buffers_))) // remove sent blocks
      {
        totalSent_ += result;
        while (buffers_ &&
               static_cast< size_t > (result) >= buffers_->length ())
          {
            ACE_Message_Block *buffer = buffers_;
            result -= buffers_->length ();
            buffers_= buffers_->cont ();
            buffer->cont (0);
            buffer->release ();
          }

        if (buffers_) // some buffers were not sent, truncate data
          buffers_->rd_ptr (result);
      }

  return result;
}

int TCPConnectionHandler::handle_close (ACE_HANDLE handle,ACE_Reactor_Mask close_mask)
{
  if (peer_.get_handle () != handle )
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Unknown handle %d TCPConnectionHandler::handle_close "
                          "with mask %x. My handle is %d\n"),
                handle,
                close_mask,
                peer_.get_handle ()));
  else
    peer_.close ();

  return 0;
}

int TCPConnectionHandler::totalReceived () const
{
  return totalReceived_;
}

int TCPConnectionHandler::totalSent () const
{
  return totalSent_;
}


/*
   HandlersRegister class implementation
*/

HandlersRegister::HandlersRegister (ACE_Reactor *p_reactor):
  reactor_ (p_reactor),
  TCPServersNo_ (0)
{
  int i;

  for (i = 0; i < HandlersNo; ++i)
    {
      // create dgram input handler
      DgramHandlers_[ i ] = new DgramHandler (p_reactor);

      TCPServers_[ i ] = 0;

      TCPClients_[ i ] = new TCPConnectionHandler (false);

      connectors_[ i ] =new TCPConnectorHandler (p_reactor, ACE_NONBLOCK);
      connectors_[ i ]->reference_counting_policy ().value (
                                                            ACE_Event_Handler::Reference_Counting_Policy::ENABLED) ;
    }

  acceptor_ = new TCPAcceptorHandler (this);
  acceptor_->reactor (p_reactor);
}

HandlersRegister::~HandlersRegister ()
{
  int i;
  if (acceptor_)
    {
      reactor_->remove_handler (acceptor_, ACE_Event_Handler::ALL_EVENTS_MASK);
      acceptor_->close ();
      acceptor_->remove_reference ();
    }

  for (i = 0; i < HandlersNo; ++i)
    {
      reactor_->cancel_timer (DgramHandlers_[ i ], 1);
      reactor_->remove_handler (DgramHandlers_[ i ],
                                ACE_Event_Handler::ALL_EVENTS_MASK);
      DgramHandlers_[ i ]->remove_reference ();

      if (TCPServers_[ i ])
        {
          reactor_->remove_handler (TCPServers_[ i ],
                                    ACE_Event_Handler::ALL_EVENTS_MASK);
          TCPServers_[ i ]->remove_reference ();
        }

      reactor_->remove_handler (connectors_[ i ],
                                ACE_Event_Handler::ALL_EVENTS_MASK);
      connectors_[ i ]->close ();
      connectors_[ i ]->remove_reference ();

      if (TCPClients_[ i ])
        {
          reactor_->remove_handler (TCPClients_[ i ],
                                    ACE_Event_Handler::ALL_EVENTS_MASK);
          TCPClients_[ i ]->remove_reference ();
        }
    }
}

int HandlersRegister::TCPServersNo () const
{
  return TCPServersNo_;
}

int HandlersRegister::scheduleTimers (const ACE_Time_Value &p_TestTime)
{
  int i;

  for (i = 0; i < HandlersNo; ++i)
    {
      if (-1 == reactor_->schedule_timer (DgramHandlers_[ i ],
                                          (const void *) 0,
                                          ACE_Time_Value::zero,
                                          p_TestTime * (0.5 / DgramsToSend)))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P) %p\n"),
                           ACE_TEXT ("Cannot schedule ACE timer")),
                          -1);

      DgramHandlers_[ i ] ->expectedTriggers (DgramsToSend);

    }

  return 0;
}

int HandlersRegister::registerDgramHandlers ()
{
  int i;

  // open dgram handlers for all ports
  for (i = 0; i < HandlersNo; ++i)
    if (-1 == DgramHandlers_[ i ]->open (
                                         ACE_INET_Addr (i + BaseDgramPort,
                                                        ACE_TEXT ("127.0.0.1"),
                                                        ACE_PROTOCOL_FAMILY_INET)))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT (" (%P) %p\n"),
                         ACE_TEXT ("Cannot open dgram handler")),
                        -1);

  // register dgram handlers
  for (i = 0; i < HandlersNo; ++i)
    if (-1 == reactor_->register_handler (DgramHandlers_[ i ],
                                          ACE_Event_Handler::READ_MASK))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT (" (%P) %p\n"),
                         ACE_TEXT ("Cannot register dgram handler")),
                        -1);
  return 0;
}

int HandlersRegister::registerTCPHandlers ()
{
  ACE_INET_Addr addr (BaseTCPPort);

  if (-1 == acceptor_->open (addr, reactor_, 1))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P) %p\n"),
                       ACE_TEXT ("Cannot open acceptor port")),
                      -1);

  int i;
  addr.set (BaseTCPPort, ACE_TEXT ("127.0.0.1"));

  for (i = 0; i < HandlersNo; ++i)
    {
      if (-1 == connectors_[ i ]->connect (
                                           TCPClients_[ i ],
                                           addr,
                                           ACE_Synch_Options::asynch))
        if (errno != EWOULDBLOCK )
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT (" (%P) %p (%d)\n"),
                             ACE_TEXT ("Cannot connect connector"),
                             i),
                            -1);
    }

  return 0;
}

int HandlersRegister::registerTCPServer (TCPConnectionHandler *handler)
{
  if (TCPServersNo_ < HandlersNo)
    {
      TCPServers_[ TCPServersNo_++ ] = handler;
      return 0;
    }
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Too many servers registered (%d). ACE_Reactor or ACE_Acceptor broken?\n"),
              handler->get_handle ()));
  return -1;
}

int HandlersRegister::analyze () const
{
  int result = 0;

  if (0 > analyzeTimeouts ())
    result = -1;

  if (0 > analyzeDgrams ())
    result = -1;

  if (0 > analyzeTCP ())
    result = -1;

  return result;
}

int HandlersRegister::analyzeTimeouts () const
{
  int i;
  int result = 0;

  for (i = 0; i < HandlersNo; ++i)
    if (DgramHandlers_[ i ]->expectedTriggers () !=
        DgramHandlers_[ i ]->timeoutsTriggered ())
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Dgram_Handlers (%d) expected %d timeouts"
                              " but triggered only %d.\n"),
                    i,
                    DgramHandlers_[ i ]->expectedTriggers (),
                    DgramHandlers_[ i ]->timeoutsTriggered ()));
        result = -1;
      }

  return result;
}

int HandlersRegister::analyzeDgrams () const
{
  int result = 0;
  int i;

  for (i = 0; i < HandlersNo; ++i)
    if (DgramHandlers_[ i ]->dgramsSent () !=
        DgramHandlers_[ i ]->dgramsReceived ())
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("DgramsHandler (%d) sent  %d dgrams but received only %d."
                              "Either reactor failed or system lost local dgrams..\n"),
                    DgramHandlers_[ i ]->dgramsSent (),
                    DgramHandlers_[ i ]->dgramsReceived ()));
        result = -1 ;
      }

  return result;
}

int HandlersRegister::analyzeTCP () const
{
  int i;
  int result = 0;

  for (i = 0; i < HandlersNo; ++i)
    {
      if (TCPClients_[ i ]->totalSent () != TCPTotalBytesToSend )
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TCPClient (%d): wanted to send  %d  but sent only (%d).\n"),
                      i,
                      TCPTotalBytesToSend,
                      TCPClients_[ i ]->totalSent ()));
          result = -1;
        }

      if (TCPClients_[ i ]->totalReceived () != TCPTotalBytesToSend )
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TCPClient (%d): expected %d bytes but received only (%d).\n"),
                      i,
                      TCPTotalBytesToSend,
                      TCPClients_[ i ]->totalReceived ()));
          result = -1;
        }

      if (TCPServers_[ i ])
        {
          if (TCPServers_[ i ]->totalSent () != TCPTotalBytesToSend )
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TCPServer (%d): wanted to send %d bytes "
                                    "but sent only (%d).\n"),
                          i,
                          TCPTotalBytesToSend,
                          TCPServers_[ i ]->totalSent ()));
              result = -1;
            }

          if (TCPServers_[ i ]->totalReceived () != TCPTotalBytesToSend )
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TCPServer (%d): expected %d bytes but received only (%d).\n"),
                          i,
                          TCPTotalBytesToSend,
                          TCPServers_[ i ]->totalReceived ()));
              result = -1;
            }

        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TCPServer (%d): not connected.\n"),
                      i));
          result = -1;
        }
    }

  return result;
}


/*
   TCPAcceptorHandler class implementation
*/

TCPAcceptorHandler::TCPAcceptorHandler (HandlersRegister *p_handlersRegister):
  handlersRegister_ (p_handlersRegister)
{
  reference_counting_policy ().value (Reference_Counting_Policy::ENABLED);
}

int TCPAcceptorHandler::make_svc_handler (TCPConnectionHandler *& sh)
{
  sh = new TCPConnectionHandler (true);
  sh->reactor (reactor ());
  if (handlersRegister_->TCPServersNo () >= HandlersNo)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TCPAcceptorHandler::make_svc_handler called to many times!\n")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TCPAcceptorHandler::make_svc_handler new TCP server created\n")));

  return 0;
}

int TCPAcceptorHandler::activate_svc_handler (TCPConnectionHandler * sh)
{
  if (0 == inherited::activate_svc_handler (sh) )
    {
      if (0 != handlersRegister_->registerTCPServer (sh))  // for analysis
        {
          // release event handler
          reactor ()->remove_handler (sh, ACE_Event_Handler::ALL_EVENTS_MASK);
          sh->remove_reference ();
          // report error
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot register server TCPConnectionHandler\n")),
                            -1);
        }

    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to create server TCPConnectionHandler\n")),
                      -1);

  return 0;
}

TCPAcceptorHandler::~TCPAcceptorHandler ()
{
  ACE_TRACE ("TCPAcceptorHandler::~TCPAcceptorHandler");
}

void testNativeReactor (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Testing autotest using native reactor\n")));

  ACE_Reactor      reactor;
  HandlersRegister handlersRegister (&reactor);
  ACE_Time_Value   testTime (TotalTestTime / 1000, (TotalTestTime % 1000) * 1000);

  if (0 <= handlersRegister.scheduleTimers (testTime)  &&
      0 <= handlersRegister.registerDgramHandlers () &&
      0 <= handlersRegister.registerTCPHandlers ())
    {
      reactor.run_reactor_event_loop (testTime);

      if (0  != handlersRegister.analyze ())
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Test failed for native reactor. "
                              "Fix QtReactor_Test or ACE_Reactor.\n")));
      else
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Test seems to work with native reactor.\n")));
    }
}

void testQtReactor (int argc, ACE_TCHAR *argv[])
{
  // Qt specific code
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Testing QtReactor\n")));

  ACE_Argv_Type_Converter ct (argc, argv);
  QTestApplication app (argc, ct.get_ASCII_argv ());

  ACE_QtReactor    qtReactor (&app);
  ACE_Reactor      reactor (&qtReactor);
  HandlersRegister handlersRegister (&reactor);
  ACE_Time_Value   testTime (TotalTestTime / 1000, (TotalTestTime % 1000) * 1000);

  if (0 <= handlersRegister.scheduleTimers (testTime)  &&
      0 <= handlersRegister.registerDgramHandlers () &&
      0 <= handlersRegister.registerTCPHandlers ())
    {

      app.exec (TotalTestTime);

      if (0  != handlersRegister.analyze ())
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("QtReactor_Test failed.\n")));
      else
        ACE_ERROR ((LM_INFO, ACE_TEXT ("QtReactor_Test passed.\n")));
    }
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("QtReactor_Test"));

  testNativeReactor (argc, argv);
  testQtReactor (argc, argv);

  ACE_END_TEST;

  return 0;
}
