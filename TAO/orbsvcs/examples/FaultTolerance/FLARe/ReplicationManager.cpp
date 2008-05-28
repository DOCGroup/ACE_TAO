// cvs-id    : $Id$

#include "ReplicationManager.h"

#include <string>

#include "LWFTC.h"

template <class T>
void myswap (T & t1, T & t2)
{
  T temp(t1);
  t1 = t2;
  t2 = temp;
}

UtilRank::UtilRank ()
 : util(0) 
{}

UtilRank::UtilRank (double u, const char * hid)
  : util(u), 
    host_id (hid)
{}

UtilRank::UtilRank (UtilRank const & ur)
  : util(ur.util), 
    host_id (ur.host_id)
{}

bool operator < (UtilRank const & u1, UtilRank const & u2)
{
  return u1.util >= u2.util;
}

RANKED_IOR_LIST::
RANKED_IOR_LIST ()
  : now (false)
{}

APP_INFO::
APP_INFO() : load() {}

APP_INFO::
APP_INFO (APP_INFO const & app_info)
  : object_id (app_info.object_id.c_str()),
    load (app_info.load),
    host_name (app_info.host_name.c_str()),
    process_id (app_info.process_id.c_str()),
    role(app_info.role),
    ior(CORBA::Object::_duplicate(app_info.ior))
{}

APP_INFO::
APP_INFO (const char *oid, const char *hname,
          const char *pid, Role r)
  : object_id (oid),
    host_name (hname),
    process_id (pid),
    role(r)
{}

APP_INFO::
APP_INFO (const char *oid, double l, const char *hname,
          const char *pid, Role r, CORBA::Object_ptr ref)
  : object_id (oid),
    load (l),
    host_name (hname),
    process_id (pid),
    role(r),
    ior(CORBA::Object::_duplicate (ref))
{}

void APP_INFO::
swap (APP_INFO & app_info)
{
  this->object_id.swap (app_info.object_id);
  this->host_name.swap (app_info.host_name);
  this->process_id.swap (app_info.process_id);

  myswap (this->ior, app_info.ior);
  myswap (this->role, app_info.role);
  myswap (this->load, app_info.load);
}

APP_INFO & 
APP_INFO::
operator = (APP_INFO const & app_info)
{
  APP_INFO temp (app_info);
  temp.swap (*this);
  return *this;
}

bool 
operator == (APP_INFO const & lhs, APP_INFO const & rhs)
{
  return ((lhs.object_id == rhs.object_id) &&
          (lhs.host_name == rhs.host_name) &&
          (lhs.process_id == rhs.process_id) &&
          (lhs.role == rhs.role)); 
}

class Algorithm : public ACE_Task_Base
{
    ReplicationManager_i * rm_;
  public: 
    Algorithm (ReplicationManager_i * rm);
    virtual int svc (void);
};

Algorithm::Algorithm(ReplicationManager_i * rm)
  : rm_(rm)
{}

int Algorithm::svc (void)
{
  while (1)
    if (rm_->replica_selection_algo() == false)
        break;

  return 0;
}


/* ******************************************************************* */

ReplicationManager_i::ReplicationManager_i (CORBA::ORB_ptr orb, double hertz, 
                                            bool proactive, AlgoMode mode)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    algo_thread_(0),
    proactive_(proactive),
    mode_(mode),
    update_available_(update_mutex_),
    update_list_full_(update_mutex_)
{
  algo_thread_ = new Algorithm (this);
  algo_thread_->activate();
  this->Timer::hertz(hertz);
  this->Timer::start();
}

ReplicationManager_i::~ReplicationManager_i (void)
{
}

void ReplicationManager_i::
register_application (const char *object_id,
                      double load,
                      const char *host_id,
                      const char *process_id,
                      CORBA::Short role,
                      CORBA::Object_ptr server_reference)
{
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
  {  
    if(update_list_full_.wait(update_mutex_, &wait_time) == -1)  // timeout
    {
      ACE_DEBUG((LM_ERROR,"register_application CORBA upcall waited too long. Skipping \
            register_application. %s:%s:%s:%d.\n",host_id,process_id,object_id,role));
      return;
    }
  }
  
  update_list_.insert_tail (
      MonitorUpdate::create_app_info_update(object_id, load, host_id,
        process_id, Role(role), server_reference));
  update_available_.broadcast();
}


void ReplicationManager_i::
update_proc_host_map (const char *pid, 
                      const char * hid,
                      STRING_TO_STRING_MAP & map)
{
  ACE_CString proc_id (pid);
  ACE_CString host_id (hid);
  if (map.find (proc_id, host_id) != 0) // if not present
    {
      map.bind (proc_id, host_id);
    }
  else 
    {
      ACE_DEBUG((LM_ERROR,"Duplicate process_id=%s. Skipping it.\n",pid));
    }
}

void ReplicationManager_i::
update_appset_map (const char * key_str,
                   APP_INFO const & app_info, 
                   OBJECTID_APPSET_MAP & map)
{
  APP_SET app_set;
  ACE_CString key (key_str);
  
  if (map.find (key, app_set) != 0) // if not present
    {
      app_set.insert_tail (app_info);
      map.bind (key, app_set);
    }
  else 
    {
      app_set.insert_tail (app_info);
      map.rebind (key, app_set);
    }
}

