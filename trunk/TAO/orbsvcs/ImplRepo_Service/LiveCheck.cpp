// -*- C++ -*-
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "LiveCheck.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"

LiveListener::LiveListener (const char *server)
  : server_(server),
    refcount_(1),
    lock_()
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
LiveListener::add_ref (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);
  ++this->refcount_;
  return this;
}

void
LiveListener::remove_ref (void)
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

LiveListener_ptr::LiveListener_ptr (void)
  : val_ (0)
{
}

LiveListener_ptr::LiveListener_ptr (LiveListener *ll)
  :val_ (ll)
{
}

LiveListener_ptr::LiveListener_ptr (const LiveListener_ptr &ll_ptr)
  :val_ (ll_ptr.clone())
{
}

LiveListener_ptr::~LiveListener_ptr (void)
{
  if (val_ != 0)
    {
      val_->remove_ref();
    }
}

LiveListener_ptr &
LiveListener_ptr::operator= (const LiveListener_ptr &ll_ptr)
{
  if (val_ != *ll_ptr)
    {
      if (val_ != 0)
        {
          val_->remove_ref();
        }
      val_ = ll_ptr.clone();
    }
  return *this;
}

LiveListener_ptr &
LiveListener_ptr::operator= (LiveListener *ll)
{
  if (val_ != ll)
    {
      if (val_ != 0)
        {
          val_->remove_ref();
        }
      val_ = ll;
    }
  return *this;
}

const LiveListener *
LiveListener_ptr::operator-> () const
{
  return val_;
}

const LiveListener *
LiveListener_ptr::operator* () const
{
  return val_;
}

LiveListener *
LiveListener_ptr::operator-> ()
{
  return val_;
}

LiveListener *
LiveListener_ptr::operator* ()
{
  return val_;
}

bool
LiveListener_ptr::operator== (const LiveListener_ptr &ll_ptr) const
{
  return val_ == *ll_ptr;
}

bool
LiveListener_ptr::operator== (const LiveListener *ll) const
{
  return val_ == ll;
}

LiveListener *
LiveListener_ptr::clone (void) const
{
  if (val_ != 0)
    {
      val_->add_ref();
    }
  return val_;
}

LiveListener *
LiveListener_ptr::_retn (void)
{
  LiveListener * ll = val_;
  val_ = 0;
  return ll;
}

void
LiveListener_ptr::assign (LiveListener *ll)
{
  if (val_ != 0)
    {
      val_->remove_ref();
    }
  val_ = ll;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

const int LiveEntry::reping_msec_[] = {10, 100, 500, 1000, 1000, 2000, 2000, 5000, 5000};
int LiveEntry::reping_limit_ = sizeof (LiveEntry::reping_msec_) / sizeof (int);

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
    retry_count_ (0),
    repings_ (0),
    max_retry_ (LiveEntry::reping_limit_),
    may_ping_ (may_ping),
    listeners_ (),
    lock_ ()
{
}

LiveEntry::~LiveEntry (void)
{
}

void
LiveEntry::add_listener (LiveListener *ll)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  LiveListener_ptr llp(ll->add_ref());
  this->listeners_.insert (llp);
}

void
LiveEntry::reset_status (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  if ( this->liveliness_ == LS_ALIVE)
    {
      this->liveliness_ = LS_UNKNOWN;
      this->next_check_ = ACE_High_Res_Timer::gettimeofday_hr();
    }
}

LiveStatus
LiveEntry::status (void) const
{
  if (!this->may_ping_)
    return LS_ALIVE;
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
LiveEntry::status (LiveStatus l)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
    this->liveliness_ = l;
    if (l == LS_ALIVE)
      {
        this->retry_count_ = 0;
        ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
        this->next_check_ = now + owner_->ping_interval();
      }
  }
  Listen_Set remove;

  LiveStatus ls = this->liveliness_;
  if (ls == LS_TRANSIENT && ! this->reping_available())
    ls = LS_LAST_TRANSIENT;
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
        this->listeners_.remove (*i);
      }
  }

  if (this->listeners_.size() > 0)
    {
      this->owner_->schedule_ping (this);
    }
  else
    {
      this->owner_->remove_per_client_entry (this);
    }
}

const ACE_Time_Value &
LiveEntry::next_check (void) const
{
  return this->next_check_;
}

