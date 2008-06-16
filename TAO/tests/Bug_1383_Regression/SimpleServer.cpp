// $Id$

#include "tao/corba.h"
#include "tao/IORTable/IORTable.h"
#include "simple_i.h"

void advertise(CORBA::ORB_ptr orb, CORBA::Object_ptr obj)
{
  // advertise our object in the IORTable so that it can be
  // accessed by a client using the "cobaloc" syntax
  CORBA::String_var str = orb->object_to_string(obj);
  CORBA::Object_var tmp = orb->resolve_initial_references("IORTable");
  IORTable::Table_var iorTable = IORTable::Table::_narrow(tmp.in ());
  if (CORBA::is_nil(iorTable.in ()))
    {
      ACE_ERROR ((LM_ERROR, "could not get the IORTable, will not register\n"));
    }
  else
    {
      iorTable->rebind("Simple", str.in());
      ACE_DEBUG ((LM_DEBUG, "regisered\n"));
    }
  // we could also advertise the object reference to a naming and/or
  // trading service
}

int main(int argc, char * argv[])
{
  try
    {
      // Initialize the orb.
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      // Get the "RootPOA"
      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in ());

      // activate its managers so it can handle incoming requests
      poa->the_POAManager()->activate();

      // Create a CORBA object that can be called by a client. Note that
      // this does not create the CORBA object. It creates the object
      // that does the real work behind the CORBA object.
      Simple_i * simple = new Simple_i(orb.in ());

      // This call creates the CORBA object and returns a reference to
      // it. It uses the "RootPOA" for dispatching its calls. There are
      // a number of ways this can be done different. Refer to "Advanced
      // CORBA Programming with C++" chapter 11 for more information.
      PortableServer::ObjectId_var id =
        poa->activate_object (simple);

      CORBA::Object_var object_act = poa->id_to_reference (id.in ());

      Simple_var simpleRef = Simple::_narrow (object_act.in ());

      advertise(orb.in(), simpleRef.in());

      // give the ORB control for only 30 seconds
      ACE_DEBUG ((LM_DEBUG, "run orb\n"));
      for (int i = 0; i < 1000; ++i) {
        ACE_Time_Value runTime(3);
        orb->run(runTime);
        for (int i = 0; i < 10000000; ++i) {}

      }
      ACE_DEBUG ((LM_DEBUG, "done running\n"));

      poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
