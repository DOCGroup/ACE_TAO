// cvs-id    : $Id$

#include "Agent.h"

Agent_i::Agent_i (bool proactive)
  : //failover_map_ (100),
    //secondary_map_ (100),
    //failure_map_ (100),
    proactive_(proactive),
    update_count_ (0)
{
}

Agent_i::~Agent_i (void)
{
}

void Agent_i::proactive(bool v)
{
  proactive_ = v;
}

CORBA::Object_ptr
Agent_i::next_member (const char *ior_string)
{
  ACE_DEBUG ((LM_DEBUG, "NEXT MEMBER CALLED for ior_string = %s.\n",ior_string));
  //sleep (3); 
  //if (! proactive_)
 // {
//    CORBA::Object_var object = RM_var_->get_next(ior_string);
    //return CORBA::Object::_duplicate(object.in());
//  }

  ACE_Guard <ACE_Thread_Mutex> guard (ior_map_mutex_);
  AGENT_RANKED_IOR_LIST ranked_ior_list; 
  if (this->objectid_rankedior_map_.find(ACE_CString(ior_string),
        ranked_ior_list) == 0)
  {
    CORBA::Object_var ior (ranked_ior_list.ior_list.front());
    ranked_ior_list.ior_list.pop_front();
    this->objectid_rankedior_map_.rebind(ACE_CString(ior_string),ranked_ior_list);
    return CORBA::Object::_duplicate(ior.in());
  }
  else
  {
    ACE_DEBUG((LM_ERROR,"No ior list for tag=%s!!!\n",ior_string));
    return 0;
  }

/*  // this->RM_var_->next_member (ior_string); 
  CORBA::Object_var replica_ior;
  CORBA::ULong failure_count = 0;
  if (this->failure_map_.find (ACE_CString(ior_string), failure_count) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "FAILURE COUNT IS %d\n", failure_count));
      if (failure_count == 1)
        {
          if (this->failover_map_.find (ior_string, replica_ior) == 0)
            {
              this->failure_map_.rebind (ior_string, 2);
              return CORBA::Object::_duplicate (replica_ior.in ());
            }
        }
      else if (failure_count == 2)
        {
          if (this->secondary_map_.find (ior_string, replica_ior) == 0)
            {
              return CORBA::Object::_duplicate (replica_ior.in ());
            }
        }
    }
  else
  {
    for (FAILURE_MAP::iterator it = this->failure_map_.begin ();
       it != this->failure_map_.end (); ++it)
    {
      ACE_CString object_id = (*it).ext_id_;
      ACE_DEBUG((LM_DEBUG,"object_id in the failure_map_ is %s.\n",object_id.c_str()));
    }
  }
  return 0;
*/
}

void Agent_i::
update_rank_list (const RankList & rank_list)
{
/*  if (update_count_ > 80)
  {
    ACE_DEBUG((LM_DEBUG,"not updated.\n"));
    return;
  }
  update_count_++;
*/
  ACE_Guard <ACE_Thread_Mutex> guard (ior_map_mutex_);
  objectid_rankedior_map_.close();
  objectid_rankedior_map_.open();
 
  // ACE_DEBUG((LM_DEBUG,"Received rank_list length = %d.\n", rank_list.length()));
  for (size_t i = 0;i < rank_list.length();++i)
  {
    AGENT_RANKED_IOR_LIST ranked_ior_list;
    ranked_ior_list.now = rank_list[i].now;
    for (size_t j = 0; j < rank_list[i].ior_list.length(); ++j)
    {
      ranked_ior_list.ior_list.push_back(
          CORBA::Object::_duplicate(rank_list[i].ior_list[j]));
    }
    ACE_CString oid (rank_list[i].object_id);
    objectid_rankedior_map_.bind(oid,ranked_ior_list);
    //ACE_DEBUG((LM_DEBUG,"object_id=%s. ior_list_size=%d.\n",
    //    oid.c_str(), ranked_ior_list.ior_list.size()));
  }
}

void Agent_i::
initialize (CORBA::Object_ptr rm_ior)
{
  this->RM_var_ = ReplicationManager::_narrow (rm_ior); 
  Agent_var temp = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  
  ACE_DEBUG ((LM_DEBUG, "calling register agent\n"));
  RankList *rank_list = this->RM_var_->register_agent (temp.in ());
  update_rank_list (*rank_list);

/*
  CORBA::String_var ior_string =
    this->orb_->object_to_string (temp.in ());

  FILE *output_file= ACE_OS::fopen (ior_file, "w");
  if (output_file == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output file <%s> for writing "
                         "IOR: %s",
                         ior_string.in ()),
                        1);
    }

  ACE_OS::fprintf (output_file, "%s", ior_string.in ());
  ACE_OS::fclose (output_file);
*/

}

/*
void
Agent_i::update_reference (const char *ior_string)
{
  this->forward_str_ = CORBA::string_dup (ior_string);
}

void
Agent_i::update_failover_list (const FailoverList &failover_list)
{
  ACE_DEBUG ((LM_DEBUG, "failure map updates\n"));
  CORBA::ULong failover_list_length = failover_list.length ();
  for (CORBA::ULong i = 0; i < failover_list_length; ++i)
    {
      const char *object_id = failover_list[i];
      ACE_DEBUG ((LM_DEBUG, "OBJECT ID is %s\n", object_id));
      this->failure_map_.rebind (object_id, 2);
    }
}

void
Agent_i::update_failover (const char * object_id, 
                          CORBA::Object_ptr next_member)
{
  ACE_DEBUG ((LM_DEBUG, "calling update next member\n"));
  this->failover_map_.rebind (object_id, 
                              CORBA::Object::_duplicate (next_member));
}

void
Agent_i::update_secondary (const char * object_id,
                           CORBA::Object_ptr next_member)
{
  ACE_DEBUG ((LM_DEBUG, "calling update next members\n"));
  this->secondary_map_.rebind (object_id, 
                              CORBA::Object::_duplicate (next_member));
}

void
Agent_i::initialize_agent (const ReplicaList & replica_list,
                           const ReplicasList & replicas_list)
{
  ACE_DEBUG ((LM_DEBUG, "calling initialize agent\n"));

  CORBA::ULong replica_list_length = replica_list.length ();
  for (CORBA::ULong i = 0; i < replica_list_length; ++i)
    {
      const char *object_id = replica_list[i].object_id;
      CORBA::Object_var replica_ior = replica_list[i].next_member;
      
      if (this->failover_map_.bind (object_id, replica_ior) != 0)
        ACE_DEBUG((LM_DEBUG,"failover_map_ did not bind %s.\n", object_id));
        
      if (this->failure_map_.bind (object_id, 1) != 0)
        ACE_DEBUG((LM_DEBUG,"failure_map_ did not bind %s.\n", object_id));
      
      ACE_DEBUG((LM_DEBUG,"object_id added to failover map = %s:\n",object_id));
    }

  CORBA::ULong replicas_list_length = replicas_list.length ();
  for (CORBA::ULong i = 0; i < replicas_list_length; ++i)
    {
      const char *object_id = replicas_list[i].object_id;
      CORBA::Object_var replicas_ior = replicas_list[i].next_members;
      this->secondary_map_.bind (object_id, replicas_ior);
      ACE_DEBUG((LM_DEBUG,"object_id added to secondary map = %s:\n",object_id));
    }
}

*/