void ReplicationManager_i::
update_map (const char * key_str, 
            const char * value_str, 
            STRING_TO_STRING_LIST_MAP & map)
{
  STRING_LIST slist;
  ACE_CString key (key_str);
  ACE_CString value (value_str);
  if (map.find (key, slist) != 0) // if not present
    {
      slist.insert_tail (value);
      map.bind (key, slist);
    }
  else 
    {
      slist.insert_tail (value);
      map.rebind (key, slist);
    }
}

void ReplicationManager_i::
update_util_map (const char * key_str, double value, STRING_TO_DOUBLE_MAP & map)
{
  double v = 0;
  if (map.find (key_str, v) != 0) // if not present
      map.bind (key_str, value);
  else 
      map.rebind (key_str, value);
}

void ReplicationManager_i::
move_update_list (ACE_DLList<MonitorUpdate> & source, 
                  ACE_DLList<MonitorUpdate> & dest)
{
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  while (update_list_.size() <= 0)
    update_available_.wait(update_mutex_);

  while (MonitorUpdate * up = source.delete_head())
    dest.insert_tail (up);
  
  update_list_full_.broadcast();
}

bool ReplicationManager_i::
process_updates(ACE_DLList<MonitorUpdate> & update_list)
{
  bool major_update = false;
  while(MonitorUpdate * up = update_list.delete_head())
  {
    switch(up->type)
    {
      case MonitorUpdate::RUN_NOW:
        {
          //ACE_DEBUG((LM_DEBUG,"RUN_NOW\n"));
          major_update = true;
          break;
        }
      case MonitorUpdate::HOST_UTIL_UPDATE:
        {
          //ACE_DEBUG((LM_DEBUG,"HOST_UTIL_UPDATE\n"));
          update_util_map(up->host_id.c_str(), up->value, this->hostid_util_map_);
          major_update = true;
          break;
        }
      case MonitorUpdate::PROC_FAIL_UPDATE: 
        {
          //ACE_DEBUG((LM_DEBUG,"PROC_FAIL_UPDATE\n"));
          process_proc_failure (up->process_id);
          major_update = true;
          break;
        }
      case MonitorUpdate::APP_REG:
        {
          //ACE_DEBUG((LM_DEBUG,"RUN_NOW\n"));
          app_reg(up->app_info);
          break;
        }
    }
    delete up;
  }
  return major_update;
}

void ReplicationManager_i::
process_proc_failure (ACE_CString const & process_id)
{
  ACE_CString host;
  if(this->processid_host_map_.find(process_id, host) == 0) // if present
  {
    replace_primary_tags(process_id, host);
    replace_backup_tags(process_id, host);
    this->processid_host_map_.unbind(process_id);
    
    STRING_LIST proc_list;
    if (this->hostid_process_map_.find(host,proc_list) == 0) // if present
    {
      proc_list.remove(process_id);
      this->hostid_process_map_.rebind(host, proc_list);
    }
    else
    {
      ACE_DEBUG((LM_ERROR,"Can't find host=%s in hostid_process_map. \
          Data structure invariant broken.\n",host.c_str()));
    }
  }
  else
  {
    ACE_DEBUG((LM_ERROR,"Can't find process_id=%s in proc_host_map. \
        Data structure invariant broken.\n",process_id.c_str()));
  }
}

void ReplicationManager_i::
replace_primary_tags (ACE_CString const & pid, ACE_CString const & host)
{
  STRING_LIST tag_list;
  if (this->processid_primary_map_.find(pid, tag_list) == 0)
  {
    for(STRING_LIST::iterator tl_iter = tag_list.begin();
        tl_iter != tag_list.end(); ++tl_iter)
    {
      remove_from_appset (host, pid, *tl_iter, PRIMARY);
      elevate_backup_to_primary (*tl_iter);
    }
    processid_primary_map_.unbind(pid);
  }
  // No worries for now if there are no primaries in the process.
}

void ReplicationManager_i::
remove_from_appset (ACE_CString const & host, ACE_CString const & pid, 
                    ACE_CString const & tag, Role role)
{
  APP_SET app_set;
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  if (this->objectid_appset_map_.find (tag, app_set) == 0)
  {
    APP_INFO app_info (tag.c_str(),host.c_str(),pid.c_str(), role);
    app_set.remove(app_info);
    this->objectid_appset_map_.rebind(tag,app_set);
    ACE_DEBUG((LM_DEBUG,"Removed application %s:%s:%s:%d.\n",
          host.c_str(),pid.c_str(),tag.c_str(),role));
  }
  else
  {
    ACE_DEBUG((LM_DEBUG,"Can't find appset for tag=%s.\n"
          "Data structure invariant broken.",tag.c_str() ));
  }
}

