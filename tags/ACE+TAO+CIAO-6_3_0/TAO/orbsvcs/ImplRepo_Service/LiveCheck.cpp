// -*- C++ -*-
// $Id$

#include "LiveCheck.h"
#include "ImR_Locator_i.h"

#include "orbsvcs/Log_Macros.h"

#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_Queue_Iterator.h"

LiveListener::LiveListener (const char *server)
  : server_ (server),
    refcount_ (1),
    lock_ ()
{
}

LiveListener::~LiveListener (void)
{
}

const char *
LiveListener::server (void) const
{
  return this->server_.c_str ();
}

LiveListener *
LiveListener::_add_ref (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);
  ++this->refcount_;
  if (ImR_Locator_i::debug () > 5)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("LiveListener::add_ref, %s, count = %d\n"),
                      server_.c_str(), refcount_));
    }
  return this;
}

void
LiveListener::_remove_ref (void)
{
  int count = 0;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    count = --this->refcount_;
    if (ImR_Locator_i::debug () > 5)
      {
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT  ("LiveListener::remove_ref, %s, count = %d\n"),
                        server_.c_str(), count));
      }
  }
  if (count == 0)
    {
      delete this;
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

const int LiveEntry::reping_msec_[] = {10, 100, 500, 1000, 1000, 2000, 2000, 5000, 5000};
int LiveEntry::reping_limit_ = sizeof (LiveEntry::reping_msec_) / sizeof (int);

const ACE_TCHAR *
LiveEntry::status_name (LiveStatus s)
{
  switch (s)
    {
    case LS_UNKNOWN:
      return ACE_TEXT ("UNKNOWN");
    case LS_PING_AWAY:
      return ACE_TEXT ("PING_AWAY");
    case LS_DEAD:
      return ACE_TEXT ("DEAD");
    case LS_ALIVE:
      return ACE_TEXT ("ALIVE");
    case LS_TRANSIENT:
      return ACE_TEXT ("TRANSIENT");
    case LS_LAST_TRANSIENT:
      return ACE_TEXT ("LAST_TRANSIENT");
    case LS_TIMEDOUT:
      return ACE_TEXT ("TIMEDOUT");
    }
  return ACE_TEXT ("<undefined status>");
}

void
LiveEntry::set_reping_limit (int max)
{
  int array_max =  sizeof (LiveEntry::reping_msec_) / sizeof (int);
  LiveEntry::reping_limit_ = max < array_max && max >= 0 ? max : array_max;
}

bool
LiveEntry::reping_available (void)
{
  return this->repings_ < this->max_retry_;
}

int
LiveEntry::next_reping (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, -1);
  return this->reping_available() ? LiveEntry::reping_msec_[this->repings_++] : -1;
}

void
LiveEntry::max_retry_msec (int msec)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  for (this->max_retry_ = 0;
       this->max_retry_ <  LiveEntry::reping_limit_ && msec > 0;
       ++this->max_retry_)
    {
      msec -= LiveEntry::reping_msec_[this->repings_];
    }
}

LiveEntry::LiveEntry (LiveCheck *owner,
                      const char *server,
                      bool may_ping,
                      ImplementationRepository::ServerObject_ptr ref)
  : owner_ (owner),
    server_ (server),
    ref_ (ImplementationRepository::ServerObject::_duplicate (ref)),
    liveliness_ (LS_UNKNOWN),
    next_check_ (ACE_High_Res_Timer::gettimeofday_hr()),
    repings_ (0),
    max_retry_ (LiveEntry::reping_limit_),
    may_ping_ (may_ping),
    listeners_ (),
    lock_ (),
    callback_ (0)
{
  if (ImR_Locator_i::debug () > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) LiveEntry::ctor server = %s, may_ping = %d\n"),
                      server, may_ping));
    }
}

LiveEntry::~LiveEntry (void)
{
  if (this->callback_.in () != 0)
    {
      PingReceiver *rec = dynamic_cast<PingReceiver *>(this->callback_.in());
      if (rec != 0)
        {
          rec->cancel ();
        }
    }
}

void
LiveEntry::release_callback (void)
{
  this->callback_ = 0;
}

