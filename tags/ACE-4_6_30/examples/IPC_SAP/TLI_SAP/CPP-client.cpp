// $Id$

#include "ace/TLI_Connector.h"
#include "ace/INET_Addr.h"                              

ACE_RCSID(TLI_SAP, CPP_client, "$Id$")

#if defined (ACE_HAS_TLI)

/* ACE_TLI Client */
                                                        
int main (int argc, char *argv[])                       
{                                                       
  const char *host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
  u_short r_port = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_SERVER_PORT;
  int timeout = argc > 3 ? ACE_OS::atoi (argv[3]) : ACE_DEFAULT_TIMEOUT;
  u_short l_port = argc > 4 ? ACE_OS::atoi (argv[4]) : ACE_DEFAULT_LOCAL_PORT;
  char buf[BUFSIZ];

  ACE_TLI_Stream cli_stream;
  ACE_INET_Addr remote_addr (r_port, host);
  ACE_INET_Addr local_addr (l_port);

  ACE_DEBUG ((LM_DEBUG,
              "starting non-blocking connect\n"));

  // Initiate timed, non-blocking connection with server.
  ACE_TLI_Connector con;
                                                        
  if (con.connect (cli_stream, remote_addr, 
		   (ACE_Time_Value *) &ACE_Time_Value::zero, 
		   local_addr, 1) == -1)
    {
      if (errno != EWOULDBLOCK)
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "connection failed"),
                          1);

      ACE_DEBUG ((LM_DEBUG,
                  "starting timed connect\n"));

      // Check if non-blocking connection is in progress, and wait up
      // to timeout seconds for it to complete.
      ACE_Time_Value tv (timeout);

      if (con.complete (cli_stream, &remote_addr, &tv) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "connection failed"), 1);
      else
	ACE_DEBUG ((LM_DEBUG,
                    "connected to %s\n", 
		    remote_addr.get_host_name ()));
    }

  // Send data to server (correctly handles "incomplete writes").
  
  for (int r_bytes;
       (r_bytes = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0; )
    if (cli_stream.send_n (buf, r_bytes) == -1) 
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "send_n"),
                        1);

  /* Explicitly close the connection */
  if (cli_stream.close () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      -1);

  return 0;
}                                                       
#else
int main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, 
		     "your platform isn't configured to support TLI\n"),
                    1);
}
#endif /* ACE_HAS_TLI */
