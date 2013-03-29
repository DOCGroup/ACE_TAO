// -*- C++ -*-
// $Id$

#include "AsyncAccessManager.h"
#include "ImR_Locator_i.h"
#include "Locator_Repository.h"



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

AsyncAccessManager::AsyncAccessManager (const Server_Info &info,
                                        bool manual,
                                        ImR_Locator_i &locator)
  :info_(0),
   manual_start_ (manual),
   locator_(locator),
   poa_(locator.root_poa()),
   rh_list_(),
   status_(AAM_INIT),
   refcount_(1),
   lock_()
{
  ACE_DEBUG ((LM_DEBUG,"New AAM: this = %x, name = %s\n",
              this, info.name.c_str()));
  this->info_ = new Server_Info (info);
}

AsyncAccessManager::~AsyncAccessManager (void)
{
  delete this->info_;
}

bool
AsyncAccessManager::has_server (const char *s)
{
  return ACE_OS::strcmp (this->info_->name.c_str(),s) == 0;
}

void
AsyncAccessManager::add_interest (ImR_ResponseHandler *rh)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    this->rh_list_.push_back (rh);
  }
  if (this->locator_.debug() > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) AsyncAccessManager::add_interest: ")
                  ACE_TEXT ("server = <%C>, status = %d count = %d\n"),
                  this->info_->name.c_str(), this->status_, this->rh_list_.size()));
    }

  if (this->status_ == AAM_SERVER_READY)
    {
      if (this->locator_.pinger().is_alive (this->info_->name.c_str()))
        {
          this->final_state();
          return;
        }
    }

  if (this->status_ == AAM_INIT || this->status_ == AAM_SERVER_READY)
    {
      // This is not a leak. The listener registers with
      // the pinger and will delete itself when done.
      AsyncLiveListener *l = 0;
      ACE_NEW (l, AsyncLiveListener (this->info_->name.c_str(),
                                     *this,
                                     this->locator_.pinger()));
      if (!l->start())
        {
          if (!this->send_start_request())
            {
              this->final_state();
            }
        }
      else
        {
          this->status (AAM_WAIT_FOR_PING);
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) AsyncAccessManager::add_interest: ")
              ACE_TEXT ("server = <%C>, status = %d returning\n"),
              this->info_->name.c_str(), this->status_));
}

void
AsyncAccessManager::final_state (void)
{
  if (this->locator_.debug() > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) AsyncAccessManager::final_state: ")
                  ACE_TEXT ("status = %d, server = <%C> list size = %d\n"),
                  this->status_, this->info_->name.c_str(), rh_list_.size()));
    }

  for (size_t i = 0; i < this->rh_list_.size(); i++)
    {
      ImR_ResponseHandler *rh = this->rh_list_[i];
      if (rh != 0)
        {
          if (this->status_ == AAM_SERVER_READY)
            {
              rh->send_ior (this->info_->partial_ior.c_str());
            }
          else
            {
              try
                {
                  switch (this->status_)
                    {
                    case AAM_NO_ACTIVATOR:
                      throw ImplementationRepository::CannotActivate
                        ("No activator registered for server.");
                    case AAM_NOT_MANUAL:
                      throw ImplementationRepository::CannotActivate
                        ("Cannot implicitly activate MANUAL server.");
                    case AAM_NO_COMMANDLINE:
                      throw ImplementationRepository::CannotActivate
                        ("No command line registered for server.");
                    default:
                      throw ImplementationRepository::CannotActivate
                        ("Unknown Failure");
                    }
                }
              catch (CORBA::Exception &ex)
                {
                  rh->send_exception (&ex);
                }
            }
        }
    }
  this->rh_list_.clear ();
  if (this->info_->activation_mode == ImplementationRepository::PER_CLIENT ||
      this->status_ != AAM_SERVER_READY)
    {
      this->locator_.remove_aam (this);
    }
}

AAM_Status
AsyncAccessManager::status (void) const
{
  return this->status_;
}

void
AsyncAccessManager::status (AAM_Status s)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->status_ = s;
}

void
AsyncAccessManager::activator_replied (bool success)
{
  if (this->locator_.debug() > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) AsyncAccessManager::activator_replied: ")
                  ACE_TEXT ("success = %d, status = %d\n"),
                  success, this->status_));
    }
  if (success)
    {
      this->status (AAM_WAIT_FOR_RUNNING);
    }
  else
    {
      this->status (AAM_NO_ACTIVATOR);
      this->final_state ();
    }
}