void
LiveEntry::add_listener (LiveListener *ll)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  LiveListener_ptr llp(ll->_add_ref());
  this->listeners_.insert (llp);
}

void
LiveEntry::remove_listener (LiveListener *ll)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  LiveListener_ptr llp(ll->_add_ref());
  int result = this->listeners_.remove (llp);
  if (ImR_Locator_i::debug() > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) LiveEntry::remove_listener, result = %d\n"),
                      result));
    }
}

void
LiveEntry::reset_status (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  if ( this->liveliness_ == LS_ALIVE ||
       this->liveliness_ == LS_LAST_TRANSIENT ||
       this->liveliness_ == LS_TIMEDOUT)
    {
      this->liveliness_ = LS_UNKNOWN;
      this->repings_ = 0;
      this->next_check_ = ACE_High_Res_Timer::gettimeofday_hr();
    }
  if (ImR_Locator_i::debug () > 2)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) LiveEntry::reset_status this = %x, ")
                      ACE_TEXT ("server = %C status = %s\n"),
                      this, this->server_.c_str(),
                      status_name (this->liveliness_)));
    }

}

LiveStatus
LiveEntry::status (void) const
{
  if (!this->may_ping_)
    {
      return LS_ALIVE;
    }


  if ( this->liveliness_ == LS_ALIVE &&
       this->owner_->ping_interval() != ACE_Time_Value::zero )
    {
      ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
      if (now >= this->next_check_)
        {
          return LS_UNKNOWN;
        }
    }
  return this->liveliness_;
}

void
LiveEntry::update_listeners (void)
{
  Listen_Set remove;
  for (Listen_Set::ITERATOR i(this->listeners_);
       !i.done();
       i.advance ())
    {

      if ((*i)->status_changed (this->liveliness_))
        {
          remove.insert (*i);
        }
    }
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    for (Listen_Set::ITERATOR i (remove);
         !i.done();
         i.advance ())
      {
        LiveListener_ptr llp (*i);
        int result = this->listeners_.remove (llp);
        if (result == -1)
          {
          }
      }
    LiveListener_ptr dummy;
    this->listeners_.remove (dummy);
  }
}

void
LiveEntry::status (LiveStatus l)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    this->liveliness_ = l;
    if (l == LS_ALIVE)
      {
        ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
        this->next_check_ = now + owner_->ping_interval();
      }
    if (l == LS_TRANSIENT && !this->reping_available())
      {
        this->liveliness_ = LS_LAST_TRANSIENT;
      }
  }
  this->update_listeners ();

  if (this->listeners_.size() > 0)
    {
      if (ImR_Locator_i::debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::status change, ")
                          ACE_TEXT ("server = %C status = %s\n"),
                          this->server_.c_str(),
                          status_name (this->liveliness_)));
        }
      this->owner_->schedule_ping (this);
    }
  else
    {
      if (this->owner_->remove_per_client_entry (this))
        {
          delete (this);
        }
    }
}

const ACE_Time_Value &
LiveEntry::next_check (void) const
{
  return this->next_check_;
}

const char *
LiveEntry::server_name (void) const
{
  return this->server_.c_str();
}

