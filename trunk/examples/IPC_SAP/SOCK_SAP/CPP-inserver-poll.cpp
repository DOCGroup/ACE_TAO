// $Id$

// IPC_SAP/poll server, which illustrates how to integrate the ACE
// socket wrappers with the SVR4 <poll> system call to create a
// single-threaded concurrent server.

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"

#if defined (ACE_HAS_POLL)

// Max number of open handles.
const int MAX_HANDLES = 200;

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
init_buffer (ACE_HANDLE handle)
{
  if (ACE::recv_n (handle,
                   (void *) &buffer_array[handle].len_,
                   sizeof (ACE_UINT32)) != sizeof (ACE_UINT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "recv_n failed"),
                      0);
  else
    {
      buffer_array[handle].len_ =
        ntohl (buffer_array[handle].len_);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) reading messages of size %d\n",
                  buffer_array[handle].len_));
      buffer_array[handle].buf_ =
        ACE_OS::malloc (buffer_array[handle].len_);
    }
}

static void
handle_data (size_t n_handles)
{
  // Handle pending logging messages first (s_handle + 1 is guaranteed
  // to be lowest client descriptor).

  for (ACE_HANDLE handle = 1; handle < n_handles; handle++)
    {
      if (ACE_BIT_ENABLED (poll_array[handle].revents, POLLIN))
        {
          // Read data from client (terminate on error).

          // First time in -- gotta initialize the buffer.
          if (buffer_array[handle].buf_ == 0)
            init_buffer (handle);

          ssize_t n = ACE::recv (poll_array[handle].fd,
                                 buffer_array[handle].buf_, 
                                 buffer_array[handle].len_);
          // <recv> will not block in this case!

          if (n == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n",
                        "read failed"));
          else if (n == 0)
            {
              // Handle client connection shutdown.
              ACE_OS::close (poll_array[handle].fd);
              poll_array[handle].fd = poll_array[--n_handles].fd;

              ACE_OS::free ((void *) buffer_array[handle].buf_);
              buffer_array[handle].buf_ = 0;

              // Send handshake back to client to unblock it.
              if (ACE::send (poll_array[handle].fd, "", 1) != 1)
                ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));
            }
          else
            ACE_OS::printf ("%*s", n, buffer_array[handle].buf_), fflush (stdout);
        }
      ACE_OS::fflush (stdout);
    }
}

static void
handle_connections (ACE_SOCK_Acceptor &peer_acceptor,
                    ACE_HANDLE &n_handles)
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
            ACE_OS::printf ("client %s\n", s);
            ACE_OS::fflush (stdout);
            poll_array[n_handles++].fd = new_stream.get_handle ();
          }
    }
}

int
main (int, char *[])
{
  // Create a server end-point.
  ACE_INET_Addr addr (ACE_DEFAULT_SERVER_PORT);
  ACE_SOCK_Acceptor peer_acceptor (addr);

  ACE_HANDLE s_handle = peer_acceptor.get_handle ();

  init_poll_array ();

  poll_array[0].fd = s_handle;

  for (int n_handles = 1;;)
    {
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
int main (int, char *[])
{
  ACE_OS::fprintf (stderr, "This feature is not supported\n");
  return 0;
}
#endif /* ACE_HAS_SVR4_POLL */