void ReplicationManager_i::
elevate_backup_to_primary (ACE_CString const & tag)
{
  RANKED_IOR_LIST ranked_ior_list;
  ACE_CString failover_host;
  
  if(this->objectid_rankedior_map_.find(tag,ranked_ior_list) == 0)
  {
    failover_host = ranked_ior_list.host_list.front();
    ACE_DEBUG((LM_DEBUG,"Failover host = %s.\n",failover_host.c_str()));
  }
  else
  {
    ACE_DEBUG((LM_DEBUG,"Can't find failover host for tag=%s.\n",tag.c_str()));
  }
  APP_SET app_set;
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  
  if (this->objectid_appset_map_.find (tag,app_set) == 0)
  {
    for(APP_SET::iterator as_iter = app_set.begin();
        as_iter != app_set.end();++as_iter)
      {
        if ((*as_iter).host_name == failover_host.c_str())
        {
          STRING_LIST backup_tag_list;
          
          if (this->processid_backup_map_.find((*as_iter).process_id, backup_tag_list) == 0)
          {
            backup_tag_list.remove(tag);
            this->processid_backup_map_.rebind((*as_iter).process_id, backup_tag_list);
            STRING_LIST primary_tag_list;
            
            if(this->processid_primary_map_.find(
                  (*as_iter).process_id, primary_tag_list) == 0)
            {
              primary_tag_list.insert_tail(tag);
              this->processid_primary_map_.rebind((*as_iter).process_id, primary_tag_list);
            }
            else
            {
              primary_tag_list.insert_tail(tag);
              this->processid_primary_map_.bind((*as_iter).process_id, primary_tag_list);
            }
          }
          else
          {
            ACE_DEBUG((LM_DEBUG,"Can't find backups for tag=%s in process=%s.\n"
                  "Data structure invariant broken\n",
                  tag.c_str(),(*as_iter).process_id.c_str()));
          }
          
          (*as_iter).role = PRIMARY;
          this->objectid_appset_map_.rebind(tag, app_set);
          break;
        }
      }
  }
  else
  {
    ACE_DEBUG((LM_DEBUG,"No process found hosting tag=%s.\n",tag.c_str()));
  }
}

void ReplicationManager_i::
replace_backup_tags (ACE_CString const & pid, ACE_CString const & host)
{
  STRING_LIST backup_tag_list;
  if (this->processid_backup_map_.find(pid, backup_tag_list) == 0)
  {
    for (STRING_LIST::iterator bt_iter = backup_tag_list.begin();
         bt_iter != backup_tag_list.end(); ++bt_iter)
    {
      remove_from_appset(host, pid, *bt_iter, BACKUP);
    }
    this->processid_backup_map_.unbind(pid);
  }
}

bool ReplicationManager_i::
replica_selection_algo () 
{
  ACE_DLList <MonitorUpdate> update_list;
  move_update_list(this->update_list_, update_list);
  
  if (process_updates(update_list) == true)
  {
    objectid_rankedior_map_.close();
    objectid_rankedior_map_.open();

    for (STRING_TO_STRING_LIST_MAP::iterator hp_iter = hostid_process_map_.begin ();
         hp_iter != hostid_process_map_.end (); ++hp_iter)
    {  
      STRING_TO_DOUBLE_MAP processor_level_host_util_map;;
      copy_map (this->hostid_util_map_, processor_level_host_util_map);
      STRING_LIST & process_list = (*hp_iter).item();
      
      for (STRING_LIST::iterator pl_iter = process_list.begin ();
           pl_iter != process_list.end (); ++pl_iter)
      {
        STRING_TO_DOUBLE_MAP process_level_host_util_map;
        copy_map (this->hostid_util_map_, process_level_host_util_map);
        STRING_TO_DOUBLE_MAP & host_util_map = 
           (this->mode_ == PROCESS_LEVEL)? process_level_host_util_map :
                                           processor_level_host_util_map;
        STRING_LIST primary_object_list;
        
        if (processid_primary_map_.find (*pl_iter, primary_object_list) == 0) // If present
        {
          for (STRING_LIST::iterator po_iter = primary_object_list.begin ();
               po_iter != primary_object_list.end (); ++po_iter)
          {
            STRING_LIST host_list = non_primary_host_list(*po_iter);
            
            if (host_list.size() >= 1) // if the object has backups.
            {
              std::priority_queue <UtilRank> util_ranked_queue = 
                util_sorted_host_list(*po_iter, host_list, host_util_map);
              //print_queue (util_ranked_queue);
              UtilRank ur (util_ranked_queue.top());
              host_util_map.rebind(ur.host_id.c_str(),ur.util);
              update_ior_map(*po_iter, util_ranked_queue);
            }
          }
        }
      }
    }
    
    build_rank_list ();
  }
  send_rank_list();
  return true;
}

void ReplicationManager_i::
print_queue (std::priority_queue <UtilRank> queue)
{
  while (!queue.empty())
  {
    UtilRank ur (queue.top());
    queue.pop();
    ACE_DEBUG((LM_DEBUG,"%s:%f, ",ur.host_id.c_str(),ur.util));
  }
  ACE_DEBUG((LM_DEBUG,"\n"));
}

