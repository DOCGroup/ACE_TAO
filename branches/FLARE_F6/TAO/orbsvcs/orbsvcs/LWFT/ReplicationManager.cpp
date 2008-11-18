// -*- C++ -*-
// $Id$

#include "ReplicationManager.h"

#include <string>

#include "ace/OS_NS_unistd.h"

#include "ForwardingAgentC.h"

template <class T>
void myswap (T & t1, T & t2)
{
  T temp (t1);
  t1 = t2;
  t2 = temp;
}

UtilRank::UtilRank (void)
  : util (0) 
{
}

UtilRank::UtilRank (double u, const char * hid)
  : util(u), 
    host_id (hid)
{
}

UtilRank::UtilRank (UtilRank const & ur)
  : util(ur.util), 
    host_id (ur.host_id)
{
}

bool operator < (UtilRank const & u1, UtilRank const & u2)
{
  return u1.util >= u2.util;
}

RANKED_IOR_LIST::RANKED_IOR_LIST (void)
  : now (false)
{
}

APP_INFO::APP_INFO (void)
  : load ()
{
}

APP_INFO::APP_INFO (APP_INFO const & app_info)
  : object_id (app_info.object_id.c_str ()),
    load (app_info.load),
    host_name (app_info.host_name.c_str ()),
    process_id (app_info.process_id.c_str ()),
    role (app_info.role),
    ior (CORBA::Object::_duplicate (app_info.ior))
{}

APP_INFO::APP_INFO (const char *oid,
                    const char *hname,
                    const char *pid,
                    Role r)
  : object_id (oid),
    host_name (hname),
    process_id (pid),
    role (r)
{
}

APP_INFO::APP_INFO (const char *oid,
                    double l,
                    const char *hname,
                    const char *pid,
                    Role r,
                    CORBA::Object_ptr ref)
  : object_id (oid),
    load (l),
    host_name (hname),
    process_id (pid),
    role (r),
    ior (CORBA::Object::_duplicate (ref))
{
}

void
APP_INFO::swap (APP_INFO & app_info)
{
  this->object_id.swap (app_info.object_id);
  this->host_name.swap (app_info.host_name);
  this->process_id.swap (app_info.process_id);

  myswap (this->ior, app_info.ior);
  myswap (this->role, app_info.role);
  myswap (this->load, app_info.load);
}

APP_INFO & 
APP_INFO::operator = (APP_INFO const & app_info)
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

Algorithm::Algorithm (ReplicationManager_i * rm)
  : rm_ (rm)
{
}

int Algorithm::svc (void)
{
  while (true)
    {
      if (! rm_->replica_selection_algo ())
        {
          break;
        }
    }

  return 0;
}


/* ******************************************************************* */

ReplicationManager_i::ReplicationManager_i (CORBA::ORB_ptr orb,
                                            double hertz, 
                                            bool proactive,
                                            bool static_mode,
					                                  AlgoMode mode)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    algo_thread_(0),
    proactive_(proactive),
    mode_(mode),
    static_mode_ (static_mode),
    update_available_(update_mutex_),
    update_list_full_(update_mutex_)
{
  algo_thread_ = new Algorithm (this);
  algo_thread_->activate ();
  this->Timer::hertz (hertz);
  this->Timer::start ();
}

ReplicationManager_i::~ReplicationManager_i (void)
{
}

void
ReplicationManager_i::register_application (
  const char *object_id,
  double load,
  const char *host_id,
  const char *process_id,
  CORBA::Short role,
  CORBA::Object_ptr server_reference)
{
  ACE_Guard <ACE_Thread_Mutex> guard (update_mutex_);
  
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
    {  
      if(update_list_full_.wait (update_mutex_, &wait_time) == -1)  // timeout
        {
          ACE_DEBUG ((LM_ERROR,
                      "register_application CORBA upcall "
                      "waited too long. Skipping"
                      "register_application. %s:%s:%s:%d.\n",
                      host_id,
                      process_id,
                      object_id,
                      role));
          return;
        }
    }
  
  update_list_.insert_tail (
      MonitorUpdate::create_app_info_update (object_id,
                                             load,
                                             host_id,
                                             process_id,
                                             Role (role),
                                             server_reference));
  update_available_.broadcast ();
}

void
ReplicationManager_i::update_proc_host_map (
  const char *pid, 
  const char * hid,
  STRING_TO_STRING_MAP & map)
{
  ACE_CString proc_id (pid);
  ACE_CString host_id (hid);
  
  // If not present.l.
  if (map.find (proc_id, host_id) != 0)
    {
      map.bind (proc_id, host_id);
    }
  else 
    {
      ACE_DEBUG ((LM_ERROR,
                  "Duplicate process_id=%s. Skipping it.\n",
                  pid));
    }
}

