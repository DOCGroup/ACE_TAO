/* Simple multi-threaded database server example. */
// @(#)db-server.cpp	1.1	10/18/96

#include "ace/Log_Msg.h"
#include "ace/TLI_Acceptor.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_TLI)

/* Global thread manager. */
ACE_Thread_Manager thr_mgr;

void *
lookup_name (ACE_HANDLE handle)
{
  ACE_Thread_Control tc (&thr_mgr);

  enum
  {
    MAXLINE = 255,
    EMPNAMELEN = 512
  };

  static struct 
    {
      int emp_id;
      const char emp_name[EMPNAMELEN];
    } employee_db[] =
      {
        {123, "John Wayne Bobbit"},
        {124, "Cindy Crawford"},
        {125, "O. J. Simpson"},
        {126, "Bill Clinton"},
        {127, "Rush Limbaugh"},
        {128, "Michael Jackson"},
        {129, "George Burns"},
	{130, "Paula Jones"},
	{0, ""}
      };

  int n;
  int flags;
  int len;
  int employee_id;
  int index;
  int found;
  ACE_TLI_Stream stream;
  char recvline[MAXLINE];
  char sendline[MAXLINE];

  ACE_DEBUG ((LM_DEBUG, "stream handle = %d, thread id = %t\n", handle));
  stream.set_handle (handle);

  if ((n = stream.recv (recvline, MAXLINE, &flags)) == -1)
    ACE_OS::t_error ("stream.recv error");

  employee_id = ACE_OS::atoi (recvline);
  found = 0;

  for (index = 0; found == 0 && employee_db[index].emp_id; index++)
    if (employee_id == employee_db[index].emp_id)
      {
	found = 1;
	n = ACE_OS::sprintf (sendline, "%s", employee_db[index].emp_name);
      }

  if (found == 0)
    n = ACE_OS::sprintf (sendline, "%s", "ERROR");

  if ((len = stream.send (sendline, n + 1, 0)) == -1)
    ACE_OS::t_error ("stream.send error");

  if (stream.sndrel () == -1)
    ACE_OS::t_error ("stream.send error");

  if (stream.close () == -1)
    ACE_OS::t_error ("stream.close error");

  return 0;
}

int 
main (int argc, char *argv[])
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
	::t_error ("server.accept error");

      if (thr_mgr.spawn (ACE_THR_FUNC (lookup_name),
			 (void *) new_stream.get_handle (), 
			 THR_DETACHED) == -1)
	ACE_DEBUG ((LM_ERROR, "server: can't create worker thread %d\n"));
    }
  return 0;
}
#else
#include <stdio.h>
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support ACE_TLI\n"), 1);
}
#endif /* ACE_HAS_THREADS */
