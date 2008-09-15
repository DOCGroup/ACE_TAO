// $Id$


#include "ace/Task.h"
#include "ace/Auto_Event.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

bool
test_string_to_object (CORBA::ORB_ptr orb, const char* ior, CORBA::ULong minor)
{
  bool succeed = false;
  try
    {
      // Get the object reference with the IOR
      CORBA::Object_var object = orb->string_to_object (ior);
    }
  catch (const CORBA::BAD_PARAM& ex)
    {
      if ((ex.minor() & 0xFFFU) == minor)
        {
          succeed = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, test_string_to_object for <%s> didn't result in minor code %d\n",
                ior,
                minor));
  }

  return succeed;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int retval = 0;

  try
    {
      CORBA::ORB_var orb =
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
                  argv[0]));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      retval = -1;
    }

  return retval;
}