bool
LiveEntry::validate_ping (bool &want_reping, ACE_Time_Value& next)
{
  if (this->owner_->debug () > 2)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, server = %C\n"),
                      this->server_.c_str()));
    }
  if (this->liveliness_ == LS_PING_AWAY ||
      this->liveliness_ == LS_DEAD ||
      this->listeners_.size () == 0)
    {
      if (this->owner_->debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, first test, ")
                          ACE_TEXT ("status = %d, listeners = %d\n"),
                          this->liveliness_, this->listeners_.size ()));
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
      if (this->owner_->debug () > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, second test, ")
                          ACE_TEXT ("status = %d, listeners = %d, ")
                          ACE_TEXT ("diff = %d,%d, msec = %d\n"),
                          this->liveliness_, this->listeners_.size (),
                          diff.sec(), diff.usec(), msec));
        }
      return false;
    }

  switch (this->liveliness_)
    {
    case LS_UNKNOWN:
      break;
    case LS_ALIVE:
    case LS_TIMEDOUT:
      this->next_check_ = now + owner_->ping_interval();
      break;
    case LS_TRANSIENT:
      {
        int ms = this->next_reping ();
        if (ms != -1)
          {
            ACE_Time_Value next (ms / 1000, (ms % 1000) * 1000);
            this->next_check_ = now + next;
          }
        else
          {
            if (this->owner_->debug () > 2)
              {
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) LiveEntry::validate_ping, ")
                                ACE_TEXT ("transient, no more repings\n")));
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
  PortableServer::ServantBase_var callback = new PingReceiver (this, poa);
  PortableServer::ObjectId_var oid = poa->activate_object (callback.in());
  CORBA::Object_var obj = poa->id_to_reference (oid.in());
  ImplementationRepository::AMI_ServerObjectHandler_var cb =
    ImplementationRepository::AMI_ServerObjectHandler::_narrow (obj.in());
  try
    {
      this->ref_->sendc_ping (cb.in());
      ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
      this->liveliness_ = LS_PING_AWAY;
      this->retry_count_++;
    }
  catch (CORBA::Exception &)
    {
      this->status (LS_DEAD);
      this->retry_count_++;
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
PingReceiver::ping (void)
{
  this->entry_->status (LS_ALIVE);
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

void
PingReceiver::ping_excep (Messaging::ExceptionHolder * excep_holder)
{
  try
    {
      excep_holder->raise_exception ();
    }
  catch (CORBA::TRANSIENT &ex)
    {
      const CORBA::ULong BITS_5_THRU_12_MASK = 0x00000f80;
      switch (ex.minor () & BITS_5_THRU_12_MASK)
        {
        case TAO_POA_DISCARDING:
        case TAO_POA_HOLDING:
          {
            this->entry_->status (LS_TRANSIENT);
            break;
          }
        default: //case TAO_INVOCATION_SEND_REQUEST_MINOR_CODE:
          {
            this->entry_->status (LS_DEAD);
          }
        }
    }
  catch (CORBA::TIMEOUT &)
    {
      this->entry_->status (LS_TIMEDOUT);
    }
  catch (CORBA::Exception &)
    {
      this->entry_->status (LS_DEAD);
    }

  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

LiveCheck::LiveCheck ()
  :ping_interval_(),
   running_ (false),
   token_ (100),
   debug_ (0)
{
}

LiveCheck::~LiveCheck (void)
{
  while (this->entry_map_.current_size() > 0)
    {
      LiveEntryMap::iterator i (this->entry_map_);
      LiveEntryMap::value_type *pair = 0;
      i.next (pair);
      this->entry_map_.unbind(pair);
      delete pair->item();
    }
}

void
LiveCheck::init (CORBA::ORB_ptr orb,
                 const ACE_Time_Value &pi,
                 int debug_level)
{
  this->ping_interval_ = pi;
  this->debug_ = debug_level;
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
  long token = reinterpret_cast<long>(tok);
  if (this->debug_ > 2)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) LiveCheck::handle_timeout(%d), ")
                      ACE_TEXT ("running = %d\n"),
                      token, this->running_));
    }
  if (!this->running_)
    return -1;

  bool want_reping = false;
  ACE_Time_Value next;
  LiveEntryMap::iterator le_end = this->entry_map_.end();
  for (LiveEntryMap::iterator le = this->entry_map_.begin();
       le != le_end;
       ++le)
    {
      LiveEntry *entry = le->item ();
      if (entry->validate_ping (want_reping, next))
        {
          entry->do_ping (poa_.in ());
          if (this->debug_ > 2)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) LiveCheck::handle_timeout(%d)")
                              ACE_TEXT (", ping sent\n"),
                              token));
            }
        }
      else
        {
          if (this->debug_ > 2)
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
          if (entry->validate_ping (want_reping, next))
            {
              entry->do_ping (poa_.in ());
            }
          LiveStatus status = entry->status ();
          if (status != LS_PING_AWAY && status != LS_TRANSIENT)
            {
              this->per_client_.remove (entry);
            }
        }
    }

  if (want_reping)
    {
      ACE_Time_Value now (ACE_High_Res_Timer::gettimeofday_hr());
      ACE_Time_Value delay = next - now;
      if (this->debug_ > 2)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) LiveCheck::handle_timeout(%d),")
                          ACE_TEXT (" want reping, delay = %d,%d\n"),
                          token, delay.sec(), delay.usec()));
        }
      ++this->token_;
      this->reactor()->schedule_timer (this, reinterpret_cast<void *>(this->token_), delay);
    }

  return 0;
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
  int result = entry_map_.unbind (s, entry);
  if (result == 0)
    delete entry;
}

void
LiveCheck::remove_per_client_entry (LiveEntry *e)
{
  this->per_client_.remove (e);
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

      ++this->token_;
      this->reactor()->schedule_timer (this,
                                       reinterpret_cast<void *>(this->token_),
                                       ACE_Time_Value::zero);
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
  ++this->token_;
  if (next <= now)
    {
      this->reactor()->schedule_timer (this,
                                       reinterpret_cast<void *>(this->token_),
                                       ACE_Time_Value::zero);
    }
  else
    {
      ACE_Time_Value delay = next - now;
      this->reactor()->schedule_timer (this,
                                       reinterpret_cast<void *>(this->token_),
                                       delay);
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

int
LiveCheck::debug (void) const
{
  return this->debug_;
}
