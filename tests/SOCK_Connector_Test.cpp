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

#if !defined (ACE_WIN32) && !defined (VXWORKS)
#include <sys/utsname.h>
#endif /* !defined (ACE_WIN32) && !defined (VXWORKS) */

// The original problem this program tested for was incorrectly saying
// a non-blocking connect completed successfully when it didn't.  The
// test doesn't always work when done to localhost
// (platform-dependant) so we look around for another host - any other
// one will do.

static void
find_another_host (char other_host[])
{
  ACE_OS::strcpy (other_host, "localhost");	// If all else fails

  // These gethost-type things don't work everywhere.
#if !defined (ACE_WIN32) && !defined (VXWORKS)
  struct hostent *h;
  struct utsname un;

  ACE_OS::uname (&un);

  h = ACE_OS::gethostbyname (un.nodename);

  // Use me if can't find another
  ACE_OS::strcpy (other_host, h->h_name);	

  // @@ We really need to add wrappers for these hostent methods.
  sethostent (1);

  while ((h = gethostent ()) != NULL) 
    {
      if (ACE_OS::strcmp (h->h_name, "localhost") == 0)
	continue;

      // If not me
      if (ACE_OS::strcmp (h->h_name, other_host) != 0) 
	{
	  ACE_OS::strcpy (other_host, h->h_name);
	  break;
	}
    }

  endhostent ();
#endif /* !defined (ACE_WIN32) && !defined (VXWORKS) */
}

static int
fail_no_listener_nonblocking (void)
{
  char test_host[MAXHOSTNAMELEN];
  int status;
  ACE_INET_Addr nobody_home;
  ACE_SOCK_Connector con;
  ACE_SOCK_Stream sock;
  ACE_Time_Value nonblock (0, 0);

  find_another_host (test_host);
  ACE_DEBUG ((LM_DEBUG, "Testing to host %s\n", test_host));
  nobody_home.set ((u_short) 4242, test_host);
  status = con.connect (sock, nobody_home, &nonblock);

  // Need a port that will fail.
  ACE_ASSERT (status == -1);		

  if (errno == EWOULDBLOCK) 
    {
      status = con.complete (sock);

      if (status != -1) 
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Connect which should fail didn't\n"));
	  status = -1;
	}
      else if (!(errno == ECONNREFUSED || errno == ENOTCONN || errno ETIMEDOUT)
	{
	  ACE_DEBUG ((LM_DEBUG, "%p at errno %d, but expected ECONNREFUSED or ENOTCONN\n",
		      "Failed with", errno));
	  status = -1;
	}
      else 
	{
	  ACE_DEBUG ((LM_DEBUG, "%p\n", "Proper fail"));
	  status = 0;
	}
    }
  else 
    {
      ACE_DEBUG ((LM_DEBUG,
		  "Test not executed fully; expected EWOULDBLOCK, %p\n",
		  "not"));
      status = -1;
    }

  // Just in case.
  sock.close ();    

  return status;
}

int
main (int, char *[])
{
  ACE_START_TEST ("SOCK_Connector_Test");

  int status = 0;

  if (fail_no_listener_nonblocking () == -1)
    status = 1;

  ACE_END_TEST;
  return status;
}

