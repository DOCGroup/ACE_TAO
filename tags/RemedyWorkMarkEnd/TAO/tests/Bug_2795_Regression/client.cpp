// $Id$

#include "tao/ORB_Core.h"
#include "tao/StringSeqC.h"

void
StringOutTest (CORBA::String_out so)
{
  CORBA::String_var sv;
  sv = "StringOutTest";
  so = sv._retn ();
}

void
StringInoutTest (char *& so)
{
  CORBA::string_free (so);
  so = CORBA::string_dup ("StringInoutTest");
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::StringSeq ts;
      ts.length (1);
      ts[0] = CORBA::string_dup ("Hello World");

      StringOutTest (ts[0].out());
      ACE_ASSERT (ACE_OS::strcmp (ts[0].in(), "StringOutTest") == 0);

      StringInoutTest (ts[0].inout());
      ACE_ASSERT (ACE_OS::strcmp (ts[0].in(), "StringInoutTest") == 0);

      CORBA::String_var sv = ts[0]._retn ();
      ACE_ASSERT (ACE_OS::strcmp (ts[0].in(), "") == 0);

      ts[0] = CORBA::string_dup ("Hello Again");
    }
  catch (const CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR, "Exception caught: %s\"%s\"\n"
      , ex._name(), ex._rep_id ()));
      return 1;
    }

  return 0;

  return 0;
}
