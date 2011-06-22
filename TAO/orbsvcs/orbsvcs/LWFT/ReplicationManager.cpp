// -*- C++ -*-
// $Id$

#include "ReplicationManager.h"
#include "AppOptions.h"

#include <string>
#include <set>

#include "ace/OS_NS_unistd.h"

#include "ForwardingAgentC.h"
#include "AppInfoC.h"

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
{
}

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
    proc_reg_ (orb),
    algo_thread_(0),
    standby_ (AppOptions::instance ()->role () > PRIMARY),
    proactive_(proactive),
    mode_(mode),
    static_mode_ (static_mode),
    update_available_(update_mutex_),
    update_list_full_(update_mutex_),
    ranklist_constraints_ (10),
    subscription_counter_ (1),
    notify_subscriptions_ (1)
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
                      "RM: register_application CORBA upcall "
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
                  "RM: Duplicate process_id=%s. Skipping it.\n",
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
  /*
  ACE_DEBUG ((LM_TRACE, 
              "RM: update_appset_map - "
              "add entry for %s, role = %d\n", 
              key_str, 
              app_info.role));
  */  
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
      // this means a new host monitor has joined and we can
      map.bind (key_str, value);

      // if the host_monitor is on the same host
      char hostname [100];
      gethostname (hostname, sizeof (hostname));

      if (ACE_OS::strcmp (hostname, key_str) == 0)
        {
          int result = proc_reg_.register_process ();

          if (result != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "AppSideReg::activate () returned %d\n",
                          result));
            }
        }
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
  bool only_util = true;  

  while (MonitorUpdate * up = update_list.delete_head ())
    {
      switch (up->type)
        {
          case MonitorUpdate::RUN_NOW:
            //ACE_DEBUG ((LM_DEBUG,"RUN_NOW\n"));
            major_update = true;
            break;
          case MonitorUpdate::HOST_UTIL_UPDATE:
            //ACE_DEBUG ((LM_DEBUG,"HOST_UTIL_UPDATE\n"));
            update_util_map (up->host_id.c_str (),
                             up->value,
                             this->hostid_util_map_);
                             
            major_update = true;
            break;
          case MonitorUpdate::PROC_FAIL_UPDATE: 
            //ACE_DEBUG ((LM_DEBUG,"PROC_FAIL_UPDATE\n"));
            process_proc_failure (up->process_id);
            major_update = true;
            only_util = false;
            break;
          case MonitorUpdate::APP_REG:
            //ACE_DEBUG ((LM_DEBUG,"RUN_NOW\n"));
            app_reg (up->app_info);
            only_util = false;
            break;
        }
        
      delete up;
    }

  if (!only_util)
    {
      //ACE_DEBUG ((LM_TRACE, "RM: state_changed () called.\n"));
      this->send_state_synchronization_rank_list ();
      agent_->state_changed (this->object_id ());
      standby_ = false;
    }

  return major_update;
}

