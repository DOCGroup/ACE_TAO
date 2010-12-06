// $Id$

#include "ace/TLI_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_TLI)
// ACE_TLI Server

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  ACE_Time_Value timeout (argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_TIMEOUT);

  // Create a server address.
  ACE_INET_Addr addr (port);

  // Create a server, reuse the addr.
  ACE_TLI_Acceptor peer_acceptor;

  // Not sure why but reuse_addr set to true/1 causes problems for
  // FORE/XTI/ATM - this is now handled in ACE_TLI_Acceptor::open()
  if (peer_acceptor.open (addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_TLI_Stream new_stream;

  ACE_DEBUG ((LM_DEBUG,
              "starting server at host %s\n",
              addr.get_host_name ()));

  // Performs the iterative server activities

  for (;;)
    {
      char buf[BUFSIZ];

      // Create a new ACE_TLI_Stream endpoint (note automatic restart
      // if errno == EINTR).
      if (peer_acceptor.accept (new_stream,
                                &addr,
                                &timeout) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "accept"));
        continue;
      }

      ACE_DEBUG ((LM_DEBUG,
                  "client %s connected\n",
                  addr.get_host_name ()));

      // Read data from client (terminate on error).

      for (int r_bytes; (r_bytes = new_stream.recv (buf, sizeof buf)) > 0; )
        if (ACE_OS::write (ACE_STDOUT,
                           buf,
                           r_bytes) != r_bytes)
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "ACE::send_n"));

       // Close new endpoint (listening endpoint stays open).
      if (new_stream.close () == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "close"));

    }
  /* NOTREACHED */
  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform isn't configured to support TLI\n"),
                    1);
}
#endif /* ACE_HAS_TLI */
