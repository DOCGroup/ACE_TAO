// cvs-id    : $Id$

#ifndef REPLICATION_MANAGER_H
#define REPLICATION_MANAGER_H

#include "LWFTS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Unbounded_Set.h"
#include "ace/Condition_T.h"
#include "ace/Containers_T.h"
#include "Timer.h"
#include "ace/Recursive_Thread_Mutex.h"

#include <list>
#include <queue>

typedef enum { PRIMARY=1, BACKUP=2 } Role;
typedef enum { PROCESS_LEVEL=1, PROCESSOR_LEVEL=2 } AlgoMode;

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
  APP_INFO (const char *oid, double l, const char *hname,
            const char *pid, Role r, CORBA::Object_ptr ref);
  APP_INFO (const char *oid, const char *hname,
            const char *pid, Role r);
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

  UtilRank ();
  UtilRank (UtilRank const & ur);
  UtilRank (double u, const char * hid);
};

bool operator < (UtilRank const & u1, UtilRank const & u2);

struct MonitorUpdate
{
    typedef enum { PROC_FAIL_UPDATE, HOST_UTIL_UPDATE, RUN_NOW, APP_REG } UpdateType;
    UpdateType type;

    ACE_CString process_id;
    ACE_CString host_id;
    double value;
    APP_INFO app_info;

    static MonitorUpdate * 
      create_proc_fail_update(const char * pid);
    static MonitorUpdate * 
      create_host_util_update(const char *hid, double value);
    static MonitorUpdate * 
      create_run_now_update();
    static MonitorUpdate * 
      create_app_info_update (const char *oid, double l, 
                              const char *hname, const char *pid, 
                              Role r, CORBA::Object_ptr ref);
};

