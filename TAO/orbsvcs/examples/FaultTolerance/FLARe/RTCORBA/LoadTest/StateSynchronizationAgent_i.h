// -*- C++ -*-

//=============================================================================
/**
 *  @file    StateSynchronizationAgent_i.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _REPLICATION_AGENT_I_H_
#define _REPLICATION_AGENT_I_H_

#include "StateSynchronizationAgentS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Thread_Mutex.h"
#include "LWFTC.h"
#include <list>

class StateSynchronizationAgent_i : public POA_StateSynchronizationAgent
{
 public:
  /// ctor
  /// @param reference to a simple long value representing
  ///        the state of the application
  StateSynchronizationAgent_i (CORBA::ORB_ptr orb);

  /// dtor
  ~StateSynchronizationAgent_i ();

  /// implementation of the StateSynchronizationAgent interface
  virtual void state_changed (const char * object_id);

  /// implementation of the StateSynchronizationAgent interface
  virtual void update_rank_list (const RankList & rank_list);

  /// implementation of the StateSynchronizationAgent interface
  virtual void register_application (const char * object_id,
				     ReplicatedApplication_ptr app);

  typedef std::list<CORBA::Object_var> REPLICA_OBJECT_LIST;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    REPLICA_OBJECT_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_REPLICA_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    ReplicatedApplication_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_APPLICATION_MAP;

 private:
  /// orb reference
  CORBA::ORB_var orb_;

  /// keeps lists of replicas associated to application ids
  OBJECTID_REPLICA_MAP replica_map_;

  /// keeps references to all applications running in this process
  OBJECTID_APPLICATION_MAP application_map_;

  /// mutex for multithreaded access of the replica map
  ACE_Thread_Mutex replica_map_mutex_;
};

#endif /* _REPLICATED_APPLICATION_H_ */
