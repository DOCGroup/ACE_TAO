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


static int
fail_no_listener_nonblocking(void)
{

int			status;
ACE_INET_Addr		nobody_home((u_short)4242, "mambo");  // "localhost"
ACE_SOCK_Connector	con;
ACE_SOCK_Stream		sock;
ACE_Time_Value		nonblock(0, 0);

    status = con.connect(sock, nobody_home, &nonblock);
    ACE_ASSERT(status == -1);		// Need a port that will fail

    if (errno == EWOULDBLOCK) {
        status = con.complete(sock);
	if (status != -1) {
	    ACE_DEBUG((LM_DEBUG, "Connect which should fail didn't\n"));
	    status = -1;
	}
	else if (errno != ECONNREFUSED) {
	    ACE_DEBUG((LM_DEBUG, "%p', expected ECONNREFUSED\n",
		       "Failed with '"));
	    status = -1;
	}
	else {
	    ACE_DEBUG((LM_DEBUG, "%p\n", "Proper fail"));
	    status = 0;
	}
    }
    else {
        ACE_DEBUG((LM_DEBUG,
		   "Test not executed fully; expected EWOULDBLOCK, %p\n",
		   "not"));
	status = -1;
    }

    sock.close();    // Just in case

    return status;

}


int
main (int, char *[])
{

int status = 0;

  ACE_START_TEST ("SOCK_Connector_Test");

  if (fail_no_listener_nonblocking() == -1)
      status = 1;

  ACE_END_TEST;
  return status;
}

