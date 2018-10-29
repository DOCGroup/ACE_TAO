// -*- C++ -*-
#include "AsyncAccessManager.h"
#include "ImR_Locator_i.h"
#include "Locator_Repository.h"
#include "UpdateableServerInfo.h"
#include "orbsvcs/Log_Macros.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

static ACE_CString unique_prefix = "\001\002\003\004";

AsyncAccessManager::AsyncAccessManager (UpdateableServerInfo &info,
                                        ImR_Locator_i &locator)
  :info_(info),
   manual_start_ (false),
   retries_ (info->start_limit_),
   remove_on_death_rh_ (0),
   locator_ (locator),
   poa_ (locator.root_poa ()),
   rh_list_ (),
   status_ (ImplementationRepository::AAM_INIT),
   refcount_ (1),
   lock_ (),
   prev_pid_ (0)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("AsyncAccessManager");
    }
  this->prev_pid_ = info_->pid;
}

AsyncAccessManager::~AsyncAccessManager (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("~AsyncAccessManager");
    }
}

void
AsyncAccessManager::started_running (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("started_running");
    }

  this->update_status(ImplementationRepository::AAM_SERVER_STARTED_RUNNING);
}

bool
AsyncAccessManager::is_terminating (void) const
{
  return this->status_ == ImplementationRepository::AAM_ACTIVE_TERMINATE ||
    remove_on_death_rh_ != 0;
}

bool
AsyncAccessManager::is_running (void) const
{
  if (this->info_->is_mode (ImplementationRepository::PER_CLIENT))
    {
      return !CORBA::is_nil (this->server_.in());
    }
  else
    {
      return this->info_->is_running ();
    }
}

bool
AsyncAccessManager::has_server (const char *s) const
{
  return ACE_OS::strcmp (this->info_->ping_id (), s) == 0;
}

void
AsyncAccessManager::report (const char* operation) const
{
  const Server_Info* si = info_.operator->();
  ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) AsyncAccessManager(%@:%@)::%C - Server <%C> pid <%d> lastpid <%d> status <%C> running <%d> waiters <%d>\n"),
                  this, si, operation, info_->ping_id (), info_->pid, this->prev_pid_, status_name (this->status_), this->is_running(), this->rh_list_.size()));
}

void
AsyncAccessManager::update_prev_pid (void)
{
  this->prev_pid_ = this->info_->pid;
}

void
AsyncAccessManager::add_interest (ImR_ResponseHandler *rh, bool manual)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    this->rh_list_.push_back (rh);
  }

  if (manual)
    {
      this->manual_start_ = true;
    }
  else if (this->is_terminating())
    {
      this->notify_waiters ();
      return;
    }

  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("add_interest");
    }

  this->info_.notify_remote_access (this->status_);

  if (this->info_->is_mode (ImplementationRepository::PER_CLIENT))
    {
     if (!this->send_start_request())
        {
          this->final_state();
        }
      return;
   }

  if (this->status_ == ImplementationRepository::AAM_SERVER_READY ||
      this->status_ == ImplementationRepository::AAM_SERVER_STARTED_RUNNING)
    {
      if (this->locator_.pinger().is_alive (this->info_->ping_id()) == LS_ALIVE)
        {
          this->status (ImplementationRepository::AAM_SERVER_READY);
          this->final_state();
          return;
        }
    }

  if (this->status_ == ImplementationRepository::AAM_INIT ||
      this->status_ == ImplementationRepository::AAM_SERVER_READY ||
      this->status_ == ImplementationRepository::AAM_SERVER_STARTED_RUNNING)
    {
      // This is not a leak. The listener registers with
      // the pinger and will delete itself when done.
      AccessLiveListener *l = 0;
      ACE_NEW (l, AccessLiveListener (this->info_->ping_id(),
                                     this,
                                     this->locator_.pinger()));
      LiveListener_ptr llp(l);
      if (!l->start())
        {
          if (!this->send_start_request())
            {
              this->final_state();
            }
        }
      else
        {
          if (this->status_ == ImplementationRepository::AAM_SERVER_STARTED_RUNNING)
            {
              this->update_status (ImplementationRepository::AAM_WAIT_FOR_ALIVE);
            }
          else
            {
              this->update_status (ImplementationRepository::AAM_WAIT_FOR_PING);
            }
        }
    }
}

