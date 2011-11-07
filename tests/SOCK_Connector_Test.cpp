
//=============================================================================
/**
 *  @file    SOCK_Connector_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of ACE_SOCK_Connector, focusing on failure cases more
 *   than on success cases.
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/Time_Value.h"
#include "ace/SOCK_Stream.h"
#include "ace/OS_NS_sys_utsname.h"
#include "ace/OS_NS_netdb.h"



// Host candidate list
struct Host_Candidate
{
  ACE_TCHAR host_name[MAXHOSTNAMELEN];
};

const int MAX_CANDIDATES = 50;
Host_Candidate candidate[MAX_CANDIDATES];

#if !defined (ACE_LACKS_GETHOSTENT)
// Determine if a host exists, is reachable, and is up.  Attempt a
// blocking connection to it; if it succeeds, then the host exists, is
// reachable, and is up.

static u_int
host_is_up (ACE_TCHAR hostname[])
{
  ACE_SOCK_Connector con;
  ACE_SOCK_Stream sock;

  // The ACE_INET_Addr construction causes gethostbyname_r to be
  // called, so we need to copy the hostname.
  ACE_TCHAR test_host[MAXHOSTNAMELEN];
  ACE_OS::strcpy (test_host, hostname);

  ACE_INET_Addr another_host ((u_short) 7, test_host);
  ACE_Time_Value timeout_value (5);
  int const status = con.connect (sock,
                                  another_host,
                                  &timeout_value);
  sock.close ();
  return status == 0  ?  1  :  0;
}
#endif /* ! ACE_LACKS_GETHOSTENT */

// The original problem this program tested for was incorrectly saying
// a non-blocking connect completed successfully when it didn't.  The
// test doesn't always work when done to localhost
// (platform-dependant) so we look around for another host - any other
// one will do.

static void
find_another_host (ACE_TCHAR other_host[])
{
  static ACE_TCHAR cached_other_host[MAXHOSTNAMELEN] = {'\0'};

  if (cached_other_host[0] == '\0')
    {

      ACE_OS::strcpy (other_host,
                      ACE_DEFAULT_SERVER_HOST); // If all else fails

#if !defined (ACE_LACKS_GETHOSTENT)
      // These gethost-type things don't work everywhere.
      struct hostent *h = 0;
      ACE_utsname un;

      ACE_OS::uname (&un);

      h = ACE_OS::gethostbyname (un.nodename);

      if (h == 0)
        ACE_OS::strcpy (other_host, ACE_LOCALHOST);
      else
        // Use me if can't find another
        ACE_OS::strcpy (other_host, ACE_TEXT_CHAR_TO_TCHAR (h->h_name));

      // @@ We really need to add wrappers for these hostent methods.

      // Optimize for sequential access of DNS or hosts file.
      sethostent (1);

      int candidate_count = 0;

      // Accumulate candidates first.  There is some interaction on
      // Linux systems between <gethostent> and <gethostbyname_r>
      // (called by ACE_INET_Addr in host_is_up) This otherwise causes
      // an infinite loop on Linux --mas 03-08-2001
      while ((h = gethostent ()) != 0)
        {
          if (ACE_OS::strcmp (h->h_name,
                              ACE_TEXT_ALWAYS_CHAR (ACE_DEFAULT_SERVER_HOST)) == 0)
            continue;
          // AIX just _has_ to be different
          if (ACE_OS::strcmp (h->h_name, "loopback") == 0)
            continue;

          // If not me.
          if (ACE_OS::strcmp
                (h->h_name, ACE_TEXT_ALWAYS_CHAR (other_host)) != 0
              && ACE_OS::strcmp (h->h_name, un.nodename) != 0)
            {
               ACE_OS::strcpy (candidate[candidate_count].host_name,
                               ACE_TEXT_CHAR_TO_TCHAR (h->h_name));
               if (++candidate_count >= MAX_CANDIDATES)
                 break;
            }
        }

      // Now try to connect to candidates
      for (int i = 0; i < candidate_count; i++)
        if (host_is_up (candidate[i].host_name))
          {
            ACE_OS::strcpy (other_host, candidate[i].host_name);
            break;
          }

      sethostent (0);
      endhostent ();
#endif /* ! ACE_LACKS_GETHOSTENT */

      ACE_OS::strcpy (cached_other_host, other_host);
    }
  else
    ACE_OS::strcpy (other_host, cached_other_host);
}

