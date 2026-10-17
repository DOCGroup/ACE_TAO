#include "ace/SPIPE_Acceptor.h"
#include "ace/SPIPE_Addr.h"
#include "ace/Service_Config.h"

#include "CPP-acceptor.h"

#if !defined (ACE_WIN32)
using SVC_HANDLER = Svc_Handler<ACE_SPIPE_STREAM>;
using IPC_SERVER = IPC_Server<SVC_HANDLER, ACE_SPIPE_ACCEPTOR>;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_SERVER peer_acceptor;

  if (peer_acceptor.init (argc,
                          argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("init")),
                      -1);

  return peer_acceptor.svc ();
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "This test is not ported to Win32 (yet)\n"),
                    -1);
}
#endif /* !ACE_WIN32 */
