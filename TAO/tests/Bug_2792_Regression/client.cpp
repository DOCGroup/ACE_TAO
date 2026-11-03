#include "tao/ORB.h"
#include "tao/StringSeqC.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::StringSeq foo;
      foo.length (1);
      foo[0] = "Hello World";
      CORBA::StringSeq const &bar = foo;
      ACE_DEBUG ((LM_DEBUG, "String: %C\n", bar[0].in ()));
      CORBA::String_var const mystring = bar[0].in ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "Exception caught: %s\"%s\"\n",
                  ex._name (),
                  ex._rep_id ()));
      return 1;
    }

  return 0;
}