void
ReplicationManager_i::update_appset_map (
  const char * key_str,
  APP_INFO const & app_info, 
  OBJECTID_APPSET_MAP & map)
{
  APP_SET app_set;
  ACE_CString key (key_str);
  
  // If not present... 
  if (map.find (key, app_set) != 0)
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

void
ReplicationManager_i::update_map (
  const char * key_str, 
  const char * value_str, 
  STRING_TO_STRING_LIST_MAP & map)
{
  STRING_LIST slist;
  ACE_CString key (key_str);
  ACE_CString value (value_str);
  
  // If not present...
  if (map.find (key, slist) != 0)
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

void
ReplicationManager_i::update_util_map (
  const char * key_str,
  double value,
  STRING_TO_DOUBLE_MAP & map)
{
  double v = 0;
 
  // If not present...
  if (map.find (key_str, v) != 0)
    {
      map.bind (key_str, value);
    }
  else
    {
      map.rebind (key_str, value);
    }
}

void
ReplicationManager_i::move_update_list (
  ACE_DLList<MonitorUpdate> & source, 
  ACE_DLList<MonitorUpdate> & dest)
{
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  
  while (update_list_.size() <= 0)
    {
      update_available_.wait (update_mutex_);
    }

  while (MonitorUpdate * up = source.delete_head ())
    {
      dest.insert_tail (up);
    }
  
  update_list_full_.broadcast ();
}

bool
ReplicationManager_i::process_updates (
  ACE_DLList<MonitorUpdate> & update_list)
{
  bool major_update = false;
  
  while (MonitorUpdate * up = update_list.delete_head ())
    {
      switch (up->type)
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
              update_util_map (up->host_id.c_str (),
                               up->value,
                               this->hostid_util_map_);
                               
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
              app_reg (up->app_info);
              break;
            }
        }
        
      delete up;
    }
  
  return major_update;
}

void
ReplicationManager_i::process_proc_failure (
  ACE_CString const & process_id)
{
  if (static_mode_)
    {
      // give the client time to failover
      ACE_OS::sleep (1);

      ACE_DEBUG ((LM_TRACE,
                  "removing failed app ior from rank list.\n"));
                  
      ACE_Guard <ACE_Thread_Mutex> protect (pid_object_mutex_);

      // Find object id for the process id.
      CORBA::Object_var app;
      
      if (pid_object_map_.find (process_id, app) == 0)
        {
          pid_object_map_.unbind (process_id);

          // copy each object ior list and leave out the removed application
          for (size_t i = 0; i < rank_list_.length (); ++i)
            {
              try
                {
                  // Index of the element that should be removed from 
                  // the ior list. This is either the failed replica
                  // or if the primary failed, the first entry in the list
                  size_t rm_index = 0;
                  
                  size_t len_i = rank_list_[i].ior_list.length ();

                  for (size_t j = 0; j < len_i; ++j)
                    {
                      if (app->_is_equivalent (
                            rank_list_[i].ior_list[j].in ()))
                        {
                          rm_index = j;
                          break;
                        }
                    }
                  
                  // We're using len_i - 1 below, and it *must*
                  // be an unsigned type, so we check for 0 here
                  // to avoid unexpected behavior caused by an
                  // overflow in the FOR loop condition below.
                  if (len_i == 0)
                    {
                      continue;
                    }

                  // Now remove the correct element from the list
                  for (size_t k = rm_index; k < len_i - 1; ++k)
                    {
                      // Move each following element one position 
                      // forward.
                      rank_list_[i].ior_list[k] = 
                      rank_list_[i].ior_list[k+1];
                    }
                  
                  // correct the length value
                  rank_list_[i].ior_list.length (
                    rank_list_[i].ior_list.length () - 1);

		  // remove the entry from the app_set
		  APP_SET as;
		  if (objectid_appset_map_.find (rank_list_[i].object_id.in (),
						 as) == 0)
		    {
		      APP_INFO info = *(as.begin ());
		      info.object_id = rank_list_[i].object_id.in ();
		      info.process_id = process_id;
		      info.role = BACKUP;

		      if (as.remove (info) != 0)
			{
			  info.role = PRIMARY;

			  if (as.remove (info) != 0)
			    {
			      ACE_DEBUG ((LM_ERROR, "RM::process_proc_failure could not find "
					  "element to remove from app_set.\n"));
			    }
			  else
			    {
			      // select the new primary
			      for (APP_SET::iterator it = as.begin ();
				   it != as.end ();
				   ++it)
				{
				  // compare the object id to the
				  // first element in the rank list
				  // and mark it as primary.
				  
				  if (rank_list_[i].ior_list[0]->_is_equivalent (
                                        (*it).ior.in ()))
				    {
				      (*it).role = PRIMARY;
				      break;
				    }
				}
			    }
			}

		      ACE_DEBUG ((LM_DEBUG, "RM::process_proc_failure processed %s-%d@%s.\n",
				  info.object_id.c_str (),
				  info.role,
				  info.process_id.c_str ()));			  
		    }
                }
              catch (const CORBA::SystemException & ex)
                {
                  // Just make sure to keep on going for the other 
                  // entries here.
                  ACE_DEBUG ((LM_ERROR, 
                              "in process_proc_update: %d\n", 
                              ex._info ().c_str ()));
                }
            } // end for loop

	  this->update_enhanced_ranklist ();
        } // end find
    }
  else // not in static_mode_
    {
      ACE_CString host;
          
      // If present...
      if (this->processid_host_map_.find (process_id, host) == 0)
        {
          replace_primary_tags (process_id, host);
          replace_backup_tags (process_id, host);
          this->processid_host_map_.unbind (process_id);
          
          STRING_LIST proc_list;
          
          if (this->hostid_process_map_.find (host,proc_list) == 0) // if present
            {
              proc_list.remove (process_id);
              this->hostid_process_map_.rebind (host, proc_list);
            }
          else
            {
              ACE_DEBUG((LM_ERROR,"Can't find host=%s in hostid_process_map. \
                Data structure invariant broken.\n",host.c_str()));
            }
        }
      else
        {
          ACE_DEBUG ((LM_ERROR,
                      "Can't find process_id=%s in proc_host_map."
                      " Data structure invariant broken.\n",
                      process_id.c_str ()));
        }
    }
}

