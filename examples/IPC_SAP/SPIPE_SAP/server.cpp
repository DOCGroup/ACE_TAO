#include "ace/SPIPE_Addr.h"
// $Id$

#include "ace/SPIPE_Acceptor.h"
#include "shared.h"

#if defined (ACE_HAS_STREAM_PIPES)

/* Maximum per-process open I/O descriptors */
const int MAX_FDS = 200;
const int PERMS	  = 0666;

int
main (int argc, char *argv[])
{
  ACE_SPIPE_Acceptor peer_acceptor;
  ACE_SPIPE_Stream new_stream;
  int		 s_handle;
  struct pollfd	 poll_array[MAX_FDS];

  if (argc > 1)
    rendezvous = argv[1];

  ACE_OS::fdetach (rendezvous);

  ACE_SPIPE_Addr addr (rendezvous);

  if ((s_handle = peer_acceptor.open (addr)) == -1)
    ACE_OS::perror ("peer_acceptor.open"), ACE_OS::exit (1);

  for (int fd = 0; fd < MAX_FDS; fd++) 
    {
      poll_array[fd].fd = -1;
      poll_array[fd].events = POLLIN;
    }

  poll_array[0].fd = s_handle;

  for (int width = 1;;)
    {
      // Block waiting for client I/O events (handle interrupts).
      while (ACE_OS::poll (poll_array, width) == -1 && errno == EINTR)
        continue;

      /* Handle pending logging messages first (s_handle + 1 
         is guaranteed to be lowest client descriptor) */

      for (int fd = s_handle + 1; fd < width; fd++)
	if ((poll_array[fd].revents & POLLIN) 
	    || (poll_array[fd].revents & POLLHUP))
	  {
	    char buf[BUFSIZ];
	    int	 n;

	    /* recv will not block in this case! */
	    if ((n = ACE_OS::read (fd, buf, sizeof buf)) == -1)
	      ACE_OS::perror ("read failed");
	    else if (n == 0)
	      {
		/* Handle client connection shutdown */
		if (ACE_OS::close (poll_array[fd].fd) == -1)
		  ACE_OS::perror ("close");
		poll_array[fd].fd = -1;

		if (fd + 1 == width)
		  {
		    while (poll_array[fd].fd == -1)
		      fd--;
		    width = fd + 1;
		  }
	      }
	    else
	      {
		::printf ("%*s\n", n, buf);
		fflush (stdout);
	      }
	  } 

      if (poll_array[0].revents & POLLIN)
	{
	  int arg;
	  int n_handle;
	  ACE_SPIPE_Addr client;
 
	  if (peer_acceptor.accept (new_stream) == -1)
	    ACE_OS::perror ("local_accept");
  
	  n_handle = new_stream.get_handle ();

	  if (new_stream.get_remote_addr (client) == -1)
	    ACE_OS::perror ("get_remote_addr");

	  ACE_OS::printf ("n_handle = %d, uid = %d, gid = %d\n", 
		    n_handle, client.user_id (), client.group_id ());

	  arg = RMSGN | RPROTDAT;

	  if (ACE_OS::ioctl (n_handle, I_SRDOPT, (void *) arg) == -1)
	    ACE_OS::perror ("I_RRDOPT");

	  poll_array[n_handle].fd = n_handle;
	  if (n_handle >= width)
	    width = n_handle + 1;
	}
    }

  return 0;
}
#else
#include <stdio.h>
int main (void)
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_STREAM_PIPES */
