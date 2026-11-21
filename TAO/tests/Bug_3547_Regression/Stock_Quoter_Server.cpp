// Stock Quoter includes
#include "Stock_Quoter_i.h"

// UPDTest
#include "UDPTest_i.h"

// Advanced Resource Factory include
#include "tao/Strategies/advanced_resource.h"

// IORTable include
#include "tao/IORTable/IORTable.h"
#include "ace/streams.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    // Get the POA from the ORB.
    CORBA::Object_var const poa_obj = orb->resolve_initial_references ("RootPOA");

    // Narrow to the POA interface
    PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_obj.in ());

    // Create a Stock Quoter implementation object
    Stock_Quoter_i *stock_quoter = new Stock_Quoter_i (orb.in ());
    // The servant wrapper retains ownership until after RootPOA is destroyed.
    PortableServer::ServantBase_var stock_quoter_owner (stock_quoter);

    UDPTest_i *udp = new UDPTest_i ();
    // The servant wrapper retains ownership until after RootPOA is destroyed.
    PortableServer::ServantBase_var udp_owner (udp);

    // Activate the Stock Quoter in the RootPOA.
    PortableServer::ObjectId_var const oid = root_poa->activate_object (stock_quoter);
    CORBA::Object_var const sq_obj = root_poa->id_to_reference (oid.in ());

    PortableServer::ObjectId_var const oid1 = root_poa->activate_object (udp);
    CORBA::Object_var const udp_obj1 = root_poa->id_to_reference (oid1.in ());

    CORBA::String_var const ior =
        orb->object_to_string (udp_obj1.in ());

    ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

    // Also make the Stock Quoter object available via corbaloc
    // ObjectURL by binding it to a simple ObjectKey in the IORTable.
    CORBA::Object_var const table_obj = orb->resolve_initial_references ("IORTable");
    IORTable::Table_var const table = IORTable::Table::_narrow (table_obj.in ());
    CORBA::String_var const quoter_ior_string = orb->object_to_string (sq_obj.in ());
    CORBA::String_var const udp_ior_string = orb->object_to_string (udp_obj1.in ());
    table->bind ("UDPTest", udp_ior_string.in ());
    table->bind ("MyStockQuoter", quoter_ior_string.in ());

    // Activate the POA Manager
    PortableServer::POAManager_var const poa_mgr = root_poa->the_POAManager ();
    poa_mgr->activate ();

    cout << "Server ready" << endl;
    orb->run ();

    root_poa->destroy (true, true);
    orb->destroy ();

    if (!udp->received_send_.load (std::memory_order_acquire))
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Not received send\n"), 1);
    }
  }
  catch (CORBA::Exception const &e)
  {
    e._tao_print_exception ("Exception caught:");
    return 1;
  }

  return 0;
}
