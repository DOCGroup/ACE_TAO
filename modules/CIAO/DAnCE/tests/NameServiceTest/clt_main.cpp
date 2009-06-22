// $Id$
#include "ace/String_Base.h"
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include "DAnCE/Logger/Log_Macros.h"
#include "RegistrationPath.h"
#include "DummyC.h"

using namespace NameServiceTest;


int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
//        CosNaming::NamingContext_var naming = CosNaming::NamingContext::_narrow(naming_obj.in());
//        CosNaming::Name name(1);
//
//        name.length(1);
//
//        name[0].id = CORBA::string_dup("Dummy");
//        name[0].kind = CORBA::string_dup("");
//
//        CORBA::Object_ptr res_obj = naming->resolve(name);
//        if ( CORBA::is_nil(res_obj) )
//        {
//            DANCE_ERROR((LM_ERROR, "[%M] Failed to resolve object. The test failed.\n"));
//            return 1;
//        }
//        DANCE_DEBUG((LM_DEBUG, "[%M] resolve object : ok.\n"));
//
//        Dummy_var res_dummy_obj = Dummy::_narrow(res_obj);
//        if ( CORBA::is_nil(res_dummy_obj) )
//        {
//            DANCE_ERROR((LM_ERROR, "[%M] Failed to narrow the resolved object to custom type. The test failed.\n"));
//            return 1;
//        }
//        DANCE_DEBUG((LM_DEBUG, "[%M] resolved object -> dummy : ok.\n"));
//
//        DANCE_DEBUG((LM_DEBUG, "[%M] Dummy object returned : %s.", res_dummy_obj->sayHello()));
//        DANCE_DEBUG((LM_DEBUG, "[%M] The test succeeded."));
      //glasgow.headquarters.eclipsesp.com:12345
      ACE_CString url = "corbaname:rir:#app1.DeploymentPlan/Dummy";
//        url += app_name;
//        url += "/";
//        url += inst_name;
//        url += "/";
//        url += port_name;
      DANCE_DEBUG((LM_DEBUG, "[%M] URL : %s\n", url.c_str()));

      CORBA::Object_var obj = orb->string_to_object (url.c_str());
      if (CORBA::is_nil (obj))
        {
          DANCE_ERROR((LM_ERROR, "[%M] Failed to convert url to object. The test failed.\n"));
          return 1;
        }
      DANCE_DEBUG((LM_DEBUG, "[%M] URL -> object : ok.\n"));

      Dummy_var dummy_obj = Dummy::_narrow (obj);
      if (CORBA::is_nil (dummy_obj))
        {
          DANCE_ERROR((LM_ERROR, "[%M] Failed to narrow the object to custom type. The test failed.\n"));
          return 1;
        }
      DANCE_DEBUG((LM_DEBUG, "[%M] object -> dummy : ok.\n"));

      DANCE_DEBUG((LM_DEBUG, "[%M] Dummy object returned : %C.", dummy_obj->sayHello()));
      DANCE_DEBUG((LM_DEBUG, "[%M] The test succeeded."));
      return 0;
    }
  catch (const CORBA::Exception & e)
    {
      DANCE_ERROR((LM_ERROR, "[%M] A CORBA exception \"%s\" (%s) was thrown. The test failed."
                   , e._name(), e._info().c_str()));
      return 1;
    }
  catch (...)
    {
      DANCE_ERROR((LM_ERROR, "[%M] An exception was thrown. The test failed."));
      return 1;
    }
}

