//=============================================================================
/**
  *  @file   server.cpp
  *
  *  $Id$
  *
  *  @author Craig Rodrigues <crodrigu@bbn.com>
  *  @brief  Start a server which listens for UDP packets on a specified port.
  */
//=============================================================================

#include "ace/Reactor.h"
#include "ace/Process.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"

// Port used to receive for dgrams.
static u_short port1;

class Dgram_Endpoint : public ACE_Event_Handler
{
public:
  Dgram_Endpoint (const ACE_INET_Addr &local_addr);

  // = Hook methods inherited from the <ACE_Event_Handler>.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_timeout (const ACE_Time_Value & tv,
                              const void *arg = 0);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  int send (const char *buf, size_t len, const ACE_INET_Addr &);
  // Send the <buf> to the peer.

private:
  ACE_SOCK_Dgram endpoint_;
  // Wrapper for sending/receiving dgrams.
};

int
Dgram_Endpoint::send (const char *buf,
                      size_t len,
                      const ACE_INET_Addr &addr)
{
  return this->endpoint_.send (buf, len, addr);
}

Dgram_Endpoint::Dgram_Endpoint (const ACE_INET_Addr &local_addr)
  : endpoint_ (local_addr)
{
}

ACE_HANDLE
Dgram_Endpoint::get_handle (void) const
{
  return this->endpoint_.get_handle ();
}

int
Dgram_Endpoint::handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask)
{
  ACE_UNUSED_ARG (handle);

  this->endpoint_.close ();
  delete this;
  return 0;
}

int
Dgram_Endpoint::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];
  ACE_INET_Addr from_addr;

  ssize_t n = this->endpoint_.recv (buf,
                                    sizeof buf,
                                    from_addr);

  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "handle_input"));
  else
    {
      buf[n] = 0;
      ACE_DEBUG ((LM_DEBUG,
                  "Received buf of size %d = %s\n",
                  n,
                  buf));
    }
  return 0;
}

int
Dgram_Endpoint::handle_timeout (const ACE_Time_Value &,
                                const void *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) timed out for endpoint\n"));
  return 0;
}

static int
run_test (u_short localport)
{
  ACE_INET_Addr local_addr (localport);

  Dgram_Endpoint *endpoint;
  
  ACE_NEW_RETURN (endpoint,
                  Dgram_Endpoint (local_addr),
                  -1);

  ACE_DEBUG((LM_DEBUG, "Starting server on port %d\n",port1));

  // Read data from other side.
  if (ACE_Reactor::instance ()->register_handler
      (endpoint,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Reactor::register_handler"),
                      -1);

  int ret; 
  while(1){
    ACE_Time_Value tv(10, 0);
    ret = ACE_Reactor::instance ()->handle_events (&tv);
    /* Error occurred while handling event */
    if (ret < 0)
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "(%P|%t) %p\n",
                           "handle_events"),
                          -1);
      }
    /* We have timed out without handling an event, break out of loop */
    if(ret == 0) break;
  }

  return 0;
}

int
main (int argc, char *argv[])
{
  // Estabish call backs and socket names.

  port1 = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;

  if(argc < 3)
    {
      run_test (port1);
    }
  else{
    ACE_DEBUG((LM_DEBUG, "\nUsage:\n  %s [port number]\n", argv[0]));
    return -1;
  }

  return 0;
}
