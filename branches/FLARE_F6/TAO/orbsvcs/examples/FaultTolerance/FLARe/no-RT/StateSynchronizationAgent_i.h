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
#include <list>
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Thread_Mutex.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include <ccpp_dds_dcps.h>
#include "LWFTC.h"
#include "StateSynchronizationAgentS.h"
#include "StatefulObject.h"
#include "DDSFailure.h"

class SSA_Export StateSynchronizationAgent_i : public POA_StateSynchronizationAgent
{
 public:
  /// ctor
  /// @param reference to a simple long value representing
  ///        the state of the application
  StateSynchronizationAgent_i (CORBA::ORB_ptr orb,
			       const std::string & host_id,
			       const std::string & process_id,
                               bool use_corba = true);

  /// dtor
  ~StateSynchronizationAgent_i ();

  /// implementation of the StateSynchronizationAgent interface
  virtual void state_changed (const char * object_id);

  /// implementation of the StateSynchronizationAgent interface
  virtual void update_rank_list (const RankList & rank_list);

  /// implementation of the StateSynchronizationAgent interface
  virtual void register_application (const char * object_id,
				     ReplicatedApplication_ptr app);

  typedef ACE_Refcounted_Auto_Ptr <StatefulObject, 
				   ACE_Null_Mutex> STATEFUL_OBJECT_PTR;

  typedef std::list<STATEFUL_OBJECT_PTR> REPLICA_OBJECT_LIST;

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
  bool create_participant ();
  bool delete_participant ();
  bool create_publisher ();
  bool delete_publisher ();
  bool create_subscriber ();
  bool delete_subscriber ();

  std::string get_unique_id (const std::string & app_name);

 private:
  /// orb reference
  CORBA::ORB_var orb_;

  /// host on which the agent is running
  std::string host_id_;

  /// process in which the agent is running
  std::string process_id_;

  /// keeps lists of replicas associated to application ids
  OBJECTID_REPLICA_MAP replica_map_;

  /// keeps references to all applications running in this process
  OBJECTID_APPLICATION_MAP application_map_;

  /// mutex for multithreaded access of the replica map
  ACE_Thread_Mutex replica_map_mutex_;

  /// id of the DDS domain
  DDS::DomainId_t domain_id_;

  /// DDS Domain Participant
  DDS::DomainParticipant_var domain_participant_;

  /// DDS Publisher for this Domain
  DDS::Publisher_var publisher_;

  /// DDS Subscriber for this Domain
  DDS::Subscriber_var subscriber_;

  /// decides whether replicas should be updated through corba or dds
  bool use_corba_;
};

#endif /* _REPLICATED_APPLICATION_H_ */
