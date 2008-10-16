// -*- C++ -*-

//=============================================================================
/**
 *  @file    StateSynchronizationAgent_i.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "StateSynchronizationAgent_i.h"
#include "CorbaStateUpdate.h"
#include "DDSStateUpdate.h"
#include "DDSStateReader_T.h"
#include "StateDcps_impl.h"

const char * DOMAIN_ID = "111";

StateSynchronizationAgent_i::StateSynchronizationAgent_i (
    CORBA::ORB_ptr orb,
    const std::string & host_id,
    const std::string & process_id)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    host_id_ (host_id),
    process_id_ (process_id),
    domain_participant_ (DDS::DomainParticipant::_nil ()),
    publisher_ (DDS::Publisher::_nil ()),
    subscriber_ (DDS::Subscriber::_nil ()),
    use_corba_ (true)
{
  if (!use_corba_)
    {
      if (this->create_participant ())
	{
	  if (this->create_publisher ())
	    {
	      if (!this->create_subscriber ())
		std::cerr << "SSA could not create DDS subscriber" 
			  << std::endl;
	    }
	  else
	    std::cerr << "SSA could not create DDS publisher" << std::endl;
	}
      else
	std::cerr << "SSA could not create DDS publisher" << std::endl;
    }
}

StateSynchronizationAgent_i::~StateSynchronizationAgent_i ()
{
  this->delete_subscriber ();
  this->delete_publisher ();
  this->delete_participant ();
}

void 
StateSynchronizationAgent_i::state_changed (const char * object_id)
{
  ACE_DEBUG ((LM_TRACE, 
	      "SSA::state_changed (%s) called.\n", 
	      object_id));

  // get application reference
  ReplicatedApplication_var app;

  if (application_map_.find (ACE_CString (object_id),
			     app) != 0)
    {
      ACE_DEBUG ((LM_ERROR, 
		  "(%P|%t) SSA::state_changed () "
		  "could not find application for object id %s\n",
		  object_id));
      return;
    }

  // get state from the application
  CORBA::Any_var state;
  try 
    {
      state = app->get_state ();
    }
   catch (const CORBA::SystemException& ex)
    {
      ACE_DEBUG ((LM_ERROR, 
		  "(%P|%t) SSA::state_changed () "
		  "exception whil calling the get_state method for application %s:\n"
		  "%s",
		  object_id, ex._info ().c_str ()));
      return;
    }

  // send state to each element in the replica_map_
  REPLICA_OBJECT_LIST replica_group;
  if (replica_map_.find (ACE_CString (object_id),
			 replica_group) != 0)
    {
      ACE_DEBUG ((LM_ERROR, 
		  "(%P|%t) SSA::state_changed () "
		  "could not find replicas for the application %s\n",
		  object_id));
      return;
    }

  ReplicatedApplication_var replica;
  for (REPLICA_OBJECT_LIST::iterator it = replica_group.begin ();
       it != replica_group.end ();
       ++it)
    {
      try
	{
	  // set the state on this replica
	  (*it)->set_state (state.in ());
	}
      catch (const CORBA::SystemException& ex)
	{
	  ACE_DEBUG ((LM_WARNING, 
		      "(%P|%t) SSA::state_changed () "
		      "exception while contacting a server replica for %s.\n",
		      object_id));
	}
    }
}

void 
StateSynchronizationAgent_i::update_rank_list (const RankList & rank_list)
{
  // protect operations on the map
  ACE_Guard <ACE_Thread_Mutex> guard (replica_map_mutex_);

  // reset content of the internal map
  replica_map_.close();
  replica_map_.open();

  ACE_DEBUG ((LM_TRACE, "SSA::update_rank_list with:\n"));

  // for each replication group in the replica group list
  for (size_t i = 0; i < rank_list.length (); ++i)
  {
    ACE_DEBUG ((LM_TRACE, "\toid = %s (%d entries)\n", 
		rank_list[i].object_id.in (),
		rank_list.length ()));

    // use the application id as a key for the map
    ACE_CString oid (rank_list[i].object_id);

    // create a new list for every replication group
    REPLICA_OBJECT_LIST replica_object_list;

    if (use_corba_)
      {
	// for each entry of a replica group
	for (size_t j = 0; j < rank_list[i].ior_list.length (); ++j)
	  {
	    try
	      {
		// it is assumed that the strings identifying rank_list are
		// stringified object references and can be resolved
		// and used to contact the corresponding StateSynchronizationAgent
		replica_object_list.push_back (
                  STATEFUL_OBJECT_PTR (
                    new CorbaStateUpdate (
                      ReplicatedApplication::_narrow (rank_list[i].ior_list[j]))));
	      }
	    catch (const CORBA::SystemException& ex)
	      {
		ACE_DEBUG ((LM_WARNING, 
			    "(%P|%t) SSA::"
			    "update_replica_groups could not resolve stringified "
			    "object reference %s\n",
			    rank_list[i].ior_list[j].in ()));
	      }
	  }
      } // end if (use_corba_)
    else
      {
	// each application needs only one DDS topic being registered,
	// which is named by the application name
        replica_object_list.push_back (
          STATEFUL_OBJECT_PTR (
            new DDSStateUpdate <State,
                                StateTypeSupport,
                                StateDataWriter,
                                CORBA::Long> (
	      oid.c_str (),
	      this->get_unique_id (oid.c_str ()),
	      domain_participant_.in (),
	      publisher_.in ())));

      }
    
    // add one replication group to the map
    replica_map_.bind (oid, replica_object_list);
  }
}

void 
StateSynchronizationAgent_i::register_application (const char * object_id,
					  ReplicatedApplication_ptr app)
{
  ACE_DEBUG ((LM_TRACE, "SSA::register_application (%s) called.\n", object_id));

  ACE_CString oid (object_id);

  if (application_map_.bind (oid, ReplicatedApplication::_duplicate (app)) < 0)
    {
      ACE_DEBUG ((LM_WARNING, 
		  "(%P|%t) SSA::register_application () "
		  "could not bind application %s to the map successfully\n",
		  object_id));
    }  

  // if we use DDS for communication
  if (!use_corba_)
    {
      // register a data reader for this object
      new DDSStateReader_T <State,
	                    StateTypeSupport,
                            StateDataReader,
                            CORBA::Long> (this->get_unique_id (object_id),
					  app);
    }
}

bool
StateSynchronizationAgent_i::create_participant ()
{
  DDS::DomainParticipantFactory_var dpf
    = DDS::DomainParticipantFactory::get_instance ();
    
  if (CORBA::is_nil (dpf.in ()))
    {
      std::cerr << "StateSynchronizationAgent_i::create_participant () error."
		<< std::endl;
     return false;
    }

  domain_participant_ =
    dpf->create_participant (DOMAIN_ID,
			     PARTICIPANT_QOS_DEFAULT,
			     DDS::DomainParticipantListener::_nil (),
			     DDS::ANY_STATUS);

  if (CORBA::is_nil (domain_participant_.in ()))
    {
      std::cerr << "StateSynchronizationAgent_i::create_participant () failed."
		<< std::endl;
      return false;
    }

  return true;
}

bool
StateSynchronizationAgent_i::delete_participant ()
{
  DDS::DomainParticipantFactory_var dpf
    = DDS::DomainParticipantFactory::get_instance ();
    
  if (CORBA::is_nil (dpf.in ()))
    {
      std::cerr << "StateSynchronizationAgent_i::delete_participant () error."
		<< std::endl;
     return false;
    }

  DDS::ReturnCode_t status = 
    dpf->delete_participant (domain_participant_.in ());
  
  if (status != DDS::RETCODE_OK)
    {
      return false;
    }
    
  return true;
}

bool
StateSynchronizationAgent_i::create_publisher ()
{
  publisher_ =
    domain_participant_->create_publisher (PUBLISHER_QOS_DEFAULT,
					   DDS::PublisherListener::_nil (),
					   DDS::ANY_STATUS);

  if (CORBA::is_nil (publisher_.in ()))
    {
      return false;
    }    

  return true;
}

bool
StateSynchronizationAgent_i::delete_publisher ()
{
  DDS::ReturnCode_t status = 
    domain_participant_->delete_publisher (publisher_.in ());

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }
    
  return true;  
}

bool
StateSynchronizationAgent_i::create_subscriber ()
{
  subscriber_ =
    domain_participant_->create_subscriber (SUBSCRIBER_QOS_DEFAULT,
					    DDS::SubscriberListener::_nil (),
					    DDS::ANY_STATUS);

  if (CORBA::is_nil (subscriber_.in ()))
    {
      return false;
    }    

  return true;
}

bool
StateSynchronizationAgent_i::delete_subscriber ()
{
  DDS::ReturnCode_t status = 
    domain_participant_->delete_subscriber (subscriber_.in ());

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }
    
  return true;  
}

std::string
StateSynchronizationAgent_i::get_unique_id (const std::string & app_name)
{
  std::string unique_id (app_name);

  // make name unique by adding host and process id
  unique_id += "_" + host_id_ + "_" + process_id_;

  return unique_id;
}
