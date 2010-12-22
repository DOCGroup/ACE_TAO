// $Id$

#include "ace/OS_main.h"
#include "ace/SPIPE_Addr.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_poll.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_STREAM_PIPES)

#include "shared.h"

// Maximum per-process open I/O descriptors.
const int MAX_HANDLES = 200;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_SPIPE_Acceptor peer_acceptor;
  ACE_SPIPE_Stream new_stream;
  struct pollfd poll_array[MAX_HANDLES];
  ACE_HANDLE handle;

  for (handle = 0; handle < MAX_HANDLES; handle++)
    {
      poll_array[handle].fd = -1;
      poll_array[handle].events = POLLIN;
    }

  if (argc > 1)
    rendezvous = argv[1];

  ACE_OS::fdetach (ACE_TEXT_ALWAYS_CHAR (rendezvous));
  ACE_SPIPE_Addr addr (rendezvous);

  ACE_HANDLE s_handle = peer_acceptor.open (addr);

  if (s_handle == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "peer_acceptor.open"), -1);

  poll_array[0].fd = s_handle;

  for (int width = 1;;)
    {
      // Block waiting for client I/O events (handle interrupts).
      while (ACE_OS::poll (poll_array, width) == -1 && errno == EINTR)
        continue;

      // Handle pending logging messages first (s_handle + 1 is
      // guaranteed to be lowest client descriptor).

      for (handle = s_handle + 1; handle < width; handle++)
        if (ACE_BIT_ENABLED (poll_array[handle].revents, POLLIN)
            || ACE_BIT_ENABLED (poll_array[handle].revents, POLLHUP))
          {
            char buf[BUFSIZ];
            ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

            // recv will not block in this case!
            if (n == -1)
              ACE_DEBUG ((LM_DEBUG, "%p\n", "read failed"));
            else if (n == 0)
              {
                // Handle client connection shutdown.
                if (ACE_OS::close (poll_array[handle].fd) == -1)
                  ACE_DEBUG ((LM_DEBUG, "%p\n", "close"));
                poll_array[handle].fd = -1;

                if (handle + 1 == width)
                  {
                    while (poll_array[handle].fd == -1)
                      handle--;
                    width = handle + 1;
                  }
              }
            else
              ACE_DEBUG ((LM_DEBUG, "%*s\n", n, buf));
          }

      if (ACE_BIT_ENABLED (poll_array[0].revents, POLLIN))
        {
          if (peer_acceptor.accept (new_stream) == -1)
            ACE_DEBUG ((LM_DEBUG, "%p\n", "accept failed"));

          ACE_SPIPE_Addr client;
          ACE_HANDLE n_handle = new_stream.get_handle ();

          if (new_stream.get_remote_addr (client) == -1)
            ACE_DEBUG ((LM_DEBUG, "%p\n",
                        "get_remote_addr failed"));

          ACE_DEBUG ((LM_DEBUG,
                      "n_handle = %d, uid = %d, gid = %d\n",
                      n_handle,
                      client.user_id (),
                      client.group_id ()));

          int arg = RMSGN | RPROTDAT;

          if (ACE_OS::ioctl (n_handle,
                             I_SRDOPT, (void *) arg) == -1)
            ACE_DEBUG ((LM_DEBUG, "%p\n", "ioctl failed"));

          poll_array[n_handle].fd = n_handle;

          if (n_handle >= width)
            width = n_handle + 1;
        }
    }

  ACE_NOTREACHED (return 0;)
}
#else
#include <stdio.h>
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_STREAM_PIPES */
