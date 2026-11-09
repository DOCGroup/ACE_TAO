#include "tao/corba.h"

// Attempts to resolve the NameService.
// Returns:
//    0  if the NameService was resolved
//    1  if the NameService could not be resolved
//    2  if something else went wrong
//
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      try
        {
          CORBA::Object_var const naming =
            orb->resolve_initial_references ("NameService");
          if (CORBA::is_nil (naming.in ()))
            return 1;
        }
      catch (CORBA::Exception const &)
        {
          return 1;
        }
    }
  catch (CORBA::Exception const &)
    {
      return 2;
    }

  return 0;
}
