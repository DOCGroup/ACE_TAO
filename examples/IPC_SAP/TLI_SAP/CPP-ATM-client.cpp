// $Id$

#include "ace/TLI_Connector.h"
#include "ace/ATM_Addr.h"                              

ACE_RCSID(TLI_SAP, CPP_ATM_client, "$Id$")

#if defined (ACE_HAS_FORE_ATM_XTI)

/* ACE_XTI/ATM Client */

int main (int argc, char *argv[])                       
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "Usage: %s hostname [QoS in KB/sec] [timeout]\n",
                       argv[0]),
                      1);
  const char *host = argv[1];
  int qos = argc > 2 ? ACE_OS::atoi (argv[2]) : 0;
  int timeout = argc > 3 ? ACE_OS::atoi (argv[3]) : ACE_DEFAULT_TIMEOUT;

  char buf[BUFSIZ];

  ACE_TLI_Stream cli_stream;

  ACE_ATM_Addr remote_addr (host);
  char hostname[MAXNAMELEN];
  ACE_OS::hostname(hostname, MAXNAMELEN);
  ACE_ATM_Addr local_addr (hostname);

  // In order to construct connections options the file handle is
  // needed.  Therefore, we need to open the TLI_Stream before we
  // construct the options.
  if (cli_stream.open (ACE_TLI_TCP_DEVICE, O_RDWR, 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open failed"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "starting non-blocking connect\n"));

  // Initiate timed, non-blocking connection with server.
  ACE_TLI_Connector con;
                                                        
  // Construct QoS options - currently FORE only supports bandwidth
  long optlen = 0;
  char *options = ACE_ATM_Addr::construct_options (cli_stream.get_handle (),
                                                   qos,
                                                   ACE_ATM_Addr::OPT_FLAGS_CPID,
                                                   &optlen);
  struct netbuf optbuf;
  optbuf.len = optlen;
  optbuf.buf = options;

  // Not sure why but reuse_addr set to true/1 causes problems for
  // FORE/XTI/ATM - this is now handled in
  // ACE_TLI_Connector::connect()
  if (con.connect (cli_stream,
                   remote_addr, 
                   (ACE_Time_Value *) &ACE_Time_Value::zero, 
                   local_addr,
                   1,
                   O_RDWR,
                   0,
                   ACE_TLI_TCP_DEVICE,
                   0,
                   1,
                   0,
                   &optbuf) == -1)
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

      if (con.complete (cli_stream,
                        &remote_addr,
                        &tv) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "connection failed"),
                          1);
      else
	ACE_DEBUG ((LM_DEBUG,
                    "connected to %s\n", 
		    remote_addr.addr_to_string ()));
    }

  // Send data to server (correctly handles "incomplete writes").
  
  for (int r_bytes;
       (r_bytes = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0; 
       )
    if (cli_stream.send_n (buf,
                           r_bytes,
                           0) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "send_n"),
                        1);

  // Explicitly close the connection.
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
		     "your platform isn't configured to support XTI/ATM\n"),
                    1);
}
#endif /* ACE_HAS_TLI */