void
ReplicationManager_i::replace_primary_tags (
  ACE_CString const & pid,
  ACE_CString const & host)
{
  STRING_LIST tag_list;
  
  if (this->processid_primary_map_.find(pid, tag_list) == 0)
    {
      for (STRING_LIST::iterator tl_iter = tag_list.begin ();
           tl_iter != tag_list.end ();
           ++tl_iter)
      {
        remove_from_appset (host, pid, *tl_iter, PRIMARY);
        elevate_backup_to_primary (*tl_iter);
      }
      
      processid_primary_map_.unbind (pid);
    }
  // No worries for now if there are no primaries in the process.
}

void
ReplicationManager_i::remove_from_appset (
  ACE_CString const & host,
  ACE_CString const & pid, 
  ACE_CString const & tag,
  Role role)
{
  APP_SET app_set;
  
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (
    this->appset_lock_);
    
  if (this->objectid_appset_map_.find (tag, app_set) == 0)
    {
      APP_INFO app_info (tag.c_str (),
                         host.c_str (),
                         pid.c_str(),
                         role);
                         
      app_set.remove (app_info);
      
      this->objectid_appset_map_.rebind(tag,app_set);
      
      ACE_DEBUG ((LM_DEBUG,
                  "Removed application %s:%s:%s:%d.\n",
                  host.c_str (),
                  pid.c_str (),
                  tag.c_str (),
                  role));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Can't find appset for tag=%s.\n"
                  "Data structure invariant broken.",
                  tag.c_str ()));
    }
}

