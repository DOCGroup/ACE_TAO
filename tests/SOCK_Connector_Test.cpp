// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    SOCK_Connector_Test.cpp
//
// = DESCRIPTION
//     This is a test of ACE_SOCK_Connector, focusing on failure cases more
//     than on success cases.
//
// = AUTHOR
//    Steve Huston
//
// ============================================================================

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "test_config.h"

// The original problem this program tested for was incorrectly saying
// a non-blocking connect completed successfully when it didn't.  The
// test doesn't always work when done to localhost
// (platform-dependant) so we look around for another host - any other
// one will do.

static void
find_another_host (ASYS_TCHAR other_host[])
{
  ACE_OS::strcpy (other_host, ACE_DEFAULT_SERVER_HOST); // If all else fails

#if !defined (ACE_LACKS_GETHOSTENT)
  // These gethost-type things don't work everywhere.
  struct hostent *h;
  struct utsname un;

  ACE_OS::uname (&un);

  h = ACE_OS::gethostbyname (un.nodename);

  // Use me if can't find another
  ACE_OS::strcpy (other_host, ASYS_WIDE_STRING (h->h_name));

  // @@ We really need to add wrappers for these hostent methods.
  sethostent (1);

  while ((h = gethostent ()) != NULL)
    {
      if (ACE_OS::strcmp (h->h_name, ACE_DEFAULT_SERVER_HOST) == 0)
        continue;
      // AIX just _has_ to be different
      if (ACE_OS::strcmp (h->h_name, "loopback") == 0)
        continue;

      // If not me
      if (ACE_OS::strcmp (h->h_name, other_host) != 0 &&
          ACE_OS::strcmp (h->h_name, un.nodename) != 0   )
        {
          ACE_OS::strcpy (other_host, h->h_name);
          break;
        }
    }

  endhostent ();
#endif /* ! ACE_LACKS_GETHOSTENT */
}

static int
fail_no_listener_nonblocking (void)
{
  ASYS_TCHAR test_host[MAXHOSTNAMELEN];
  int status;
  ACE_INET_Addr nobody_home;
  ACE_SOCK_Connector con;
  ACE_SOCK_Stream sock;
  ACE_Time_Value nonblock (0, 0);

  find_another_host (test_host);
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("Testing to host %s\n"), test_host));
  nobody_home.set ((u_short) 42000, test_host);
  status = con.connect (sock, nobody_home, &nonblock);

  // Need a port that will fail.
  ACE_ASSERT (status == -1);

  // On some systems, a failed connect to localhost will return
  // ECONNREFUSED or ENETUNREACH directly, instead of
  // EWOULDBLOCK. That is also fine.

  if (errno == EWOULDBLOCK || errno == ECONNREFUSED || errno == ENETUNREACH)
    {
      if (sock.get_handle () != ACE_INVALID_HANDLE)
        status = con.complete (sock);

      if (status != -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("Connect which should fail didn't\n")));
          status = -1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%p\n"), ASYS_TEXT ("Proper fail")));
          status = 0;
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("Test not executed fully; expected EWOULDBLOCK, %p\n"),
                  ASYS_TEXT ("not")));
      status = -1;
    }

  // Just in case.
  sock.close ();

  return status;
}


// This test tries to hit a port that's listening.  SMTP (25) is pretty
// popular.  Just in case, though, it won't report a failure if it gets
// 'refused" (no listener) since the real fixed bug this is testing is
// a returned error of EWOULDBLOCK when the connect really did work.
// That was a side-affect of how ACE::handle_timed_complete does checks
// on some systems.
static int
succeed_nonblocking (void)
{
  ASYS_TCHAR test_host[MAXHOSTNAMELEN];
  int status;
  ACE_INET_Addr smtp_server;
  ACE_SOCK_Connector con;
  ACE_SOCK_Stream sock;
  ACE_Time_Value nonblock (0, 0);

  find_another_host (test_host);
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("Testing to host %s\n"), test_host));
  smtp_server.set ((u_short) 25, test_host);
  status = con.connect (sock, smtp_server, &nonblock);

  // Need to test the call to 'complete' really.
  if (status == 0 || (status == -1 && errno != EWOULDBLOCK))
    {
      ACE_DEBUG((LM_DEBUG,
		 ASYS_TEXT ("Immediate success/fail; test not completed\n")));
      status = 0;
    }
  else
    {
      if (sock.get_handle () != ACE_INVALID_HANDLE)
        status = con.complete (sock);

      if (status == -1)
	ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("%p\n"),
		    ASYS_TEXT("connect:complete")));
      else
	ACE_DEBUG((LM_DEBUG,
		   ASYS_TEXT("Connect which should succeed, did\n")));
    }

  // Just in case.
  sock.close ();

  return status;
}


int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("SOCK_Connector_Test"));

  int status = 0;

  if (fail_no_listener_nonblocking () == -1)
    status = 1;

  if (succeed_nonblocking () == -1)
    status = 1;

  ACE_END_TEST;
  return status;
}
