#include "Hello.h"
#include "tao/Strategies/advanced_resource.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Before creating ORB\n"));

      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (),
                             0),
                      1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Activating servant\n"));

      // The servant var owns the implementation while the POA uses it.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id {root_poa->activate_object (hello_impl)};

      CORBA::Object_var const object {root_poa->id_to_reference (id.in ())};

      Test::Hello_var const hello {Test::Hello::_narrow (object.in ())};

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Activating POA Manager\n"));

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Activated POA Manager\n"));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Doing direct call\n"));

      CORBA::String_var const the_string_d {hello->get_string ()};

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
        the_string_d.in ()));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Object to string\n"));

      CORBA::String_var const ior {orb->object_to_string (hello.in ())};

      CORBA::Object_var const tmp {orb->string_to_object (ior.in ())};

      Test::Hello_var const hello_ref {Test::Hello::_narrow (tmp.in ())};

      if (CORBA::is_nil (hello_ref.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.in ()),
                             1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Going to call method\n"));

      CORBA::String_var const the_string {hello_ref->get_string ()};

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
        the_string.in ()));

      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions..
    }
  return 0;
}