void
ReplicationManager_i::elevate_backup_to_primary (
  ACE_CString const & tag)
{
  RANKED_IOR_LIST ranked_ior_list;
  ACE_CString failover_host;
  
  if (this->objectid_rankedior_map_.find (tag,ranked_ior_list) == 0)
    {
      failover_host = ranked_ior_list.host_list.front();
      ACE_DEBUG ((LM_DEBUG,
                  "Failover host = %s.\n",
                  failover_host.c_str ()));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Can't find failover host for tag=%s.\n",
                  tag.c_str ()));
    }
  
  APP_SET app_set;
  
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  
  if (this->objectid_appset_map_.find (tag, app_set) == 0)
    {
      for (APP_SET::iterator as_iter = app_set.begin ();
           as_iter != app_set.end ();
           ++as_iter)
        {
          if ((*as_iter).host_name == failover_host.c_str ())
            {
              STRING_LIST backup_tag_list;
              
              if (this->processid_backup_map_.find (
                    (*as_iter).process_id,
                    backup_tag_list)
                  == 0)
                {
                  backup_tag_list.remove(tag);
                  
                  this->processid_backup_map_.rebind (
                    (*as_iter).process_id,
                    backup_tag_list);
                    
                  STRING_LIST primary_tag_list;
                  
                  if (this->processid_primary_map_.find (
                        (*as_iter).process_id,
                        primary_tag_list)
                      == 0)
                    {
                      primary_tag_list.insert_tail (tag);
                      this->processid_primary_map_.rebind (
                        (*as_iter).process_id,
                        primary_tag_list);
                    }
                  else
                    {
                      primary_tag_list.insert_tail (tag);
                      this->processid_primary_map_.bind (
                        (*as_iter).process_id,
                        primary_tag_list);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Can't find backups for "
                              "tag=%s in process=%s.\n"
                              "Data structure invariant broken\n",
                              tag.c_str (),
                              (*as_iter).process_id.c_str ()));
                }
              
              (*as_iter).role = PRIMARY;
              this->objectid_appset_map_.rebind (tag, app_set);
              break;
            }
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "No process found hosting tag=%s.\n",
                  tag.c_str ()));
    }
}

void
ReplicationManager_i::replace_backup_tags (
  ACE_CString const & pid,
  ACE_CString const & host)
{
  STRING_LIST backup_tag_list;
  
  if (this->processid_backup_map_.find (pid, backup_tag_list) == 0)
  {
    for (STRING_LIST::iterator bt_iter = backup_tag_list.begin ();
         bt_iter != backup_tag_list.end ();
         ++bt_iter)
      {
        remove_from_appset (host, pid, *bt_iter, BACKUP);
      }
    
    this->processid_backup_map_.unbind(pid);
  }
}

bool
ReplicationManager_i::replica_selection_algo (void) 
{
  ACE_DLList <MonitorUpdate> update_list;
  move_update_list(this->update_list_, update_list);
  
  if (process_updates (update_list))
    {
      objectid_rankedior_map_.close ();
      objectid_rankedior_map_.open ();

      if (!static_mode_)
        {
          for (STRING_TO_STRING_LIST_MAP::iterator hp_iter =
                 hostid_process_map_.begin ();
               hp_iter != hostid_process_map_.end ();
               ++hp_iter)
            {  
              STRING_TO_DOUBLE_MAP processor_level_host_util_map;
              
              copy_map (this->hostid_util_map_,
                        processor_level_host_util_map);
                        
              STRING_LIST & process_list = (*hp_iter).item ();
              
              for (STRING_LIST::iterator pl_iter =
                     process_list.begin ();
                   pl_iter != process_list.end ();
                   ++pl_iter)
                {
                  STRING_TO_DOUBLE_MAP process_level_host_util_map;
                  
                  copy_map (this->hostid_util_map_,
                            process_level_host_util_map);
                            
                  STRING_TO_DOUBLE_MAP & host_util_map = 
                    (this->mode_ == PROCESS_LEVEL)
                      ? process_level_host_util_map
                      : processor_level_host_util_map;
                      
                  STRING_LIST primary_object_list;
                  
                  if (processid_primary_map_.find (*pl_iter,
                                                   primary_object_list) == 0) // If present
                    {
                      for (STRING_LIST::iterator po_iter =
                             primary_object_list.begin ();
                           po_iter != primary_object_list.end ();
                           ++po_iter)
                        {
                          STRING_LIST host_list =
                            non_primary_host_list (*po_iter);
                          
                          // If the object has backups...
                          if (host_list.size () >= 1)
                            {
                              std::priority_queue<UtilRank> util_ranked_queue = 
                                util_sorted_host_list(*po_iter, host_list, host_util_map);
                              
                              UtilRank ur (util_ranked_queue.top ());
                              
                              host_util_map.rebind (ur.host_id.c_str (),
                                                    ur.util);
                              
                              update_ior_map (*po_iter,
                                              util_ranked_queue);
                            }
                        }
                    }
                }
            }
          
          build_rank_list ();
      update_enhanced_ranklist ();
        }
    }
  
  send_rank_list ();
  send_state_synchronization_rank_list ();
  
  return true;
}

void
ReplicationManager_i::print_queue (
  std::priority_queue<UtilRank> queue)
{
  while (!queue.empty ())
    {
      UtilRank ur (queue.top ());
      queue.pop ();
      ACE_DEBUG ((LM_DEBUG,
                  "%s:%f, ",
                  ur.host_id.c_str (),
                  ur.util));
    }
  
  ACE_DEBUG((LM_DEBUG,"\n"));
}