bool
AsyncAccessManager::force_remove_rh (ImR_ResponseHandler *rh)
{
  bool busy = true;
  if (this->remove_on_death_rh_ == 0 || rh == 0)
    {
      if (rh == 0)
        {
          delete this->remove_on_death_rh_;
        }
      this->remove_on_death_rh_ = rh;
      busy = false;
    }
  return busy;
}

void
AsyncAccessManager::remote_state (ImplementationRepository::AAM_Status state)
{
  this->status (state);
  if (this->is_terminating ())
    {
      AsyncAccessManager_ptr aam (this->_add_ref());
      this->locator_.make_terminating (aam,this->info_->ping_id(), this->info_->pid);
      this->notify_waiters ();
    }
  if (AsyncAccessManager::is_final (state))
    {
      this->final_state (false);
    }
}

void
AsyncAccessManager::final_state (bool active)
{
  bool const success = this->status_ == ImplementationRepository::AAM_SERVER_READY;
  this->info_.edit (active)->started (success);
  this->retries_ = this->info_->start_limit_;
  if (active)
    {
      this->info_.update_repo ();
    }
  this->notify_waiters ();
  this->manual_start_ = false;

  if (active)
    {
      this->info_.notify_remote_access (this->status_);
    }
  if (this->info_->is_mode (ImplementationRepository::PER_CLIENT) ||
      this->status_ != ImplementationRepository::AAM_SERVER_READY)
    {
      if (ImR_Locator_i::debug () > 5)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::final_state ")
                          ACE_TEXT ("removing this from map, server <%C>\n"),
                          this, info_->ping_id ()));
        }
      if (this->remove_on_death_rh_ != 0)
        {
          this->locator_.remove_server_i (this->info_.edit());
          this->remove_on_death_rh_->send_ior("");
          this->remove_on_death_rh_ = 0;
        }
      AsyncAccessManager_ptr aam (this);
      this->locator_.remove_aam (aam);
      aam._retn(); // release w/o decrementing since table held last reference.
    }
}

void
AsyncAccessManager::notify_waiters (void)
{
  for (size_t i = 0; i < this->rh_list_.size(); i++)
    {
      ImR_ResponseHandler *rh = this->rh_list_[i];
      if (rh != 0)
        {
          if (this->status_ == ImplementationRepository::AAM_SERVER_READY)
            {
              if (this->info_->is_mode (ImplementationRepository::PER_CLIENT))
                {
                  rh->send_ior (this->partial_ior_.c_str());
                }
              else
                {
                  rh->send_ior (this->info_->partial_ior.c_str());
                }
            }
          else
            {
              try
                {
                  switch (this->status_)
                    {
                    case ImplementationRepository::AAM_NO_ACTIVATOR:
                      throw ImplementationRepository::CannotActivate
                        ("No activator registered for server.");
                    case ImplementationRepository::AAM_NOT_MANUAL:
                      throw ImplementationRepository::CannotActivate
                        ("Cannot implicitly activate MANUAL server.");
                    case ImplementationRepository::AAM_NO_COMMANDLINE:
                      throw ImplementationRepository::CannotActivate
                        ("No command line registered for server.");
                    case ImplementationRepository::AAM_RETRIES_EXCEEDED:
                      throw ImplementationRepository::CannotActivate
                        ("Restart attempt count exceeded.");
                    case  ImplementationRepository::AAM_ACTIVE_TERMINATE:
                      throw ImplementationRepository::CannotActivate
                        ("Server terminating.");
                    default: {
                      ACE_CString reason = ACE_CString ("AAM_Status is ") +
                        status_name (this->status_);
                      throw ImplementationRepository::CannotActivate (reason.c_str());
                    }
                    }
                }
              catch (const CORBA::Exception &ex)
                {
                  rh->send_exception (ex._tao_duplicate());
                }
            }
        }
    }
  this->rh_list_.clear ();
}

bool
AsyncAccessManager::is_final (ImplementationRepository::AAM_Status s)
{
  return (s == ImplementationRepository::AAM_SERVER_READY ||
          s == ImplementationRepository::AAM_SERVER_DEAD ||
          s == ImplementationRepository::AAM_NOT_MANUAL ||
          s == ImplementationRepository::AAM_NO_ACTIVATOR ||
          s == ImplementationRepository::AAM_NO_COMMANDLINE ||
          s == ImplementationRepository::AAM_RETRIES_EXCEEDED);
}

