// $Id$

#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_errno.h"
#include "ace/Default_Constants.h"



/* BSD socket server. */

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Initialize WinSock DLL on Win32...
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  u_short port_num =
    ACE_HTONS (argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT);
  int sockbufsize = argc > 2 ? ACE_OS::atoi (argv[2]) : 0;
  struct sockaddr_in saddr;
  ACE_HANDLE s_handle, n_handle;

  /* Create a local endpoint of communication */
  if ((s_handle = ACE_OS::socket (PF_INET, SOCK_STREAM, 0)) == ACE_INVALID_HANDLE)
    ACE_OS::perror (ACE_TEXT("socket")), ACE_OS::exit (1);

  // If a sockbufsize was specified, set it for both send and receive.
  if (sockbufsize > 0)
    {
      if (ACE_OS::setsockopt (s_handle, SOL_SOCKET, SO_SNDBUF,
                              (const char *) &sockbufsize,
                              sizeof (sockbufsize)) != 0)
        ACE_OS::perror (ACE_TEXT("SO_SNDBUF")), ACE_OS::exit (1);
      if (ACE_OS::setsockopt (s_handle, SOL_SOCKET, SO_RCVBUF,
                              (const char *) &sockbufsize,
                              sizeof (sockbufsize)) != 0)
        ACE_OS::perror (ACE_TEXT("SO_RCVBUF")), ACE_OS::exit (1);
    }

  /* Set up the address information to become a server */
  ACE_OS::memset ((void *) &saddr, 0, sizeof saddr);
  saddr.sin_family = AF_INET;
  saddr.sin_port = port_num;
  saddr.sin_addr.s_addr = INADDR_ANY;

  /* Associate address with endpoint */
  if (ACE_OS::bind (s_handle,
                    reinterpret_cast<struct sockaddr *> (&saddr),
                    sizeof saddr) == -1)
    ACE_OS::perror (ACE_TEXT("bind")), ACE_OS::exit (1);

  /* Make endpoint listen for service requests */
  if (ACE_OS::listen (s_handle, 5) == -1)
    ACE_OS::perror (ACE_TEXT("listen")), ACE_OS::exit (1);

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
        n_handle =
          ACE_OS::accept (s_handle,
                          reinterpret_cast<struct sockaddr *> (&cli_addr),
                          &cli_addr_len);
      while (n_handle == ACE_INVALID_HANDLE && errno == EINTR);

      if (n_handle == ACE_INVALID_HANDLE)
        {
          ACE_OS::perror (ACE_TEXT("accept"));
          continue;
        }

#if !defined(_UNICOS)
      int addr_len = sizeof cli_addr.sin_addr.s_addr;
#else /* ! _UNICOS */
      // sizeof on bitfield fails
      int addr_len = sizeof cli_addr.sin_addr;  // 32 bit biffield in UNICOS
#endif /* ! _UNICOS */
      hp = ACE_OS::gethostbyaddr ((char *) &cli_addr.sin_addr,
                                  addr_len, AF_INET);

      if (hp != 0)
        ACE_OS::printf ("client %s\n", hp->h_name), ACE_OS::fflush (stdout);
      else
        ACE_OS::perror (ACE_TEXT("gethostbyaddr"));

      /* Read data from client (terminate on error) */

      while ((r_bytes = ACE_OS::recv (n_handle, buf, sizeof buf)) > 0)
        if (ACE_OS::write (ACE_STDOUT, buf, r_bytes) != r_bytes)
          ACE_OS::perror (ACE_TEXT("write")), ACE_OS::exit (1);

      if (ACE_OS::send (n_handle, "", 1) != 1)
        ACE_OS::perror ("write"), ACE_OS::exit (1);

      /* Close the new endpoint
         (listening endpoint remains open) */
      if (ACE_OS::closesocket (n_handle) == -1)
        ACE_OS::perror (ACE_TEXT("close")), ACE_OS::exit (1);
      ACE_OS::exit (0);
    }

  ACE_NOTREACHED (return 0;)
}
