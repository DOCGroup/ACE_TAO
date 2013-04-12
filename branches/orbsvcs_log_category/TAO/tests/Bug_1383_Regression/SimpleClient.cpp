// $Id$

#include "tao/corba.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread.h"
#include "simpleC.h"
#include "Callee_i.h"
#include "tao/IORManipulation/IORManip_Loader.h"

CORBA::ORB_var _orb;

ACE_THR_FUNC_RETURN RunFunc(void *)
{
  ACE_DEBUG ((LM_DEBUG, "running the _orb\n"));
  _orb->run();
  ACE_DEBUG ((LM_DEBUG, "done running _orb\n"));
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // initialize the ORB
      _orb = CORBA::ORB_init (argc, argv);

      // Get the "RootPOA"
      CORBA::Object_var obj = _orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in ());

      // activate its managers so it can handle incoming requests
      poa->the_POAManager()->activate();

      // get a thread going to handle incoming requests
      ACE_Thread::spawn(RunFunc);

      // we could also use the c_orbaloc syntax to get a reference to the
      // object. This will cause the _orb to talk directly to the other
      // _orb with no name service or common file involved.
      obj = _orb->string_to_object("c_orbaloc::localhost:9999/Simple");

      if (CORBA::is_nil(obj.in ()))
        {
          ACE_ERROR ((LM_ERROR, "could not get reference\n"));
          return 1;
        }

      // narrow the reference to the particular type we wish to deal with
      Simple_var simple = Simple::_narrow(obj.in ());
      if (CORBA::is_nil(simple.in ()))
        {
          ACE_ERROR ((LM_ERROR, "could not get reference\n"));
          return 1;
        }

      // create an object reference that has a bogus value for the first
      // profile
      obj = _orb->resolve_initial_references("IORManipulation");
      TAO_IOP::TAO_IOR_Manipulation_var iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow(obj.in());

      // Status: The following scenarios appear to work:
      // - first profile invalid host name, second profile valid.
      // - first profile invalid ip number, second profiel valid.
      // - first profiel invalid port number, second profile valid.
      //
      // The following causes a core dump of the server:
      // - first and second invalid
      //
      // Note that he iormanip is being used since it is not practical
      // to have a test case that depends on a VPN being present.
      //
      CORBA::Object_var name1 =
        _orb->string_to_object ("c_orbaloc:iiop:10.0.2.3:6060/xyz");
      CORBA::Object_var name2 =
        _orb->string_to_object ("c_orbaloc:iiop:daisnot:7070/xyz");
      CORBA::String_var name1_ior = _orb->object_to_string(name1.in());
      CORBA::String_var name2_ior = _orb->object_to_string(name2.in());

      // create a callback object
      Callee_i * callee_i = new Callee_i;
      // get the CORBA reference
      PortableServer::ObjectId_var id =
        poa->activate_object (callee_i);

      CORBA::Object_var object_act = poa->id_to_reference (id.in ());

      Callee_var callee = Callee::_narrow (object_act.in ());

      if (CORBA::is_nil(callee.in ()))
        {
          ACE_ERROR ((LM_ERROR, "could not get callback object\n"));
          return 1;
        }
      else
        {
          CORBA::String_var str = _orb->object_to_string(callee.in ());
          FILE *output_file= ACE_OS::fopen ("ior", "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                              "Cannot open output file for writing IOR: ior\n"),
                              1);
          ACE_OS::fprintf (output_file, "%s", str.in ());
          ACE_OS::fclose (output_file);
        }

      // create a reference with two profiles with the first on being
      // bogus. If things work as they should the callback should be
      // succesful with the ORB trying the second profile.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length (2);
      iors [0] = name1;
      iors [1] = name2;

      CORBA::Object_var merged = iorm->merge_iors(iors);
        Callee_var doubleCallee = Callee::_unchecked_narrow(merged.in());

      ACE_DEBUG ((LM_DEBUG, "Profile count is %d\n",
                  iorm->get_profile_count(merged.in())));

      simple->registerCallee(doubleCallee.in ());

      ACE_OS::sleep(120);

      ACE_DEBUG ((LM_DEBUG, "done sleeping\n"));

      _orb->shutdown(1);
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("Caught unexpected CORBA exception :");
    }
  return 0;
}
