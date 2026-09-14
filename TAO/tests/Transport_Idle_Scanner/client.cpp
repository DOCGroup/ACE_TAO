#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

// Probe without consuming anything. A timeout means the peer is still open.
bool expect_open (ACE_SOCK_Stream &stream, bool open)
{
  char byte;
  ACE_Time_Value timeout (0, 100000);
  ssize_t result = stream.recv (&byte, 1, MSG_PEEK, &timeout);
  bool actual = result == -1
    && (errno == ETIME || errno == ETIMEDOUT || errno == EWOULDBLOCK);
  if (actual != open)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) socket open=%d, expected=%d, recv=%d errno=%d\n"),
                  actual, open, result, errno));
      return false;
    }
  return true;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc != 2)
    return 1;
  ACE_INET_Addr address (argv[1]);
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream idle, partial, fragmented;
  ACE_Time_Value connect_timeout (5);
  if (connector.connect (idle, address, &connect_timeout) == -1
      || connector.connect (partial, address, &connect_timeout) == -1
      || connector.connect (fragmented, address, &connect_timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) connect failed: %m\n")), 1);

  // Big-endian GIOP 1.2 CancelRequest, request id 1.
  const unsigned char cancel[] = {
    'G','I','O','P', 1,2,0,2, 0,0,0,4, 0,0,0,1
  };
  // Incomplete GIOP 1.2 Request with more-fragments flag, request id 1.
  const unsigned char fragment[] = {
    'G','I','O','P', 1,2,2,0, 0,0,0,4, 0,0,0,1
  };
  if (partial.send_n (cancel, 6) != 6
      || fragmented.send_n (fragment, sizeof fragment) != sizeof fragment)
    return 1;

  // Y=2, X=2: eligible idle sockets should close within ~4 seconds.
  ACE_OS::sleep (1);
  bool ok = expect_open (idle, true);
  ACE_OS::sleep (4);
  ok = expect_open (idle, false)
    && expect_open (partial, true)
    && expect_open (fragmented, true) && ok;
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) idle/partial/fragment scan checks: %d\n"), ok));

  // Finish the partial message and cancel the outstanding fragmented request.
  // Both transports must then become eligible for eventual idle closure.
  if (partial.send_n (cancel + 6, sizeof cancel - 6) != sizeof cancel - 6
      || fragmented.send_n (cancel, sizeof cancel) != sizeof cancel)
    return 1;
  ACE_OS::sleep (1);
  ok = expect_open (partial, true) && expect_open (fragmented, true) && ok;
  ACE_OS::sleep (4);
  ok = expect_open (partial, false) && expect_open (fragmented, false) && ok;
  idle.close ();
  partial.close ();
  fragmented.close ();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) completed-input idle scan checks: %d\n"), ok));
  return ok ? 0 : 1;
}
