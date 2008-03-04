
#include "ace/String_Base.h"
#include "RegistrationPath.h"
#include "DummyC.h"

using namespace ObjectLocatorTest;


int main (int argc, char * argv[])
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
      ACE_DEBUG((LM_DEBUG, "[%M] URL : %s\n", url.c_str()));

      CORBA::Object_var obj = orb->string_to_object (url.c_str());
      if (CORBA::is_nil (obj))
        {
          ACE_ERROR((LM_ERROR, "[%M] Failed to convert url to object. The test failed.\n"));
          return 1;
        }
      ACE_DEBUG((LM_DEBUG, "[%M] URL -> object : ok.\n"));

      Dummy_var dummy_obj = Dummy::_narrow (obj);
      if (CORBA::is_nil (dummy_obj))
        {
          ACE_ERROR((LM_ERROR, "[%M] Failed to narrow the object to custom type. The test failed.\n"));
          return 1;
        }
      ACE_DEBUG((LM_DEBUG, "[%M] object -> dummy : ok.\n"));

      ACE_DEBUG((LM_DEBUG, "[%M] Dummy object returned : %s.", dummy_obj->sayHello()));
      ACE_DEBUG((LM_DEBUG, "[%M] The test succeeded."));
      return 0;
    }
  catch (CORBA::Exception & e)
    {
      ACE_ERROR((LM_ERROR, "[%M] A CORBA exception \"%s\" (%s) was thrown. The test failed."
                   , e._name(), e._info().c_str()));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR((LM_ERROR, "[%M] An exception was thrown. The test failed."));
      return 1;
    }
}

