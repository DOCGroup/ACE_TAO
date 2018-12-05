
//=============================================================================
/**
 *  @file    SOCK_Acceptor_Test.cpp
 *
 *   This is a test of the <ACE_SOCK_Acceptor> class.
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Time_Value.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"


static int
test_accept (ACE_Addr listen_addr, int v6_only)
{
  int status = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting listen, ipv6-only %d\n"),
              v6_only));
  // Bind listener to any port and then find out what the port was.
  ACE_SOCK_Acceptor acceptor;
  ACE_INET_Addr listening_at;
  if (acceptor.open (listen_addr,
                     0,
                     PF_UNSPEC,
                     ACE_DEFAULT_BACKLOG,
                     0,
                     v6_only) == -1
      || acceptor.get_local_addr (listening_at) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("open")),
                      -1);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) listening at port %d\n"),
              listening_at.get_port_number ()));

  // Try IPv4 and, if v6 is available, v6. If v6-capable and v6-only
  // is selected, v4 should NOT work and v6 should. If not v6-capable
  // just try v4 and it should work regardless of v6-only.
  ACE_SOCK_Connector connector;

  ACE_INET_Addr v4_addr (listening_at.get_port_number(),
                         ACE_LOCALHOST,
                         PF_INET);
  ACE_SOCK_Stream v4_stream;
  bool v4_ok = (connector.connect (v4_stream, v4_addr) == 0);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("v4 connect %s\n"),
              v4_ok ? ACE_TEXT ("OK") : ACE_TEXT ("FAIL")));
  v4_stream.close();

#if defined (ACE_HAS_IPV6)
  ACE_INET_Addr v6_addr (listening_at.get_port_number(),
                         ACE_IPV6_LOCALHOST,
                         PF_INET6);
  ACE_SOCK_Stream v6_stream;
  bool v6_ok = (connector.connect (v6_stream, v6_addr) == 0);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("v6 connect %s\n"),
              v6_ok ? ACE_TEXT ("OK") : ACE_TEXT ("FAIL")));
  v6_stream.close();

  if (v6_only && v4_ok)
    status = 1;
  if (!v6_only && !v4_ok)
    status = 1;
  if (!v6_ok)
    status = 1;
#else
  status = v4_ok ? 0 : 1;
#endif /* ACE_HAS_IPV6 */
  acceptor.close ();
  return status;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_Acceptor_Test"));

  int status = 0;

  if (test_accept (ACE_Addr::sap_any, 1) != 0)
    status = 1;
  if (test_accept (ACE_Addr::sap_any, 0) != 0)
    status = 1;

  ACE_END_TEST;
  return status;
}
