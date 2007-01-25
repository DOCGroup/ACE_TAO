// $Id$

# include "Collocation_Tester.h"

ACE_RCSID (tests, client, "$Id$")

int main (int argc, char *argv[])
{

  try
    {
      // ORB Initialization
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TAO");

      CORBA::Object_var object;

      // Get The IOR from a file
      object = orb->string_to_object ("file://s.ior");

      if (CORBA::is_nil (object.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "Invalid IOR file\n"));
          return 1;
        }

      Collocation_Tester tester (object.in ());
      tester.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CORBA Exception Raised");
      return 1;
    }

  return 0;
}
