
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

bool
test_string_to_object (CORBA::ORB_ptr orb, char const *ior, CORBA::ULong minor)
{
  try
    {
      // Get the object reference with the IOR
      CORBA::Object_var const object = orb->string_to_object (ior);
    }
  catch (CORBA::BAD_PARAM const &ex)
    {
      if ((ex.minor () & 0xFFFU) == minor)
        {
          return true;
        }
    }
  catch (CORBA::Exception const &)
    {
    }

  ACE_ERROR ((LM_ERROR,
              "(%t) ERROR, test_string_to_object for <%C> didn't result in minor code %u\n",
              ior,
              minor));
  return false;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval {};

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // string_to_object conversion failed due to bad scheme name
      if (!test_string_to_object (orb.in (), "", 7))
        ++retval;
      // string_to_object conversion failed due to bad address
      if (!test_string_to_object (orb.in (), "", 8))
        ++retval;
      // string_to_object conversion failed due to bad bad schema specific part
      if (!test_string_to_object (orb.in (), "", 9))
        ++retval;
      // string_to_object conversion failed due to non specific reason
      if (!test_string_to_object (orb.in (), "", 10))
        ++retval;

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv [0]));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      retval = -1;
    }

  return retval;
}
