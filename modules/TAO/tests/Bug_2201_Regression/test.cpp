// $Id$

#include "ace/Log_Msg.h"
#include "TestDataC.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int retval = 0;

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  SeqTest::NodeSeq ns;
  ns.length (1);
  ns[0].ls.length (1);
  ns[0].ls[0] = 42;

  ns.length (0); // Shrink sequence
  ns.length (1); // Re-grow sequence; should re-initialize meber sequence
                 // "as if" default constructed. I.e., the "ls" member
                 // should have a length of zero.

  if (ns[0].ls.length() == 0) {
    ACE_DEBUG ((LM_DEBUG, "Test passed\n"));
  }
  else {
    ACE_ERROR ((LM_ERROR, "Test failed\n"));
    retval = 1;
  }

  orb->destroy ();

  return retval;
}
