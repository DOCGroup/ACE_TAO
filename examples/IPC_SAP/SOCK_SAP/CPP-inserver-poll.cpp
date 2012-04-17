// $Id$

// IPC_SAP/poll server, which illustrates how to integrate the ACE
// socket wrappers with the SVR4 <poll> system call to create a
// single-threaded concurrent server.  This server program can be
// driven by the oneway test mode of CPP-inclient.cpp.

#include "ace/OS_main.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_poll.h"
#include "ace/OS_NS_stdio.h"



#if defined (ACE_HAS_POLL)

// Should we be verbose?
static int verbose = 0;

// Max number of open handles.
static const int MAX_HANDLES = 200;

struct Buffer_Info
{
  void *buf_;
  // Pointer to the buffer.

  size_t len_;
  // Length of the buffer.
};

// Array of <pollfd>'s.
static struct pollfd poll_array[MAX_HANDLES];

// Array of <Buffer_Info>.
static Buffer_Info buffer_array[MAX_HANDLES];

static void
init_poll_array (void)
{
  int i;

  for (i = 0; i < MAX_HANDLES; i++)
    {
      poll_array[i].fd = ACE_INVALID_HANDLE;
      poll_array[i].events = POLLIN;
    }
}

static int
init_buffer (size_t index)
{
  ACE_INT32 len;

 if (ACE::recv_n (poll_array[index].fd,
                  (void *) &len,
                  sizeof (ACE_INT32)) != sizeof (ACE_INT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "recv_n failed"),
                      -1);
  else
    {
      len = ntohl (len);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) reading messages of size %d from handle %d\n",
                  len,
                  poll_array[index].fd));

      ACE_ALLOCATOR_RETURN (buffer_array[index].buf_,
                            ACE_OS::malloc (len),
                            -1);
      buffer_array[index].len_ = len;
    }
  return 0;
}

static void
handle_data (size_t &n_handles)
{
  // Handle pending logging messages first (s_handle + 1 is guaranteed
  // to be lowest client descriptor).

  for (size_t index = 1; index < n_handles; index++)
    {
      if (ACE_BIT_ENABLED (poll_array[index].revents, POLLIN))
        {
          // First time in, we need to initialize the buffer.
          if (buffer_array[index].buf_ == 0
              && init_buffer (index) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) %p\n",
                          "init_buffer"));
              continue;
            }

          // Read data from client (terminate on error).

          ssize_t n = ACE::recv (poll_array[index].fd,
                                 buffer_array[index].buf_,
                                 buffer_array[index].len_);
          // <recv> will not block in this case!

          if (n == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n",
                        "read failed"));
          else if (n == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) closing oneway server at handle %d\n",
                          poll_array[index].fd));

              // Handle client connection shutdown.
              ACE_OS::close (poll_array[index].fd);
              poll_array[index].fd = poll_array[--n_handles].fd;

              ACE_OS::free ((void *) buffer_array[index].buf_);
              buffer_array[index].buf_ = 0;
              buffer_array[index].len_ = 0;
            }
          else if (verbose)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) %*s",
                        n,
                        buffer_array[index].buf_));
        }
    }
}

static void
handle_connections (ACE_SOCK_Acceptor &peer_acceptor,
                    size_t &n_handles)
{
  if (ACE_BIT_ENABLED (poll_array[0].revents, POLLIN))
    {
      ACE_SOCK_Stream new_stream;

      ACE_INET_Addr client;
      ACE_Time_Value nonblock (0, 0);

      // Handle all pending connection requests (note use of "polling"
      // feature that doesn't block).

      while (ACE_OS::poll (poll_array, 1, nonblock) > 0)
        if (peer_acceptor.accept (new_stream, &client) == -1)
          ACE_OS::perror ("accept");
        else
          {
            const char *s = client.get_host_name ();

            ACE_ASSERT (s != 0);
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) client %s\n",
                        s));
            poll_array[n_handles++].fd = new_stream.get_handle ();
          }
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  u_short port = ACE_DEFAULT_SERVER_PORT + 1;

  // Create a server end-point.
  ACE_INET_Addr addr (port);
  ACE_SOCK_Acceptor peer_acceptor (addr);

  ACE_HANDLE s_handle = peer_acceptor.get_handle ();

  init_poll_array ();

  poll_array[0].fd = s_handle;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting oneway server at port %d\n",
              port));

  for (size_t n_handles = 1;;)
    {
      ACE_ENDLESS_LOOP

      // Wait for client I/O events (handle interrupts).
      while (ACE_OS::poll (poll_array, n_handles) == -1
             && errno == EINTR)
        continue;

      handle_data (n_handles);
      handle_connections (peer_acceptor, n_handles);
    }

  /* NOTREACHED */
  return 0;
}
#else
#include <stdio.h>
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_POLL */
