// $Id$

#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_errno.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Listing 1 code/ch06
  ACE_INET_Addr port_to_listen ("HAStatus");
  ACE_SOCK_Acceptor acceptor;

  if (acceptor.open (port_to_listen, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("acceptor.open")),
                      100);
  // Listing 1

  //FUZZ: disable check_for_lack_ACE_OS
  /*
   * The complete open signature:
   *
   // Listing 2 code/ch06
  int open (const ACE_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_INET,
            int backlog = ACE_DEFAULT_BACKLOG,
            int protocol = 0);
   // Listing 2
   *
   */
  //FUZZ: enable check_for_lack_ACE_OS

  while (1)
    {
      ACE_SOCK_Stream peer;
      ACE_INET_Addr peer_addr;
      ACE_Time_Value timeout (10, 0);

      /*
       * Basic acceptor usage
       */
#if 0
      // Listing 3 code/ch06
      if (acceptor.accept (peer) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Failed to accept ")
                           ACE_TEXT ("client connection\n")),
                          100);
      // Listing 3
#endif /* 0 */

      // Listing 4 code/ch06
      if (acceptor.accept (peer, &peer_addr, &timeout, 0) == -1)
        {
          if (ACE_OS::last_error() == EINTR)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) Interrupted while ")
                        ACE_TEXT ("waiting for connection\n")));
          else
            if (ACE_OS::last_error() == ETIMEDOUT)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Timeout while ")
                          ACE_TEXT ("waiting for connection\n")));
        }
      // Listing 4
      // Listing 5 code/ch06
      else
        {
          ACE_TCHAR peer_name[MAXHOSTNAMELEN];
          peer_addr.addr_to_string (peer_name, MAXHOSTNAMELEN);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Connection from %s\n"),
                      peer_name));
          // Listing 5
          // Listing 7 code/ch06
          char buffer[4096];
          ssize_t bytes_received;

          while ((bytes_received =
                    peer.recv (buffer, sizeof(buffer))) != -1)
            {
              peer.send_n (buffer, bytes_received);
            }

          peer.close ();
          // Listing 7
        }
    }

  ACE_NOTREACHED (return 0);
}