const char *
AsyncAccessManager::status_name (ImplementationRepository::AAM_Status s)
{
  switch (s)
    {
    case ImplementationRepository::AAM_INIT:
      return "INIT";
    case ImplementationRepository::AAM_SERVER_STARTED_RUNNING:
      return "SERVER_STARTED_RUNNING";
    case ImplementationRepository::AAM_ACTIVATION_SENT:
      return "ACTIVATION_SENT";
    case ImplementationRepository::AAM_WAIT_FOR_RUNNING:
      return "WAIT_FOR_RUNNING";
    case ImplementationRepository::AAM_WAIT_FOR_PING:
      return "WAIT_FOR_PING";
    case ImplementationRepository::AAM_WAIT_FOR_ALIVE:
      return "WAIT_FOR_ALIVE";
    case ImplementationRepository::AAM_WAIT_FOR_DEATH:
      return "WAIT_FOR_DEATH";
    case ImplementationRepository::AAM_SERVER_READY:
      return "SERVER_READY";
    case ImplementationRepository::AAM_SERVER_DEAD:
      return "SERVER_DEAD";
    case ImplementationRepository::AAM_NOT_MANUAL:
      return "NOT_MANUAL";
    case ImplementationRepository::AAM_NO_ACTIVATOR:
      return "NO_ACTIVATOR";
    case ImplementationRepository::AAM_NO_COMMANDLINE:
      return "NO_COMMANDLINE";
    case ImplementationRepository::AAM_RETRIES_EXCEEDED:
      return "RETRIES_EXCEEDED";
    case ImplementationRepository::AAM_UPDATE_FAILED:
      return "UPDATE_FAILED";
    case ImplementationRepository::AAM_ACTIVE_TERMINATE:
      return "ACTIVE_TERMINATE";

    }
  return "<undefined status>";
}

ImplementationRepository::AAM_Status
AsyncAccessManager::status (void) const
{
  return this->status_;
}

void
AsyncAccessManager::status (ImplementationRepository::AAM_Status s)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->status_ = s;
  if (s == ImplementationRepository::AAM_SERVER_DEAD)
    {
      this->info_.edit()->pid = 0;
    }
}

void
AsyncAccessManager::update_status (ImplementationRepository::AAM_Status s)
{
  this->status (s);
  this->info_.notify_remote_access (s);
}

void
AsyncAccessManager::activator_replied (bool success, int pid)
{
  if (success)
    {
      if (pid == 0)
        {
          this->update_status (ImplementationRepository::AAM_WAIT_FOR_RUNNING);
        }
      else
        {
          if (ImR_Locator_i::debug () > 4)
            {
              this->report ("activator_replied");
            }
          this->update_status (ImplementationRepository::AAM_SERVER_READY);
          this->info_.edit()->pid = pid;
          this->final_state ();
        }
    }
  else
    {
      this->status (ImplementationRepository::AAM_NO_ACTIVATOR);
      this->final_state ();
    }
}

void
AsyncAccessManager::shutdown_initiated (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("shutdown_initiated");
    }
  this->prev_pid_ = this->info_->pid;
  this->status (ImplementationRepository::AAM_ACTIVE_TERMINATE);
  if (this->info_->pid != 0)
    {
      AsyncAccessManager_ptr aam (this->_add_ref());
      this->locator_.make_terminating (aam,this->info_->ping_id(), this->info_->pid);
    }
  this->notify_waiters ();
}

void
AsyncAccessManager::server_is_shutting_down (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("server_is_shutting_down");
    }
  this->prev_pid_ = this->info_->pid;
  this->status (ImplementationRepository::AAM_SERVER_DEAD);
  this->final_state ();
}

void
AsyncAccessManager::server_is_running (const char *partial_ior,
                                       ImplementationRepository::ServerObject_ptr ref)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("server_is_running-start");
    }

  this->update_status (ImplementationRepository::AAM_WAIT_FOR_ALIVE);
  // Only when we are not using per client activation we should store the
  // information of the started server within our repository
  if (this->info_->is_mode (ImplementationRepository::PER_CLIENT))
    {
      this->partial_ior_ = partial_ior;
      this->server_ = ImplementationRepository::ServerObject::_duplicate (ref);
    }
  else
    {
      this->info_.edit ()->partial_ior = partial_ior;
      this->info_.edit ()->server = ImplementationRepository::ServerObject::_duplicate (ref);
    }

  if (this->locator_.pinger().is_alive (this->info_->ping_id()) == LS_ALIVE)
    {
      this->status (ImplementationRepository::AAM_SERVER_READY);
      this->final_state ();
    }

  AccessLiveListener *l = 0;
  if (this->info_->is_mode (ImplementationRepository::PER_CLIENT))
    {
      ACE_NEW (l, AccessLiveListener (this->info_->ping_id(),
                                      this,
                                      this->locator_.pinger(),
                                      this->server_.in ()));
    }
  else
    {
      ACE_NEW (l, AccessLiveListener (this->info_->ping_id(),
                                      this,
                                      this->locator_.pinger()));
    }

  LiveListener_ptr llp(l);
  if (!l->start())
    {
      this->status (ImplementationRepository::AAM_SERVER_DEAD);
      this->final_state ();
    }

  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("server_is_running-end");
    }
}

