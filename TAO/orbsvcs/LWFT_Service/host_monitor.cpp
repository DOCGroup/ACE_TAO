// -*- C++ -*-
// $Id$

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>

#include "orbsvcs/orbsvcs/LWFT/HostMonitorImpl.h"
#include "orbsvcs/orbsvcs/LWFT/Monitor_Thread.h"
#include "orbsvcs/orbsvcs/LWFT/HMOptions.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Client_Init.h"

int main (int argc, char* argv[])
{
  try 
    {
      /// First initialize the ORB, that will remove some arguments...
      LWFT_Client_Init ci;
      CORBA::ORB_var orb = ci.init (argc, argv);

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
      
      HMOptions *opts = HMOptions::instance ();

      if (! opts->parse_args (argc, argv))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Host Monitor options "
                             "are incorrect.\n"),
                            -1);
        }
        
      Monitor_Thread monitor_thread;
      
      /// Initilize the timedate object on heap.
      HostMonitorImpl * host_monitor =
        new HostMonitorImpl (orb, &monitor_thread);
      PortableServer::ServantBase_var safe_host (host_monitor);
      ACE_UNUSED_ARG (safe_host);
      
      HostMonitor_var hmvar = host_monitor->_this ();
      CORBA::String_var hmstr = orb->object_to_string (hmvar.in ());

      /// Copy the IOR in the IORFILE.
      std::string ior_file (opts->HM_ior_file ());
      std::ofstream outfile (ior_file.c_str ());
      outfile << hmstr;
      outfile.close ();

      monitor_thread.activate ();
      orb->run ();

      /// Destroy the POA, waiting until the destruction terminates
      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex, "A CORBA exception was raised:");
      return -1;
    }
  
  return 0;
}