void
ReplicationManager_i::copy_map (
  STRING_TO_DOUBLE_MAP const & source,
  STRING_TO_DOUBLE_MAP & dest)
{
  dest.close ();
  dest.open ();
  
  for (STRING_TO_DOUBLE_MAP::const_iterator iter (source.begin ());
       iter != source.end ();
       ++iter)
    {
      dest.bind ((*iter).key().c_str (), (*iter).item ());
    }
}

ReplicationManager_i::STRING_LIST
ReplicationManager_i::non_primary_host_list (
  ACE_CString const & primary_object_id)
{
  APP_SET app_set;
  STRING_LIST host_list;
  
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (
    this->appset_lock_);
    
  if (objectid_appset_map_.find (primary_object_id, app_set) == 0)
    {
      for (APP_SET::iterator as_iter = app_set.begin ();
           as_iter != app_set.end ();
           ++as_iter)
        {
          if ((*as_iter).role != PRIMARY)
            {
              host_list.insert_tail ((*as_iter).host_name);
            }
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "No processes for tag = %s.\n",
                  primary_object_id.c_str ()));
    }
  
  return host_list;
}


void
ReplicationManager_i::app_reg (APP_INFO & app)
{
  const char * object_id = app.object_id.c_str ();
  double load = app.load;
  const char * host_name = app.host_name.c_str ();
  const char * process_id = app.process_id.c_str ();
  Role role = app.role;

  if (!static_mode_)
    {
      update_map (host_name, process_id,
                  this->hostid_process_map_);
                  
      update_proc_host_map (process_id,
                            host_name,
                            this->processid_host_map_);
                            
      update_util_map (object_id,
                       load,
                       this->objectid_load_map_);
  
      ACE_Guard <ACE_Recursive_Thread_Mutex> guard (
        this->appset_lock_);
        
      update_appset_map (object_id,
                         app,
                         this->objectid_appset_map_);

      switch (role)
        {
          case PRIMARY:
            {
              update_map (process_id,
                          object_id,
                          processid_primary_map_);
              break;
            }
          case BACKUP:
            {
              update_map (process_id,
                          object_id,
                          processid_backup_map_);
                          
              static_ranklist_update (object_id,
                                      app.ior,
                                      role);
              break;
            }
          default:
            ACE_DEBUG((LM_ERROR,"Unknown Role!!\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Registered successfully %s:%s:%s:%d "
                  "with Replication manager.\n",
                  host_name,
                  process_id,
                  object_id,
                  role));
    }
  else // If in static_mode_
    {
      ACE_Guard <ACE_Recursive_Thread_Mutex> guard (
        this->appset_lock_);
      
      update_appset_map (object_id,
       app,
       this->objectid_appset_map_);

      switch (role)
        {
          case PRIMARY:
      break;
          case BACKUP:
            {
              // Update only the rank list - ignore the other maps.
              static_ranklist_update (object_id,
                                      app.ior,
                                      role);    
              
	      this->update_enhanced_ranklist ();

              ACE_DEBUG ((LM_DEBUG,
                          "Registered %s:%s:%s:%d with "
                          "Replication manager in static mode.\n",
                          host_name,
                          process_id,
                          object_id,
                          role));
              break;
            }
          default:
          ACE_DEBUG ((LM_ERROR,"Unknown Role!!\n"));
        }

      // add entry to the processid map
      {
        ACE_Guard <ACE_Thread_Mutex> protect (pid_object_mutex_);

        pid_object_map_.bind (process_id, app.ior);
      }
    }
}

void
ReplicationManager_i::static_ranklist_update (
  const char * object_id, 
  CORBA::Object_ptr ior,
  Role role)
{
  ACE_Write_Guard <ACE_RW_Thread_Mutex> guard (
    rank_list_mutex_);

  bool found = false;
  size_t i = 0;
  
  for (i = 0; i < rank_list_.length(); ++i)
    {
      if (ACE_OS::strcmp (rank_list_[i].object_id, object_id) == 0)
        {
          found = true;
          break;
        }
    }
    
  if (found && (role == BACKUP))
    {
      size_t ior_list_length = rank_list_[i].ior_list.length ();
      
      rank_list_[i].ior_list.length (ior_list_length + 1);
      
      rank_list_[i].ior_list[ior_list_length] =
        CORBA::Object::_duplicate (ior);
    }
  else
    {
      size_t rl_length = rank_list_.length ();
      
      rank_list_.length (rl_length + 1);
      
      rank_list_[rl_length].object_id =
        CORBA::string_dup (object_id);
        
      rank_list_[rl_length].now = false;
      
      size_t ior_list_length =
        rank_list_[rl_length].ior_list.length ();
        
      rank_list_[rl_length].ior_list.length (ior_list_length + 1);
      
      rank_list_[rl_length].ior_list[ior_list_length] =
        CORBA::Object::_duplicate (ior);
    }
}

std::priority_queue<UtilRank>
ReplicationManager_i::util_sorted_host_list (
  ACE_CString const & oid, 
  STRING_LIST const & host_list,
  STRING_TO_DOUBLE_MAP const & hu_map)
{
  std::priority_queue <UtilRank> rank_list;
  double object_load;
  
  if (objectid_load_map_.find (oid, object_load) == 0)
    {
      for (STRING_LIST::const_iterator hl_iter =
             host_list.begin ();
           hl_iter != host_list.end ();
           ++hl_iter)
        {
          double host_utilization;
          
          // If present...
          if (hu_map.find (*hl_iter, host_utilization) == 0)
            {
              double effective_load =
                object_load + host_utilization;
                
              rank_list.push (UtilRank (effective_load,
                                        (*hl_iter).c_str ()));
            }
          else
            {
              ACE_DEBUG ((LM_ERROR,
                          "Can't find utilization "
                          "of host_id=%s\n",
                          (*hl_iter).c_str ()));
              ACE_DEBUG ((LM_ERROR,
                          "Size of utilmap=%d\n",
                          hu_map.current_size ()));
              break;
            }
        }
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "Can't find load of object_id=%s\n",
                  oid.c_str ()));
    }

  return rank_list;
}