bool
LiveEntry::validate_ping (bool &want_reping, ACE_Time_Value& next)
{
  if (this->liveliness_ == LS_PING_AWAY ||
      this->liveliness_ == LS_DEAD ||
      this->listeners_.size () == 0)
    {
      if (ImR_Locator_i::debug () > 4)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, status ")
                          ACE_TEXT ("= %s, listeners = %d server %C\n"),
                          status_name (this->liveliness_), this->listeners_.size (),
                          this->server_.c_str()));
        }
      return false;
    }
  ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
  ACE_Time_Value diff = this->next_check_ - now;
  long msec = diff.msec();
  if (msec > 0)
    {
      if (!want_reping || this->next_check_ < next)
        {
          want_reping = true;
          next = this->next_check_;
        }
      if (ImR_Locator_i::debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, ")
                          ACE_TEXT ("status = %s, listeners = %d, ")
                          ACE_TEXT ("diff = %d,%d, msec = %d ")
                          ACE_TEXT ("server %C\n"),
                          status_name (this->liveliness_), this->listeners_.size (),
                          diff.sec(), diff.usec(), msec,
                          this->server_.c_str()));
        }
      return false;
    }
  switch (this->liveliness_)
    {
    case LS_UNKNOWN:
      break;
    case LS_ALIVE:
    case LS_TIMEDOUT:
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, false);
        this->next_check_ = now + owner_->ping_interval();
      }
      break;
    case LS_TRANSIENT:
    case LS_LAST_TRANSIENT:
      {
        int ms = this->next_reping ();
        if (ms != -1)
          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, false);
            if (this->liveliness_ == LS_LAST_TRANSIENT)
              {
                this->liveliness_ = LS_TRANSIENT;
              }
            ACE_Time_Value next (ms / 1000, (ms % 1000) * 1000);
            this->next_check_ = now + next;
            if (ImR_Locator_i::debug () > 4)
              {
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, ")
                                ACE_TEXT ("transient, reping in %d ms, ")
                                ACE_TEXT ("server %C\n"),
                                ms, this->server_.c_str()));
              }
          }
        else
          {
            if (this->liveliness_ == LS_TRANSIENT)
              {
                ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, false);
                this->liveliness_ = LS_LAST_TRANSIENT;
              }
            if (ImR_Locator_i::debug () > 2)
              {
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, ")
                                ACE_TEXT ("transient, no more repings, ")
                                ACE_TEXT ("server %C\n"),
                                this->server_.c_str()));
              }
            if (this->listeners_.size() > 0)
              {
                this->update_listeners ();
              }
            return false;
          }
      }
      break;
    default:;
    }
  return true;
}

void
LiveEntry::do_ping (PortableServer::POA_ptr poa)
{
  this->callback_ = new PingReceiver (this, poa);
  PortableServer::ObjectId_var oid = poa->activate_object (this->callback_.in());
  CORBA::Object_var obj = poa->id_to_reference (oid.in());
  ImplementationRepository::AMI_ServerObjectHandler_var cb =
    ImplementationRepository::AMI_ServerObjectHandler::_narrow (obj.in());
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    this->liveliness_ = LS_PING_AWAY;
  }
  try
    {
      this->ref_->sendc_ping (cb.in());
      if (ImR_Locator_i::debug () > 3)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::do_ping, ")
                          ACE_TEXT ("sendc_ping returned OK\n")));
        }
    }
  catch (CORBA::Exception &ex)
    {
      if (ImR_Locator_i::debug () > 3)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::do_ping, ")
                          ACE_TEXT ("sendc_ping threw %C\n"), ex._name()));
        }
      this->status (LS_DEAD);
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

PingReceiver::PingReceiver (LiveEntry *entry, PortableServer::POA_ptr poa)
  :poa_ (PortableServer::POA::_duplicate(poa)),
   entry_ (entry)
{
}

PingReceiver::~PingReceiver (void)
{
}

void
PingReceiver::cancel (void)
{
  if (ImR_Locator_i::debug () > 4)
    {
      const char *server = "<not available>";
      if (this->entry_ != 0)
        {
          server = this->entry_->server_name ();
        }
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) PingReceiver::cancel server = %s\n"),
                      server));
    }

  this->entry_ = 0;
  try
    {
      PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
      poa_->deactivate_object (oid.in());
    }
  catch (CORBA::Exception &ex)
    {
      if (ImR_Locator_i::debug () > 4)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) PingReceiver::cancel caught %C\n"),
                          ex._name ()));
        }
    }
}

void
PingReceiver::ping (void)
{
  if (this->entry_ != 0)
    {
      if (ImR_Locator_i::debug () > 5)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) PingReceiver::ping received from %C\n"),
                          this->entry_->server_name ()));
        }
      this->entry_->release_callback ();
      this->entry_->status (LS_ALIVE);
    }
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

