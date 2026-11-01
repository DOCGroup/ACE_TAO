//=============================================================================
/**
  *  @file   server.cpp
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

#include <memory>

class Dgram_Endpoint : public ACE_Event_Handler
{
public:
  explicit Dgram_Endpoint (ACE_INET_Addr const &local_addr);
  Dgram_Endpoint (Dgram_Endpoint const &) = delete;
  Dgram_Endpoint (Dgram_Endpoint &&) = delete;
  Dgram_Endpoint &operator= (Dgram_Endpoint const &) = delete;
  Dgram_Endpoint &operator= (Dgram_Endpoint &&) = delete;

  // = Hook methods inherited from the <ACE_Event_Handler>.
  ACE_HANDLE get_handle () const override;
  int handle_input (ACE_HANDLE handle) override;
  int handle_timeout (ACE_Time_Value const &tv,
                      void const *arg = nullptr) override;
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask) override;

  //FUZZ: disable check_for_lack_ACE_OS
  int send (char const *buf, size_t len, ACE_INET_Addr const &);
  // Send the <buf> to the peer.
  //FUZZ: enable check_for_lack_ACE_OS

private:
  ACE_SOCK_Dgram endpoint_;
  // Wrapper for sending/receiving dgrams.
};

int
Dgram_Endpoint::send (char const *buf,
                      size_t len,
                      ACE_INET_Addr const &addr)
{
  return this->endpoint_.send (buf, len, addr);
}

Dgram_Endpoint::Dgram_Endpoint (ACE_INET_Addr const &local_addr)
  : endpoint_ (local_addr)
{
}

ACE_HANDLE
Dgram_Endpoint::get_handle () const
{
  return this->endpoint_.get_handle ();
}

int
Dgram_Endpoint::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  this->endpoint_.close ();
  delete this;
  return 0;
}

int
Dgram_Endpoint::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ + 1];
  ACE_INET_Addr from_addr;

  ssize_t n = this->endpoint_.recv (buf, sizeof buf - 1, from_addr);

  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "handle_input"));
  else
    {
      buf[n] = '\0';
      ACE_DEBUG ((LM_DEBUG,
                  "Received buf of size %d = %s\n",
                  n,
                  buf));
    }
  return 0;
}

int
Dgram_Endpoint::handle_timeout (ACE_Time_Value const &,
                                void const *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) timed out for endpoint\n"));
  return 0;
}

static int
run_test (u_short localport)
{
  ACE_INET_Addr local_addr (localport);

  Dgram_Endpoint *raw_endpoint;

  ACE_NEW_RETURN (raw_endpoint,
                  Dgram_Endpoint (local_addr),
                  -1);
  std::unique_ptr<Dgram_Endpoint> endpoint (raw_endpoint);

  ACE_DEBUG ((LM_DEBUG, "Starting server on port %d\n", localport));

  // Read data from other side.
  if (ACE_Reactor::instance ()->register_handler
      (endpoint.get (),
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Reactor::register_handler"),
                      -1);
  endpoint.release ();

  int ret;
  while (true)
    {
      ACE_Time_Value tv (10, 0);
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
      if (ret == 0)
        break;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Establish call backs and socket names.
  u_short const port = argc > 1 ? ACE_OS::atoi (argv[1])
                                : ACE_DEFAULT_SERVER_PORT;

  if (argc < 3)
    {
      run_test (port);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "\nUsage:\n  %s [port number]\n", argv[0]));
      return -1;
    }

  return 0;
}