void
ReplicationManager_i::process_proc_failure (
  ACE_CString const & process_id)
{
  ACE_DEBUG ((LM_TRACE, "RM: process_proc_failure (%s)\n", process_id.c_str ()));

  if (static_mode_)
    {
      //ACE_DEBUG ((LM_TRACE, "RM: process_proc_failure ()\n"));

      // Collect the app_info of all failed applications
      // of a process.
      std::vector <APP_INFO> failed;
      
      for (OBJECTID_APPSET_MAP::iterator it = 
	     objectid_appset_map_.begin ();
	   it != objectid_appset_map_.end ();
	   ++it)
        {
          // For every object_id appset...
          APP_SET & as = (*it).item ();
      	  
          // find the primary
          for (APP_SET::iterator s_it = as.begin ();
               s_it != as.end ();
               ++s_it)
            {
              APP_INFO & ai = *s_it;
      	      
              // by looking at its role member
              if (ai.process_id == process_id)
                {
                  failed.push_back (ai);
                }
            }
        }
      
      ACE_DEBUG ((LM_DEBUG,
                  "RM::process_proc_failure - found %d failed applications\n",
                  failed.size ()));

      CORBA::Object_var new_primary;
      // For each failed application in this process...
      for (std::vector <APP_INFO>::iterator fit = failed.begin ();
	         fit != failed.end ();
	         ++fit)
        {
	  // ACE_DEBUG ((LM_TRACE,
          //             "RM::process_proc_failure dealing with failed app "
	  //             "for %s\n",
          //              (*fit).object_id.c_str ()));
          
          // Remove entry from the appset.
          APP_SET as;
	  
          if (objectid_appset_map_.find ((*fit).object_id,
                                         as) == 0)
            {
	      //ACE_DEBUG ((LM_TRACE,
	      //            "RM::process_proc_failure remove APP_INFO from APP_SET\n"));
      	      
              // Remove appinfo from the appset.
              as.remove (*fit);
      	      
              objectid_appset_map_.rebind ((*fit).object_id, as);
            } // end if find
          
          // We don't have to care about a rank_list
          // that is already empty.
          if (rank_list_.length () != 0)
            {
              // Find the right rank list.
              size_t r = 0;
              bool found_ranklist = false;
	      
              for (; r < rank_list_.length (); ++r)
                {
                  if ((*fit).object_id == rank_list_[r].object_id)
                    {
                      found_ranklist = true;
                      break;
                    }
                }
              
              if (!found_ranklist)
		{
		  /*
		  ACE_DEBUG ((LM_TRACE,
			      "RM::process_proc_failure - "
			      "found no rank_list for object_id=%s",
			      (*fit).object_id.c_str ()));
		  */
		  break;
		}
              
              //ACE_DEBUG ((LM_DEBUG,
              //            "RM::process_proc_failure - "
	      //            "rank_list index found is %d.\n", r));
      	      
              // Remove application from the rank_list.
              if (rank_list_[r].ior_list.length () == 0)
                {
                  //ACE_DEBUG ((LM_WARNING, "RM::process_proc_failure - "
		  //            "empty ior list.\n"));
                }
              else 
                {
                  new_primary =
                    CORBA::Object::_duplicate (rank_list_[r].ior_list[0]);

                  if (rank_list_[r].ior_list.length () == 1)
                    {
		      /*
                      ACE_DEBUG ((LM_DEBUG,
                                  "RM::process_proc_failure - "
				  "remove complete ranklist.\n",
                                  r));
		      */
                      // Remove complete rank_list entry.
                      for (size_t k = r; k < rank_list_.length () - 1; ++k)
                        {
                          // Move each following element one position 
                          // forward.
                          rank_list_[k] = rank_list_[k+1];
                        }
                      
                      rank_list_.length (rank_list_.length () - 1);
                    }
                  else
                    {
                      try
                        {
                          // Index of the element that should be removed from 
                          // the ior list.
                          size_t rm_index = 0;
                          size_t len_i = rank_list_[r].ior_list.length ();
                          
                          if ((*fit).role != PRIMARY)
                            {
                              for (size_t j = 0; j < len_i; ++j)
                                {
                                  if ((*fit).ior->_is_equivalent (
                                                    rank_list_[r].ior_list[j].in ()))
                                    {
                                      rm_index = j;
                                      break;
                                    }
                                }
                            }
                  			      
                          ACE_DEBUG ((LM_TRACE,
                                      "RM::process_proc_failure - "
				      "remove entry %d in ior list\n",
                                      rm_index));
                          
                          // Now remove the correct element from the list
                          for (size_t k = rm_index; k < len_i - 1; ++k)
                            {
                              // Move each following element one position 
                              // forward.
                              rank_list_[r].ior_list[k] = 
                                rank_list_[r].ior_list[k + 1];
                            }
                          
                          // Adjust length of ior list.
                          rank_list_[r].ior_list.length (
                            rank_list_[r].ior_list.length () - 1);
                        } // end try
                      catch (const CORBA::SystemException & ex)
                        {
                          // Just make sure to keep on going for the other 
                          // entries here.
                          ACE_DEBUG ((LM_ERROR, 
                                      "RM::process_proc_failure - caught %d\n",
                                      ex._info ().c_str ()));
                        }
                    } // end else
                } // end else
              
              // Elevate new primary if necessary.
              if ((*fit).role == PRIMARY)
                {
		  /*
                  ACE_DEBUG ((LM_TRACE,
                              "RM::process_proc_failure - "
			      "select new primary in APP_SET (%d)\n",
                              as.size ()));
		  */  
                  for (APP_SET::iterator it = as.begin ();
                       it != as.end ();
                       ++it)
                    {
                      // Compare the object id to the first element in
                      // the rank list and mark it as primary.
            		      
                      if (new_primary->_is_equivalent ((*it).ior.in ()))
                        {
                          (*it).role = PRIMARY;
                    
			  /*
                          ACE_DEBUG ((LM_DEBUG,
                                      "RM::process_proc_failure - "
				      "found a new primary\n"));
			  */
                          break;
                        }
                    } // end for
                  
                  objectid_appset_map_.rebind ((*fit).object_id, as);
                }
            } // end if rank_list is not empty
        } // end for every failed application
      
      this->update_enhanced_ranklist ();
    }
  else // not in static_mode_
    {
      ACE_CString host;
          
      // If present...
      if (this->processid_host_map_.find (process_id, host) == 0)
        {
          ACE_DEBUG ((LM_TRACE,
                      "RM: process %s on host %s failed\n",
                      process_id.c_str (),
                      host.c_str ()));

          STRING_LIST primaries;
          if (processid_primary_map_.find (process_id, primaries) == 0)
            {
              FLARE::ApplicationList ids;
              CORBA::ULong index = 0;
              ids.length (primaries.size ());
              for (STRING_LIST::iterator it = primaries.begin ();
                   it != primaries.end ();
                   ++it)
                {
                  ids[index++] = (*it).c_str ();
                }
              // for now just take the first entry and send it
              send_failure_notice (host.c_str (),
                                   ids);
            }

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
              ACE_DEBUG ((LM_ERROR,
                          "RM: Can't find host=%s in hostid_process_map. "
                          "Data structure invariant broken.\n",
                          host.c_str()));
            }
        }
      else
        {
          ACE_DEBUG ((LM_ERROR,
                      "RM: Can't find process_id=%s in proc_host_map."
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
                  "RM: Removed application %s:%s:%s:%d.\n",
                  host.c_str (),
                  pid.c_str (),
                  tag.c_str (),
                  role));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "RM: Can't find appset for tag=%s.\n"
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
                  "RM: Failover host = %s.\n",
                  failover_host.c_str ()));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "RM: Can't find failover host for tag=%s.\n",
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
                              "RM: Can't find backups for "
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
                  "RM: No process found hosting tag=%s.\n",
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

