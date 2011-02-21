// $Id$

#include "ace/TLI_Connector.h"
#include "ace/ATM_QoS.h"
#include "ace/ATM_Addr.h"
#include "ace/Log_Msg.h"



#if defined (ACE_HAS_FORE_ATM_XTI)

/* ACE_XTI/ATM Client */

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Usage: %s [-s selector] hostname [QoS in KB/sec]\n",
                       argv[0]),
                      1);

  unsigned char selector = ACE_ATM_Addr::DEFAULT_SELECTOR;
  int selector_specified = 0;
  extern int optind;
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
                           "Usage: %s hostname [-s selector] [QoS in KB/s]\n",
                           argv[0]),
                          1);
      } // switch
    } // while getopt

  const char *host = argv[optind];

  int rate = (argc == 3) ? ACE_OS::atoi (argv[2]) :
    (argc == 5) ? ACE_OS::atoi (argv[4]) : 0;
  // The timeout really gets ignored since FORE's drivers don't work when
  //  ioctl or fcntl calls are made on the transport id/file descriptor
  int timeout = ACE_DEFAULT_TIMEOUT;

  char buf[BUFSIZ];

  ACE_TLI_Stream cli_stream;

  ACE_ATM_Addr remote_addr (host);
  if (selector_specified)
    remote_addr.set_selector(selector);
  char hostname[MAXNAMELEN];
  ACE_OS::hostname(hostname, MAXNAMELEN);
  ACE_ATM_Addr local_addr (hostname);

  // In order to construct connections options the file handle is
  // needed.  Therefore, we need to open the TLI_Stream before we
  // construct the options.
  if (cli_stream.open (ACE_XTI_ATM_DEVICE, O_RDWR, 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open failed"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "starting non-blocking connect\n"));

  // Initiate timed, non-blocking connection with server.
  ACE_TLI_Connector con;

  // Construct QoS options - currently FORE only supports bandwidth
  ACE_ATM_QoS qos;
  qos.set_rate(cli_stream.get_handle (),
               rate,
               ACE_ATM_QoS::OPT_FLAGS_CPID);

  struct netbuf optbuf = qos.get_qos();
//   long optlen = 0;
//   char *options = remote_addr.construct_options (cli_stream.get_handle (),
//                                                  rate,
//                                                  ACE_ATM_Addr::OPT_FLAGS_CPID,
//                                                  &optlen);
//   struct netbuf optbuf;
//   optbuf.len = optlen;
//   optbuf.buf = options;

  // Not sure why but reuse_addr set to true/1 causes problems for
  // FORE/XTI/ATM - this is now handled in ACE_TLI_Connector::connect()
  if (con.connect (cli_stream,
                   remote_addr,
                   (ACE_Time_Value *) &ACE_Time_Value::zero,
                   local_addr,
                   1,
                   O_RDWR,
                   0,
                   ACE_XTI_ATM_DEVICE,
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
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform isn't configured to support XTI/ATM\n"),
                    1);
}
#endif /* ACE_HAS_TLI */
