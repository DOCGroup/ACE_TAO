// $Id$

#include "testC.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"

const char *ior =
  "corbaloc:iiop:192.3.47/10007/RandomObject";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      // First connection happens here..
      test::_narrow (tmp.in ());
    }
  catch (const CORBA::TRANSIENT& )
    {
      // This exception is correct.
      ACE_DEBUG ((LM_DEBUG,
                  "_narrow() to non-existent object failed correctly.  Test succeeded.\n"));
      return 0;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
    }
  // Should not reach here.
  ACE_ERROR_RETURN ((LM_ERROR,
                     "_narrow() succeeded to non-existent object when it should not have...\n"),
                     -1);
}
