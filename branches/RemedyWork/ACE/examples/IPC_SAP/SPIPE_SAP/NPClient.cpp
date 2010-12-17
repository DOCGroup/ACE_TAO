// $Id$

#include "ace/OS_main.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_Memory.h"



#if defined (ACE_WIN32)
#define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\") ACE_TEXT (X)
#else
#define MAKE_PIPE_NAME(X) ACE_TEXT (X)
#endif

const int DEFAULT_SIZE = 8;
const int DEFAULT_COUNT = 10000;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int  size = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_SIZE;
  int  iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_COUNT;
  char *buf;

  ACE_NEW_RETURN (buf,
                  char[size],
                  1);

  const ACE_TCHAR *rendezvous = MAKE_PIPE_NAME ("acepipe");

  ACE_SPIPE_Stream cli_stream;
  ACE_SPIPE_Connector con;
  int i;

  if (con.connect (cli_stream,
                   ACE_SPIPE_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       rendezvous),
                      -1);

  ACE_OS::strcpy (buf, "hello");
  size = ACE_Utils::truncate_cast<int> (ACE_OS::strlen (buf) + 1);

  for (i = 0; i < iterations; i++)
    if (cli_stream.send (buf, size) != size)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("putmsg")),
                        -1);

  if (cli_stream.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("close")),
                        -1);
  return 0;
}