bool
AsyncAccessManager::notify_child_death (int pid)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager(%@), child death, pid <%d>, status <%C> ")
                      ACE_TEXT ("this info_.pid <%d> prev_pid <%d> waiter count <%d>\n"),
                      this, pid, status_name (status_),
                      this->info_->pid, this->prev_pid_, this->rh_list_.size() ));
    }
  if (this->info_->pid == pid || this->prev_pid_ == pid)
    {
      if (this->status_ == ImplementationRepository::AAM_WAIT_FOR_DEATH &&
          this->rh_list_.size() > 0)
        {
          this->send_start_request ();
          return true;
        }
      this->status (ImplementationRepository::AAM_SERVER_DEAD);
      this->final_state ();
      return true;
    }
  return false;
}

void
AsyncAccessManager::listener_disconnected (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      this->report ("listener_disconnected");
    }

  this->status (ImplementationRepository::AAM_SERVER_DEAD);

}

void
AsyncAccessManager::ping_replied (LiveStatus server)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::ping_replied <%C>,")
                      ACE_TEXT (" this status <%C>\n"),
                      this, LiveEntry::status_name (server), status_name (this->status_)));
    }

  switch (server)
    {
    case LS_ALIVE:
    case LS_LAST_TRANSIENT:
    case LS_TIMEDOUT:
      this->status (ImplementationRepository::AAM_SERVER_READY);
      break;
    case LS_CANCELED:
      {
        if (this->status_ == ImplementationRepository::AAM_WAIT_FOR_PING)
          {
            AccessLiveListener *l = 0;
            ACE_NEW (l, AccessLiveListener (this->info_->ping_id(),
                                            this,
                                            this->locator_.pinger()));
            LiveListener_ptr llp(l);
          }
        return;
      }
    case LS_DEAD:
      {
        if (this->status_ == ImplementationRepository::AAM_WAIT_FOR_PING)
          {
            if (this->info_->death_notify && this->info_->pid != 0)
              {
                if (ImR_Locator_i::debug () > 4)
                  {
                    ORBSVCS_DEBUG ((LM_DEBUG,
                                    ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::ping_replied pid = %d,")
                                    ACE_TEXT (" transition to WAIT_FOR_DEATH\n"),
                                    this, this->info_->pid));
                  }
                this->status (ImplementationRepository::AAM_WAIT_FOR_DEATH);
                return;
              }
            if (ImR_Locator_i::debug () > 4)
              {
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::ping_replied pid = %d,")
                                ACE_TEXT (" trying to restart server\n"),
                                this, this->info_->pid));
              }
            if (this->send_start_request ())
              {
                return;
              }
          }
        else
          {
            this->status (ImplementationRepository::AAM_SERVER_DEAD);
          }
      }
      break;
    default:
      return;
    }
  this->final_state();
}

