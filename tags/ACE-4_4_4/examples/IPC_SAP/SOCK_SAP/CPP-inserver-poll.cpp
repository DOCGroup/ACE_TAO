// $Id$

// IPC_SAP/poll server, which illustrates how to integrate the ACE
// socket wrappers with the SVR4 poll() system call to create a
// single-threaded concurrent server.

#include "ace/SOCK_Acceptor.h"                             
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"                                       

#if defined (ACE_HAS_SVR4_POLL)

// Maximum per-process open I/O descriptors.
const int MAX_FDS = 200;

int
main (int, char *[])
{
  // Create a server end-point.
  ACE_INET_Addr addr (ACE_DEFAULT_SERVER_PORT);
  ACE_SOCK_Acceptor peer_acceptor (addr);
  ACE_SOCK_Stream new_stream;
  ACE_HANDLE s_handle = peer_acceptor.get_handle ();
  struct pollfd poll_array[MAX_FDS];

  for (int i = 0; i < MAX_FDS; i++)
    {
      poll_array[i].fd = ACE_INVALID_HANDLE;
      poll_array[i].events = POLLIN;
    }

  poll_array[0].fd = s_handle;

  for (int n_handles = 1;;) 
    {
      // Wait for client I/O events (handle interrupts).
      while (ACE_OS::poll (poll_array, n_handles) == -1
             && errno == EINTR)
        continue;

      // Handle pending logging messages first (s_handle + 1 is
      // guaranteed to be lowest client descriptor).

      for (i = 1; i < n_handles; i++)
	{
	  if (poll_array[i].revents & POLLIN) 
	    {
	      char buf[BUFSIZ];
	      int  n;
	      // recv will not block in this case!
	      if ((n = ACE_OS::recv (poll_array[i].fd, buf, sizeof buf, 0)) == -1)
		ACE_OS::perror ("read failed");
	      else if (n == 0) 
		{
		  // Handle client connection shutdown.
		  if (ACE_OS::close (poll_array[i].fd) == -1)
		    ACE_OS::perror ("close");
		  poll_array[i].fd = poll_array[--n_handles].fd;

		  // Send handshake back to client to unblock it.
		  if (ACE_OS::send (poll_array[i].fd, "", 1) != 1)
		    ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));
		}
	      else 
		ACE_OS::printf ("%*s", n, buf), fflush (stdout);
	    } 
	  ACE_OS::fflush (stdout);
	}
      if (poll_array[0].revents & POLLIN)
	{
	  ACE_INET_Addr client;
	  ACE_Time_Value nonblock (0, 0);

	  // Handle all pending connection requests (note use of
	  // "polling" feature that doesn't block).

	  while (ACE_OS::poll (poll_array, 1, nonblock) > 0)
	    if (peer_acceptor.accept (new_stream, &client) == -1)
	      ACE_OS::perror ("accept");
	    else 
	      {
		const char *s = client.get_host_name ();

		ACE_ASSERT (s != 0);
		ACE_OS::printf ("client %s\n", s);
		ACE_OS::fflush (stdout);
		poll_array[n_handles++].fd = new_stream.get_handle ();
	      }
	}
    }
  /* NOTREACHED */
  return 0;
}
#else
#include <stdio.h>
int main (int, char *[])
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_SVR4_POLL */
