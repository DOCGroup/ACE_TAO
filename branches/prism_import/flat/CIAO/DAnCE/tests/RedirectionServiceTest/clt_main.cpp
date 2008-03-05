
#include "ace/String_Base.h"
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include "RegistrationPath.h"
#include "DummyC.h"
#include "ace/OS.h"

using namespace Test;

CORBA::ORB_var orb;
CORBA::Object_var naming_obj;

void inspect ()
  {
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var naming = CosNaming::NamingContext::_narrow(obj.in());
    CosNaming::BindingList_var bl;
    CosNaming::BindingIterator_var bi;
    //naming->list(10L, bl.out(), bi.out());
    CosNaming::Name name(1);
    name.length(2);
    name[0].id = CORBA::string_dup("Host1");//CORBA::string_dup("CcmDance1Test");
    name[1].id = CORBA::string_dup ("CcmDance2_1@Host1");
    name[1].kind = CORBA::string_dup ("DeploymentPlan");
    //name[2].id = CORBA::string_dup("ASSEMBLYANDDEPLOYMENT");
    obj = naming->resolve(name);
    ACE_DEBUG((LM_DEBUG, "[%M] create_external_connections - After resolve\n"));
    CosNaming::NamingContext_var CcmDance1Test = CosNaming::NamingContext::_narrow(obj.in());
    CcmDance1Test->list(10L, bl.out(), bi.out());
    bool exit = false;
    while (!exit)
    {
        ACE_DEBUG((LM_DEBUG, "[%M] create_external_connections - naming size is %u\n", bl->length()));
        for ( size_t i = 0; i< bl->length(); i++ )
        {
            for ( size_t j = 0; j < (*bl)[i].binding_name.length(); j++ )
            {
                ACE_DEBUG((LM_DEBUG,  "create_external_connections - Binding name %s, binding kind %s, binding type %i\n"
                                      , (*bl)[i].binding_name[j].id.in()
                                      , (*bl)[i].binding_name[j].kind.in()
                                      , (*bl)[i].binding_type));
            }
        }
        if(!bi->next_n(10L, bl.out()))
        {
            exit = true;
        }
    }

  }
bool test (const ACE_CString& url)
{
  bool res = false;
  try
    {
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

      ACE_DEBUG((LM_DEBUG, "[%M] Dummy object returned : %s.\n", dummy_obj->sayHello()));
      ACE_DEBUG((LM_DEBUG, "[%M] The test succeeded.\n"));
      res = true;
    }
  catch (CORBA::Exception & e)
    {
      ACE_ERROR((LM_ERROR, "[%M] A CORBA exception \"%s\" (%s) was thrown. The test failed."
                   , e._name(), e._info().c_str()));
    }
  catch (...)
    {
      ACE_ERROR((LM_ERROR, "[%M] An exception was thrown. The test failed.\n"));
    }

  return res;
};


int main (int argc, char * argv[])
{
  orb = CORBA::ORB_init (argc, argv);

  //naming_obj = orb->resolve_initial_references("NameService");
  //glasgow.headquarters.eclipsesp.com:12345
  ACE_CString url = "corbaname::glasgow:12345#" + node_name + "/" + app_name + ".DeploymentPlan" + "/" + inst_name + "/" + port_name + ".Port";
  test (url);
  url = "corbaname::glasgow:12345#" + node_name + "/" + app_name + ".DeploymentPlan" + "/" + inst_name + ".Component";
  test (url);
  //node_name + "/" +                                               %14%01%0f%00NUP%00%00%00%17%01%00%00%00%01%00%00%00RootPOA%00MyNode%00
  url = "corbaloc:iiop:1.2@glasgow.headquarters.eclipsesp.com:12345/%14%01%0f%00NUP%00%00%00%17%01%00%00%00%01%00%00%00RootPOA%00MyNode%00" + app_name + "/" + inst_name + "/" + port_name;
  test (url);
  url = "corbaloc:iiop:1.2@glasgow.headquarters.eclipsesp.com:12345/%14%01%0f%00NUP%00%00%00%17%01%00%00%00%01%00%00%00RootPOA%00MyNode%00" + app_name + "/" + inst_name;
  test (url);

  return 0;
}
