// $Id$
#include "ace/String_Base.h"
#include <orbsvcs/CosNamingC.h>
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
//            ACE_ERROR ((LM_ERROR, "Failed to resolve object. The test failed.\n"));
//            return 1;
//        }
//        ACE_DEBUG ((LM_DEBUG, "resolve object : ok.\n"));
//
//        Dummy_var res_dummy_obj = Dummy::_narrow(res_obj);
//        if ( CORBA::is_nil(res_dummy_obj) )
//        {
//            ACE_ERROR ((LM_ERROR, "Failed to narrow the resolved object to custom type. The test failed.\n"));
//            return 1;
//        }
//        ACE_DEBUG ((LM_DEBUG, "resolved object -> dummy : ok.\n"));
//
//        ACE_DEBUG ((LM_DEBUG, "Dummy object returned : %s.", res_dummy_obj->sayHello()));
//        ACE_DEBUG ((LM_DEBUG, "The test succeeded."));
      //glasgow.headquarters.eclipsesp.com:12345
      ACE_CString url = "corbaname:rir:#app1.DeploymentPlan/Dummy";
//        url += app_name;
//        url += "/";
//        url += inst_name;
//        url += "/";
//        url += port_name;
      ACE_DEBUG ((LM_DEBUG, "URL : %C\n", url.c_str()));

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

      ACE_DEBUG ((LM_DEBUG, "Dummy object returned : %C.", dummy_obj->sayHello()));
      ACE_DEBUG ((LM_DEBUG, "The test succeeded."));
      return 0;
    }
  catch (const CORBA::Exception & e)
    {
      ACE_ERROR ((LM_ERROR, "A CORBA exception \"%C\" (%C) was thrown. The test failed."
                   , e._name(), e._info().c_str()));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "An exception was thrown. The test failed."));
      return 1;
    }
}

