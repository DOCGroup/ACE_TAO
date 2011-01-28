// $Id$

// The following code illustrates how the ACE_Dumpable mechanisms are
// integrated into ACE components like the SOCK_Acceptor and
// SOCK_Stream.

#include "ace/OS_main.h"
#include "ace/Dump.h"
#include "test_dump.h"

#include "ace/Reactor.h"



int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  SOCK outer_sock;
  // Note that the SOCK superclass is *not* printed.
  SOCK_Stream outer_stream;
  SOCK_Acceptor outer_acceptor;
  ACE_ODB::instance ()->dump_objects ();
  {
    SOCK inner_sock;
    // Note that the SOCK superclass is *not* printed.
    SOCK_Stream inner_stream;
    SOCK_Acceptor inner_acceptor;
    ACE_ODB::instance ()->dump_objects ();
  }
  ACE_ODB::instance ()->dump_objects ();
  return 0;
}

