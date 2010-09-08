// $Id$

// Listens to multicast address.  After first message received, will
// listen for 5 more seconds.  Prints Mbits/sec received from client.

#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "ace/Service_Config.h"



#if defined (ACE_HAS_IP_MULTICAST)
// Network interface to subscribe to.  This is hardware specific.  use
// netstat(1M) to find whether your interface is le0 or ie0

static const ACE_TCHAR *INTERFACE = 0;
static const char *MCAST_ADDR = ACE_DEFAULT_MULTICAST_ADDR;
static const u_short UDP_PORT = ACE_DEFAULT_MULTICAST_PORT;

class Handler : public ACE_Event_Handler
{
  // = TITLE
  //     Handle both multicast and stdin events.
public:
  // = Initialization and termination methods.
  Handler (u_short udp_port,
           const char *ip_addr,
           const ACE_TCHAR *a_interface,
           ACE_Reactor & );
  // Constructor.

  ~Handler (void);
  // Destructor.

  // Event demuxer hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask);
  virtual ACE_HANDLE get_handle (void) const;

private:
  ACE_SOCK_Dgram_Mcast mcast_;
  // Multicast wrapper.

  ACE_INET_Addr sockmc_addr_;
  // Address to multicast to.
};

ACE_HANDLE
Handler::get_handle (void) const
{
  return this->mcast_.get_handle ();
}

int
Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];

  if (h == ACE_STDIN)
    {
      ssize_t result = ACE_OS::read (h, buf, BUFSIZ);

      if (result > 0)
        {
          if (this->mcast_.send (buf, result) != result)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "send error"),
                              -1);
          return 0;
        }
      else if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "can't read from STDIN"),
                              -1);
      else // result == 0
        {
          ACE_Reactor::end_event_loop ();
          return -1;
        }
    }
  else
    {
      ACE_INET_Addr remote_addr;

      // Receive message from multicast group.
      ssize_t result = this->mcast_.recv (buf,
                                          sizeof buf,
                                          remote_addr);

      if (result != -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "received datagram from host %s on port %d bytes = %d\n",
                      remote_addr.get_host_name (),
                      remote_addr.get_port_number (),
                      result));
          ACE_OS::write (ACE_STDERR, buf, result);
          ACE_DEBUG ((LM_DEBUG,
                      "\n"));
          return 0;
        }

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "something amiss"),
                              -1);
    }
}

int
Handler::handle_close (ACE_HANDLE h, ACE_Reactor_Mask)
{
  if (h == ACE_STDIN)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "STDIN_Events handle removed from reactor.\n"));
      if (ACE_Reactor::instance ()->remove_handler
          (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "remove_handler"),
                           -1);
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                "Mcast_Events handle removed from reactor.\n"));
  return 0;
}

Handler::~Handler (void)
{
  if (this->mcast_.leave (sockmc_addr_) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "leave fails"));
}

Handler::Handler (u_short udp_port,
                  const char *ip_addr,
                  const ACE_TCHAR *a_interface,
                  ACE_Reactor &reactor)
{
  // Create multicast address to listen on.

  this->sockmc_addr_ = ACE_INET_Addr (udp_port, ip_addr);

  // subscribe to multicast group.

  if (this->mcast_.join (sockmc_addr_, 1, a_interface) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "can't subscribe to multicast group"));

  // Disable loopbacks.
  // if (this->mcast_.set_option (IP_MULTICAST_LOOP, 0) == -1 )
  //   ACE_OS::perror (" can't disable loopbacks " ), ACE_OS::exit (1);

  // Register callbacks with the ACE_Reactor.
  else if (reactor.register_handler (this->mcast_.get_handle (),
                                     this,
                                     ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "can't register with Reactor\n"));
  // Register the STDIN handler.
  else if (ACE_Event_Handler::register_stdin_handler (this,
                                                      ACE_Reactor::instance (),
                                                      ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_stdin_handler"));
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:u"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':
        INTERFACE = get_opt.opt_arg ();
        break;
      case 'u':
        // Usage fallthrough.
      default:
        ACE_DEBUG ((LM_DEBUG,
                    "%s -i interface\n",
                    argv[0]));
        ACE_OS::exit (1);
      }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  Handler handler (UDP_PORT,
                   MCAST_ADDR,
                   INTERFACE,
                   *ACE_Reactor::instance ());

  // Run the event loop.
  ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "talker Done.\n"));
  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "error: %s must be run on a platform that support IP multicast\n",
                     argv[0]),
                    0);
}
#endif /* ACE_HAS_IP_MULTICAST */

