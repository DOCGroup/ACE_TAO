// $Id$

// Exercise the ACE_SOCK_CODgram wrapper along with the ACE_Reactor.
// This test simply ping-pongs datagrams back and forth between the
// parent and child process.

#include "ace/Reactor.h"
#include "ace/SOCK_CODgram.h"
#include "ace/INET_Addr.h"
#include "ace/Process.h"

ACE_RCSID(Dgram, CODgram, "$Id$")

// Port used to receive for dgrams.
static u_short port1;

class Dgram_Endpoint : public ACE_Event_Handler
{
public:
  Dgram_Endpoint (const ACE_INET_Addr &remote_addr,
          const ACE_INET_Addr &local_addr);

  // = Hook methods inherited from the <ACE_Event_Handler>.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_timeout (const ACE_Time_Value & tv,
                              const void *arg = 0);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  int send (const char *buf, size_t len);
  // Send the <buf> to the peer.

private:
  ACE_SOCK_CODgram endpoint_;
  // Wrapper for sending/receiving dgrams.
};

int
Dgram_Endpoint::send (const char *buf, size_t len)
{
  return this->endpoint_.send (buf, len);
}

int
Dgram_Endpoint::handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask)
{
  ACE_UNUSED_ARG (handle);

  this->endpoint_.close ();
  return 0;
}

Dgram_Endpoint::Dgram_Endpoint (const ACE_INET_Addr &remote_addr,
                                const ACE_INET_Addr &local_addr)
  : endpoint_ (remote_addr, local_addr)
{
}

ACE_HANDLE
Dgram_Endpoint::get_handle (void) const
{
  return this->endpoint_.get_handle ();
}

int
Dgram_Endpoint::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) activity occurred on handle %d!\n",
              this->endpoint_.get_handle ()));

  ssize_t n = this->endpoint_.recv (buf, sizeof buf);

  if (n == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "handle_input"));
  else
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) buf of size %d = %*s\n", n, n, buf));

  return 0;
}

int
Dgram_Endpoint::handle_timeout (const ACE_Time_Value &,
                                const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) timed out for endpoint\n"));
  return 0;
}

static int
run_test (u_short localport,
          const char *remotehost,
          u_short remoteport)
{
  ACE_INET_Addr remote_addr (remoteport,
                             remotehost);
  ACE_INET_Addr local_addr (localport);

  Dgram_Endpoint endpoint (remote_addr, local_addr);

  // Read data from other side.
  if (ACE_Reactor::instance ()->register_handler
      (&endpoint, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Reactor::register_handler"),
                      -1);

  char buf[BUFSIZ];
  ACE_OS::strcpy (buf, "Data to transmit");
  size_t len = ACE_OS::strlen (buf);

  if (localport == port1)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) sending data\n"));

      for (size_t i = 0; i < 20; i++)
        {
          endpoint.send (buf, len);
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) .\n"));
          ACE_OS::sleep (1);
        }
    }

  for (int i = 0; i < 40; i++)
    {
      // Wait up to 4 seconds for data.
      ACE_Time_Value tv (4, 0);

      if (ACE_Reactor::instance ()->handle_events (tv) <= 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "(%P|%t) %p\n",
                           "handle_events"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) return from handle events\n"));

      endpoint.send (buf, len);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) .\n"));
    }

  if (ACE_Reactor::instance ()->remove_handler
      (&endpoint, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Reactor::remove_handler"),
                      -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) exiting\n"));

  return 0;
}

int
main (int argc, char *argv[])
{
  // Estabish call backs and socket names.

  port1 = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  const char *remotehost = argc > 2 ? argv[2] : ACE_DEFAULT_SERVER_HOST;
  const u_short port2 = argc > 3 ? ACE_OS::atoi (argv[3]) : port1 + 1;

  if (argc > 4)                 // Providing the fourth command line argument
    {                           // indicate we don't want to spawn a new process.
                                // On Win32, we use this to exec the new program.
      run_test (port1, remotehost, port2);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) local port = %d, remote host = %s, remote port = %d\n",
                  port1, remotehost, port2));

      ACE_Process_Options options;
      options.command_line ("%s %d %s %d %c", argv[0], port1, remotehost, port2, 'c');
      options.creation_flags (ACE_Process_Options::NO_EXEC); // This has no effect on
                                                             // NT and will spawn a
                                                             // process that exec
                                                             // the above run_test
                                                             // function.

      ACE_Process new_process;
      switch (new_process.spawn (options))
        {
        case -1:
          return -1;

        case 0:
          run_test (port1, remotehost, port2);
          break;

        default:
          run_test (port2, remotehost, port1);
          new_process.wait ();
          break;
        }
    }
  return 0;
}
