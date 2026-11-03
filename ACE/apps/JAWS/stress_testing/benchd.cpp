// benchd: Adapted from the "ntalker" example.
// Sumedh Mungee

#include "ace/Process.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/Signal.h"
#include "ace/OS_NS_stdio.h"
#include <iostream>

#if defined (ACE_HAS_IP_MULTICAST)
// network interface to subscribe to
//   this is hardware specific.
//   use netstat (1M) to find whether your interface
//   is le0 or ie0

// Name of the client benchmarking tool
static constexpr char const *TESTER = "http_tester";
static bool QUIET {};
static char const *INTERFACE = "le0";
static constexpr char const *MCAST_ADDR = ACE_DEFAULT_MULTICAST_ADDR;
static constexpr u_short UDP_PORT = ACE_DEFAULT_MULTICAST_PORT;
static constexpr char const *OUTPUT_FILE_NAME = "benchd.log";
static ACE_HANDLE OUTPUT_FILE = ACE_INVALID_HANDLE;

// Handle both multicast and stdin events.

class Handle_Events : public ACE_Event_Handler
{
public:
  Handle_Events (u_short udp_port,
                 char const *ip_addr,
                 char const *interface_,
                 ACE_Reactor &reactor);
  ~Handle_Events () override = default;
  Handle_Events (Handle_Events const &) = delete;
  Handle_Events (Handle_Events &&) = delete;
  Handle_Events &operator= (Handle_Events const &) = delete;
  Handle_Events &operator= (Handle_Events &&) = delete;

  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;

private:
  int serve (char *buf);
  ACE_SOCK_Dgram_Mcast mcast_;
  ACE_Handle_Set handle_set_;
};

int
Handle_Events::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ + 1] {};

  if (h == 0)
    {
      int const readresult = ACE_OS::read (h, buf, BUFSIZ);
      if (readresult > 0)
        {
          if (this->mcast_.send (buf, readresult) != readresult)
            {
              ACE_OS::perror ("send error");
              return -1;
            }
          return 0;
        }
      else if (readresult == -1)
        ACE_OS::perror ("can't read from STDIN");

      return -1;
    }
  else
    {
      ACE_INET_Addr remote_addr;

      // receive message from multicast group
      ssize_t const retcode = this->mcast_.recv (buf, sizeof buf - 1, remote_addr);

      if (retcode != -1)
        {
          buf[retcode] = '\0';
          /*
            cout << "received datagram from host " << remote_addr.get_host_name ()
            << " on port " << remote_addr.get_port_number ()
            << " bytes = " << retcode << endl;
            */
          serve (buf);
          return 0;
        }

      ACE_OS::perror ("Something amiss.");
      return -1;
    }
}

int
Handle_Events::handle_close (ACE_HANDLE h, ACE_Reactor_Mask)
{
  if (h == 0)
    ACE_DEBUG ((LM_DEBUG, "STDIN_Events handle removed from reactor.\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Mcast_Events handle removed from reactor.\n"));
  return 0;
}

Handle_Events::Handle_Events (u_short udp_port,
                              char const *ip_addr,
                              char const *interface_,
                              ACE_Reactor &reactor)
{
  // Create multicast address to listen on.

  ACE_INET_Addr sockmc_addr (udp_port, ip_addr);

  // subscribe to multicast group.

  if (this->mcast_.join (sockmc_addr, 1, ACE_TEXT_CHAR_TO_TCHAR (interface_)) == -1)
    ACE_OS::perror ("can't subscribe to multicast group"), ACE_OS::exit (1);

  // Disable loopbacks.
  //  if (this->mcast_.set_option (IP_MULTICAST_LOOP, 0) == -1 )
  //    ACE_OS::perror (" can't disable loopbacks " ), ACE_OS::exit (1);

  if (!QUIET) {
    this->handle_set_.set_bit (0);
  }
  this->handle_set_.set_bit (this->mcast_.get_handle ());

  // Register callbacks with the ACE_Reactor.
  if (reactor.register_handler (this->handle_set_,
                                this,
                                ACE_Event_Handler::READ_MASK) == -1)
    ACE_OS::perror ("can't register events"), ACE_OS::exit (1);
}


// This method handles multicast requests..
// These requests are of the following form:
// command (arguments)


// currently only one is supported (and indeed needed :-)) http_tester
// arguments

int
Handle_Events::serve (char *buf)
{
  ACE_ARGV arguments (buf);

  if (arguments[0] != nullptr && ACE_OS::strcmp (arguments[0], TESTER) == 0)
    {
      ACE_Process_Options po;
      ACE_Process p;

      po.set_handles (ACE_INVALID_HANDLE, OUTPUT_FILE, OUTPUT_FILE);
      po.command_line (arguments.argv ());

      p.spawn (po);
      return 0;
    }
  else
    return -1;
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:u:q"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'q':
        QUIET = true;
        break;
      case 'i':
        INTERFACE = get_opt.opt_arg ();
        break;
      case 'u':
        // Usage, same as error
        ACE_FALLTHROUGH;
      default:
        ACE_DEBUG ((LM_DEBUG, "%s -i interface\n", argv[0]));
        ACE_OS::exit (1);
      }
}

static sig_atomic_t done = 0;

// Signal handler.

extern "C" void
handler (int)
{
  done = 1;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Sig_Action sa (handler, SIGINT);
  ACE_OS::signal (SIGCLD, SIG_IGN);
  ACE_UNUSED_ARG (sa);

  parse_args (argc, argv);

  OUTPUT_FILE = ACE_OS::open (OUTPUT_FILE_NAME, O_CREAT | O_WRONLY, 0644);
  if (OUTPUT_FILE == ACE_INVALID_HANDLE)
    return 1;

  ACE_Reactor reactor;
  Handle_Events handle_events (UDP_PORT, MCAST_ADDR, INTERFACE, reactor);

  // main loop

  while (!done)
    reactor.handle_events ();

  ACE_OS::close (OUTPUT_FILE);
  std::cout << "\nbenchd done.\n";
  return 0;
}
#else
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_ERROR ((LM_ERROR, "error: %s must be run on a platform that support IP multicast\n",
              argv[0]));
  return 0;
}
#endif /* ACE_HAS_IP_MULTICAST */
