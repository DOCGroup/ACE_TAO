// $Id$

#include "JAWS/Server.h"
#include "HTTP_10.h"

#ifdef ACE_HAS_SIG_C_FUNC
extern "C"
{
#endif /* ACE_HAS_SIG_C_FUNC */

static void
handler (int)
{
  ACE_OS::exit (0);
  // call exit() so that static destructors get called
}

#ifdef ACE_HAS_SIG_C_FUNC
}
#endif /* ACE_HAS_SIG_C_FUNC */

int
main (int argc, char *argv[])
{
  ACE_OS::signal (SIGCHLD, SIG_IGN);

  // SigAction not needed since the handler will shutdown the server.
  ACE_OS::signal (SIGINT, (ACE_SignalHandler) handler);
  ACE_OS::signal (SIGUSR2, (ACE_SignalHandler) handler);

  JAWS_Server server (argc, argv);
  JAWS_HTTP_10_Read_Task HTTP_Read;
  JAWS_HTTP_10_Parse_Task HTTP_Parse;
  JAWS_HTTP_10_Write_Task HTTP_Write;

  HTTP_Read.next (&HTTP_Parse);
  HTTP_Parse.next (&HTTP_Write);

  if (server.open (&HTTP_Read) == -1)
    ACE_DEBUG ((LM_DEBUG, "JAWS: Error openning server\n"));

  return 0;
}