void ReplicationManager_i::
copy_map (STRING_TO_DOUBLE_MAP const & source, STRING_TO_DOUBLE_MAP & dest)
{
  dest.close();
  dest.open();
  for(STRING_TO_DOUBLE_MAP::const_iterator iter (source.begin());
      iter != source.end(); ++iter)
  {
    dest.bind ((*iter).key().c_str(), (*iter).item());
    //ACE_DEBUG((LM_DEBUG, "%s:%f, ",(*iter).key().c_str(),(*iter).item()));
  }
  //ACE_DEBUG((LM_DEBUG,"\n"));
}

ReplicationManager_i::STRING_LIST ReplicationManager_i::
non_primary_host_list (ACE_CString const & primary_object_id)
{
  APP_SET app_set;
  STRING_LIST host_list;
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  if (objectid_appset_map_.find (primary_object_id, app_set) == 0)
  {
    for (APP_SET::iterator as_iter = app_set.begin();
         as_iter != app_set.end(); ++as_iter)
    {
      if ((*as_iter).role != PRIMARY)
        host_list.insert_tail((*as_iter).host_name);
    }
  }
  else
  {
    ACE_DEBUG((LM_DEBUG,"No processes for tag = %s.\n",primary_object_id.c_str()));
  }
  return host_list;
}


void ReplicationManager_i::
app_reg (APP_INFO & app)
{
  const char * object_id = app.object_id.c_str();
  double load = app.load;
  const char * host_name = app.host_name.c_str();
  const char * process_id = app.process_id.c_str();
  Role role = app.role;

  update_map (host_name, process_id, this->hostid_process_map_);
  update_proc_host_map (process_id, host_name, this->processid_host_map_);
  update_util_map (object_id, load, this->objectid_load_map_);
  
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  update_appset_map (object_id, app, this->objectid_appset_map_);

  switch (role)
  {
    case PRIMARY:
      {
        update_map (process_id, object_id, processid_primary_map_);
        break;
      }
    case BACKUP:
      {
        update_map (process_id, object_id, processid_backup_map_);
        static_ranklist_update(object_id, app.ior, role);
        break;
      }
    default:
      ACE_DEBUG((LM_ERROR,"Unknown Role!!\n"));
  }

  ACE_DEBUG((LM_DEBUG,"Registered successfully %s:%s:%s:%d with Replication manager.\n",
        host_name, process_id, object_id, role));
}

void ReplicationManager_i::
static_ranklist_update (const char * object_id, 
                        CORBA::Object_ptr ior,
                        Role role)
{
  ACE_Guard <ACE_Thread_Mutex> guard (rank_list_agent_list_combined_mutex_);
  bool found = false;
  size_t i = 0;
  for (i = 0; i < rank_list_.length(); ++i)
    if (rank_list_[i].object_id == object_id)
    {
      found = true;
      break;
    }
  if (found && (role == BACKUP))
  {
    size_t ior_list_length =  rank_list_[i].ior_list.length();
    rank_list_[i].ior_list.length(ior_list_length + 1);
    rank_list_[i].ior_list[ior_list_length] =
      CORBA::Object::_duplicate(ior);
  }
  else
  {
    size_t rl_length = rank_list_.length();
    rank_list_.length(rl_length + 1);
    rank_list_[rl_length].object_id = CORBA::string_dup(object_id);
    rank_list_[rl_length].now = false;
    size_t ior_list_length = rank_list_[rl_length].ior_list.length();
    rank_list_[rl_length].ior_list.length(ior_list_length + 1);
    rank_list_[rl_length].ior_list[ior_list_length] =
      CORBA::Object::_duplicate(ior);
  }
}

std::priority_queue <UtilRank> ReplicationManager_i::
util_sorted_host_list (ACE_CString const & oid, 
                       STRING_LIST const & host_list,
                       STRING_TO_DOUBLE_MAP const & hu_map)
{
  std::priority_queue <UtilRank> rank_list;
  double object_load;
  if (objectid_load_map_.find (oid, object_load) == 0)
  {
    for (STRING_LIST::const_iterator hl_iter = host_list.begin ();
         hl_iter != host_list.end (); ++hl_iter)
    {
      double host_utilization;
      if (hu_map.find (*hl_iter, host_utilization) == 0) // If present
      {
        double effective_load = object_load + host_utilization;
        rank_list.push(UtilRank(effective_load, (*hl_iter).c_str()));
      }
      else
      {
        ACE_DEBUG((LM_ERROR,"Can't find utilization of host_id=%s\n",(*hl_iter).c_str()));
        ACE_DEBUG((LM_ERROR,"Size of utilmap=%d\n",hu_map.current_size()));
        break;
      }
    }
  }
  else
      ACE_DEBUG((LM_ERROR,"Can't find load of object_id=%s\n",oid.c_str()));

  return rank_list;
}


