#include <iostream>
#include <fstream>
#include <string>

#include "HostMonitorImpl.h"
#include "Monitor_Thread.h"
#include "HMOptions.h"
#include <iostream>
#include <iterator>
#include <algorithm>

int main (int argc, char* argv[])
{
  try {

    HMOptions::instance()->parse_args (argc, argv);
    //std::copy (argv, argv + argc, 
    //           std::ostream_iterator <std::string> (std::cout, "\n"));
    /// First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv, "ORB");
    //std::copy (argv, argv + argc, 
    //           std::ostream_iterator <std::string> (std::cout, "\n"));

    /// Initilize RootPOA.
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");

    /// Create the POA object reference to type POA.
    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (poa_object.in ());

    /// Activate the POA manager.
    PortableServer::POAManager_var poa_manager =
      poa->the_POAManager ();
    poa_manager->activate ();


    Monitor_Thread monitor_thread;
    /// Initilize the timedate object on heap.
    HostMonitorImpl * host_monitor = new HostMonitorImpl (orb, &monitor_thread);
    PortableServer::ServantBase_var safe_host (host_monitor);
    ACE_UNUSED_ARG (safe_host);
    
    HostMonitor_var hmvar = host_monitor->_this();
    CORBA::String_var hmstr = orb->object_to_string (hmvar.in());

    /// Copy the IOR in the IORFILE.
    std::ofstream outfile(HMOptions::instance()->HM_ior_file().c_str());
    outfile << hmstr;
    outfile.close ();

    monitor_thread.activate ();
    orb->run();

    /// Destroy the POA, waiting until the destruction terminates
    poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &ex) {
    //std::cerr << "A CORBA exception was raised!" << std::endl;
    ACE_PRINT_EXCEPTION (ex, "A CORBA exception was raised:");
    return -1;
  }
  return 0;
}
