// cvs-id    : $Id$

#ifndef AGENT_H
#define AGENT_H

#include "LWFTS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Unbounded_Queue.h"

#include <list>

struct AGENT_RANKED_IOR_LIST
{
  bool now;
  std::list<CORBA::Object_var> ior_list;
};


class Agent_i : public virtual POA_Agent
{
public:
  Agent_i (bool proactive = true);

  ~Agent_i (void);

  virtual CORBA::Object_ptr next_member (const char * m);
  
  virtual void update_rank_list (const RankList & rank_list);
  
  void initialize (CORBA::Object_ptr);
  void proactive (bool v);

/*
  virtual void update_reference (const char * m);

  virtual void update_failover (const char * object_id,
                                CORBA::Object_ptr next_object);

  virtual void update_secondary (const char * object_id,
                                 CORBA::Object_ptr next_member);


  virtual void initialize_agent (const ReplicaList & replica_list,
                                 const ReplicasList & replicas_list));

  
  virtual void update_failover_list (const FailoverList &failover_list);

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    CORBA::Object_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> REPLICA_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    CORBA::ULong,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> FAILURE_MAP;
*/
  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    AGENT_RANKED_IOR_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_RANKED_IOR_MAP;


private:
  //CORBA::String_var forward_str_;
  //REPLICA_MAP failover_map_;
  //REPLICA_MAP secondary_map_;
  //FAILURE_MAP failure_map_;
  ReplicationManager_var RM_var_;
  OBJECTID_RANKED_IOR_MAP objectid_rankedior_map_;
  ACE_Thread_Mutex ior_map_mutex_;
  bool proactive_;
  size_t update_count_;
};

#endif  /* AGENT_H */