static int
fail_no_listener_nonblocking (void)
{
  ACE_TCHAR test_host[MAXHOSTNAMELEN], test_addr[MAXHOSTNAMELEN + 8];
  int status;
  ACE_INET_Addr nobody_home;
  ACE_SOCK_Connector con;
  ACE_SOCK_Stream sock;
  ACE_Time_Value nonblock (0, 0);

  find_another_host (test_host);
  if (nobody_home.set ((u_short) 42000, test_host) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Host lookup for %s %p\n"),
                  test_host,
                  ACE_TEXT ("failed")));
      return -1;
    }
  nobody_home.addr_to_string (test_addr, MAXHOSTNAMELEN + 8);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing to host \"%s\" (%s)\n"),
              test_host, test_addr));

  status = con.connect (sock, nobody_home, &nonblock);

  // Need a port that will fail.
  if (status == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Connect which should fail didn't\n")));
      status = -1;
    }

  // On some systems, a failed connect to localhost will return
  // ECONNREFUSED or ENETUNREACH directly, instead of
  // EWOULDBLOCK. That is also fine.

  else if (errno == EWOULDBLOCK ||
           errno == ECONNREFUSED ||
           errno == ENETUNREACH)
    {
      if (sock.get_handle () != ACE_INVALID_HANDLE)
        status = con.complete (sock);

      if (status != -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Connect which should fail didn't\n")));
          status = -1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Proper fail")));
          status = 0;
        }
    }
  else
    {
      ACE_DEBUG ((LM_WARNING,
                  ACE_TEXT ("Test not executed fully; ")
                  ACE_TEXT ("expected EWOULDBLOCK, %p (%d)\n"),
                  ACE_TEXT ("not"), ACE_ERRNO_GET));
      status = -1;
    }

  // Just in case.
  sock.close ();

  return status;
}


// This test tries to hit a port that's listening.  Echo (7) is pretty
// popular.  Just in case, though, it won't report a failure if it
// gets "refused" (no listener) since the real fixed bug this is
// testing is a returned error of EWOULDBLOCK when the connect really
// did work.  That was a side-affect of how
// <ACE::handle_timed_complete> does checks on some systems.

static int
succeed_nonblocking (void)
{
  ACE_TCHAR test_host[MAXHOSTNAMELEN], test_addr[MAXHOSTNAMELEN + 8];
  int status;
  ACE_INET_Addr echo_server;
  ACE_SOCK_Connector con;
  ACE_SOCK_Stream sock;
  ACE_Time_Value nonblock (0, 0);
  u_short test_port = 7;          // Echo

  find_another_host (test_host);
  if (ACE_OS::strcmp (ACE_TEXT ("localhost"), test_host) == 0)
    {
#if defined (ACE_WIN32)
      test_port = 80;        // Echo not available on Win32; try web server
#endif /* ACE_WIN32 */
    }
  if (echo_server.set (test_port, test_host) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Host lookup for %s %p\n"),
                  test_host,
                  ACE_TEXT ("failed")));
      return -1;
    }
  echo_server.addr_to_string (test_addr, MAXHOSTNAMELEN + 8);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing to host \"%s\", port %d (%s)\n"),
              test_host, test_port, test_addr));
  status = con.connect (sock, echo_server, &nonblock);

  // Need to test the call to 'complete' really.
  if (status == 0 || (status == -1 && errno != EWOULDBLOCK))
    {
      ACE_DEBUG((LM_WARNING,
                 ACE_TEXT ("Immediate success/fail; test not completed\n")));
      status = 0;
    }
  else
    {
      if (sock.get_handle () != ACE_INVALID_HANDLE)
        {
          status = con.complete (sock);
        }

      if (status == -1)
        {
          // Reset the status _before_ doing the printout, in case the
          // printout overwrites errno.
          if (errno == ECONNREFUSED)
            {
              status = 0;
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Should succeed, but refused: ok\n")));
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("Errno <%d>: %p\n"),
                          ACE_ERRNO_GET,
                          ACE_TEXT("connect should succeed, but")));
            }
        }
      else
        ACE_DEBUG((LM_DEBUG,
                   ACE_TEXT("Connect which should succeed, did\n")));
    }

  // Just in case.
  sock.close ();

  return status;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_Connector_Test"));

  int status = 0;

  if (fail_no_listener_nonblocking () == -1)
    status = 1;

  if (succeed_nonblocking () == -1)
    status = 1;

  ACE_END_TEST;
  return status;
}



