//$Id$
#include "ace/Get_Opt.h"
#include "Hello.h"
#include "tao/Strategies/advanced_resource.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Before creating ORB\n"));

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (),
                             0),
                      1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Activating servant\n"));

      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello =
        Test::Hello::_narrow (object.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Activating POA Manager\n"));

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Activated POA Manager\n"));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Doing direct call\n"));

      CORBA::String_var the_string_d =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
        the_string_d.in ()));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Object to string\n"));

     CORBA::String_var ior =
       orb->object_to_string (hello.in ());

      CORBA::Object_var tmp =
        orb->string_to_object (ior.in ());

      Test::Hello_var hello_ref =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello_ref.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.in ()),
                             1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Going to call method\n"));

      CORBA::String_var the_string =
        hello_ref->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
        the_string.in ()));

      orb->destroy ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }
  return 0;
}