bool
AsyncAccessManager::send_start_request (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::send_start_request, manual_start <%d>\n"),
                      this, this->manual_start_));
    }

  if ((this->locator_.opts ()->lockout () && !this->info_.edit ()->start_allowed ()) ||
      (this->retries_ == 0))
    {
      this->status (ImplementationRepository::AAM_RETRIES_EXCEEDED);
      return false;
    }

  --this->retries_;

  if (this->info_->is_mode (ImplementationRepository::MANUAL) &&
      !this->manual_start_)
    {
      this->status (ImplementationRepository::AAM_NOT_MANUAL);
      return false;
    }

  const Server_Info *startup = this->info_->active_info ();

  if (startup->cmdline.length () == 0)
    {
      this->status (ImplementationRepository::AAM_NO_COMMANDLINE);
      return false;
    }

  Activator_Info_Ptr ainfo =
    this->locator_.get_activator (startup->activator);

  if (ainfo.null () || CORBA::is_nil (ainfo->activator.in ()))
    {
      this->status (ImplementationRepository::AAM_NO_ACTIVATOR);
      return false;
    }

  PortableServer::ServantBase_var callback = new ActivatorReceiver (this,
                                                                    this->poa_.in());
  PortableServer::ObjectId_var oid = this->poa_->activate_object (callback.in());
  CORBA::Object_var obj = this->poa_->id_to_reference (oid.in());
  ImplementationRepository::AMI_ActivatorHandler_var cb =
    ImplementationRepository::AMI_ActivatorHandler::_narrow (obj.in());

  ACE_CString servername;

  if (this->info_->is_mode (ImplementationRepository::PER_CLIENT))
    {
      servername = startup->key_name_;
    }
  else
    {
      servername = unique_prefix + startup->key_name_;
    }

  ainfo->activator->sendc_start_server (cb.in(),
                                        servername.c_str (),
                                        startup->cmdline.c_str (),
                                        startup->dir.c_str (),
                                        startup->env_vars);
  this->update_status (ImplementationRepository::AAM_ACTIVATION_SENT);
  return true;
}

AsyncAccessManager *
AsyncAccessManager::_add_ref (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);
  ++this->refcount_;

  return this;
}

void
AsyncAccessManager::_remove_ref (void)
{
  int count = 0;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    count = --this->refcount_;
  }
  if (count == 0)
    {
      delete this;
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

ActivatorReceiver::ActivatorReceiver (AsyncAccessManager *aam,
                                      PortableServer::POA_ptr poa)
  :aam_ (aam->_add_ref ()),
   poa_ (PortableServer::POA::_duplicate (poa))
{
}


ActivatorReceiver::~ActivatorReceiver (void)
{
}

void
ActivatorReceiver::start_server (void)
{
  this->aam_->activator_replied (true, 0);
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

void
ActivatorReceiver::start_server_excep (Messaging::ExceptionHolder *holder)
{
  try
    {
      holder->raise_exception ();
    }
  catch (const ImplementationRepository::CannotActivate &ca)
    {
      if (ACE_OS::strstr (ca.reason.in(),"pid:") == ca.reason.in())
        {
          int const pid = ACE_OS::atoi (ca.reason.in()+4);
          this->aam_->activator_replied (true, pid);
        }
      else
        {
          this->aam_->activator_replied (false, 0);
        }
    }
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

void
ActivatorReceiver::shutdown (void)
{
  // no-op, just satisfy virtual function
}

void
ActivatorReceiver::shutdown_excep (Messaging::ExceptionHolder * )
{
  // no-op, just satisfy virtual function
}

void
ActivatorReceiver::kill_server (CORBA::Boolean )
{
  // no-op, just satisfy virtual function
}

void
ActivatorReceiver::kill_server_excep (Messaging::ExceptionHolder * )
{
  // no-op, just satisfy virtual function
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

AccessLiveListener::AccessLiveListener (const char *server,
                                        AsyncAccessManager *aam,
                                        LiveCheck &pinger)
  :LiveListener (server),
   aam_ (aam->_add_ref ()),
   pinger_ (pinger),
   status_ (LS_UNKNOWN),
   per_client_ (false),
   srv_ref_ (ImplementationRepository::ServerObject::_nil())
{
}

AccessLiveListener::AccessLiveListener (const char *server,
                                        AsyncAccessManager *aam,
                                        LiveCheck &pinger,
                                        ImplementationRepository::ServerObject_ptr ref)
  :LiveListener (server),
   aam_ (aam->_add_ref ()),
   pinger_ (pinger),
   status_ (LS_UNKNOWN),
   per_client_ (true),
   srv_ref_ (ImplementationRepository::ServerObject::_duplicate (ref))
{
}

AccessLiveListener::~AccessLiveListener (void)
{
  if (!this->aam_.is_nil())
  {
    aam_->listener_disconnected();
  }
}

bool
AccessLiveListener::start (void)
{
  bool const started = this->per_client_ ?
    this->pinger_.add_per_client_listener (this, srv_ref_.in()) :
    this->pinger_.add_listener (this);
  if (!started)
    {
      this->aam_ = 0;
    }
  return started;
}

bool
AccessLiveListener::status_changed (LiveStatus status)
{
  this->status_ = status;
  switch (status_) {
    case LS_TRANSIENT:
    {
      return false;
    }
    default:
      if (!this->aam_.is_nil())
        {
          this->aam_->ping_replied (status);
        }
      this->aam_ = 0;
    }
  return true;
}
