// $Id$

#include "tao/ORB_Core.h"
#include "tao/StringSeqC.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::StringSeq foo;
      foo.length (1);
      foo[0] = "Hello World";
      const CORBA::StringSeq & bar = foo;
      ACE_DEBUG ((LM_DEBUG, "String: %C\n", bar[0].in ()));
      CORBA::String_var mystring = bar[0].in ();
    }
  catch (const CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR, "Exception caught: %s\"%s\"\n"
      , ex._name(), ex._rep_id ()));
      return 1;
    }

  return 0;
}
