// -*- C++ -*-
// $Id$

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
   locator_ (locator),
   poa_ (locator.root_poa ()),
   rh_list_ (),
   status_ (ImplementationRepository::AAM_INIT),
   refcount_ (1),
   lock_ ()
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::ctor server = %C\n"),
                      this, info->ping_id ()));
    }
}

AsyncAccessManager::~AsyncAccessManager (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::dtor server = %C\n"),
                      this, info_->ping_id ()));
    }
}

void
AsyncAccessManager::started_running (void)
{
  this->update_status(ImplementationRepository::AAM_SERVER_STARTED_RUNNING);
}

bool
AsyncAccessManager::has_server (const char *s)
{
  return ACE_OS::strcmp (this->info_->ping_id (), s) == 0;
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
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager(%@)::add_interest status = %s\n"),
                      this,
                      status_name (this->status_)));
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

void
AsyncAccessManager::remote_state (ImplementationRepository::AAM_Status state)
{
  this->status (state);
  if (AsyncAccessManager::is_final (state))
    {
      this->final_state (false);
    }
}

void
AsyncAccessManager::final_state (bool active)
{
  bool success = this->status_ == ImplementationRepository::AAM_SERVER_READY;
  this->info_.edit ()->started (success);
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
                          ACE_TEXT ("removing this from map, server = %s\n"),
                          this, info_->ping_id ()));
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
              rh->send_ior (this->info_->partial_ior.c_str());
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
                    default:
                      throw ImplementationRepository::CannotActivate
                        ("Unknown Failure");
                    }
                }
              catch (CORBA::Exception &ex)
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

const ACE_TCHAR *
AsyncAccessManager::status_name (ImplementationRepository::AAM_Status s)
{
  switch (s)
    {
    case ImplementationRepository::AAM_INIT:
      return ACE_TEXT ("INIT");
    case ImplementationRepository::AAM_SERVER_STARTED_RUNNING:
      return ACE_TEXT ("SERVER_STARTED_RUNNING");
    case ImplementationRepository::AAM_ACTIVATION_SENT:
      return ACE_TEXT ("ACTIVATION_SENT");
    case ImplementationRepository::AAM_WAIT_FOR_RUNNING:
      return ACE_TEXT ("WAIT_FOR_RUNNING");
    case ImplementationRepository::AAM_WAIT_FOR_PING:
      return ACE_TEXT ("WAIT_FOR_PING");
    case ImplementationRepository::AAM_WAIT_FOR_ALIVE:
      return ACE_TEXT ("WAIT_FOR_ALIVE");
    case ImplementationRepository::AAM_WAIT_FOR_DEATH:
      return ACE_TEXT ("WAIT_FOR_DEATH");
    case ImplementationRepository::AAM_SERVER_READY:
      return ACE_TEXT ("SERVER_READY");
    case ImplementationRepository::AAM_SERVER_DEAD:
      return ACE_TEXT ("SERVER_DEAD");
    case ImplementationRepository::AAM_NOT_MANUAL:
      return ACE_TEXT ("NOT_MANUAL");
    case ImplementationRepository::AAM_NO_ACTIVATOR:
      return ACE_TEXT ("NO_ACTIVATOR");
    case ImplementationRepository::AAM_NO_COMMANDLINE:
      return ACE_TEXT ("NO_COMMANDLINE");
    case ImplementationRepository::AAM_RETRIES_EXCEEDED:
      return ACE_TEXT ("RETRIES_EXCEEDED");
    }
  return ACE_TEXT ("<undefined status>");
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
}

void
AsyncAccessManager::update_status (ImplementationRepository::AAM_Status s)
{
  this->status (s);
  this->info_.notify_remote_access (s);
}

void
AsyncAccessManager::activator_replied (bool success)
{
  if (success)
    {
      this->update_status (ImplementationRepository::AAM_WAIT_FOR_RUNNING);
    }
  else
    {
      this->status (ImplementationRepository::AAM_NO_ACTIVATOR);
      this->final_state ();
    }
}

void
AsyncAccessManager::server_is_shutting_down (void)
{
  this->status (ImplementationRepository::AAM_SERVER_DEAD);
  this->final_state ();
}

void
AsyncAccessManager::server_is_running (const char *partial_ior,
                                       ImplementationRepository::ServerObject_ptr ref)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager::server_is_running\n")));
    }

  this->update_status (ImplementationRepository::AAM_WAIT_FOR_ALIVE);
  this->info_.edit ()->partial_ior = partial_ior;
  this->info_.edit ()->server =
    ImplementationRepository::ServerObject::_duplicate (ref);

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
                                      this->info_->active_info()->server.in()));
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
}

void
AsyncAccessManager::notify_child_death (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager, child death\n")));
    }
  if (this->status_ == ImplementationRepository::AAM_WAIT_FOR_DEATH &&
      this->rh_list_.size() > 0)
    {
      this->send_start_request ();
      return;
    }
  this->status (ImplementationRepository::AAM_SERVER_DEAD);
  this->final_state ();
}

void
AsyncAccessManager::ping_replied (LiveStatus server)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncAccessManager::ping_replied %s,")
                      ACE_TEXT (" this status %s\n"),
                      LiveEntry::status_name (server), status_name (this->status_)));
    }

  switch (server)
    {
    case LS_ALIVE:
    case LS_LAST_TRANSIENT:
    case LS_TIMEDOUT:
      this->status (ImplementationRepository::AAM_SERVER_READY);
      break;
    case LS_DEAD:
      {
        if (this->status_ == ImplementationRepository::AAM_WAIT_FOR_PING)
          {
            if (this->info_->death_notify && this->info_->pid != 0)
              {
                if (ImR_Locator_i::debug () > 4)
                  {
                    ORBSVCS_DEBUG ((LM_DEBUG,
                                    ACE_TEXT ("(%P|%t) AsyncAccessManager::ping_replied pid = %d,")
                                    ACE_TEXT (" transition to WAIT_FOR_DEATH\n"),
                                    this->info_->pid));
                  }
                this->status (ImplementationRepository::AAM_WAIT_FOR_DEATH);
                return;
              }
            if (ImR_Locator_i::debug () > 4)
              {
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) AsyncAccessManager::ping_replied pid = %d,")
                                ACE_TEXT (" trying to restart server\n"),
                                this->info_->pid));
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
                      ACE_TEXT ("(%P|%t) AsyncAccessManager::send_start_request, manual_start_ %d\n"),
                      this->manual_start_));
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
  this->aam_->activator_replied (true);
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

void
ActivatorReceiver::start_server_excep (Messaging::ExceptionHolder *)
{
  this->aam_->activator_replied (false);
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
}

bool
AccessLiveListener::start (void)
{
  bool rtn = this->per_client_ ?
    this->pinger_.add_per_client_listener (this, srv_ref_.in()) :
    this->pinger_.add_listener (this);
  return rtn;
}

bool
AccessLiveListener::status_changed (LiveStatus status)
{
  this->status_ = status;
  if (status == LS_TRANSIENT)
    {
      return false;
    }
  else
    {
      this->aam_->ping_replied (status);
    }
  return true;
}
