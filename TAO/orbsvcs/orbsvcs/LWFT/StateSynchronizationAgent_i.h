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

#ifndef STATE_SYNCHRONIZATION_AGENT_I_H_
#define STATE_SYNCHRONIZATION_AGENT_I_H_

#include <list>
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Thread_Mutex.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "StateSynchronizationAgentS.h"
#include "StatefulObject.h"
#include "ssa_export.h"

#if defined (FLARE_USES_DDS)
# include <ccpp_dds_dcps.h>
# include "DDSFailure.h"
#endif 

class SSA_Export StateSynchronizationAgent_i
  : public POA_StateSynchronizationAgent
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
  ~StateSynchronizationAgent_i (void);

  /// Implementation of the StateSynchronizationAgent interface.
  virtual void state_changed (const char * object_id);

  /// Implementation of the StateSynchronizationAgent interface.
  virtual void update_rank_list (const RankList & rank_list);

  /// Registers application for statesynchronization with CORBA.
  virtual void register_application (const char * object_id,
				     ReplicatedApplication_ptr app);

#ifdef FLARE_USES_DDS
  /// Registers application for state synchronization with DDS
  template <typename DATA_TYPE>
  void register_application_with_dds (const char * object_id,
                                      ReplicatedApplication_ptr app);
#endif /* FLARE_USES_DDS */

  typedef
  ACE_Refcounted_Auto_Ptr <StatefulObject,
				                   ACE_Null_Mutex>
    STATEFUL_OBJECT_PTR;

  typedef std::list<STATEFUL_OBJECT_PTR> REPLICA_OBJECT_LIST;

  struct ReplicaGroup 
  {
    REPLICA_OBJECT_LIST replicas;
    bool use_dds;
  };

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    ReplicaGroup,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Thread_Mutex> OBJECTID_REPLICA_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    ReplicatedApplication_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_APPLICATION_MAP;

#ifdef FLARE_USES_DDS

  bool create_participant (void);
  bool delete_participant (void);
  bool create_publisher (void);
  bool delete_publisher (void);
  bool create_subscriber (void);
  bool delete_subscriber (void);

#endif /* FLARE_USES_DDS */

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

#ifdef FLARE_USES_DDS

  /// id of the DDS domain
  DDS::DomainId_t domain_id_;

  /// DDS Domain Participant
  DDS::DomainParticipant_var domain_participant_;

  /// DDS Publisher for this Domain
  DDS::Publisher_var publisher_;

  /// DDS Subscriber for this Domain
  DDS::Subscriber_var subscriber_;

#endif /* FLARE_USES_DDS */

  /// decides whether replicas should be updated through corba or dds
  bool use_corba_;
};

#include "StateSynchronizationAgent_i_T.cpp"

#endif /* STATE_SYNCHRONIZATION_AGENT_I_H_ */
