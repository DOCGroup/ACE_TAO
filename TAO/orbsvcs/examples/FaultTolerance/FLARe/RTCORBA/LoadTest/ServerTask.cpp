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

#include "ServerTask.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tests/RTCORBA/common_args.cpp"
#include "AppOptions.h"
#include "LWFTC.h"
#include "test_i.h"
#include <fstream>
#include <sstream>
#include <vector>

static RTCORBA::Priority default_thread_priority = 0;
static CORBA::ULong dynamic_threads = 0;

std::vector<std::string> object_ids;
std::vector<size_t> object_roles;
std::vector<double> object_loads;

const char *rm_ior_file = "file://rm.ior";

ServerTask::ServerTask (ServerOptions & options,
			CORBA::ORB_ptr orb,
			StateSynchronizationAgent_ptr agent)
  : options_ (options),
    orb_ (CORBA::ORB::_duplicate (orb)),
    agent_ (StateSynchronizationAgent::_duplicate (agent))
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
	  ACE_DEBUG ((LM_ERROR, "(%t) ServerTask::create_rt_poa () "
		                "did not succeed.\n"));
	  return 1;
	}

      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();

      this->read_object_info (AppOptions::instance ()->object_info_file (),
			      options_.number_of_servants);

      CORBA::Object_var tmp = this->orb_->string_to_object (rm_ior_file);
      ReplicationManager_var rm =
        ReplicationManager::_narrow (tmp.in ());

      // ***************************************************
      // register replication agent with the replication manager

      rm->register_state_synchronization_agent (
        AppOptions::instance ()->host_id ().c_str (),
        AppOptions::instance ()->process_id ().c_str (),
	agent_.in ());

      // ***************************************************
      // activate as many servants as required

      for (int i = 0; i < options_.number_of_servants; ++i)
	{
	  test_i *servant =
	    new test_i (this->orb_.in (),
			poa.in (),
			object_ids[i].c_str (),
			agent_.in (),
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
	    this->write_ior_to_file (ostr.str().c_str(),
				     this->orb_.in (),
				     test.in ());

	  rm->register_application (object_ids[i].c_str (), 
				    object_loads[i],
				    AppOptions::instance ()->host_id ().c_str (),
				    AppOptions::instance ()->process_id ().c_str (),
				    object_roles[i],
				    test.in ());


	  agent_->register_application (object_ids[i].c_str (),
					test.in ());

	  ACE_DEBUG ((LM_DEBUG, "activated servant %s:%d.\n", 
		      object_ids[i].c_str (), 
		      object_roles[i]));

	  if (result != 0)
	    return result;
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
ServerTask::create_rt_poa ()
{
  try 
    {
      CORBA::Object_var object =
	this->orb_->resolve_initial_references ("RootPOA");
  
      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager ();

      object =
	this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
	RTCORBA::RTORB::_narrow (object.in ());

      object =
	this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
	RTCORBA::Current::_narrow (object.in ());

      default_thread_priority =	current->the_priority ();

      int result = 0;
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;
      CORBA::PolicyList policies;
      CORBA::Boolean allow_borrowing = 0;

      if (options_.number_of_lanes != 0)
	{
	  get_auto_priority_lanes_and_bands (options_.number_of_lanes,
					     rt_orb.in (),
					     stacksize,
					     options_.static_threads,
					     dynamic_threads,
					     allow_request_buffering,
					     max_buffered_requests,
					     max_request_buffer_size,
					     allow_borrowing,
					     policies,
					     1);
	}
      else if (ACE_OS::strcmp (options_.lanes_file, "empty-file") != 0)
	{
	  result =
	    get_priority_lanes ("server",
				options_.lanes_file,
				rt_orb.in (),
				stacksize,
				options_.static_threads,
				dynamic_threads,
				allow_request_buffering,
				max_buffered_requests,
				max_request_buffer_size,
				allow_borrowing,
				policies,
				1);

	  if (result != 0)
	    return PortableServer::POA::_nil ();

	  result =
	    get_priority_bands ("server",
				options_.bands_file,
				rt_orb.in (),
				policies,
				1);

	  if (result != 0)
	    return PortableServer::POA::_nil ();
	}
      else
	{
	  if (options_.pool_priority == ACE_INT16_MIN)
	    options_.pool_priority =
	      default_thread_priority;
      
	  RTCORBA::ThreadpoolId threadpool_id =
	    rt_orb->create_threadpool (stacksize,
				       options_.static_threads,
				       dynamic_threads,
				       options_.pool_priority,
				       allow_request_buffering,
				       max_buffered_requests,
				       max_request_buffer_size);

	  policies.length (policies.length () + 1);
	  policies[policies.length () - 1] =
	    rt_orb->create_threadpool_policy (threadpool_id);

	  if (ACE_OS::strcmp (options_.bands_file, "empty-file") != 0)
	    {
	      result =
		get_priority_bands ("server",
				    options_.bands_file,
				    rt_orb.in (),
				    policies,
				    1);

	      if (result != 0)
		return PortableServer::POA::_nil ();
	    }
	}
  
      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
					      default_thread_priority);
  
      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
  
      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      return root_poa->create_POA ("RT POA",
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
      input_file >> oid;
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
  
  FILE *output_file =
    ACE_OS::fopen (ior_file,
                   "w");
  
  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      -1);

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);
  
  return 0;
}
