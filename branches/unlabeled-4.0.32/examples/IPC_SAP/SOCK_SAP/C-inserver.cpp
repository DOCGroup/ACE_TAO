#include "ace/OS.h"
// @(#)C-inserver.cpp	1.1	10/18/96


/* BSD socket server. */

int main (int argc, char *argv[])
{
  // Initialize WinSock DLL on Win32...
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  u_short port_num = 
    htons (argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT);
  struct sockaddr_in saddr;
  ACE_HANDLE s_handle, n_handle;                                             

  /* Create a local endpoint of communication */
  if ((s_handle = ACE_OS::socket (PF_INET, SOCK_STREAM, 0)) == ACE_INVALID_HANDLE)
    ACE_OS::perror ("socket"), ACE_OS::exit (1);

  /* Set up the address information to become a server */
  ACE_OS::memset ((void *) &saddr, 0, sizeof saddr);
  saddr.sin_family = AF_INET;
  saddr.sin_port = port_num;
  saddr.sin_addr.s_addr = INADDR_ANY;

  /* Associate address with endpoint */
  if (ACE_OS::bind (s_handle, (struct sockaddr *) &saddr, 
            sizeof saddr) == -1) 
    ACE_OS::perror ("bind"), ACE_OS::exit (1);

  /* Make endpoint listen for service requests */
  if (ACE_OS::listen (s_handle, 5) == -1) 
    ACE_OS::perror ("listen"), ACE_OS::exit (1);

  /* Performs the iterative server activities */

  for (;;) 
    {
      char buf[BUFSIZ];
      int r_bytes;
      struct sockaddr_in cli_addr;
      int cli_addr_len = sizeof cli_addr;
      struct hostent *hp;

      /* Create a new endpoint of communication */
      do 
	n_handle = ACE_OS::accept (s_handle, (struct sockaddr *) 
			     &cli_addr, &cli_addr_len);
      while (n_handle == ACE_INVALID_HANDLE && errno == EINTR);
	
      if (n_handle == ACE_INVALID_HANDLE) 
	{
	  ACE_OS::perror ("accept");
	  continue;
	}

      int addr_len = sizeof cli_addr.sin_addr.s_addr;
      hp = ACE_OS::gethostbyaddr ((char *) &cli_addr.sin_addr,
                          addr_len, AF_INET);

      if (hp != 0) 
        ACE_OS::printf ("client %s\n", hp->h_name), ACE_OS::fflush (stdout);
      else
        ACE_OS::perror ("gethostbyaddr");

      /* Read data from client (terminate on error) */
        
      while ((r_bytes = ACE_OS::recv (n_handle, buf, sizeof buf)) > 0)
        if (ACE_OS::write (ACE_STDOUT, buf, r_bytes) != r_bytes)
          ACE_OS::perror ("write"), ACE_OS::exit (1);

      if (ACE_OS::send (n_handle, "", 1) != 1)
	::perror ("write"), ACE_OS::exit (1);

      /* Close the new endpoint 
         (listening endpoint remains open) */
      if (ACE_OS::closesocket (n_handle) == -1)   
        ACE_OS::perror ("close"), ACE_OS::exit (1);
      ACE_OS::exit (0);
    }
  /* NOTREACHED */
  return 0;
}
