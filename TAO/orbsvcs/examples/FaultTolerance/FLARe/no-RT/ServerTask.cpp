// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerTask.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <fstream>
#include <sstream>
#include <vector>

#include "tao/PortableServer/PortableServer.h"

#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "ServerTask.h"
#include "test_i.h"
#ifdef FLARE_USES_DDS
#  include "ccpp_State.h"
#endif


std::vector<std::string> object_ids;
std::vector<size_t> object_roles;
std::vector<double> object_loads;

ServerTask::ServerTask (ServerOptions & options,
			CORBA::ORB_ptr orb,
			StateSynchronizationAgent_i * agent)
  : options_ (options),
    orb_ (CORBA::ORB::_duplicate (orb)),
    agent_ (agent)
{
}

int
ServerTask::svc (void)
{
  try
    {
      PortableServer::POA_var poa = this->create_rt_poa ();

      if (CORBA::is_nil (poa.in ()))
	      {
	        ACE_ERROR_RETURN ((LM_ERROR,
	                           "(%t) ServerTask::create_rt_poa () "
		                         "did not succeed.\n"),
		                        1);
	      }

      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();

      this->read_object_info (AppOptions::instance ()->object_info_file (),
			                        options_.number_of_servants);

      ReplicationManager_var rm;
      if (options_.use_ns)
	{
	  TAO_Naming_Client naming_client;
	  naming_client.init (orb_.in ());
      
	  CosNaming::NamingContextExt_var ns =
	    CosNaming::NamingContextExt::_narrow (naming_client.get_context ());

	  CORBA::Object_var rm_obj =  ns->resolve_str ("ReplicationManager");

	  if (CORBA::is_nil (rm_obj.in ()))
	    {
	      ACE_ERROR ((LM_ERROR,
			  "ServerTask: Null RM objref from Naming Service\n"));
	    }
          else
	    {
	      rm = ReplicationManager::_narrow (rm_obj.in ());

	      ACE_DEBUG ((LM_TRACE,
			  "ServerTask: RM resolved from Naming Service\n"));
	    }
	}
      else
	{
	  CORBA::Object_var tmp = 
	    this->orb_->string_to_object (options_.rm_ior_file);

	  rm = ReplicationManager::_narrow (tmp.in ());
	}

      if (CORBA::is_nil (rm.in ()))
	{
	  ACE_DEBUG ((LM_ERROR, "ServerTask: could not narrow RM reference."));
	}
      else
	{
          // get reference of the state synchronization agent
          CORBA::Object_var obj =
            orb_->resolve_initial_references ("RootPOA");

          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow (obj.in ());

          obj = root_poa->servant_to_reference (agent_);

          StateSynchronizationAgent_var agent =
            StateSynchronizationAgent::_narrow (obj.in ());

          if (CORBA::is_nil (agent.in ()))
            {
              ACE_DEBUG ((LM_ERROR, "ServerTask: Could not get object "
                          "reference to StateSynchronizationAgent.\n"));
              return 1;
            }

	  // ***************************************************
	  // register replication agent with the replication manager

	  rm->register_state_synchronization_agent (
            AppOptions::instance ()->host_id ().c_str (),
	    AppOptions::instance ()->process_id ().c_str (),
	    agent.in ());

	  // ***************************************************
	  // activate as many servants as required

	  for (int i = 0; i < options_.number_of_servants; ++i)
	    {
	      test_i *servant =
		new test_i (this->orb_.in (),
			    poa.in (),
			    object_ids[i].c_str (),
			    agent.in (),
			    options_.stop);

	      PortableServer::ServantBase_var safe_servant (servant);
	      ACE_UNUSED_ARG (safe_servant);

	      PortableServer::ObjectId_var oid =
		PortableServer::string_to_ObjectId (object_ids[i].c_str ());

	      poa->activate_object_with_id (oid.in (), servant);

	      CORBA::Object_var servant_object =
		poa->id_to_reference (oid.in ());

	      test_var test = test::_narrow (servant_object.in ());

	      std::ostringstream ostr;
	      ostr << object_ids[i] << object_roles[i] << ".ior";

	      int result =
		this->write_ior_to_file (ostr.str ().c_str (),
					 this->orb_.in (),
					 test.in ());

	      rm->register_application (
	          object_ids[i].c_str (), 
	          object_loads[i],
	          AppOptions::instance ()->host_id ().c_str (),
	          AppOptions::instance ()->process_id ().c_str (),
	          object_roles[i],
	          test.in ());

              if (options_.use_corba)
                agent->register_application (object_ids[i].c_str (),
                                             test.in ());
#ifdef FLARE_USES_DDS
              else
                agent_->register_application_with_dds <
                  CORBA::Long,
                  State,
                  StateTypeSupport,
                  StateDataWriter,
                  StateDataReader, 
                  StateSeq> (object_ids[i].c_str (),
                             test.in ());
#endif

	      ACE_DEBUG ((LM_DEBUG,
			  "ServerTask::svc() activated servant %s:%d.\n", 
			  object_ids[i].c_str (), 
			  object_roles[i]));
	      
	      if (result != 0)
		{
		  return result;
		}
	    }
	}

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

PortableServer::POA_ptr
ServerTask::create_rt_poa (void)
{
  try 
    {
      CORBA::Object_var object =
	      this->orb_->resolve_initial_references ("RootPOA");
  
      PortableServer::POA_var root_poa =
	      PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
	      root_poa->the_POAManager ();

      CORBA::PolicyList policies;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	      root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
  
      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	      root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      return root_poa->create_POA ("Servant POA",
                                   poa_manager.in (),
                                   policies);
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "Exception in ServerTask::create_rt_poa (): %s", 
		  ex._info ().c_str ()));

      return PortableServer::POA::_nil ();
    }
}

void
ServerTask::read_object_info (std::string file_name, int count)
{
  std::ifstream input_file (file_name.c_str ());

  std::string oid;
  size_t role;
  double load;

  for (int i = 0; i < count; ++i)
    {
      if (! (input_file >> oid))
	{
	  ACE_DEBUG ((LM_ERROR, "ServerTask could not read from %s.\n", 
		      file_name.c_str ()));
	  return;
	}

      input_file >> role;
      input_file >> load;

      object_ids.push_back (oid);
      object_roles.push_back (role);
      object_loads.push_back (load);
    }
}

int
ServerTask::write_ior_to_file (const char *ior_file,
			                         CORBA::ORB_ptr orb,
			                         CORBA::Object_ptr object)
{
  CORBA::String_var ior =
    orb->object_to_string (object);
  
  FILE *output_file = ACE_OS::fopen (ior_file, "w");
  
  if (output_file == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output file for writing IOR: %s",
                         ior_file),
                        -1);
    }

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);
  
  return 0;
}
