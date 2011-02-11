// $Id$

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
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get the POA from the ORB.
    CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");

    // Narrow to the POA interface
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_obj.in ());

    // Create a Stock Quoter implementation object
    Stock_Quoter_i* stock_quoter = new Stock_Quoter_i (orb.in ());

    UDPTest_i* UDP = new UDPTest_i ();

    // Activate the Stock Quoter in the RootPOA.
    PortableServer::ObjectId_var oid = root_poa->activate_object (stock_quoter);
    CORBA::Object_var sq_obj = root_poa->id_to_reference (oid.in());

    PortableServer::ObjectId_var oid1 = root_poa->activate_object (UDP);
    CORBA::Object_var udp_obj1 = root_poa->id_to_reference (oid1.in());

    CORBA::String_var ior =
        orb->object_to_string (udp_obj1.in ());

    ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

    // Also make the Stock Quoter object available via corbaloc
    // ObjectURL by binding it to a simple ObjectKey in the IORTable.
    CORBA::Object_var table_obj = orb->resolve_initial_references ("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow (table_obj.in());
    CORBA::String_var quoter_ior_string = orb->object_to_string (sq_obj.in());
    CORBA::String_var UDP_ior_string = orb->object_to_string (udp_obj1.in());
    table->bind ("UDPTest", UDP_ior_string.in());
    table->bind ("MyStockQuoter", quoter_ior_string.in());

    // Activate the POA Manager
    PortableServer::POAManager_var poa_mgr = root_poa->the_POAManager ();
    poa_mgr->activate ();

    cout << "Server ready" << endl;
    orb->run ();

    if (!UDP->received_send_)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Not received send\n"), 1);
    }
  }
  catch (CORBA::Exception& e)
  {
    e._tao_print_exception ("Exception caught:");
    return 1;
  }

  return 0;
}
