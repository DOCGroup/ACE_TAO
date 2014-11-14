// $Id$

#include "ace/OS_main.h"
#include "ace/OS_Memory.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Time_Value.h"



#if defined (ACE_HAS_STREAM_PIPES)

#include "shared.h"

const int DEFAULT_SIZE = 4 * 1024;
const int DEFAULT_COUNT = 100;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int size = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_SIZE;
  int iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_COUNT;
  char *buf;

  ACE_NEW_RETURN (buf, char[size], -1);

  if (argc > 3)
    rendezvous = argv[3];

  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;
  int i;

  if (con.connect (cli_stream, ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", rendezvous), 1);

  for (i = 0; i < size; i++)
    buf[i] = 'a';

  ACE_Str_Buf buffer (buf, size);

  for (i = 0; i < iterations; i++)
    if (cli_stream.send ((ACE_Str_Buf *) 0,
                         &buffer,
                         1,
                         MSG_BAND) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 1);

  if (cli_stream.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  delete buf;
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
