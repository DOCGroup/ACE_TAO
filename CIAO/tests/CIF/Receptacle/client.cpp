// $Id$

#include "Common/CIF_Common.h"
#include "ReceptacleEC.h"


const char * artifact_name = "Receptacle";

const char * entrypoint_name1 = "Receptacle";
const char * entrypoint_name2 = "Provider";

::Components::Cookie *
connect (Receptacle_ptr rec,
         ::CORBA::Object_ptr facet)
{
  ::Components::Cookie_var ck;
  try
    {
      ck = rec->connect ("rec_foo", facet);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidConnection "
                            "exception during connect\n"));
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: AlreadyConnected "
                            "exception during connect\n"));
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: ExceededConnectionLimit "
                            "exception during connect\n"));
    }
  return ck._retn ();
}

::CORBA::Object_ptr
disconnect (Receptacle_ptr rec,
            ::Components::Cookie * ck)
{
  ::CORBA::Object_var obj;
  try
    {
      obj = rec->disconnect ("rec_foo", ck);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: InvalidConnection "
                            "exception during connect\n"));
    }
  catch (const ::Components::CookieRequired &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: CookieRequired "
                            "exception during connect\n"));
    }
  catch (const ::Components::NoConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: NoConnection "
                            "exception during connect\n"));
    }
  return obj._retn ();
}

int
run_test (Receptacle_ptr rec,
          Provider_ptr ,
          ::CORBA::Object_ptr facet)
{
  int ret = 0;
  try
    {
      ::Components::Cookie_var ck = connect (rec, facet);
      if (ck.in ())
        {
          ACE_DEBUG ((LM_DEBUG, "Receptacle run_test - "
                                "connect test passed !\n"));
        }
      ::CORBA::Object_var obj = disconnect (rec, ck.in ());
      if (!::CORBA::is_nil (obj.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "Receptacle run_test - "
                                "disconnect test passed !\n"));
        }

//       TODO:
//       Exception tests for connect/disconnect
//       #if !defined (CCM_LW)
//           ConnectionDescriptions get_connections (in FeatureName name)
//             raises (InvalidName);
//       #endif
//       #if !defined (CCM_LW)
//           ReceptacleDescriptions get_all_receptacles ();
//       #endif
//       #if !defined (CCM_LW)
//           ReceptacleDescriptions get_named_receptacles (in NameList names)
//             raises (InvalidName);
//       #endif
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Receptacle run_test - "
                            "Error: Unknown "
                            "exception during run_test\n"));
      ++ret;
    }
  return ret;
}

int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  using namespace ::CIAO::Deployment;

  CIF_Common cmd;
  int ret = 0;
  try
    {
      if (cmd.init (argc, argv, artifact_name) != 0)
        return 1;

      ComponentServer_var server1 = cmd.create_componentserver ();

      ComponentServer_var server2 = cmd.create_componentserver ();
      if (CORBA::is_nil (server1.in ()) ||
          CORBA::is_nil (server2.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object references from create_component_server "
                                "operation\n"));
          return 1;
        }

      Container_var cont1 = cmd.create_container (server1.in ());
      Container_var cont2 = cmd.create_container (server2.in ());
      if (CORBA::is_nil (cont1.in ()) ||
          CORBA::is_nil (cont2.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object references from create_container "
                                "operation on server\n"));
          return 1;
        }

      Components::CCMObject_var comp1 = cmd.install_component (cont1.in (),
                                                               entrypoint_name1);
      Components::CCMObject_var comp2 = cmd.install_component (cont2.in (),
                                                               entrypoint_name2);
      if (CORBA::is_nil (comp1.in ()) ||
          CORBA::is_nil (comp2.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Installing components failed.\n"));
          return 1;
        }
      Receptacle_var rec = Receptacle::_narrow (comp1.in ());
      Provider_var prov = Provider::_narrow (comp2.in ());

      if (CORBA::is_nil (rec.in ()) ||
          CORBA::is_nil (prov.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Narrow failed from CCMObjects to Receptacles\n"));
          return 1;
        }

      ::CORBA::Object_var facet;
      try
        {
          facet = prov->provide_facet ("prov_foo");
        }
      catch (const ::Components::InvalidName &)
        {
          ACE_ERROR ((LM_ERROR, "Error: Unexpected InvalidName exception caught "
                                "while providing facet\n"));
        }

      ret = run_test (rec.in (), prov.in (), facet.in ());

      cmd.shutdown (server1.in (), cont1.in (), comp1.in (), false);
      cmd.shutdown (server2.in (), cont2.in (), comp2.in ());
    }
  catch (const ::Components::CreateFailure &)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught CreateFailure exception.\n"));
      return  1;
    }
  catch (const ::Components::RemoveFailure &)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught RemoveFailure exception.\n"));
      return  1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught unknown exception\n"));
      return  1;
    }
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN : "
              " %d error found during tests.\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN : "
              " No error found during tests.\n"));
    }
  return ret;
}
