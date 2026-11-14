#include "tao/corba.h"
#include "tao/IORTable/IORTable.h"
#include "simple_i.h"

#include <memory>

namespace
{
  void advertise (CORBA::ORB_ptr orb, CORBA::Object_ptr obj)
    {
      // Advertise the object in the IORTable for clients using the "cobaloc" syntax.
      CORBA::String_var const str = orb->object_to_string (obj);
      CORBA::Object_var const tmp = orb->resolve_initial_references ("IORTable");
      IORTable::Table_var const ior_table = IORTable::Table::_narrow (tmp.in ());
      if (CORBA::is_nil (ior_table.in ()))
        {
          ACE_ERROR ((LM_ERROR, "could not get the IORTable, will not register\n"));
        }
      else
        {
          ior_table->rebind ("Simple", str.in ());
          ACE_DEBUG ((LM_DEBUG, "regisered\n"));
        }
      // We could also advertise the object reference to a naming or trading service.
    }
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the orb.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Get the "RootPOA"
      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      // activate its managers so it can handle incoming requests
      poa->the_POAManager ()->activate ();

      // Create a CORBA object that can be called by a client. Note that
      // this does not create the CORBA object. It creates the object
      // that does the real work behind the CORBA object.
      PortableServer::ServantBase_var simple = new Simple_i (orb.in ());

      // This call creates the CORBA object and returns a reference to
      // it. It uses the "RootPOA" for dispatching its calls. There are
      // a number of ways this can be done different. Refer to "Advanced
      // CORBA Programming with C++" chapter 11 for more information.
      PortableServer::ObjectId_var id =
        poa->activate_object (simple.in ());

      CORBA::Object_var const object_act = poa->id_to_reference (id.in ());

      Simple_var const simple_ref = Simple::_narrow (object_act.in ());

      advertise (orb.in (), simple_ref.in ());

      // give the ORB control for only 30 seconds
      ACE_DEBUG ((LM_DEBUG, "run orb\n"));
      for (int i = 0; i < 1000; ++i)
        {
          ACE_Time_Value run_time (3);
          orb->run (run_time);
          for (int j = 0; j < 10000000; ++j)
            {
            }
        }
      ACE_DEBUG ((LM_DEBUG, "done running\n"));

      poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