void ReplicationManager_i::
build_rank_list ()
{
  ACE_Guard <ACE_Thread_Mutex> guard (rank_list_agent_list_combined_mutex_);
  this->rank_list_.length(0);
  for(OBJECTID_RANKED_IOR_MAP::iterator or_iter = 
      this->objectid_rankedior_map_.begin();
      or_iter != this->objectid_rankedior_map_.end(); ++or_iter)
  {
    CORBA::ULong rank_list_length = this->rank_list_.length();
    rank_list_.length(rank_list_length + 1);
    rank_list_[rank_list_length].object_id =
      CORBA::string_dup((*or_iter).key().c_str());
    RANKED_IOR_LIST & ranked_ior_list = (*or_iter).item();
    rank_list_[rank_list_length].now = ranked_ior_list.now;
    for (std::list<CORBA::Object_var>::iterator ior_iter =
        ranked_ior_list.ior_list.begin(); 
        ior_iter != ranked_ior_list.ior_list.end();
        ++ior_iter)
    {
      CORBA::ULong ior_list_length = rank_list_[rank_list_length].ior_list.length();
      rank_list_[rank_list_length].ior_list.length(ior_list_length + 1);
      rank_list_[rank_list_length].ior_list[ior_list_length] =
        CORBA::Object::_duplicate(*ior_iter);
    }
  }
}

void ReplicationManager_i::
send_rank_list()
{
  if (! proactive_)
    return;
  //ACE_DEBUG((LM_DEBUG,"Sending rank list.\n"));

  ACE_Guard <ACE_Thread_Mutex> guard (rank_list_agent_list_combined_mutex_);
  for (AGENT_LIST::iterator al_iter = agent_list_.begin();
       al_iter != agent_list_.end(); ++ al_iter)
  {
    Agent_var agent = Agent::_narrow (*al_iter);
    try 
    {
       agent->update_rank_list(this->rank_list_);
    }
    catch (CORBA::SystemException &)
    {
      ACE_DEBUG ((LM_DEBUG,"An agent died.\n"));
    }
  }
}

void ReplicationManager_i::update_ior_map (
  ACE_CString const & oid,
  std::priority_queue<UtilRank> const & rl)
{
  std::priority_queue <UtilRank> rank_list (rl);
  APP_SET app_set;
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  
  if (objectid_appset_map_.find (oid, app_set) == 0) // if present
  {
    RANKED_IOR_LIST ranked_ior_list;
    
    while (!rank_list.empty ())
    {
      UtilRank ur = rank_list.top ();
      rank_list.pop ();
      
      for (APP_SET::iterator as_iter = app_set.begin();
           as_iter != app_set.end();
           ++as_iter)
      {
        if (ur.host_id == (*as_iter).host_name.c_str()) 
        {
          ranked_ior_list.ior_list.push_back((*as_iter).ior);
          ranked_ior_list.host_list.push_back(
              ACE_CString(ur.host_id.c_str()));
          app_set.remove(*as_iter);
          break;
        }
      }
    }  
    RANKED_IOR_LIST temp_ior_list;
    
    if (objectid_rankedior_map_.find (oid, temp_ior_list) != 0) // if not present
        objectid_rankedior_map_.bind (oid, ranked_ior_list);
    else 
        objectid_rankedior_map_.rebind (oid, ranked_ior_list);
  }
  else
  {
    ACE_DEBUG ((LM_ERROR, "Objectid=%s not present in APP_SET\n",oid.c_str()));
  }
}

void ReplicationManager_i::
proc_failure (const char *process_id)
{
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
  {
    if(update_list_full_.wait(update_mutex_, &wait_time) == -1)  // timeout
    {
      ACE_DEBUG((LM_ERROR,"proc_failure CORBA upcall waited too long. Skipping \
            proc_failure update. process_id=%s\n",process_id));
      return;
    }
  }
  update_list_.insert_tail (MonitorUpdate::create_proc_fail_update(process_id));
  update_available_.broadcast();
}

void ReplicationManager_i::
util_update (const char *host_id, 
             double util)
{
  //ACE_DEBUG ((LM_DEBUG, "Update from %s with UTIL %d\n", host_id, (int)util));
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
  {  
    if(update_list_full_.wait(update_mutex_, &wait_time) == -1)  // timeout
    {
      ACE_DEBUG((LM_ERROR,"util_update CORBA upcall waited too long. Skipping \
            util_update. host_id=%s,util=%l\n",host_id,util));
      return;
    }
  }
  update_list_.insert_tail (
      MonitorUpdate::create_host_util_update(host_id,util));
  update_available_.broadcast();
}

int
ReplicationManager_i::pulse (void)
{
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  ACE_Time_Value wait_time (5);
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
  {  
    if(update_list_full_.wait(update_mutex_, &wait_time) == -1)  // timeout
    {
      ACE_DEBUG((LM_ERROR,"pulse waited too long. Skipping pulse.\n"));
      return 0;
    }
  }
  update_list_.insert_tail (
      MonitorUpdate::create_run_now_update());
  update_available_.broadcast();
  
  return 0;
}

RankList * ReplicationManager_i::
register_agent (CORBA::Object_ptr agent_reference)
{
  ACE_DEBUG ((LM_DEBUG, "register_agent called\n"));
  Agent_var agent = Agent::_narrow (agent_reference);
 
  ACE_Guard <ACE_Thread_Mutex> guard (rank_list_agent_list_combined_mutex_);
  this->agent_list_.insert_tail (CORBA::Object::_duplicate (agent.in ()));
  return new RankList (rank_list_);
}