void
PingReceiver::ping_excep (Messaging::ExceptionHolder * excep_holder)
{
  const CORBA::ULong TAO_MINOR_MASK = 0x00000f80;
  try
    {
      if (ImR_Locator_i::debug () > 5)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) PingReceiver::ping_excep received from %C\n"),
                          this->entry_->server_name ()));
        }
      excep_holder->raise_exception ();
    }
  catch (CORBA::TRANSIENT &ex)
    {
      switch (ex.minor () & TAO_MINOR_MASK)
        {
        case TAO_POA_DISCARDING:
        case TAO_POA_HOLDING:
          {
            if (this->entry_ != 0)
              {
                this->entry_->release_callback ();
                this->entry_->status (LS_TRANSIENT);
             }
            break;
          }
        default: //case TAO_INVOCATION_SEND_REQUEST_MINOR_CODE:
          {
            if (this->entry_ != 0)
              {
                this->entry_->release_callback ();
                this->entry_->status (LS_DEAD);
             }
          }
        }
    }
  catch (CORBA::TIMEOUT &ex)
    {
      if (this->entry_ != 0)
        {
          this->entry_->release_callback ();
          if ((ex.minor () & TAO_MINOR_MASK) == TAO_TIMEOUT_CONNECT_MINOR_CODE)
            {
              this->entry_->status (LS_DEAD);
            }
          else
            {
              this->entry_->status (LS_TIMEDOUT);
            }
        }
    }
  catch (CORBA::Exception &)
    {
      if (this->entry_ != 0)
        {
          this->entry_->release_callback ();
          this->entry_->status (LS_DEAD);
        }
    }

  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

LC_TimeoutGuard::LC_TimeoutGuard (LiveCheck *owner, int token)
  :owner_ (owner),
   token_ (token),
   blocked_ (owner->handle_timeout_busy_ == 0)
{
  if (!blocked_)
    {
      --owner_->handle_timeout_busy_;
    }
}

LC_TimeoutGuard::~LC_TimeoutGuard (void)
{
  if (blocked_)
    {
      return;
    }

  ++owner_->handle_timeout_busy_;

  owner_->remove_deferred_servers ();

  if (owner_->want_timeout_)
    {
      ACE_Time_Value delay = ACE_Time_Value::zero;
      if (owner_->deferred_timeout_ != ACE_Time_Value::zero)
        {
          ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
          if (owner_->deferred_timeout_ > now)
            delay = owner_->deferred_timeout_ - now;
        }
      ++owner_->token_;
      if (ImR_Locator_i::debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LC_TimeoutGuard(%d)::dtor,")
                          ACE_TEXT ("scheduling new timeout(%d), delay = %d,%d\n"),
                          this->token_, owner_->token_, delay.sec(), delay.usec()));
        }
      owner_->reactor()->schedule_timer (owner_,
                                         reinterpret_cast<void *>(owner_->token_),
                                         delay);
      owner_->want_timeout_ = false;
    }
  else
    {
      if (ImR_Locator_i::debug () > 3)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LC_TimeoutGuard(%d)::dtor,")
                          ACE_TEXT ("no pending timeouts requested\n"),
                          this->token_));
        }
    }
}