void
ReplicationManager_i::build_rank_list (void)
{
  // this is only necessary for several hosts.
  ACE_Write_Guard <ACE_RW_Thread_Mutex> guard (
    rank_list_mutex_);

  //  this has now to be called outside of the method
  //  ACE_Guard <ACE_Thread_Mutex> guard (
  //  rank_list_agent_list_combined_mutex_);
    
  this->rank_list_.length (0);
  
  for(OBJECTID_RANKED_IOR_MAP::iterator or_iter = 
        this->objectid_rankedior_map_.begin();
      or_iter != this->objectid_rankedior_map_.end();
      ++or_iter)
    {
      CORBA::ULong rank_list_length = this->rank_list_.length ();
      
      rank_list_.length (rank_list_length + 1);
      
      rank_list_[rank_list_length].object_id =
        CORBA::string_dup((*or_iter).key().c_str());
        
      RANKED_IOR_LIST & ranked_ior_list = (*or_iter).item ();
      
      rank_list_[rank_list_length].now = ranked_ior_list.now;
      
      for (std::list<CORBA::Object_var>::iterator ior_iter =
             ranked_ior_list.ior_list.begin (); 
           ior_iter != ranked_ior_list.ior_list.end ();
           ++ior_iter)
        {
          CORBA::ULong ior_list_length =
            rank_list_[rank_list_length].ior_list.length ();
            
          rank_list_[rank_list_length].ior_list.length (
            ior_list_length + 1);
            
          rank_list_[rank_list_length].ior_list[ior_list_length] =
            CORBA::Object::_duplicate (*ior_iter);
        }
    }
}

void
ReplicationManager_i::update_enhanced_ranklist (void)
{
  ACE_Guard <ACE_Thread_Mutex> enhanced_list_guard (
    enhanced_rank_list_agent_list_combined_mutex_);

  ACE_Read_Guard <ACE_RW_Thread_Mutex> guard (
    rank_list_mutex_);

  // create a copy of the rank list here
  enhanced_rank_list_.length (rank_list_.length ());

  std::string object_id;
  APP_SET as;
  
  // add the primary for each ior_list
  for (size_t i = 0; i < rank_list_.length (); ++i)
    {
      object_id = rank_list_[i].object_id.in ();
      enhanced_rank_list_[i].object_id = object_id.c_str ();
      enhanced_rank_list_[i].now = rank_list_[i].now;

      // create a new list that is one element larger than the old one
      size_t old_length = rank_list_[i].ior_list.length ();
      ObjectList_var list_with_primary (new ObjectList (old_length + 1));
      list_with_primary->length (old_length + 1);
      
      // get app set and search for the contained primary
      if (objectid_appset_map_.find (object_id.c_str (),
             as) == 0)
  {
    // go through the appset to find the primary
    for (APP_SET::iterator it = as.begin ();
         it != as.end ();
         ++it)
      {
        if ((*it).role == PRIMARY)
    {
      // add the primary as first entry
      (*list_with_primary)[0] =
        CORBA::Object::_duplicate ((*it).ior.in ());

      break;
    }
      }

    // add all the other entries behind it
    for (size_t j = 0; j < old_length; ++j)
      (*list_with_primary)[j+1] =
        CORBA::Object::_duplicate (rank_list_[i].ior_list[j].in ());
  }
      else
  {
          ACE_DEBUG ((LM_DEBUG,
                      "RM::send_rank_list - could not find %s in appset_map.\n",
          object_id.c_str ()));

    list_with_primary->length (0);
  }

      // put the new list into the rank list instead of the old one
      enhanced_rank_list_[i].ior_list = list_with_primary;
    } // end for
}

