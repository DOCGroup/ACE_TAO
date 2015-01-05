// $Id$

#include "ace/OS_main.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"




#if defined (ACE_HAS_STREAM_PIPES)

#include "shared.h"

const int DEFAULT_SIZE = 8 * 1024;
const int DEFAULT_COUNT = 100;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int  size = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_SIZE;
  int  iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_COUNT;
  char *buf = new char[size];

  if (argc > 3)
    rendezvous = argv[3];

  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;
  int i;

  if (con.connect (cli_stream, ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", rendezvous), -1);

  for (i = 0; i < size; i++)
    buf[i] = 'a';

  for (i = 0; i < iterations; i++)
    if (cli_stream.send (buf, size) != size)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putmsg"), -1);

  if (cli_stream.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
#else
#include <stdio.h>
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "this feature is not supported"), -1);
}
#endif /* ACE_HAS_STREAM_PIPES */
