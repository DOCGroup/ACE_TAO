// $Id$

#include "ForwardingAgent.h"

ForwardingAgent_i::ForwardingAgent_i (bool proactive)
  : proactive_ (proactive),
    update_count_ (0)
{
}

ForwardingAgent_i::~ForwardingAgent_i (void)
{
}

void
ForwardingAgent_i::proactive (bool v)
{
  proactive_ = v;
}

CORBA::Object_ptr
ForwardingAgent_i::next_member (const char *ior_string)
{
 // ACE_DEBUG ((LM_DEBUG,
 //             "FA: NEXT MEMBER CALLED for ior_string = %s.\n",           
 //             ior_string));
  ACE_Guard <ACE_Thread_Mutex> guard (ior_map_mutex_);
  
  AGENT_RANKED_IOR_LIST ranked_ior_list;
  
  int result =
    this->objectid_rankedior_map_.find (ACE_CString (ior_string),
                                        ranked_ior_list);
                                        
  //size_t siz = ranked_ior_list.ior_list.size ();
  //ACE_DEBUG ((LM_DEBUG,
  //            "next_member: IOR list size = %u\n",
  //            siz));
  
  if ((result == 0) && (ranked_ior_list.ior_list.size () > 0))
    {
      CORBA::Object_var ior (ranked_ior_list.ior_list.front ());
      ranked_ior_list.ior_list.pop_front ();
      this->objectid_rankedior_map_.rebind (ACE_CString (ior_string),
					    ranked_ior_list);
      return CORBA::Object::_duplicate (ior.in ());
    }

  ACE_ERROR_RETURN ((LM_ERROR,
		     "FA: No ior list entry for tag=%s!!!\n",
		     ior_string),
		    0);
}

void
ForwardingAgent_i::update_rank_list (const RankList & rank_list)
{
  ACE_Guard <ACE_Thread_Mutex> guard (ior_map_mutex_);
  objectid_rankedior_map_.close ();
  objectid_rankedior_map_.open ();
  /*
  ACE_DEBUG((LM_TRACE,
             "ForwardingAgent - "
             "Received rank_list length = %d.\n",
             rank_list.length ()));
  */
  for (size_t i = 0; i < rank_list.length (); ++i)
    {
      /*
      ACE_DEBUG ((LM_TRACE,
                  "\toid(%s) = %d entries\n", 
                  rank_list[i].object_id.in (), 
                  rank_list[i].ior_list.length ()));
      */
      AGENT_RANKED_IOR_LIST ranked_ior_list;
      ranked_ior_list.now = rank_list[i].now;
      
      for (size_t j = 0; j < rank_list[i].ior_list.length (); ++j)
        {
          ranked_ior_list.ior_list.push_back (
            CORBA::Object::_duplicate (rank_list[i].ior_list[j]));
        }
      
      ACE_CString oid (rank_list[i].object_id);
      objectid_rankedior_map_.bind (oid, ranked_ior_list);
    }
}

void
ForwardingAgent_i::initialize (CORBA::Object_ptr rm_ior)
{
  this->RM_var_ = ReplicationManager::_narrow (rm_ior); 
  ForwardingAgent_var temp = this->_this ();
  
  //ACE_DEBUG ((LM_DEBUG, "FA: calling register agent\n"));
  RankList *rank_list = this->RM_var_->register_agent (temp.in ());
  update_rank_list (*rank_list);
}