void
ReplicationManager_i::load_based_selection_algo (void) 
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

                      // this check is necessary to make sure
                      // there is utilization information for
                      // this host available. When a
                      // ReplicationManager starts up for
                      // example the host_monitor on that host
                      // is not yet running.
                      if (util_ranked_queue.size () > 0)
                        {
                          UtilRank ur (util_ranked_queue.top ());
                          
                          host_util_map.rebind (ur.host_id.c_str (),
                                                ur.util);
                          
                          update_ior_map (*po_iter,
                                          util_ranked_queue);
                        }
                    } //end if (host_list.size () >= 1)
                } // end for primary_object_list
            } // end if processid_primary_map_.find ()
        } // end for process_list
    } // end for hostid_process_map
}

void
ReplicationManager_i::static_selection_algo (void)
{
  // enter entries from objectid_appset_map_ into
  // objectid_rankedior_map_ according to the sorted list given by an
  // external source.

  ACE_DEBUG ((LM_INFO, "RM: static ranklist order(%d):\n",
              objectid_appset_map_.current_size ()));

  APP_SET tmp_apps;
  for (OBJECTID_APPSET_MAP::iterator appset_it = objectid_appset_map_.begin ();
       appset_it != objectid_appset_map_.begin ();
       ++appset_it)
    {
      ACE_DEBUG ((LM_INFO, "[\toid %s:", appset_it->key ().c_str ()));

      RANKED_IOR_LIST iorlist;
      iorlist.now = false;

      tmp_apps = appset_it->item ();
      
      RANKLIST_CONSTRAINT constr;
      // add all entries mentioned in the constraints in sorted order
      if (ranklist_constraints_.find (appset_it->key (),
                                      constr) == 0)
        {
          for (RANKLIST_CONSTRAINT::iterator const_it = constr.begin ();
               const_it != constr.end ();
               ++const_it)
            {
              // this is inefficient, but we'll go through the set and find
              // the appropriate entry
              for (APP_SET::iterator as_it = tmp_apps.begin ();
                   as_it != tmp_apps.end ();
                   ++as_it)
                {
                  // if we found the right guy
                  if (const_it->compare ((*as_it).host_name) == 0)
                    {
                      if ((*as_it).role != PRIMARY)
                        {
                          ACE_DEBUG ((LM_INFO, "[\n\t\t%s", (*as_it).host_name.c_str ()));
                      
                          // add to ranklist
                          iorlist.host_list.push_back ((*as_it).host_name);
                          iorlist.ior_list.push_back (CORBA::Object::_duplicate ((*as_it).ior.in ()));
                        }

                      // remove from set
                      tmp_apps.remove (*as_it);

                      break;
                    }
                }              
            }
        } // end if find in ranklist_constraints_

      // add remaing application entries
      for (APP_SET::iterator app_it = tmp_apps.begin ();
           app_it != tmp_apps.end ();
           ++app_it)
        { 
          if ((*app_it).role != PRIMARY)
            {
              ACE_DEBUG ((LM_INFO, "[\n\t\t%s", (*app_it).host_name.c_str ()));

              iorlist.host_list.push_back ((*app_it).host_name);
              iorlist.ior_list.push_back (CORBA::Object::_duplicate ((*app_it).ior.in ()));
            }
        } // end for tmp_apps

      objectid_rankedior_map_.bind (appset_it->key (),
                                    iorlist);

      ACE_DEBUG ((LM_INFO, "\n"));
    } // end for objectid_appset_map_
}

