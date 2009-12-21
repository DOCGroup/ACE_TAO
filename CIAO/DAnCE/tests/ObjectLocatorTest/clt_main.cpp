// $Id$
#include "ace/String_Base.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "RegistrationPath.h"
#include "DummyC.h"

using namespace ObjectLocatorTest;


int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ACE_CString url = "corbaloc:iiop:glasgow.headquarters.eclipsesp.com:12345/";
      url += app_name;
      url += "/";
      url += inst_name;
      url += "/";
      url += port_name;
      ACE_DEBUG ((LM_DEBUG, "URL : %s\n", url.c_str()));

      CORBA::Object_var obj = orb->string_to_object (url.c_str());
      if (CORBA::is_nil (obj))
        {
          ACE_ERROR ((LM_ERROR, "Failed to convert url to object. The test failed.\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "URL -> object : ok.\n"));

      Dummy_var dummy_obj = Dummy::_narrow (obj);
      if (CORBA::is_nil (dummy_obj))
        {
          ACE_ERROR ((LM_ERROR, "Failed to narrow the object to custom type. The test failed.\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "object -> dummy : ok.\n"));

      ACE_DEBUG ((LM_DEBUG, "Dummy object returned : %s.", dummy_obj->sayHello()));
      ACE_DEBUG ((LM_DEBUG, "The test succeeded."));
      return 0;
    }
  catch (CORBA::Exception & e)
    {
      ACE_ERROR ((LM_ERROR, "A CORBA exception \"%s\" (%s) was thrown. The test failed."
                   , e._name(), e._info().c_str()));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "An exception was thrown. The test failed."));
      return 1;
    }
}