CORBA::Object_ptr ReplicationManager_i::
get_next (const char * object_id)
{
/*  if (proactive_)
    ACE_DEBUG((LM_DEBUG,"Replication Manager is in proactive mode.\n"));
 
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  APP_SET app_set;
  
  if (this->objectid_appset_map_.find (object_id,app_set) == 0)
  {
    for (APP_SET::iterator as_iter = app_set.begin();
         as_iter != app_set.end(); ++as_iter)
    {
      if ((*as_iter).role == PRIMARY)
      {
        return CORBA::Object::_duplicate((*as_iter).ior.in());
      }
    }
  }
  else
  {
    ACE_DEBUG((LM_DEBUG,"No more replicas for tag = %s.\n",object_id));
  }
  */
  return 0;
}
/*
RankList * ReplicationManager_i::
rank_list_clone ()
{
  RankList * rl = new RankList;
  rl->length(this->rank_list_.length());
  for (size_t i = 0;i < rank_list_.length(); ++i)
  {
    (*rl)[i].object_id = CORBA::string_dup(rank_list_[i].object_id);
    (*rl)[i].now = rank_list_[i].now;
    (*rl)[i].ior_list.length(rank_list_[i].ior_list.length());
    ACE_DEBUG((LM_DEBUG,"ior_list size = %d.\n",rank_list_[i].ior_list.length()));
    for (size_t j = 0;j < rank_list_[i].ior_list.length(); ++j)
    {
      (*rl)[i].ior_list[j] = CORBA::Object::_duplicate(rank_list_[i].ior_list[j]);
    }
  }
  return rl;
}
*/
MonitorUpdate * MonitorUpdate::
create_proc_fail_update(const char * pid)
{
  ACE_Auto_Ptr<MonitorUpdate> up(new MonitorUpdate());
  up->process_id = pid;
  up->type = MonitorUpdate::PROC_FAIL_UPDATE;
  return up.release();
}

MonitorUpdate * MonitorUpdate::
create_host_util_update(const char *hid, double value)
{
  ACE_Auto_Ptr <MonitorUpdate> up (new MonitorUpdate());
  up->host_id = hid;
  up->value = value;
  up->type = MonitorUpdate::HOST_UTIL_UPDATE;
  return up.release();
}

MonitorUpdate * MonitorUpdate::
create_run_now_update()
{
  ACE_Auto_Ptr <MonitorUpdate> up (new MonitorUpdate());
  up->type = MonitorUpdate::RUN_NOW;
  return up.release();
}


MonitorUpdate * MonitorUpdate::
create_app_info_update (const char *oid, double l, 
                        const char *hname, const char *pid, 
                        Role r, CORBA::Object_ptr ref)
{
  ACE_Auto_Ptr <MonitorUpdate> up (new MonitorUpdate());
  up->type = MonitorUpdate::APP_REG;
  up->app_info = APP_INFO (oid, l, hname, pid, r, ref);
  return up.release();
}









