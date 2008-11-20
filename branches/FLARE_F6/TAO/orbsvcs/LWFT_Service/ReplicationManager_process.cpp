// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "orbsvcs/orbsvcs/LWFT/ForwardingAgent.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManager.h"

#include "RMOptions.h"

char *ior_output_file = "rm.ior";

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      if (! RMOptions::instance ()->parse_args (argc, argv))
        {  
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Replication Manager options "
                             "are incorrect.\n"),
                            -1);
        }
        
      ReplicationManager_i *rm_i = 0;
      ACE_NEW_RETURN (rm_i, 
                      ReplicationManager_i (orb.in (), 
                                            RMOptions::instance()->hertz(), 
                                            RMOptions::instance()->proactive(),
					    RMOptions::instance()->static_mode ()),
					    1);
      PortableServer::ServantBase_var owner_transfer (rm_i);
      PortableServer::ObjectId_var id =
        poa->activate_object (rm_i);
      CORBA::Object_var rm_object = poa->id_to_reference (id.in ());
      ReplicationManager_var rm = ReplicationManager::_narrow (rm_object.in ());

      CORBA::String_var ior =
        orb->object_to_string (rm.in ());
      
//      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));
      
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          
          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot open output file "
                                 "for writing IOR: %s",
                                 ior_output_file),
                                1);
            }
            
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }
        
      if (RMOptions::instance ()->use_naming_service ())
        {
          TAO_Naming_Client naming_client;
          naming_client.init (orb.in ());
            
          CosNaming::Name rm_name;
          rm_name.length (1UL);
          rm_name[0UL].id = "ReplicationManager";
          
	        try
	          {
	            naming_client->bind (rm_name, rm.in ());
	          }
	        catch (CosNaming::NamingContext::AlreadyBound &)
	          {
	            naming_client->rebind (rm_name, rm.in ());
	          }

          ACE_DEBUG ((LM_INFO,
                      "ReplicationManager registered with Naming Service\n"));
        }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) RM - event loop finished\n"));

      poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("RM Process: caught exception:");
      return -1;
    }

  return 0;
}