void
ReplicationManager_i::send_rank_list (void)
{
  if (! proactive_)
    {
      return;
    }

  ACE_Guard <ACE_Thread_Mutex> guard (enhanced_rank_list_agent_list_combined_mutex_);
    
  for (AGENT_LIST::iterator al_iter = agent_list_.begin ();
       al_iter != agent_list_.end (); )
    {
      ForwardingAgent_var agent = ForwardingAgent::_narrow (*al_iter);

      try 
        {
          agent->update_rank_list(enhanced_rank_list_);
          ++al_iter;
        }
      catch (CORBA::SystemException &)
        {
          ACE_DEBUG ((LM_DEBUG, "A client agent agent died.\n"));
    
          // make sure to remove the failed agent from the list
          AGENT_LIST::iterator tmp_it = al_iter;
          ++tmp_it;
          agent_list_.remove (*al_iter);
          al_iter = tmp_it;
        }
    }
}

void
ReplicationManager_i::send_state_synchronization_rank_list (void)
{
  if (! proactive_)
    {
      return;
    }

  ACE_Read_Guard <ACE_RW_Thread_Mutex> guard (
    rank_list_mutex_);

  ACE_Guard <ACE_Thread_Mutex> state_sync_guard (
    state_sync_agent_list_mutex_);
    
  // Helpers for removing dead agents from the list.
  StateSynchronizationAgent_var remove_holder;
  StateSynchronizationAgent_var &remove_holder_ref = remove_holder;
  bool dirty = false;
    
  for (STATE_SYNC_AGENT_LIST::iterator al_iter = 
         state_synchronization_agent_list_.begin ();
       al_iter != state_synchronization_agent_list_.end ();
       al_iter.advance ())
    {
      StateSynchronizationAgent_var agent = *al_iter;

      try 
        {
          agent->update_rank_list (this->rank_list_);
        }
      catch (CORBA::SystemException &)
        {
          dirty = true;
          remove_holder_ref = *al_iter;
          ACE_DEBUG ((LM_DEBUG,
                      "RM: A state synchronization agent died.\n"));
        }
    }
 
  // This approach will remove at most one dead agent per list
  // traversal, but it's better than none. 
  if (dirty)
    {
      (void) state_synchronization_agent_list_.remove (remove_holder_ref);
    }
}

void
ReplicationManager_i::update_ior_map (
  ACE_CString const & oid,
  std::priority_queue<UtilRank> const & rl)
{
  std::priority_queue <UtilRank> rank_list (rl);
  APP_SET app_set;
  ACE_Guard <ACE_Recursive_Thread_Mutex> guard (this->appset_lock_);
  
  // If present...
  if (objectid_appset_map_.find (oid, app_set) == 0)
    {
      RANKED_IOR_LIST ranked_ior_list;
      
      while (!rank_list.empty ())
        {
          UtilRank ur = rank_list.top ();
          rank_list.pop ();
          
          for (APP_SET::iterator as_iter = app_set.begin ();
               as_iter != app_set.end ();
               ++as_iter)
            {
              if (ur.host_id == (*as_iter).host_name.c_str ()) 
                {
                  ranked_ior_list.ior_list.push_back (
                    (*as_iter).ior);
                    
                  ranked_ior_list.host_list.push_back(
                    ACE_CString(ur.host_id.c_str()));
                    
                  app_set.remove (*as_iter);
                  
                  break;
                }
            }
        }
      
      RANKED_IOR_LIST temp_ior_list;
      
      // If not present...
      if (objectid_rankedior_map_.find (oid, temp_ior_list) != 0)
        {
          objectid_rankedior_map_.bind (oid, ranked_ior_list);
        }
      else
        {
          objectid_rankedior_map_.rebind (oid, ranked_ior_list);
        }
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "Objectid=%s not present in APP_SET\n",
                  oid.c_str ()));
    }
}