void
AsyncAccessManager::server_is_running (const char *partial_ior)
{
  if (this->locator_.debug() > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) AsyncAccessManager::server_is_running: ")
                  ACE_TEXT ("name = %C, status = %d\n"),
                  this->info_->name.c_str(), this->status_));
    }
  this->status (AAM_WAIT_FOR_ALIVE);
  this->info_->partial_ior = partial_ior;

  if (this->locator_.pinger().is_alive (this->info_->name.c_str()))
    {
      this->status (AAM_SERVER_READY);
      this->final_state ();
    }

  // This is not a leak. The listener registers with
  // the pinger and will delete itself when done.
  AsyncLiveListener *l = 0;
  ACE_NEW (l, AsyncLiveListener (this->info_->name.c_str(),
                                 *this,
                                 this->locator_.pinger()));
  if (!l->start())
    {
      this->status (AAM_SERVER_DEAD);
      this->final_state ();
    }
}

void
AsyncAccessManager::ping_replied (LiveStatus server)
{
  switch (server)
    {
    case LS_ALIVE:
    case LS_LAST_TRANSIENT:
    case LS_TIMEDOUT:
      this->status (AAM_SERVER_READY);
      break;
    case LS_DEAD:
      {
        if (this->status_ == AAM_WAIT_FOR_PING)
          {
            if (this->send_start_request ())
              {
                return;
              }
          }
        else
          {
            this->status (AAM_SERVER_DEAD);
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
  if (this->info_->activation_mode == ImplementationRepository::MANUAL &&
      !this->manual_start_)
    {
      this->status (AAM_NOT_MANUAL);
      return false;
    }

  if (this->info_->cmdline.length () == 0)
    {
      this->status (AAM_NO_COMMANDLINE);
      return false;
    }

  Activator_Info_Ptr ainfo =
    this->locator_.get_activator (this->info_->activator);

  if (ainfo.null () || CORBA::is_nil (ainfo->activator.in ()))
    {
      this->status (AAM_NO_ACTIVATOR);
      return false;
    }

  PortableServer::ServantBase_var callback = new ActivatorReceiver (this,
                                                                    this->poa_.in());
  PortableServer::ObjectId_var oid = this->poa_->activate_object (callback.in());
  CORBA::Object_var obj = this->poa_->id_to_reference (oid.in());
  ImplementationRepository::AMI_ActivatorHandler_var cb =
    ImplementationRepository::AMI_ActivatorHandler::_narrow (obj.in());

  ainfo->activator->sendc_start_server (cb.in(),
                                        this->info_->name.c_str (),
                                        this->info_->cmdline.c_str (),
                                        this->info_->dir.c_str (),
                                        this->info_->env_vars);
  this->status (AAM_ACTIVATION_SENT);
  return true;
}

void
AsyncAccessManager::add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  ++this->refcount_;
}

void
AsyncAccessManager::remove_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  if (--this->refcount_ == 0)
    {
      delete this;
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

ActivatorReceiver::ActivatorReceiver (AsyncAccessManager *aam,
                                      PortableServer::POA_ptr poa)
  :aam_ (aam),
   poa_ (PortableServer::POA::_duplicate (poa))
{
  this->aam_->add_ref ();
}


ActivatorReceiver::~ActivatorReceiver (void)
{
  this->aam_->remove_ref ();
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


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

AsyncLiveListener::AsyncLiveListener (const char *server,
                                      AsyncAccessManager &aam,
                                      LiveCheck &pinger)
  :LiveListener (server),
   aam_ (aam),
   pinger_ (pinger),
   status_ (LS_UNKNOWN)
{
  this->aam_.add_ref ();
}

AsyncLiveListener::~AsyncLiveListener (void)
{
  this->aam_.remove_ref ();
}

bool
AsyncLiveListener::start (void)
{
  bool rtn = this->pinger_.add_listener (this);
  ACE_DEBUG ((LM_DEBUG,
              "AsyncLiveListener::start, add_listener returned %d\n", rtn));
  if (!rtn)
    delete this;
  return rtn;
}

bool
AsyncLiveListener::status_changed (LiveStatus status)
{
  this->status_ = status;
  if (status == LS_TRANSIENT)
    {
      if (!this->pinger_.add_listener (this))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "AsyncLiveListener::status_changed,  deleting(1)\n"));
          this->aam_.ping_replied (status);
          delete this;
          return true;
        }
      return false;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "AsyncLiveListener::status_changed, status = %d, deleting(2)\n", status));
      this->aam_.ping_replied (status);
      delete this;
      return true;
    }
  return true;
}
