// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

ACE_RCSID(Bug_2869_Regression, client, "$Id$")

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var tmp =
        orb->string_to_object("iiop://1\"/2$/$3211:2500/EndPoint");
    }
  catch (const CORBA::INV_OBJREF&)
    {
      ACE_DEBUG ((LM_DEBUG, "Test succeeded\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught invalid exception\n"));
      return 1;
    }

  // Write dummy file to trigger the test framework we are ready.
  FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
  if (output_file == 0)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "Cannot open output file for writing IOR: %s\n",
                         ior_output_file),
                         1);
  ACE_OS::fprintf (output_file, "dummy");
  ACE_OS::fclose (output_file);

  return 0;
}