void
ReplicationManager_i::proc_failure (const char *process_id)
{
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
    {
      if (update_list_full_.wait (update_mutex_, &wait_time) == -1)  // timeout
        {
          ACE_DEBUG ((LM_ERROR,
                      "proc_failure CORBA upcall waited "
                      "too long. Skipping proc_failure "
                      "update. process_id=%s\n",
                      process_id));
          return;
        }
    }
  
  update_list_.insert_tail (MonitorUpdate::create_proc_fail_update(process_id));
  update_available_.broadcast();
}

void
ReplicationManager_i::util_update (const char *host_id, 
                                   double util)
{
  //ACE_DEBUG ((LM_DEBUG, "Update from %s with UTIL %d\n", host_id, (int)util));
  ACE_Guard <ACE_Thread_Mutex> guard(update_mutex_);
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size() >= UPDATE_LIST_MAX_SIZE)
    {  
      if (update_list_full_.wait (update_mutex_, &wait_time) == -1)  // timeout
        {
          ACE_DEBUG ((LM_ERROR,
                      "util_update CORBA upcall "
                      "waited too long. Skipping "
                      "util_update. host_id=%s,util=%l\n",
                      host_id,
                      util));
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
  ACE_Guard <ACE_Thread_Mutex> guard (update_mutex_);
  ACE_Time_Value wait_time (5);
  
  while (update_list_.size () >= UPDATE_LIST_MAX_SIZE)
    {  
      if (update_list_full_.wait (update_mutex_, &wait_time) == -1)  // timeout
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "pulse waited too long. "
                             "Skipping pulse.\n"),
                            0);
        }
    }
    
  update_list_.insert_tail (
      MonitorUpdate::create_run_now_update ());
  update_available_.broadcast ();
  
  return 0;
}

RankList *
ReplicationManager_i::register_agent (
  CORBA::Object_ptr agent_reference)
{
  ACE_DEBUG ((LM_DEBUG, "register_agent called\n"));
  ForwardingAgent_var agent = ForwardingAgent::_narrow (agent_reference);
 
  ACE_Guard <ACE_Thread_Mutex> agent_list_guard (
    enhanced_rank_list_agent_list_combined_mutex_);

  this->agent_list_.insert_tail (CORBA::Object::_duplicate (agent.in ()));

  return new RankList (enhanced_rank_list_);
}

RankList *
ReplicationManager_i::register_state_synchronization_agent (
  const char * /* host_id */,
  const char * /* process_id */,
  StateSynchronizationAgent_ptr agent)
{
  ACE_DEBUG ((LM_DEBUG, "register_state_synchronization_agent called\n"));
 
  ACE_Guard <ACE_Thread_Mutex> agent_guard (
    state_sync_agent_list_mutex_);

  this->state_synchronization_agent_list_.insert_tail (
    StateSynchronizationAgent::_duplicate (agent));

  ACE_Read_Guard <ACE_RW_Thread_Mutex> rl_guard (rank_list_mutex_);

  return new RankList (rank_list_);
}


CORBA::Object_ptr
ReplicationManager_i::get_next (const char * /* object_id */)
{
  return CORBA::Object::_nil ();
}

MonitorUpdate *
MonitorUpdate::create_proc_fail_update (const char * pid)
{
  ACE_Auto_Ptr<MonitorUpdate> up(new MonitorUpdate ());
  up->process_id = pid;
  up->type = MonitorUpdate::PROC_FAIL_UPDATE;
  return up.release ();
}

MonitorUpdate *
MonitorUpdate::create_host_util_update (const char *hid,
                                        double value)
{
  ACE_Auto_Ptr <MonitorUpdate> up (new MonitorUpdate ());
  up->host_id = hid;
  up->value = value;
  up->type = MonitorUpdate::HOST_UTIL_UPDATE;
  return up.release ();
}

MonitorUpdate *
MonitorUpdate::create_run_now_update (void)
{
  ACE_Auto_Ptr <MonitorUpdate> up (new MonitorUpdate());
  up->type = MonitorUpdate::RUN_NOW;
  return up.release ();
}


MonitorUpdate *
MonitorUpdate::create_app_info_update (const char *oid,
                                       double l, 
                                       const char *hname,
                                       const char *pid, 
                                       Role r,
                                       CORBA::Object_ptr ref)
{
  ACE_Auto_Ptr <MonitorUpdate> up (new MonitorUpdate ());
  up->type = MonitorUpdate::APP_REG;
  up->app_info = APP_INFO (oid, l, hname, pid, r, ref);
  return up.release ();
}