bool LC_TimeoutGuard::blocked (void)
{
  return this->blocked_;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

LiveCheck::LiveCheck ()
  :ping_interval_(),
   running_ (false),
   token_ (100),
   handle_timeout_busy_ (1),
   want_timeout_ (false),
   deferred_timeout_ (0,0)
{
}

LiveCheck::~LiveCheck (void)
{
  for (LiveEntryMap::iterator em (this->entry_map_); !em.done(); em++)
    {
      delete em->int_id_;
    }
  this->entry_map_.unbind_all();

  for (PerClientStack::iterator pc (this->per_client_); !pc.done(); pc++)
    {
      delete *pc;
    }
  this->per_client_.reset ();
  this->removed_entries_.reset ();
}

void
LiveCheck::init (CORBA::ORB_ptr orb,
                 const ACE_Time_Value &pi)
{
  this->ping_interval_ = pi;
  ACE_Reactor *r = orb->orb_core()->reactor();
  this->reactor (r);
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
  this->poa_ = PortableServer::POA::_narrow (obj.in());
  this->running_ = true;
}

void
LiveCheck::shutdown (void)
{
  this->running_ = false;
  this->reactor()->cancel_timer (this);
}

const ACE_Time_Value &
LiveCheck::ping_interval (void) const
{
  return this->ping_interval_;
}

int
LiveCheck::handle_timeout (const ACE_Time_Value &,
                           const void * tok)
{
#if (ACE_SIZEOF_VOID_P == 8)
  ACE_INT64 token = reinterpret_cast<ACE_INT64>(tok);
#else
  ACE_INT32 token = reinterpret_cast<ACE_INT32>(tok);
#endif
  if (ImR_Locator_i::debug () > 2)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) LiveCheck::handle_timeout(%d), ")
                      ACE_TEXT ("running = %d\n"),
                      token, this->running_));
    }
  if (!this->running_)
    return -1;

  LC_TimeoutGuard tg (this, static_cast<int>(token));
  if (tg.blocked ())
    return 0;

  LiveEntryMap::iterator le_end = this->entry_map_.end();
  for (LiveEntryMap::iterator le = this->entry_map_.begin();
       le != le_end;
       ++le)
    {
      LiveEntry *entry = le->item ();
      if (entry->validate_ping (this->want_timeout_, this->deferred_timeout_))
        {
          entry->do_ping (poa_.in ());
          if (ImR_Locator_i::debug () > 2)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) LiveCheck::handle_timeout(%d)")
                              ACE_TEXT (", ping sent\n"),
                              token));
            }
        }
      else
        {
          if (ImR_Locator_i::debug () > 4)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) LiveCheck::handle_timeout(%d)")
                              ACE_TEXT (", ping skipped\n"),
                              token));
            }
        }
    }

  PerClientStack::iterator pe_end = this->per_client_.end();
  for (PerClientStack::iterator pe = this->per_client_.begin();
       pe != pe_end;
       ++pe)
    {
      LiveEntry *entry = *pe;
      if (entry != 0)
        {
          if (entry->validate_ping (this->want_timeout_, this->deferred_timeout_))
            {
              entry->do_ping (poa_.in ());
            }
          LiveStatus status = entry->status ();
          if (status != LS_PING_AWAY && status != LS_TRANSIENT)
            {
              this->per_client_.remove (entry);
              delete entry;
            }
        }
    }

  return 0;
}

bool
LiveCheck::has_server (const char *server)
{
  ACE_CString s (server);
  LiveEntry *entry = 0;
  int result = entry_map_.find (s, entry);
  return (result == 0 && entry != 0);
}

void
LiveCheck::add_server (const char *server,
                       bool may_ping,
                       ImplementationRepository::ServerObject_ptr ref)
{
  if (!this->running_)
    return;

  ACE_CString s (server);
  LiveEntry *entry = 0;
  ACE_NEW (entry, LiveEntry (this, server, may_ping, ref));
  int result = entry_map_.bind (s, entry);
  if (result != 0)
    {
      LiveEntry *old = 0;
      result = entry_map_.rebind (s, entry, old);
      delete old;
    }
}

void
LiveCheck::remove_server (const char *server)
{
  ACE_CString s(server);
  LiveEntry *entry = 0;
  if (this->handle_timeout_busy_ > 0)
    {
      int result = entry_map_.unbind (s, entry);
      if (result == 0)
        delete entry;
    }
  else
    {
      if (ImR_Locator_i::debug () > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveCheck::remove_server %s ")
                          ACE_TEXT ("called during handle_timeout\n"), server));
        }
      int result = entry_map_.find (s, entry);
      if (result != -1 && entry != 0)
        {
          this->removed_entries_.insert_tail (s);
        }
    }
}

void
LiveCheck::remove_deferred_servers (void)
{
  if (this->removed_entries_.size () == 0)
    return;

  NameStack::iterator re_end = this->removed_entries_.end();
  for (NameStack::iterator re = this->removed_entries_.begin();
       re != re_end;
       ++re)
    {
      if (ImR_Locator_i::debug () > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveCheck::remove_deferred_entries ")
                          ACE_TEXT ("removing %s\n"), (*re).c_str()));
        }
      LiveEntry *entry = 0;
      int result = entry_map_.unbind (*re, entry);
      if (result == 0)
        {
          delete entry;
        }
    }
  this->removed_entries_.reset ();

}

bool
LiveCheck::remove_per_client_entry (LiveEntry *e)
{
  return (this->per_client_.remove (e) == 0);
}