bool
ReplicationManager_i::replica_selection_algo (void)
{
  ACE_DLList <MonitorUpdate> update_list;
  move_update_list(this->update_list_, update_list);
  
  if (!static_mode_)
    {
      if (process_updates (update_list))
        {
          objectid_rankedior_map_.close ();
          objectid_rankedior_map_.open ();

#ifdef ORIGINAL_SELECTION_ALGORITHM
          this->load_based_selection_algo ();
#else
          this->static_selection_algo ();
#endif
          this->build_rank_list ();
          this->update_enhanced_ranklist ();
        }  // end if process_updates
    } // end if static_mode

  if (!standby_)
    {
      send_rank_list ();
      send_state_synchronization_rank_list ();
    }

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
                  "RM: No processes for tag = %s.\n",
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

  // add the received information to a map that knows which object ids
  // are in a process

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
            update_map (process_id,
                        object_id,
                        processid_primary_map_);
            break;
          case BACKUP:
            update_map (process_id,
                        object_id,
                        processid_backup_map_);
                        
            static_ranklist_update (object_id,
                                    app.ior,
                                    role);

            break;
          default:
            ACE_ERROR ((LM_ERROR,
                        "RM: in app_reg () - Unknown Role!!\n"));
        }

      ACE_DEBUG ((LM_TRACE,
                  "RM: Registered successfully %s:%s:%s:%d "
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
            // Update only the rank list - ignore the other maps.
            static_ranklist_update (object_id,
                                    app.ior,
                                    role);    
	    /*
            ACE_DEBUG ((LM_DEBUG,
                        "RM: Registered %s:%s:%s:%d with "
                        "Replication manager in static mode.\n",
                        host_name,
                        process_id,
                        object_id,
                        role));*/
            break;
          default:
            ACE_ERROR ((LM_ERROR,
                        "RM: in app_reg () - Unknown Role!!\n"));
        }

      this->update_enhanced_ranklist ();
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
              ACE_DEBUG ((LM_WARNING,
                          "RM: Can't find utilization "
                          "of host_id=%s\n",
                          (*hl_iter).c_str ()));

              /*
              ACE_DEBUG ((LM_ERROR,
                          "Size of utilmap=%d\n",
                          hu_map.current_size ()));
              */
              break;
            }
        }
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "RM: Can't find load of object_id=%s\n",
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

  this->rank_list_.length (0);
  
  for (OBJECTID_RANKED_IOR_MAP::iterator or_iter = 
         this->objectid_rankedior_map_.begin();
       or_iter != this->objectid_rankedior_map_.end();
       ++or_iter)
    {
      CORBA::ULong rank_list_length =
        this->rank_list_.length ();
      
      rank_list_.length (rank_list_length + 1);
      
      rank_list_[rank_list_length].object_id =
        CORBA::string_dup((*or_iter).key().c_str());
        
      RANKED_IOR_LIST & ranked_ior_list =
        (*or_iter).item ();
      
      rank_list_[rank_list_length].now =
        ranked_ior_list.now;
      
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

  // get all existing primaries from the appset map
  // for some reason this creates a trace output
  // "ACE_Hash_Map_Manager_Ex" on stdout.
  OBJECTID_APPINFO_MAP primaries;

  for (OBJECTID_APPSET_MAP::iterator it = 
	       objectid_appset_map_.begin ();
       it != objectid_appset_map_.end ();
       ++it)
    {
      // for every object_id appset
      APP_SET & as = (*it).item ();

      // find the primary
      for (APP_SET::iterator s_it = as.begin ();
	         s_it != as.end ();
           ++s_it)
        {
          APP_INFO & ai = *s_it;
          
          // by looking at its role member
          if (ai.role == PRIMARY)
            {
              primaries.bind ((*it).key (), ai);
              
              // if the primary has been found we can abort this loop
              break;
            }
        }
    }

  // to determine the size of the enhanced rank_list we have to
  // account for the union of all existing primaries (including those
  // without backup entries in the rank_list) and for all the
  // rank_list entries that have no primary running yet.  To achieve
  // this we create to sets with all the application names: One for
  // the primaries and one for the rank_list and do a union on them.

  // create a set of strings for the primary names
  std::set <std::string> primary_names;
  for (OBJECTID_APPINFO_MAP::iterator p_it = primaries.begin ();
       p_it != primaries.end ();
       ++p_it)
    primary_names.insert (p_it->key ().c_str ());

  // create a set of strings for the backup names
  std::set <std::string> backup_names;
  for (size_t rl_i = 0; rl_i < rank_list_.length (); ++rl_i)
    backup_names.insert (rank_list_[rl_i].object_id.in ());

  std::set <std::string> name_union;
  std::set_union (primary_names.begin (), primary_names.end (),
                  backup_names.begin (), backup_names.end (),
                  std::inserter (name_union,
                                 name_union.begin ()));

  enhanced_rank_list_.length (name_union.size ());

  std::string object_id;
  APP_INFO ai;
  
  // Add the primary for each ior_list that
  // is already in the rank_list.
  size_t i = 0;
  
  for (; i < rank_list_.length (); ++i)
    {
      object_id = rank_list_[i].object_id.in ();
      enhanced_rank_list_[i].object_id = object_id.c_str ();
      enhanced_rank_list_[i].now = rank_list_[i].now;

      // Create a new list that is one element larger than the old one.
      size_t old_length = rank_list_[i].ior_list.length ();
      ObjectList_var list_with_primary (new ObjectList (old_length + 1));

      // Get primary from the map.
      if (primaries.find (object_id.c_str (), ai) == 0)
        {
          list_with_primary->length (old_length + 1);
      
          (*list_with_primary)[0] =
            CORBA::Object::_duplicate (ai.ior.in ());
          
          primaries.unbind (ai.object_id);

          // Add all the other entries behind it.
          for (size_t j = 0; j < old_length; ++j)
            {
              (*list_with_primary)[j + 1] =
                CORBA::Object::_duplicate (
                  rank_list_[i].ior_list[j].in ());
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RM::send_rank_list - "
                      "could not find primary for %s.\n",
		      object_id.c_str ()));

          list_with_primary->length (old_length);

          // Add all the other entries behind it.
          for (size_t j = 0; j < old_length; ++j)
            {
              (*list_with_primary)[j] =
                CORBA::Object::_duplicate (
                  rank_list_[i].ior_list[j].in ());
            }
        }

      // put the new list into the rank list instead of the old one
      enhanced_rank_list_[i].ior_list = list_with_primary;
    } // end for

  // add primaries for applications that have no entries in the
  // rank_list yet.
  for (OBJECTID_APPINFO_MAP::iterator pit = primaries.begin ();
       !pit.done ();
       ++pit)
    {
      ObjectList_var iorlist (new ObjectList (1));
      iorlist->length (1);

      // add object reference of primary to the list
      (*iorlist)[0] = 
        CORBA::Object::_duplicate ((*pit).item ().ior.in ());

      enhanced_rank_list_[i].object_id = (*pit).key ().c_str ();
      enhanced_rank_list_[i].now = false;
      enhanced_rank_list_[i].ior_list = iorlist;

      ++i;
    }
}

