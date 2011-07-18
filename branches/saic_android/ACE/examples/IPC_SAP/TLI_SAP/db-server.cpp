// $Id$

// Simple multi-threaded database server example.

#include "ace/OS_NS_stdio.h"
#include "ace/TLI_Acceptor.h"
#include "ace/Thread_Manager.h"



#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_TLI)

// Global thread manager.
ACE_Thread_Manager thr_mgr;

void *
lookup_name (ACE_HANDLE handle)
{
  enum
  {
    MAXLINE = 255,
    EMPNAMELEN = 512
  };

  static struct
  {
    int emp_id;
    const char *emp_name;
  } employee_db[] =
    {
      {123, "John Wayne Bobbit"},
      {124, "Woody Allen"},
      {125, "O. J. Simpson"},
      {126, "Bill Clinton"},
      {127, "Rush Limbaugh"},
      {128, "Michael Jackson"},
      {129, "Kenneth Starr"},
      {130, "Paula Jones"},
      {131, "Monica Lewinsky"},
      {132, "Marv Albert"},
      {0, ""}
    };

  int flags;
  int employee_id;
  int index;
  int found;
  ACE_TLI_Stream stream;
  char recvline[MAXLINE];
  char sendline[MAXLINE];

  ACE_DEBUG ((LM_DEBUG,
              "stream handle = %d, thread id = %t\n",
              handle));
  stream.set_handle (handle);

  ssize_t n = stream.recv (recvline, MAXLINE, &flags);

  if (n == -1)
    ACE_OS::t_error ("stream.recv error");

  employee_id = ACE_OS::atoi (recvline);
  found = 0;

  for (index = 0; found == 0 && employee_db[index].emp_id; index++)
    if (employee_id == employee_db[index].emp_id)
      {
        found = 1;
        n = ACE_OS::sprintf (sendline,
                             "%s", employee_db[index].emp_name);
      }

  if (found == 0)
    n = ACE_OS::sprintf (sendline, "%s", "ERROR");

  if (stream.send (sendline, n + 1, 0) == -1)
    ACE_OS::t_error ("stream.send error");

  if (stream.sndrel () == -1)
    ACE_OS::t_error ("stream.send error");

  if (stream.close () == -1)
    ACE_OS::t_error ("stream.close error");

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  ACE_INET_Addr l_addr (port);
  ACE_TLI_Acceptor server (l_addr, 1); // Create server, reuse addr if in use.
  ACE_TLI_Stream new_stream;

  // Wait for a connection from a client.  This is an example of a
  // concurrent server.

  for (;;)
    {
      if (server.accept (new_stream) == -1)
        ACE_OS::t_error ("server.accept error");

      if (thr_mgr.spawn (ACE_THR_FUNC (lookup_name),
                         (void *) new_stream.get_handle (),
                         THR_DETACHED) == -1)
        ACE_DEBUG ((LM_ERROR,
                    "server: can't create worker thread %d\n"));
    }

  ACE_NOTREACHED (return 0);
}
#else
#include <stdio.h>
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "platform isn't configured to support TLI\n"),
                    1);
}
#endif /* ACE_HAS_THREADS */
