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

#include <string>

#include "StateSynchronizationAgent_i.h"
#include "CorbaStateUpdate.h"

#ifdef FLARE_USES_DDS
# include "DDSStateUpdate_T.h"
#endif

StateSynchronizationAgent_i::StateSynchronizationAgent_i (
  const std::string & host_id,
  const std::string & process_id,
  bool use_corba)
  : host_id_ (host_id),
    process_id_ (process_id),
#if defined (FLARE_USES_DDS)
    domain_id_ (0),//"FLAREDomain"),
    domain_participant_ (DDS::DomainParticipant::_nil ()),
    publisher_ (DDS::Publisher::_nil ()),
    subscriber_ (DDS::Subscriber::_nil ()),
#endif /* FLARE_USES_DDS */
    use_corba_ (use_corba)
{
#if defined (FLARE_USES_DDS)
  if (!use_corba_)
    {
      if (!this->create_participant ())
        {
          throw
            DDSFailure ("SSA could not create DDS participant\n");
        }

      if (!this->create_publisher ())
        {
          throw
            DDSFailure ("SSA could not create DDS publisher\n");
        }
      
      if (!this->create_subscriber ())
        {
          throw
            DDSFailure ("SSA could not create DDS subscriber\n");
        }
    }
#endif /* FLARE_USES_DDS */
}

StateSynchronizationAgent_i::~StateSynchronizationAgent_i (void)
{
#ifdef FLARE_USES_DDS
  if (!use_corba_)
    {
      DDS::ReturnCode_t status =
        domain_participant_->delete_contained_entities ();
        
      if (status != DDS::RETCODE_OK)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("SSA - ")
                      ACE_TEXT ("Bad retcode from ")
                      ACE_TEXT ("delete_contained_entities()\n")));
        }
        
      if (! this->delete_participant ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("SSA - ")
                      ACE_TEXT ("Bad retcode from ")
                      ACE_TEXT ("delete_participant()\n")));
        }
    }
#endif /* FLARE_USES_DDS */
}

void 
StateSynchronizationAgent_i::state_changed (const char * object_id)
{
  /*
  ACE_DEBUG ((LM_TRACE, 
              ACE_TEXT ("SSA::state_changed (%s) called.\n"), 
              object_id));
  */
  // Get application reference.
  ReplicatedApplication_var app;

  if (application_map_.find (ACE_CString (object_id),
			                       app) != 0)
    {
      ACE_ERROR ((LM_ERROR, 
		              ACE_TEXT ("(%P|%t) SSA::state_changed () ")
		              ACE_TEXT ("could not find application ")
		              ACE_TEXT ("for object id %s\n"),
		              object_id));
      return;
    }

  // Get state from the application.
  CORBA::Any_var state;
  
  try 
    {
      state = app->get_state ();
    }
   catch (const CORBA::SystemException& ex)
    {
      ACE_DEBUG ((LM_ERROR, 
		              ACE_TEXT ("(%P|%t) SSA::state_changed () ")
		              ACE_TEXT ("exception while calling the ")
		              ACE_TEXT ("get_state method for application ")
		              ACE_TEXT ("%s:\n")
		              ACE_TEXT ("%s"),
		              object_id,
		              ex._info ().c_str ()));
      return;
    }

  // Send state to each element in the replica_map_.
  ReplicaGroup replica_group;
  
  if (replica_map_.find (ACE_CString (object_id),
			 replica_group) != 0)
    {
      /*
      ACE_ERROR ((LM_WARNING, 
                  ACE_TEXT ("(%P|%t) SSA::state_changed () ")
                  ACE_TEXT ("could not find replicas ")
                  ACE_TEXT ("for the application %s\n")_,
		  object_id));
      */
      return;
    }

  ReplicatedApplication_var replica;
  
  for (REPLICA_OBJECT_LIST::iterator it = replica_group.replicas.begin ();
       it != replica_group.replicas.end ();
       ++it)
    {
      try
	      {
	        // Set the state on this replica.
	        (*it)->set_state (state.in ());
	      }
      catch (const CORBA::SystemException& ex)
	      {
	        ACE_DEBUG ((LM_WARNING, 
		                  ACE_TEXT ("(%P|%t) SSA::state_changed () ")
		                  ACE_TEXT ("exception while contacting a ")
		                  ACE_TEXT ("server replica for %s.\n"),
		                  object_id));
	      }
    }
}

