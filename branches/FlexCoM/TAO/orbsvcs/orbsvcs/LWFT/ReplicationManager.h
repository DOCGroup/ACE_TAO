// -*- C++ -*-
// $Id$

#ifndef REPLICATION_MANAGER_H
#define REPLICATION_MANAGER_H

#include <list>
#include <queue>

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Unbounded_Set.h"
#include "ace/Condition_T.h"
#include "ace/Containers_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "AppSideReg.h"
#include "Timer.h"

#include "ReplicationManagerS.h"

#include "rm_impl_export.h"

enum Role
{
  PRIMARY = 1,
  BACKUP = 2
};

enum AlgoMode
{
  PROCESS_LEVEL = 1,
  PROCESSOR_LEVEL = 2
};

class Algorithm;

struct APP_INFO
{
  ACE_CString object_id;
  double load;
  ACE_CString host_name;
  ACE_CString process_id;
  Role role;
  CORBA::Object_var ior;

  APP_INFO (void);
  
  APP_INFO (APP_INFO const & app_info);
  
  APP_INFO (const char *oid,
            double l,
            const char *hname,
            const char *pid, 
            Role r,
            CORBA::Object_ptr ref);
            
  APP_INFO (const char *oid,
            const char *hname,
            const char *pid,
            Role r);
            
  void swap (APP_INFO & app_info);
  APP_INFO & operator = (APP_INFO const & app_info);
};

bool operator == (APP_INFO const & lhs, APP_INFO const & rhs);

struct RANKED_IOR_LIST
{
  bool now;
  std::list<CORBA::Object_var> ior_list;
  std::list<ACE_CString> host_list;

  RANKED_IOR_LIST (void);
};

struct UtilRank 
{
  double util;
  std::string host_id;

  UtilRank (void);
  UtilRank (UtilRank const & ur);
  UtilRank (double u, const char * hid);
};

bool operator < (UtilRank const & u1, UtilRank const & u2);

struct MonitorUpdate
{
  enum UpdateType
  {
    PROC_FAIL_UPDATE,
    HOST_UTIL_UPDATE,
    RUN_NOW,
    APP_REG
  };
  
  UpdateType type;

  ACE_CString process_id;
  ACE_CString host_id;
  double value;
  APP_INFO app_info;

  static MonitorUpdate * 
  create_proc_fail_update (const char * pid);
  
  static MonitorUpdate * 
  create_host_util_update (const char *hid, double value);
  
  static MonitorUpdate * 
  create_run_now_update (void);
  
  static MonitorUpdate * 
  create_app_info_update (const char *oid,
                          double l, 
                          const char *hname,
                          const char *pid, 
                          Role r,
                          CORBA::Object_ptr ref);
};

