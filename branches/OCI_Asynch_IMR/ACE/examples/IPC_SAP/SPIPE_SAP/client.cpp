// $Id$

#include "ace/OS_main.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"



#if defined (ACE_HAS_STREAM_PIPES)

#include "shared.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR, "usage: %s string [rendezvous]\n", argv[0]), 1);

  if (argc > 2)
    rendezvous = argv[2];

  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;

  if (con.connect (cli_stream, ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", rendezvous), 1);

  ssize_t len = ACE_OS::strlen (argv[1]) + 1;

  if (cli_stream.send (argv[1], len) != len)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);

  if (cli_stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  return 0;
}
#else
#include <stdio.h>
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_STREAM_PIPES */