void 
StateSynchronizationAgent_i::update_rank_list (const RankList & rank_list)
{
  if (use_corba_)
    {
      // If only corba is used, we can simply reset the map.
      replica_map_.close ();
      replica_map_.open ();
    }
  else
    {
      // only remove entries replicated by CORBA
      for (OBJECTID_REPLICA_MAP::iterator it = replica_map_.begin ();
           it != replica_map_.end ();
           ++it)
        {
          if (!it->item ().use_dds)
            {
              replica_map_.unbind (it);
            }
        }
    }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("SSA::update_rank_list with:\n")));

  // for each replication group in the replica group list
  for (size_t i = 0; i < rank_list.length (); ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("\toid = %s (%d entries)\n"), 
                  rank_list[i].object_id.in (),
                  rank_list[i].ior_list.length ()));

      // Use the application id as a key for the map.
      ACE_CString oid (rank_list[i].object_id);

      // If there is already an entry for this object_id it means that
      // we have a registered DDS object and should not override it.
      if (replica_map_.find (oid) != 0)
        {
          // Create a new list for every replication group.
          ReplicaGroup replica_group;
          replica_group.use_dds = false;      

          // For each entry of a replica group...
          for (size_t j = 0;
               j < rank_list[i].ior_list.length ();
               ++j)
            {
              try
                {
                  // it is assumed that the strings identifying
                  // rank_list are stringified object references and
                  // can be resolved and used to contact the
                  // corresponding StateSynchronizationAgent.
                  replica_group.replicas.push_back (
                    STATEFUL_OBJECT_PTR (
                      new CorbaStateUpdate (
                          rank_list[i].ior_list[j])));
                }
              catch (const CORBA::SystemException& ex)
                {
		              /*
                  ACE_DEBUG ((
                    LM_WARNING, 
                    ACE_TEXT ("(%P|%t) SSA::")
                    ACE_TEXT ("update_rank_list could ")
                    ACE_TEXT ("not resolve stringified "(
                    ACE_TEXT ("object reference for %s : %s\n"),
                    oid.c_str (),
                    ex._info ().c_str ()));
		              */
                }
            }

          // Add one replication group to the map.
          replica_map_.bind (oid, replica_group);
        }
    }
}

void
StateSynchronizationAgent_i::register_application (
  const char * object_id,
  ReplicatedApplication_ptr app)
{
  /*
  ACE_DEBUG ((LM_TRACE,
              ACE_TEXT ("SSA::register_application (%s) called.\n"),
              object_id));
  */
  ACE_CString oid (object_id);
  int result =
    application_map_.bind (oid,
                           ReplicatedApplication::_duplicate (app));
    
  if (result < 0)
    {
      ACE_DEBUG ((LM_WARNING, 
		              ACE_TEXT ("(%P|%t) SSA::register_application () ")
		              ACE_TEXT ("could not bind application %s to ")
		              ACE_TEXT ("the map successfully\n"),
		              object_id));
    }
}

#ifdef FLARE_USES_DDS

bool
StateSynchronizationAgent_i::create_participant (void)
{
  DDS::DomainParticipantFactory_var dpf
    = DDS::DomainParticipantFactory::get_instance ();
    
  if (CORBA::is_nil (dpf.in ()))
    {
      return false;
    }

  domain_participant_ =
    dpf->create_participant (
      domain_id_,
      PARTICIPANT_QOS_DEFAULT,
      DDS::DomainParticipantListener::_nil (),
      DDS::ANY_STATUS);

  if (CORBA::is_nil (domain_participant_.in ()))
    {
      return false;
    }

  return true;
}

bool
StateSynchronizationAgent_i::delete_participant (void)
{
  DDS::DomainParticipantFactory_var dpf
    = DDS::DomainParticipantFactory::get_instance ();
    
  if (CORBA::is_nil (dpf.in ()))
    {
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
StateSynchronizationAgent_i::create_publisher (void)
{
  DDS::PublisherQos pub_qos;
  domain_participant_->get_default_publisher_qos (pub_qos);

  publisher_ =
    domain_participant_->create_publisher (
      pub_qos,
	    DDS::PublisherListener::_nil (),
	    DDS::ANY_STATUS);

  if (CORBA::is_nil (publisher_.in ()))
    {
      return false;
    }    

  return true;
}

bool
StateSynchronizationAgent_i::delete_publisher (void)
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
StateSynchronizationAgent_i::create_subscriber (void)
{
  subscriber_ =
    domain_participant_->create_subscriber (
      SUBSCRIBER_QOS_DEFAULT,
	    DDS::SubscriberListener::_nil (),
	    DDS::ANY_STATUS);

  if (CORBA::is_nil (subscriber_.in ()))
    {
      return false;
    }    

  return true;
}

bool
StateSynchronizationAgent_i::delete_subscriber (void)
{
  DDS::ReturnCode_t status = 
    domain_participant_->delete_subscriber (subscriber_.in ());

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }
    
  return true;  
}

#endif /* FLARE_USES_DDS */

std::string
StateSynchronizationAgent_i::get_unique_id (
  const std::string & app_name)
{
  std::string unique_id (app_name);

  // Make name unique by adding host and process id.
  unique_id += "_" + host_id_ + "_" + process_id_;

  return unique_id;
}