class ReplicationManager_i : public virtual POA_ReplicationManager,
                             protected Timer
{
public:
  
  ReplicationManager_i (CORBA::ORB_ptr orb, double hertz, 
                        bool proactive = true, AlgoMode mode = PROCESS_LEVEL);

  ~ReplicationManager_i (void);

  virtual void register_application (const char *object_id,
                                     double load,
                                     const char *host_name,
                                     const char *process_id,
                                     CORBA::Short role,
                                     CORBA::Object_ptr server_reference
     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
     ACE_THROW_SPEC ((::CORBA::SystemException));

  void util_update (const char *host_id, 
                    double util
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));
  
  virtual void proc_failure (const char *process_id
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));
  
  virtual RankList * register_agent (CORBA::Object_ptr agent_reference
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual CORBA::Object_ptr get_next (const char * object_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

  bool replica_selection_algo ();

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
    RANKED_IOR_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECTID_RANKED_IOR_MAP;

  typedef ACE_Unbounded_Set<CORBA::Object_var> AGENT_LIST;
  
  enum { UPDATE_LIST_MAX_SIZE = 100 };
  
private:  
  
  CORBA::ORB_var orb_;
  Algorithm * algo_thread_;
  bool proactive_;
  AlgoMode mode_; 
    
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
  
  STRING_TO_STRING_LIST_MAP processid_backup_map_;
  STRING_TO_STRING_LIST_MAP processid_primary_map_;
  STRING_TO_STRING_LIST_MAP hostid_process_map_;
  
  RankList rank_list_;
  AGENT_LIST agent_list_;
  ACE_Thread_Mutex rank_list_agent_list_combined_mutex_;
  
  
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

  virtual int pulse();

  void move_update_list (ACE_DLList<MonitorUpdate> & source, 
                         ACE_DLList<MonitorUpdate> & dest);

  bool process_updates(ACE_DLList<MonitorUpdate> & update_list);
  void remove_process(ACE_CString const & pid, 
                  STRING_TO_STRING_LIST_MAP & map,
                  ACE_CString const & host,
                  Role role);

  void send_rank_list ();
  void build_rank_list ();
  void app_reg(APP_INFO & app_info);
  void static_ranklist_update (const char * object_id, 
                               CORBA::Object_ptr ior,
                               Role role);
  void process_proc_failure (ACE_CString const & process_id);
  STRING_LIST non_primary_host_list (ACE_CString const & primary_object_id);
  void replace_primary_tags (ACE_CString const & pid, ACE_CString const & host);
  void remove_from_appset (ACE_CString const & host, ACE_CString const & pid, 
                           ACE_CString const & tag, Role role);
  void elevate_backup_to_primary (ACE_CString const & tag);
  void replace_backup_tags (ACE_CString const & pid, ACE_CString const & host);
  void copy_map (STRING_TO_DOUBLE_MAP const & source, STRING_TO_DOUBLE_MAP & dest);
  void print_queue (std::priority_queue <UtilRank> queue);
  // RankList * rank_list_clone ();
};

/*
class ReplicationManager_i : public virtual POA_ReplicationManager,
                             protected Timer
{
public:

  using Timer::start;
  using Timer::stop;
  using Timer::hertz;

  ReplicationManager_i (CORBA::ORB_ptr orb);

  ~ReplicationManager_i (void);

// *************************************************************************

  virtual void register_application (const char *object_id,
                                     CORBA::Long load,
                                     const char *host_name,
                                     const char *process_id,
                                     CORBA::Short role,
                                     CORBA::Object_ptr server_reference
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));
  
  virtual void register_agent (CORBA::Object_ptr agent_reference
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual CORBA::Long next_member (const char *
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));
    
  virtual void util_update (const char *host_id, double utilization
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual void proc_failure (const char *process_id
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));


// *************************************************************************

  virtual int pulse (void);

  void update_replica_group_map (const char *object_id,
                                 const char *host_name,
                                 const char *process_id,
                                 CORBA::Object_ptr server_reference);

  void update_replica_list (CORBA::Object_ptr server_reference, 
                            const char *object_id);

  void update_replicas_list (CORBA::Object_ptr server_reference, 
                             const char *object_id);

  void update_primary_map (const char *process_id, const char *object_id);

  void update_failover_map (const char *process_id, const char *object_id);

  void update_secondary_map (const char *process_id, const char *object_id);

  void update_host_list (const char *object_id, const char *host_id);

  void update_object_primary_map (const char *process_id, 
                                   const char *host_id, 
                                   const char *object_id);

  void update_object_failover_map (const char *process_id, 
                                   const char *host_id, 
                                   const char *object_id);

  void update_object_secondary_map (const char *process_id, 
                                    const char *host_id, 
                                    const char *object_id);

// ***************************************************************************

  typedef ACE_Unbounded_Set<ACE_CString> PRIMARY_LIST;
  typedef ACE_Unbounded_Set<ACE_CString> FAILOVER_LIST;
  typedef ACE_Unbounded_Set<ACE_CString> SECONDARY_LIST;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    PRIMARY_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> PRIMARY_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    FAILOVER_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> FAILOVER_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    SECONDARY_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> SECONDARY_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    ACE_CString,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> PROCESS_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    CORBA::Object_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> REF_MAP;

  typedef ACE_Unbounded_Set<ACE_CString> HOST_LIST;

  typedef ACE_Unbounded_Set<CORBA::Object_ptr> AGENT_LIST;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    HOST_LIST,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> REPLICA_HOST_MAP;

  struct Replica_Group_Entry
  {
    PROCESS_MAP process_map;
    REF_MAP ref_map;
    HOST_LIST host_list;
  };

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    Replica_Group_Entry *,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> REPLICA_GROUP_MAP;

  typedef ACE_Hash_Map_Iterator_Ex <
    ACE_CString,
    Replica_Group_Entry *,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> REPLICA_GROUP_MAP_ITERATOR;

  struct Object_Info
  {
    ACE_CString process_id;
    ACE_CString host_id;
  };

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    Object_Info,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECT_PRIMARY_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    Object_Info,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECT_FAILOVER_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    Object_Info,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> OBJECT_SECONDARY_MAP;

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    double,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> HOST_UTIL_MAP;

private:

  CORBA::ORB_var orb_;
  TAO_SYNCH_MUTEX lock_;
  TAO_SYNCH_MUTEX primary_map_lock_;
  TAO_SYNCH_MUTEX failover_map_lock_;
  TAO_SYNCH_MUTEX secondary_map_lock_;
  TAO_SYNCH_MUTEX replica_group_map_lock_;
  TAO_SYNCH_MUTEX replica_host_map_lock_;
  TAO_SYNCH_MUTEX object_primary_map_lock_;
  TAO_SYNCH_MUTEX object_failover_map_lock_;
  TAO_SYNCH_MUTEX object_secondary_map_lock_;
  ReplicaList replica_list_;
  ReplicasList replicas_list_;
  PRIMARY_MAP primary_map_;
  FAILOVER_MAP failover_map_;
  SECONDARY_MAP secondary_map_;
  REPLICA_GROUP_MAP replica_group_map_;
  REPLICA_HOST_MAP replica_host_map_;
  OBJECT_PRIMARY_MAP object_primary_map_;
  OBJECT_FAILOVER_MAP object_failover_map_;
  OBJECT_SECONDARY_MAP object_secondary_map_;
  HOST_UTIL_MAP host_util_map_;
  AGENT_LIST agent_list_;

};
*/
#endif  /* REPLICATION_MANAGER_H */