/**********************************************************************/
/*
ReplicationManager_i::ReplicationManager_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    lock_ (),
    primary_map_lock_ (),
    failover_map_lock_ (),
    secondary_map_lock_ (),
    replica_group_map_lock_ (),
    replica_host_map_lock_ (),
    object_primary_map_lock_ (),
    object_failover_map_lock_ (),
    object_secondary_map_lock_ (),
    replica_list_ (0),
    replicas_list_ (0),
    primary_map_ (100),
    failover_map_ (100),
    secondary_map_ (100),
    replica_group_map_ (100),
    replica_host_map_ (100),
    object_primary_map_ (100),
    object_failover_map_ (100),
    object_secondary_map_ (100),
    host_util_map_ (100),
    agent_list_ (0)
{
}

ReplicationManager_i::~ReplicationManager_i (void)
{
}

CORBA::Long ReplicationManager_i::next_member  (const char * object_id)
{
  ACE_DEBUG ((LM_DEBUG, "register_agent called\n"));
  return 0;
}

void
ReplicationManager_i::register_agent (CORBA::Object_ptr agent_reference)
{
  ACE_DEBUG ((LM_DEBUG, "register_agent called\n"));
  Agent_var agent = Agent::_narrow (agent_reference);

  this->agent_list_.insert_tail (CORBA::Object::_duplicate (agent.in ()));
  agent->initialize_agent (this->replica_list_, this->replicas_list_);
}

void
ReplicationManager_i::update_primary_map (const char *process_id, 
                                          const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update primary map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->primary_map_lock_);

  PRIMARY_LIST primary_list;
  ACE_CString process_name = CORBA::string_dup (process_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  if (this->primary_map_.find (process_name, primary_list) != 0)
    {
      primary_list.insert_tail (object_name);
      this->primary_map_.bind (process_name, primary_list);
    }
  else if (this->primary_map_.find (process_name, primary_list) == 0)
    {
      primary_list.insert_tail (object_name);
      this->primary_map_.rebind (process_name, primary_list);
    }
}

void
ReplicationManager_i::update_failover_map (const char *process_id, 
                                           const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update failover map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->failover_map_lock_);

  FAILOVER_LIST failover_list;
  ACE_CString process_name = CORBA::string_dup (process_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  if (this->failover_map_.find (process_name, failover_list) != 0)
    {
      failover_list.insert_tail (object_name);
      this->failover_map_.bind (process_name, failover_list);
    }
  else if (this->failover_map_.find (process_name, failover_list) == 0)
    {
      failover_list.insert_tail (object_name);
      this->failover_map_.rebind (process_name, failover_list);
    }
}

void
ReplicationManager_i::update_object_primary_map (const char *process_id,
  const char *host_id, const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update object primary map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->object_primary_map_lock_);

  ACE_CString process_name = CORBA::string_dup (process_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  ACE_CString host_name = CORBA::string_dup (host_id);
  Object_Info object_info;
  object_info.process_id = process_name;
  object_info.host_id = host_name;
  this->object_primary_map_.bind (object_id, object_info); 
}

void
ReplicationManager_i::update_object_secondary_map (const char *process_id,
  const char *host_id, const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update object secondary map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->object_secondary_map_lock_);

  ACE_CString process_name = CORBA::string_dup (process_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  ACE_CString host_name = CORBA::string_dup (host_id);
  // this->object_secondary_map_.bind (object_name, process_name);
  Object_Info object_info;
  object_info.process_id = process_name;
  object_info.host_id = host_name;
  this->object_secondary_map_.bind (object_id, object_info);
}

void
ReplicationManager_i::update_object_failover_map (const char *process_id,
  const char *host_id, const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update object failover map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->object_failover_map_lock_);

  ACE_CString process_name = CORBA::string_dup (process_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  ACE_CString host_name = CORBA::string_dup (host_id);
  // this->object_failover_map_.bind (object_name, process_name);
  Object_Info object_info;
  object_info.process_id = process_name;
  object_info.host_id = host_name;
  this->object_failover_map_.bind (object_id, object_info);
}

void
ReplicationManager_i::update_secondary_map (const char *process_id, 
                                            const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update secondary map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->secondary_map_lock_);

  SECONDARY_LIST secondary_list;
  ACE_CString process_name = CORBA::string_dup (process_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  if (this->secondary_map_.find (process_name, secondary_list) != 0)
    {
      secondary_list.insert_tail (object_name);
      this->secondary_map_.bind (process_name, secondary_list);
    }
  else if (this->secondary_map_.find (process_name, secondary_list) == 0)
    {
      secondary_list.insert_tail (object_name);
      this->secondary_map_.rebind (process_name, secondary_list);
    }
}

void
ReplicationManager_i::update_replica_list (CORBA::Object_ptr server_reference,
  const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update replica list called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  CORBA::ULong replica_list_length = this->replica_list_.length ();
  this->replica_list_.length (replica_list_length + 1);
  this->replica_list_[replica_list_length].object_id = object_id;
  this->replica_list_[replica_list_length].next_member =
    CORBA::Object::_duplicate (server_reference);
}

void
ReplicationManager_i::update_replicas_list (CORBA::Object_ptr server_reference,
  const char *object_id)
{
  // ACE_DEBUG ((LM_DEBUG, "update replicas list called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);
  CORBA::ULong replicas_list_length = this->replicas_list_.length ();
  this->replicas_list_.length (replicas_list_length + 1);
  this->replicas_list_[replicas_list_length].object_id = object_id;
  this->replicas_list_[replicas_list_length].next_members =
    CORBA::Object::_duplicate (server_reference);
}

void
ReplicationManager_i::update_replica_group_map (const char *object_id,
  const char *host_id, const char *process_id, CORBA::Object_ptr server_ref)
{
  // ACE_DEBUG ((LM_DEBUG, "update replica group map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->replica_group_map_lock_);

  Replica_Group_Entry *replica_group_entry = 0;
  CORBA::Object_var replica_ref;
  ACE_CString obj_id = CORBA::string_dup (object_id);
  ACE_CString host_name = CORBA::string_dup (host_id);
  ACE_CString process_name = CORBA::string_dup (process_id);

  if (this->replica_group_map_.find (obj_id, replica_group_entry) == 0)
    {
      if (replica_group_entry->process_map.find (host_name, process_name) != 0)
        {
          replica_group_entry->process_map.bind (host_name, process_name);
        }
      if (replica_group_entry->ref_map.find (host_name, replica_ref) != 0)
        {
          replica_group_entry->ref_map.bind (host_name,
            CORBA::Object::_duplicate (server_ref));
        }
      replica_group_entry->host_list.insert_tail (host_name);
    }
  else
    {
      ACE_NEW_THROW_EX (replica_group_entry, Replica_Group_Entry,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID, ENOMEM), CORBA::COMPLETED_NO));
      if (replica_group_entry->process_map.find (host_name, process_name) != 0)
        {
          replica_group_entry->process_map.bind (host_name, process_name);
        }
      if (replica_group_entry->ref_map.find (host_name, replica_ref) != 0)
        {
          replica_group_entry->ref_map.bind (host_name,
            CORBA::Object::_duplicate (server_ref));
        }
      replica_group_entry->host_list.insert_tail (host_name);
    }
}

void
ReplicationManager_i::register_application (const char *object_id,
                                            CORBA::Long load,
                                            const char *host_id,
                                            const char *process_id,
                                            CORBA::Short role,
                                            CORBA::Object_ptr server_reference)
{
  ACE_DEBUG ((LM_DEBUG, "register application called process_id = %s, \
        object_id = %s, role = %d\n", process_id, object_id, role));

  this->update_replica_group_map (object_id, host_id, process_id,
                                  server_reference);

  if (role == 1)
    {
      this->update_primary_map (process_id, object_id);
      this->update_object_primary_map (process_id, host_id, object_id);
    }
  else if (role == 2)
    {
      this->update_failover_map (process_id, object_id);
      this->update_replica_list (server_reference, object_id);
      this->update_host_list (object_id, host_id);
      this->update_object_failover_map (process_id, host_id, object_id);
    }
  else if (role == 3)
    {
      this->update_secondary_map (process_id, object_id);
      this->update_replicas_list (server_reference, object_id);
      this->update_host_list (object_id, host_id);
      this->update_object_secondary_map (process_id, host_id, object_id);
    }
}

void
ReplicationManager_i::update_host_list (const char *object_id,
                                        const char *host_id)
{
  ACE_DEBUG ((LM_DEBUG, "update host list called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->replica_host_map_lock_);

  HOST_LIST host_list;
  ACE_CString host_name = CORBA::string_dup (host_id);
  ACE_CString object_name = CORBA::string_dup (object_id);
  if (this->replica_host_map_.find (object_name, host_list) == 0)
    {
      host_list.insert_tail (host_name);
      this->replica_host_map_.rebind (object_name, host_list);
    }
  else
    {
      host_list.insert_tail (host_name);
      this->replica_host_map_.bind (object_name, host_list);
    }
}

void
ReplicationManager_i::util_update (const char *host_id, double util)
{
  //ACE_DEBUG ((LM_DEBUG, "Update from %s with UTIL %l\n", host_id, util));
  double current_util;
  
  if (this->host_util_map_.find (host_id, current_util) == 0)
    {
      this->host_util_map_.rebind (host_id, util);
    }
  else
    {
      this->host_util_map_.bind (host_id, util);
    }
}

void
ReplicationManager_i::proc_failure (const char *process_id)
{
  ACE_DEBUG ((LM_DEBUG, "Update from Monitor that %s failed\n", process_id));

  // ACE_DEBUG ((LM_DEBUG, "update failover map called\n"));
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->failover_map_lock_);

  ACE_CString process_name = CORBA::string_dup (process_id);
  FAILOVER_LIST failover_list;
  
  if (this->failover_map_.find (process_name, failover_list) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Update from Monitor that %s failed\n", process_id));

      FailoverList fail_list;
      fail_list.length (0);
      CORBA::ULong fail_list_length;

      for (FAILOVER_LIST::iterator iter = failover_list.begin ();
           iter != failover_list.end (); ++iter)
        {
          ACE_CString object_id = *iter;
          char *object_name = CORBA::string_dup (object_id.c_str ());
          fail_list_length = fail_list.length ();
          fail_list.length (fail_list_length + 1);
          fail_list[fail_list_length] = object_name;
        }
        
      ACE_DEBUG ((LM_DEBUG, "Update from Monitor that %s failed\n", process_id));

      for (AGENT_LIST::iterator it = this->agent_list_.begin ();
            it != this->agent_list_.end (); ++it)
        {
          CORBA::Object_var agent_ref = *it;
          Agent_var agent = Agent::_narrow (agent_ref);
          agent->update_failover_list (fail_list);
        }
        
      ACE_DEBUG ((LM_DEBUG, "Update from Monitor that %s failed\n", process_id));
    }
    
  ACE_DEBUG ((LM_DEBUG, "END Update from Monitor that %s failed\n", process_id));
}

int
ReplicationManager_i::pulse (void)
{
  // ACE_DEBUG ((LM_DEBUG, "Timer wake-up\n"));
  for (REPLICA_GROUP_MAP_ITERATOR it = this->replica_group_map_.begin ();
       it != this->replica_group_map_.end (); ++it)
    {
      ACE_CString object_id = (*it).ext_id_;
      Replica_Group_Entry *replica_group_entry = (*it).int_id_;

      Object_Info primary_object_info;
      Object_Info failover_object_info;
      Object_Info secondary_object_info;
      this->object_primary_map_.find (object_id, primary_object_info);
      this->object_failover_map_.find (object_id, failover_object_info);
      this->object_secondary_map_.find (object_id, secondary_object_info);

      ACE_CString primary_host_id = primary_object_info.host_id;
      ACE_CString failover_host_id = failover_object_info.host_id;
      ACE_CString secondary_host_id = secondary_object_info.host_id;

      ACE_CString primary_process_id = primary_object_info.process_id;
      ACE_CString failover_process_id = failover_object_info.process_id;
      ACE_CString secondary_process_id = secondary_object_info.process_id;

      double primary_util;
      double failover_util;
      double secondary_util;
      this->host_util_map_.find (primary_host_id, primary_util);
      this->host_util_map_.find (failover_host_id, failover_util);
      this->host_util_map_.find (secondary_host_id, secondary_util);

      if (secondary_util < failover_util)
        {
          // change of failover
        }
    }
  return 0;
}

*/
