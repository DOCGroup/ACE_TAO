// $Id$

// Exercise the ACE_SOCK_Dgram wrapper along with the ACE_Reactor.
// This test simply ping-pongs datagrams back and forth between the
// parent and child process.

#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

static u_short port1;

class AAL_CP : public ACE_Event_Handler, public ACE_SOCK_Dgram
{
public:
  AAL_CP (const ACE_INET_Addr &local_addr);

  virtual ACE_HANDLE get_handle (void) const;

  virtual int handle_input (ACE_HANDLE handle);

  virtual int handle_timeout (const ACE_Time_Value & tv, 
			      const void *arg = 0);

  int timeouts_;
  // Number of timeouts thus far.
};

AAL_CP::AAL_CP (const ACE_INET_Addr &local_addr)
  : ACE_SOCK_Dgram (local_addr),
    timeouts_ (0)
{
}

ACE_HANDLE
AAL_CP::get_handle (void) const
{
  return ACE_SOCK_Dgram::get_handle ();
}

int
AAL_CP::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];
  ACE_INET_Addr from_addr;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) activity occurred on handle %d!\n",
	      ACE_SOCK_Dgram::get_handle ()));

  ssize_t n = ACE_SOCK_Dgram::recv (buf, sizeof buf, from_addr);

  if (n == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "handle_input"));
  else
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) buf of size %d = %*s\n", n, n, buf));
  
  return 0;
}

int
AAL_CP::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) timed out for aa1\n"));
  this->timeouts_++;
  return 0;
}

static int
run_test (u_short localport, 
          const char *remotehost,
          u_short remoteport)
{
  AAL_CP aal (localport);

  // Read data from other side.
  if (ACE_Reactor::instance ()->register_handler 
      (&aal, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Reactor::register_handler"),
                      -1);

  char buf[BUFSIZ];
  ACE_OS::strcpy (buf, "Data to transmit");
  size_t len = ACE_OS::strlen (buf);

  ACE_INET_Addr remote_addr (remoteport,
                             remotehost);
  ACE_INET_Addr local_addr (localport);

  if (localport == port1)	
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) sending data\n"));

      for (size_t i = 0; i < 20; i++)
        {
          aal.send (buf, len, remote_addr);
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

      aal.send (buf, len, remote_addr);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) .\n"));
    }

  if (ACE_Reactor::instance ()->remove_handler
      (&aal, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Reactor::remove_handler"),
                      -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) exiting\n"));
}

int
main (int argc, char *argv[])
{
  // Estabish call backs and socket names.

  port1 = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  const char *remotehost = argc > 2 ? argv[2] : ACE_DEFAULT_SERVER_HOST;
  const u_short port2 = argc > 3 ? ACE_OS::atoi (argv[3]) : port1 + 1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) local port = %d, remote host = %s, remote port = %d\n",
              port1, remotehost, port2));

  switch (ACE_OS::fork (argv[0]))
    {
    case -1: 
      return -1;

    case 0:
      run_test (port1, remotehost, port2);
      break;

    default:
      run_test (port2, remotehost, port1);
      break;
    }

  return 0;
}