bool
LiveCheck::add_per_client_listener (LiveListener *l,
                                    ImplementationRepository::ServerObject_ptr ref)
{
  if (!this->running_)
    return false;

  LiveEntry *entry = 0;
  ACE_NEW_RETURN (entry, LiveEntry (this, 0, true, ref), false);

  if (this->per_client_.insert_tail(entry) == 0)
    {
      entry->add_listener (l);

      if (this->handle_timeout_busy_ > 0)
        {
          ++this->token_;
          this->reactor()->schedule_timer (this,
                                           reinterpret_cast<void *>(this->token_),
                                           ACE_Time_Value::zero);
        }
      else
        {
          this->want_timeout_ = true;
          this->deferred_timeout_ = ACE_Time_Value::zero;
        }
      return true;
    }
  return false;
}

bool
LiveCheck::add_poll_listener (LiveListener *l)
{
  if (!this->running_)
    return false;

  LiveEntry *entry = 0;
  ACE_CString key (l->server());
  int result = entry_map_.find (key, entry);
  if (result == -1 || entry == 0)
    {
      return false;
    }

  entry->add_listener (l);
  entry->reset_status ();
  l->status_changed (entry->status());
  return this->schedule_ping (entry);
}

bool
LiveCheck::add_listener (LiveListener *l)
{
  if (!this->running_)
    return false;

  LiveEntry *entry = 0;
  ACE_CString key (l->server());
  int result = entry_map_.find (key, entry);
  if (result == -1 || entry == 0)
    {
      return false;
    }

  entry->add_listener (l);
  return this->schedule_ping (entry);
}

void
LiveCheck::remove_listener (LiveListener *l)
{
  if (!this->running_)
    return;

  LiveEntry *entry = 0;
  ACE_CString key (l->server());
  int result = entry_map_.find (key, entry);
  if (result != -1 && entry != 0)
    {
      entry->remove_listener (l);
    }
}

bool
LiveCheck::schedule_ping (LiveEntry *entry)
{
  if (!this->running_)
    return false;

  LiveStatus status = entry->status();
  if (status == LS_PING_AWAY || status == LS_DEAD)
    {
      return status != LS_DEAD;
    }

  ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
  ACE_Time_Value next = entry->next_check ();

  if (this->handle_timeout_busy_ > 0)
    {
      ACE_Time_Value delay = ACE_Time_Value::zero;
      if (next > now)
        {
          delay = next - now;
        }

      ACE_Timer_Queue *tq = this->reactor ()->timer_queue ();
      if (!tq->is_empty ())
        {
          for (ACE_Timer_Queue_Iterator_T<ACE_Event_Handler*> &i = tq->iter ();
               !i.isdone (); i.next())
            {
              if (i.item ()->get_type () == this)
                {
                  if (next >= tq->earliest_time ())
                    {
                      if (ImR_Locator_i::debug () > 2)
                        {
                          ORBSVCS_DEBUG ((LM_DEBUG,
                                          ACE_TEXT ("(%P|%t) LiveCheck::schedule_ping ")
                                          ACE_TEXT ("already scheduled\n")));
                        }
                      return true;
                    }
                  break;
                }
            }
        }
      ++this->token_;
      if (ImR_Locator_i::debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveCheck::schedule_ping (%d),")
                          ACE_TEXT (" delay = %d,%d\n"),
                          this->token_, delay.sec(), delay.usec()));
        }
      this->reactor()->schedule_timer (this,
                                       reinterpret_cast<void *>(this->token_),
                                       delay);
    }
  else
    {
      if (ImR_Locator_i::debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveCheck::schedule_ping deferred")));
        }
      if (!this->want_timeout_ || next < this->deferred_timeout_)
        {
          this->want_timeout_ = true;
          this->deferred_timeout_ = next;
        }
    }
  return true;
}

LiveStatus
LiveCheck::is_alive (const char *server)
{
  if (!this->running_)
    return LS_DEAD;

  if (this->ping_interval_ == ACE_Time_Value::zero)
    {
      return LS_ALIVE;
    }

  ACE_CString s(server);
  LiveEntry *entry = 0;
  int result = entry_map_.find (s, entry);
  if (result == 0 && entry != 0)
    {
      return entry->status ();
    }
  return LS_DEAD;
}