class RM_Impl_Export ReplicationManager_i
  : public virtual POA_ReplicationManager,
    protected Timer
{
public:
  ReplicationManager_i (CORBA::ORB_ptr orb,
                        double hertz, 
                        bool proactive = true,
                        bool static_mode = false,
                        AlgoMode mode = PROCESS_LEVEL);

  ~ReplicationManager_i (void);

  virtual void
  register_application (const char *object_id,
                        double load,
                        const char *host_name,
                        const char *process_id,
                        CORBA::Short role,
                        CORBA::Object_ptr server_reference);

  void
  util_update (const char *host_id, 
               double util);
  
  virtual void
  proc_failure (const char *process_id);
  
  virtual RankList *
  register_agent (CORBA::Object_ptr agent_reference);

  virtual RankList *
  register_state_synchronization_agent (
    const char * host_id,
    const char * process_id,
    StateSynchronizationAgent_ptr agent);

  virtual CORBA::Object_ptr
  get_next (const char * object_id);

  virtual void set_state (const ::CORBA::Any & state_value);
  
  virtual ::CORBA::Any * get_state (void);

  virtual ::StateSynchronizationAgent_ptr agent (void);
  
  virtual void agent (StateSynchronizationAgent_ptr agent);
  
  virtual char * object_id (void);
  
  virtual void object_id (const char * object_id);

  virtual FLARE::NotificationId register_fault_notification (
    FLARE::FaultNotification_ptr receiver);

  virtual void unregister_fault_notification (
    FLARE::NotificationId id);

  virtual void set_ranklist_constraints (
     const RankListConstraints & constraints);

  void
  load_based_selection_algo (void);

  void
  static_selection_algo (void);

  bool
  replica_selection_algo (void);

  typedef ACE_Unbounded_Set<ACE_CString> STRING_LIST;
  typedef ACE_Unbounded_Set<APP_INFO> APP_SET;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    STRING_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> STRING_TO_STRING_LIST_MAP;
 
  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    ACE_CString,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> STRING_TO_STRING_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    double,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> STRING_TO_DOUBLE_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    APP_SET,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_APPSET_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    APP_INFO,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_APPINFO_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    RANKED_IOR_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_RANKED_IOR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  CORBA::Object_var,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    STRING_OBJECT_MAP;

  typedef ACE_Unbounded_Set<CORBA::Object_var> AGENT_LIST;

  typedef ACE_Hash_Map_Manager_Ex <
    ACE_CString,
    StateSynchronizationAgent_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> STATE_SYNC_AGENT_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    FLARE::NotificationId,
    FLARE::FaultNotification_var,
    ACE_Hash<FLARE::NotificationId>,
    ACE_Equal_To<FLARE::NotificationId>,
    ACE_Null_Mutex> NOTIFICATION_MAP;

  typedef std::list<ACE_CString> RANKLIST_CONSTRAINT;

  typedef ACE_Hash_Map_Manager_Ex <
    ACE_CString,
    RANKLIST_CONSTRAINT,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> RANKLIST_CONSTRAINT_MAP;
  
  enum
  {
    UPDATE_LIST_MAX_SIZE = 100
  };
  
private:
  CORBA::ORB_var orb_;
  AppSideReg proc_reg_;

  Algorithm * algo_thread_;
  bool standby_;
  bool proactive_;
  AlgoMode mode_;

  // local StateSynchonizationAgent
  StateSynchronizationAgent_var agent_;
  
  /// This flag disables the usage of host load calculations and
  /// therefore enables single host scenarios.
  bool static_mode_;
  
  ACE_DLList <MonitorUpdate> update_list_;
  ACE_Thread_Mutex update_mutex_;
  ACE_Condition <ACE_Thread_Mutex> update_available_;
  ACE_Condition <ACE_Thread_Mutex> update_list_full_;
  
  OBJECTID_APPSET_MAP objectid_appset_map_;
  ACE_Recursive_Thread_Mutex appset_lock_;
  OBJECTID_RANKED_IOR_MAP objectid_rankedior_map_;
  STRING_TO_DOUBLE_MAP hostid_util_map_;
  STRING_TO_DOUBLE_MAP objectid_load_map_;
  STRING_TO_STRING_MAP processid_host_map_;

  RANKLIST_CONSTRAINT_MAP ranklist_constraints_;
  ACE_RW_Thread_Mutex constraint_lock_;

  STRING_TO_STRING_LIST_MAP processid_backup_map_;
  STRING_TO_STRING_LIST_MAP processid_primary_map_;
  STRING_TO_STRING_LIST_MAP hostid_process_map_;

  RankList rank_list_;
  RankList enhanced_rank_list_;
  AGENT_LIST agent_list_;
  ACE_RW_Thread_Mutex rank_list_mutex_;
  ACE_Thread_Mutex enhanced_rank_list_agent_list_combined_mutex_;
  STATE_SYNC_AGENT_MAP state_synchronization_agent_map_;
  ACE_Thread_Mutex state_sync_agent_list_mutex_;

  ACE_Thread_Mutex notify_mutex_;
  FLARE::NotificationId subscription_counter_;
  NOTIFICATION_MAP notify_subscriptions_;

  void update_enhanced_ranklist (void);
    
  void update_map (const char * key_str, 
                   const char * value_str,
                   STRING_TO_STRING_LIST_MAP & map);

  void update_util_map (const char * key_str, 
                        double value,
                        STRING_TO_DOUBLE_MAP & map);

  void update_appset_map (const char * key_str,
                          APP_INFO const & app_info, 
                          OBJECTID_APPSET_MAP &);
  
  void update_ior_map (ACE_CString const & oid,
                       std::priority_queue<UtilRank> const & rank_list);

  void update_proc_host_map (const char *pid, 
                             const char * hid,
                             STRING_TO_STRING_MAP & map);

  std::priority_queue <UtilRank> 
    util_sorted_host_list (ACE_CString const & oid, 
                           STRING_LIST const & host_list,
                           STRING_TO_DOUBLE_MAP const & hu_map);

  virtual int pulse (void);

  void move_update_list (ACE_DLList<MonitorUpdate> & source, 
                         ACE_DLList<MonitorUpdate> & dest);

  bool process_updates(ACE_DLList<MonitorUpdate> & update_list);
  
  void remove_process(ACE_CString const & pid, 
                  STRING_TO_STRING_LIST_MAP & map,
                  ACE_CString const & host,
                  Role role);

  void send_rank_list (void);
  
  void send_state_synchronization_rank_list (void);
  
  void build_rank_list (void);
  
  void app_reg(APP_INFO & app_info);
  
  void static_ranklist_update (const char * object_id, 
                               CORBA::Object_ptr ior,
                               Role role);
                               
  void process_proc_failure (ACE_CString const & process_id);
  
  STRING_LIST non_primary_host_list (ACE_CString const & primary_object_id);
  
  void replace_primary_tags (ACE_CString const & pid,
                             ACE_CString const & host);
                             
  void remove_from_appset (ACE_CString const & host,
                           ACE_CString const & pid, 
                           ACE_CString const & tag,
                           Role role);
                           
  void elevate_backup_to_primary (ACE_CString const & tag);
  
  void replace_backup_tags (ACE_CString const & pid,
                            ACE_CString const & host);
                            
  void copy_map (STRING_TO_DOUBLE_MAP const & source,
                 STRING_TO_DOUBLE_MAP & dest);
                 
  void print_queue (std::priority_queue <UtilRank> queue);

  void send_failure_notice (const char * host,
                            const ::FLARE::ApplicationList & object_ids);
};

#endif  /* REPLICATION_MANAGER_H */
