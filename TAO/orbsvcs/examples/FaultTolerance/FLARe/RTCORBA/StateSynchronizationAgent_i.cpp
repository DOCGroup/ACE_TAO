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

StateSynchronizationAgent_i::StateSynchronizationAgent_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

StateSynchronizationAgent_i::~StateSynchronizationAgent_i ()
{
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
	  replica = ReplicatedApplication::_narrow (*it);
	  if (!CORBA::is_nil (replica.in ()))
	    {
	      //	      if (!app->_is_equivalent (replica.in ())) // do not reset local state
		{
		  ACE_DEBUG ((LM_TRACE, "SSA::state_changed () calls set "
			                "state on replica.\n"));
		  replica->set_state (state.in ());
		}
	    }
	  else
	    ACE_DEBUG ((LM_ERROR,
			"SSA::state_changed () "
			"could not contact application replica for %s.",
			object_id));
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

    // create a new list for every replication group
    REPLICA_OBJECT_LIST replica_object_list;

    // for each entry of a replica group
    for (size_t j = 0; j < rank_list[i].ior_list.length (); ++j)
    {
      try
	{
	  // it is assumed that the strings identifying rank_list are
	  // stringified object references and can be resolved
	  // and used to contact the corresponding StateSynchronizationAgent
	  replica_object_list.push_back (
            CORBA::Object::_duplicate (rank_list[i].ior_list[j]));
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
    
    // use the application id as a key for the map
    ACE_CString oid (rank_list[i].object_id);

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
}
