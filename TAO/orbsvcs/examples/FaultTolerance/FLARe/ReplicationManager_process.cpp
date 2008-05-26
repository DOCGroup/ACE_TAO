// cvs-id    : $Id$

#include "ace/Get_Opt.h"
#include "LWFTC.h"
#include "Agent.h"
#include "ReplicationManager.h"
// #include "orbsvcs/CosNamingC.h"
#include "RMOptions.h"

char *ior_output_file = "rm.ior";

int
main (int argc, char *argv[])
{
  int status = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (RMOptions::instance()->parse_args(argc,argv) != true)
      {  
        ACE_DEBUG((LM_DEBUG,"Replication Manager options are incorrect.\n"));
        return -1;
      }
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /*

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      ReplicationManager_i rm_i (orb.in ());
      // rm_i.hertz (5);
      // rm_i.start ();

      ReplicationManager_var rm = rm_i._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      */

      ReplicationManager_i *rm_i = 0;
      ACE_NEW_RETURN (rm_i, 
                      ReplicationManager_i (orb.in (), 
                                            RMOptions::instance()->hertz(), 
                                            RMOptions::instance()->proactive()),
                      1);
      PortableServer::ServantBase_var owner_transfer (rm_i);
      PortableServer::ObjectId_var id =
        poa->activate_object (rm_i);
      CORBA::Object_var rm_object = poa->id_to_reference (id.in ());
      ReplicationManager_var rm = ReplicationManager::_narrow (rm_object.in ());

      /*

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("ReplicationManager");
      name[0].kind = CORBA::string_dup ("");

      // Register with the name server
      naming_context->bind (name, rm.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      */

      CORBA::String_var ior =
        orb->object_to_string (rm.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) RM - event loop finished\n"));

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return status;
}
