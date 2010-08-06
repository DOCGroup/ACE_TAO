// $Id$

#include "plan.h"

#include "NodeManager/NodeManager_Impl.h"

#include "tao/IORTable/IORTable.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB so that CORBA::Any will work.
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        "");
                                        
  try
  {
    // Get reference to Root POA.
    CORBA::Object_var obj =
      orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (obj.in ());

    // Activate POA manager.
    PortableServer::POAManager_var mgr = poa->the_POAManager ();

    mgr->activate ();

    int homes_table_size =
      sizeof (homes_table) / sizeof (HomeAttributes);

    CIAO::HOMECREATOR_FUNCPTR_MAP home_creator_fptr_map;
    CIAO::HOMESERVANTCREATOR_FUNCPTR_MAP homesvnt_creator_fptr_map;
    CIAO::Static_Config_EntryPoints_Maps static_config_entrypoints_maps;
    
    static_config_entrypoints_maps.home_creator_funcptr_map_ =
      &home_creator_fptr_map;
      
    static_config_entrypoints_maps.home_servant_creator_funcptr_map_ =
      &homesvnt_creator_fptr_map;

    for (int i = 0; i < homes_table_size; ++i)
      {
        home_creator_fptr_map.bind (homes_table[i].executor_entrypt_,
                                    homes_table[i].executor_fptr_);

        homesvnt_creator_fptr_map.bind (homes_table[i].servant_entrypt_,
                                        homes_table[i].servant_fptr_);
      }

    CIAO::Static_NodeManager_Impl *static_node_manager_impl;
    
    // Create and install the CIAO Daemon servant.

    ACE_DEBUG ((LM_DEBUG, "creating static_node_manager\n"));

    static_node_manager_impl =
      new CIAO::Static_NodeManager_Impl ("NodeManager",
                                         orb.in (),
                                         poa.in (),
                                         "", //exe location
                                         "", //exe options
                                         0, //spawn delay
                                         &static_config_entrypoints_maps);

    static_node_manager_impl->init ();

    CORBA::Object_var table_object =
      orb->resolve_initial_references ("IORTable");

    IORTable::Table_var adapter =
      IORTable::Table::_narrow (table_object.in ());

    if (CORBA::is_nil (adapter.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR, "Nil IORTable\n"), -1);
      }

    CIAO::NodeManagerDaemon_var manager =
      static_node_manager_impl->_this ();

    CORBA::String_var str =
      orb->object_to_string (manager.in ());

    adapter->bind ("NodeManager", str.in ());

    // Run the main event loop for the ORB.
    orb->run ();

    poa->destroy (true, true);

    orb->destroy ();
  }
  catch (CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Caught CORBA Exception: ");
    return -1;
  }

  return 0;
}
