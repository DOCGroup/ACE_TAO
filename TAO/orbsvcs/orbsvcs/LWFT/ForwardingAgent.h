// $Id$

#ifndef FORWARDINGAGENT_H
#define FORWARDINGAGENT_H

#include "ForwardingAgentS.h"
#include "ReplicationManagerC.h"

#include <list>

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Unbounded_Queue.h"

struct AGENT_RANKED_IOR_LIST
{
  bool now;
  std::list<CORBA::Object_var> ior_list;
};


class LWFT_Client_Export ForwardingAgent_i : public virtual POA_ForwardingAgent
{
public:
  ForwardingAgent_i (bool proactive = true);

  ~ForwardingAgent_i (void);

  virtual CORBA::Object_ptr next_member (const char * m);
  
  virtual void update_rank_list (const RankList & rank_list);
  
  void initialize (CORBA::Object_ptr);
  void proactive (bool v);

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  AGENT_RANKED_IOR_LIST,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    OBJECTID_RANKED_IOR_MAP;


private:
  ReplicationManager_var RM_var_;
  OBJECTID_RANKED_IOR_MAP objectid_rankedior_map_;
  ACE_Thread_Mutex ior_map_mutex_;
  bool proactive_;
  size_t update_count_;
};

#endif  /* FORWARDINGAGENT_H */