void
ReplicationManager_i::send_rank_list (void)
{
  if (! proactive_)
    {
      return;
    }

  ACE_Guard <ACE_Thread_Mutex> guard (
    enhanced_rank_list_agent_list_combined_mutex_);
    
  for (AGENT_LIST::iterator al_iter = agent_list_.begin ();
       al_iter != agent_list_.end (); )
    {
      ForwardingAgent_var agent =
        ForwardingAgent::_narrow (*al_iter);

      try 
        {
          agent->update_rank_list (enhanced_rank_list_);
          ++al_iter;
        }
      catch (CORBA::SystemException &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RM: A client agent agent died.\n"));
    
          // Make sure to remove the failed agent from the list.
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
    
  for (STATE_SYNC_AGENT_MAP::iterator al_iter = 
         state_synchronization_agent_map_.begin ();
       al_iter != state_synchronization_agent_map_.end (); )
    {
      StateSynchronizationAgent_var agent = al_iter->item ();

      try 
        {
          agent->update_rank_list (this->rank_list_);
          ++al_iter;
        }
      catch (CORBA::SystemException &)
        {
          STATE_SYNC_AGENT_MAP::iterator tmp_it = al_iter;          
          ++tmp_it;

          ACE_CString process_id = al_iter->key ();

          (void) state_synchronization_agent_map_.unbind (al_iter);
          al_iter = tmp_it;

          this->proc_failure (process_id.c_str ());

          ACE_DEBUG ((LM_TRACE,
                      "RM: A state synchronization agent (pid=%s) died.\n",
                      process_id.c_str ()));
        }
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
                  "RM: Objectid=%s not present in APP_SET\n",
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
                      "RM: proc_failure CORBA upcall waited "
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
      // Timeout  
      if (update_list_full_.wait (update_mutex_, &wait_time) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "RM: util_update CORBA upcall "
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
                             "RM: pulse waited too long. "
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
  //ACE_DEBUG ((LM_DEBUG, "RM: register_agent () called\n"));
  ForwardingAgent_var agent =
    ForwardingAgent::_narrow (agent_reference);

  // Make sure the agent gets an up-to-date list.
  this->update_enhanced_ranklist ();
 
  ACE_Guard <ACE_Thread_Mutex> agent_list_guard (
    enhanced_rank_list_agent_list_combined_mutex_);

  this->agent_list_.insert_tail (CORBA::Object::_duplicate (agent.in ()));

  return new RankList (enhanced_rank_list_);
}

RankList *
ReplicationManager_i::register_state_synchronization_agent (
  const char * /* host_id */,
  const char * process_id,
  StateSynchronizationAgent_ptr agent)
{
  // ((LM_DEBUG, "RM: register_state_synchronization_agent () called\n"));
 
  ACE_Guard <ACE_Thread_Mutex> agent_guard (
    state_sync_agent_list_mutex_);

  this->state_synchronization_agent_map_.bind (
    process_id,
    StateSynchronizationAgent::_duplicate (agent));

  ACE_Read_Guard <ACE_RW_Thread_Mutex> rl_guard (rank_list_mutex_);

  return new RankList (rank_list_);
}


CORBA::Object_ptr
ReplicationManager_i::get_next (const char * /* object_id */)
{
  return CORBA::Object::_nil ();
}

void
ReplicationManager_i::set_state (const ::CORBA::Any & state_value)
{
  //  ACE_DEBUG ((LM_INFO, "RM: set_state with\n"));
  FLARE::ReplicationManager::ReplicationManagerState * value;

  if (state_value >>= value);
  else
    ACE_DEBUG ((LM_WARNING,
                "ReplicationManager_i::set_state () "
                "could not extract state value from Any."));

  for (size_t i = 0;
       i < value->utilization.length ();
       ++i)
    {
      hostid_util_map_.bind (CORBA::string_dup(value->utilization[i].hostname),
                             value->utilization[i].utilization);
    }

  for (size_t i = 0;
       i < value->app_set_list.length ();
       ++i)
    {
      APP_INFO info (CORBA::string_dup (value->app_set_list[i].object_id.in ()),
                     value->app_set_list[i].load,
                     CORBA::string_dup (value->app_set_list[i].host_name.in ()),
                     CORBA::string_dup (value->app_set_list[i].process_id.in ()),
                     (value->app_set_list[i].role == FLARE::ReplicationManager::PRIMARY ? PRIMARY : BACKUP),
                     CORBA::Object::_duplicate (value->app_set_list[i].ior.in ()));

      this->app_reg (info);
    }

  // delete old entries and take over new forwarding agent entries
  agent_list_.reset ();
  for (size_t i = 0;
       i < value->forwarding_agents.length ();
       ++i)
    {
      agent_list_.insert_tail (
        CORBA::Object::_duplicate (value->forwarding_agents[i].in ()));
    }

  // delete old entries and take over new forwarding agent entries
  state_synchronization_agent_map_.unbind_all ();
  for (size_t i = 0;
       i < value->state_sync_agents.length ();
       ++i)
    {
      state_synchronization_agent_map_.bind (
        value->state_sync_agents[i].process_id.in (),
        StateSynchronizationAgent::_duplicate (value->state_sync_agents[i].agent.in ()));
    }
}
  
CORBA::Any *
ReplicationManager_i::get_state (void)
{
  // create new any object
  CORBA::Any_var state (new CORBA::Any);
  
  FLARE::ReplicationManager::ReplicationManagerState_var value (
    new FLARE::ReplicationManager::ReplicationManagerState ());

  size_t index = 0;
  for (OBJECTID_APPSET_MAP::iterator it = 
         objectid_appset_map_.begin ();
       it != objectid_appset_map_.end ();
       ++it)
    {
      value->app_set_list.length (value->app_set_list.length () + 
                                  it->item ().size ());

      for (APP_SET::iterator ait =
             it->item ().begin ();
           ait != it->item ().end ();
           ++ait)
        {
          FLARE::ReplicationManager::AppInfo ai;

          ai.object_id = (*ait).object_id.c_str ();
          ai.load = (*ait).load;
          ai.host_name = (*ait).host_name.c_str ();
          ai.process_id = (*ait).process_id.c_str ();
          ((*ait).role == PRIMARY ? 
             ai.role = FLARE::ReplicationManager::PRIMARY : 
             ai.role = FLARE::ReplicationManager::BACKUP);
          ai.ior = CORBA::Object::_duplicate ((*ait).ior.in ());

          value->app_set_list[index++] = ai;
        }
    }

  index = 0;
  value->forwarding_agents.length (agent_list_.size ());
  for (AGENT_LIST::iterator al_iter = agent_list_.begin ();
       al_iter != agent_list_.end (); 
       ++al_iter)
    {
      value->forwarding_agents[index++] = 
        CORBA::Object::_duplicate ((*al_iter).in ());
    }  

  index = 0;
  value->state_sync_agents.length (state_synchronization_agent_map_.current_size ());
  for (STATE_SYNC_AGENT_MAP::iterator ssal_iter = state_synchronization_agent_map_.begin ();
       ssal_iter != state_synchronization_agent_map_.end (); 
       ++ssal_iter)
    {
      value->state_sync_agents[index].agent = 
        StateSynchronizationAgent::_duplicate (ssal_iter->item ().in ());
      value->state_sync_agents[index].process_id =
        ssal_iter->key ().c_str ();
    }  

  index = 0;
  value->utilization.length (hostid_util_map_.current_size ());
  FLARE::ReplicationManager::HostUtil util;
  for (STRING_TO_DOUBLE_MAP::iterator h_it = hostid_util_map_.begin ();
       h_it != hostid_util_map_.end ();
       ++h_it)
    {
      util.hostname = h_it->key ().c_str ();
      util.utilization = h_it->item ();

      value->utilization[index++] = util;
    }

  // insert value into the any object
  *state <<= value._retn ();

  return state._retn ();
}

StateSynchronizationAgent_ptr 
ReplicationManager_i::agent (void)
{
  return StateSynchronizationAgent::_duplicate (agent_.in ());
}
  
void
ReplicationManager_i::agent (StateSynchronizationAgent_ptr agent)
{
  agent_ = agent;
}

char * 
ReplicationManager_i::object_id (void)
{
  return "ReplicationManager";
}

void 
ReplicationManager_i::object_id (const char * /* object_id */)
{
  // no-op
}

FLARE::NotificationId
ReplicationManager_i::register_fault_notification (
  FLARE::FaultNotification_ptr receiver)
{
  ACE_Guard <ACE_Thread_Mutex> guard (notify_mutex_);

  if (notify_subscriptions_.bind (
        subscription_counter_,
        FLARE::FaultNotification::_duplicate (receiver)) != 0)
    {
      throw FLARE::NotifyRegistrationError ();
    }

  return subscription_counter_++;
}

void
ReplicationManager_i::unregister_fault_notification (
  FLARE::NotificationId id)
{
  ACE_Guard <ACE_Thread_Mutex> guard (notify_mutex_);

  notify_subscriptions_.unbind (id);
}

void 
ReplicationManager_i::send_failure_notice (const char * host,
                                           const ::FLARE::ApplicationList & object_ids)
{
  ACE_DEBUG ((LM_TRACE, 
              "RM: '%s' on '%s' failed.\n",
              object_ids[0].in (),
              host));

  try
    {
      ACE_Guard <ACE_Thread_Mutex> guard (notify_mutex_);
      
      for (NOTIFICATION_MAP::iterator it = notify_subscriptions_.begin ();
           it != notify_subscriptions_.end ();
           ++it)
        {
          it->item ()->app_failure (host, object_ids);
        }
    }
  catch (const CORBA::Exception & ex)
    {
      ACE_DEBUG ((LM_ERROR,
                  "RM: send_failure_notice - caught %s\n",
                  ex._info ().c_str ()));
    }
}

void 
ReplicationManager_i::set_ranklist_constraints (
  const RankListConstraints & constraints)
{
  ACE_Write_Guard <ACE_RW_Thread_Mutex> guard (constraint_lock_);

  ACE_DEBUG ((LM_TRACE, "RM: received ranklist constraints:\n"));

  RANKLIST_CONSTRAINT constraint;
  for (CORBA::ULong i = 0; i < constraints.length (); ++i)
    {
      ACE_DEBUG ((LM_TRACE, "|\toid %s:", constraints[i].object_id.in ()));

      constraint.clear ();
      for (CORBA::ULong j = 0; j < constraints[i].hosts.length (); ++j)
        {
          ACE_DEBUG ((LM_TRACE, 
                      "\n|\t\t%s", 
                      constraints[i].hosts[j].in ()));

          constraint.push_back (constraints[i].hosts[j].in ());
        }

      ranklist_constraints_.bind (constraints[i].object_id.in (),
                                  constraint);

      ACE_DEBUG ((LM_TRACE, "\n"));
    }
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
