// $Id$

#include "ace/ATM_Acceptor.h"                             
#include "ace/ATM_Addr.h"

ACE_RCSID(ATM_SAP, CPP_ATM_server, "$Id$")

#if defined (ACE_HAS_ATM)
// ACE_ATM Server

int 
main (int argc, char *argv[])
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
  ACE_ATM_Acceptor peer_acceptor;

  // Not sure why but reuse_addr set to true/1 causes problems for
  // FORE/XTI/ATM - this is now handled in ACE_ATM_Acceptor::open()
  if (peer_acceptor.open (addr,
                          5) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_ATM_Stream new_stream;                                   

  ACE_DEBUG ((LM_DEBUG,
              "starting server at address %s\n", 
              addr.addr_to_string ()));


  // Performs the iterative server activities 

  for (;;) 
    {
      char buf[BUFSIZ];                                          

      // Create a new ACE_ATM_Stream endpoint (note automatic restart
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

      ACE_UINT16 vpi, vci;
      new_stream.get_vpi_vci(vpi, vci);
      ACE_DEBUG ((LM_DEBUG,
                  "connected to VPI %d VCI %d\n",
                  vpi, vci));

      ACE_DEBUG ((LM_DEBUG,
                  "client %s connected to host\n",
                  new_stream.get_peer_name ()));

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
int main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform isn't configured to support ATM\n"),
                    1);
}
#endif /* ACE_HAS_ATM */
