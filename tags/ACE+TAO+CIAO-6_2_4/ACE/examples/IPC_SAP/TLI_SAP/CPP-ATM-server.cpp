// $Id$

#include "ace/TLI_Acceptor.h"
#include "ace/ATM_Addr.h"
#include "ace/Log_Msg.h"



#if defined (ACE_HAS_FORE_ATM_XTI)
// ACE_TLI Server

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

  unsigned char selector = ACE_ATM_Addr::DEFAULT_SELECTOR;
  int selector_specified = 0;
  int opt;
  while ((opt = ACE_OS::getopt (argc, argv, "s:?h")) != EOF)
    {
    switch(opt)
      {
      case 's':
        selector = ACE_OS::atoi (optarg);
        selector_specified = 1;
        break;
      case '?':
      case 'h':
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s [-s selector]\n", argv[0]),
                          1);
      } // switch
    } // while getopt

  // Create a server address.
  ACE_ATM_Addr addr;
  if (selector_specified)
    addr.set_selector(selector);

  // Create a server, reuse the addr.
  ACE_TLI_Acceptor peer_acceptor;

  // Not sure why but reuse_addr set to true/1 causes problems for
  // FORE/XTI/ATM - this is now handled in ACE_TLI_Acceptor::open()
  if (peer_acceptor.open (addr,
                          1,
                          O_RDWR,
                          0,
                          5,
                          ACE_XTI_ATM_DEVICE) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_TLI_Stream new_stream;

  ACE_DEBUG ((LM_DEBUG,
              "starting server at address %s\n",
              addr.addr_to_string ()));


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
                  addr.addr_to_string ()));

      // Read data from client (terminate on error).

      for (int r_bytes;
           (r_bytes = new_stream.recv (buf, sizeof buf, 0)) > 0; )
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
                     "your platform isn't configured to support XTI/ATM\n"),
                    1);
}
#endif /* ACE_HAS_TLI */
