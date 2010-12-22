// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Network_Events.cpp
//
// = DESCRIPTION
//
//    This application tests Reactor to make sure that it responds
//    correctly to different kinds of network events.
//
//    The test starts off by creating a Network_Listener, that listens
//    for connections at ACE_DEFAULT_SERVER_PORT. When a client
//    connects, a Network_Handler is created. Network_Handler reads
//    messages off the socket and prints them out. This is done until
//    the remote side shuts down. Multiple clients can connect at the
//    same time.
//
//    Events tested in this example includes ACCEPT, READ, and CLOSE masks.
//
//    To run this example, start an instance of this example and
//    connect to it using telnet (to port
//    ACE_DEFAULT_SERVER_PORT(20002)).
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/OS_main.h"



class Network_Handler : public ACE_Event_Handler
{
public:
  Network_Handler (ACE_SOCK_Stream &s);
  // Default constructor

  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  virtual ACE_HANDLE  get_handle (void) const;

  ACE_SOCK_Stream stream_;

};

Network_Handler::Network_Handler (ACE_SOCK_Stream &s)
  : stream_ (s)
{
  this->reactor (ACE_Reactor::instance ());

  int result = this->reactor ()->register_handler (this, READ_MASK);
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);
}

ACE_HANDLE
Network_Handler::get_handle (void) const
{
  return this->stream_.get_handle ();
}

int
Network_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_DEBUG ((LM_DEBUG, "Network_Handler::handle_input handle = %d\n", handle));

  while (1)
    {
      char message[BUFSIZ];
      int result = this->stream_.recv (message, sizeof message);
      if (result > 0)
        {
          message[result] = 0;
          ACE_DEBUG ((LM_DEBUG, "Remote message: %s\n", message));
        }
      else if (result == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Connection closed\n"));
          return -1;
        }
      else if (errno == EWOULDBLOCK)
        {
          return 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Problems in receiving data, result = %d", result));
          return -1;
        }
    }
}

int
Network_Handler::handle_close (ACE_HANDLE handle,
                               ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Network_Handler::handle_close handle = %d\n", handle));

  this->stream_.close ();
  delete this;

  ACE_Reactor::end_event_loop ();

  return 0;
}

class Network_Listener : public ACE_Event_Handler
{
public:
  Network_Listener (void);
  // Default constructor
  ~Network_Listener (void);
  // Default constructor

  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  ACE_HANDLE get_handle (void) const;

  ACE_INET_Addr local_address_;
  ACE_SOCK_Acceptor acceptor_;
};

Network_Listener::Network_Listener (void)
  : local_address_ (ACE_DEFAULT_SERVER_PORT),
    acceptor_ (local_address_, 1)
{
  this->reactor (ACE_Reactor::instance ());
  int result = this->reactor ()->register_handler (this,
                                                   ACE_Event_Handler::ACCEPT_MASK);
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);
}

Network_Listener::~Network_Listener (void)
{
}

ACE_HANDLE
Network_Listener::get_handle (void) const
{
  return this->acceptor_.get_handle ();
}

int
Network_Listener::handle_input (ACE_HANDLE handle)
{
  ACE_DEBUG ((LM_DEBUG, "Network_Listener::handle_input handle = %d\n", handle));

  ACE_INET_Addr remote_address;
  ACE_SOCK_Stream stream;

  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle = this->reactor ()->uses_event_associations ();

  int result = this->acceptor_.accept (stream, // stream
                                       &remote_address, // remote address
                                       0, // timeout
                                       1, // restart
                                       reset_new_handle);  // reset new handler
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  ACE_DEBUG ((LM_DEBUG, "Remote connection from: "));
  remote_address.dump ();

  Network_Handler *handler;
  ACE_NEW_RETURN (handler, Network_Handler (stream), -1);

  return 0;
}

int
Network_Listener::handle_close (ACE_HANDLE handle,
                                ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Network_Listener::handle_close handle = %d\n", handle));

  this->acceptor_.close ();

  delete this;

  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Network_Listener *listener = 0;
  listener = new Network_Listener;

  ACE_Reactor::run_event_loop ();

  return 0;
};
