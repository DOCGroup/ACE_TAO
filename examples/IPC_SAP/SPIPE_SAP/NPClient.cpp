#include "ace/Log_Msg.h"
// $Id$

#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"

#if defined (ACE_WIN32)
#define MAKE_PIPE_NAME(X) \\\\.\\pipe\\#X
#else
#define MAKE_PIPE_NAME(X) X
#endif

const int DEFAULT_SIZE = 8;
const int DEFAULT_COUNT = 10000;

int 
main (int argc, char *argv[])
{
  int  size = argc > 1 ? atoi (argv[1]) : DEFAULT_SIZE;
  int  iterations = argc > 2 ? atoi (argv[2]) : DEFAULT_COUNT;
  char *buf = new char[size];

  //char *pipe_name = ACE_DEFAULT_RENDEZVOUS;
  char *pipe_name = "acepipe";
  char *rendezvous;
  rendezvous = MAKE_PIPE_NAME (pipe_name);

  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;
  int i;

  if (con.connect (cli_stream, ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", rendezvous), -1);

  ACE_OS::strcpy (buf, "hello");
  size = ACE_OS::strlen (buf) + 1;

  for (i = 0; i < iterations; i++)
    if (cli_stream.send (buf, size) != size)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putmsg"), -1);

  if (cli_stream.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
