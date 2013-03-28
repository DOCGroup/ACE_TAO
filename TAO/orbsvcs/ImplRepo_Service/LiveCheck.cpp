// -*- C++ -*-
// $Id$

#include "LiveCheck.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_time.h"

LiveListener::LiveListener (const char *server)
  : server_(server)
{
}

const char *
LiveListener::server (void) const
{
  return this->server_;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

const int LiveEntry::reping_msec_[] = {0, 10, 100, 500, 1000, 1000,
                                             1000, 1000, 5000, 5000};
int LiveEntry::reping_limit_ = sizeof (LiveEntry::reping_msec_) / sizeof (int);

void
LiveEntry::set_reping_limit (int max)
{
  LiveEntry::reping_limit_ = max;
}

bool
LiveEntry::reping_available (void)
{
  return this->repings_ < LiveEntry::reping_limit_;
}

int
LiveEntry::next_reping (void)
{
  if ( this->repings_ < LiveEntry::reping_limit_)
    {
      return LiveEntry::reping_msec_[this->repings_++];
    }
  else
    return -1;
}

LiveEntry::LiveEntry (LiveCheck *owner,
                      const char *server,
                      ImplementationRepository::ServerObject_ptr ref)
  : owner_ (owner),
    server_ (server),
    ref_ (ImplementationRepository::ServerObject::_duplicate (ref)),
    liveliness_ (LS_UNKNOWN),
    next_check_ (ACE_OS::time()),
    retry_count_ (0),
    repings_ (0),
    ping_away_ (false),
    listeners_ (),
    lock_ ()
{
}

LiveEntry::~LiveEntry (void)
{
}

void
LiveEntry::add_listener (LiveListener* ll)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->listeners_.push_back (ll);
}

LiveStatus
LiveEntry::status (void) const
{
  if ( this->liveliness_ == LS_ALIVE &&
       this->owner_->ping_interval() != ACE_Time_Value::zero )
    {
      ACE_Time_Value now (ACE_OS::time());
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
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->liveliness_ = l;
  this->ping_away_ = false;

  if (l == LS_ALIVE)
    {
      this->retry_count_ = 0;
      ACE_Time_Value now (ACE_OS::time());
      this->next_check_ = now + owner_->ping_interval();
    }
  for (size_t i = 0; i < this->listeners_.size(); i++)
    {
      LiveListener *ll = this->listeners_[i];
      if (ll != 0)
        {
          ll->status_changed (this->liveliness_, this->reping_available());
        }
    }
  this->listeners_.clear();
}

const ACE_Time_Value &
LiveEntry::next_check (void) const
{
  return this->next_check_;
}

bool
LiveEntry::do_ping (PortableServer::POA_ptr poa)
{
  ACE_Time_Value now (ACE_OS::time());
  if (this->ping_away_)
    {
      return true;
    }

  if (this->next_check_ > now || this->liveliness_ == LS_DEAD)
    {
      return false;
    }

  if (this->owner_->ping_interval() == ACE_Time_Value::zero)
    return false;

  switch (this->liveliness_)
    {
    case LS_UNKNOWN:
      this->next_check_ = now;
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
          return false;
      }
      break;
    default:;
    }

  this->ping_away_ = true;
  this->retry_count_++;
  PortableServer::ServantBase_var callback = new PingReceiver (this, poa);
  PortableServer::ObjectId_var oid = poa->activate_object (callback.in());
  CORBA::Object_var obj = poa->id_to_reference (oid.in());
  ImplementationRepository::AMI_ServerObjectHandler_var cb =
    ImplementationRepository::AMI_ServerObjectHandler::_narrow (obj.in());
  this->ref_->sendc_ping (cb.in());
  return true;
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
  :ping_interval_()
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
                 const ACE_Time_Value &pi )
{
  this->ping_interval_ = pi;
  ACE_Reactor *r = orb->orb_core()->reactor();
  this->reactor (r);
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
  this->poa_ = PortableServer::POA::_narrow (obj.in());
}

const ACE_Time_Value &
LiveCheck::ping_interval (void) const
{
  return this->ping_interval_;
}

int
LiveCheck::handle_timeout (const ACE_Time_Value &,
                           const void *)
{
  for (LiveEntryMap::iterator le (this->entry_map_);
       !le.done ();
       le.advance ())
    {
      LiveEntryMap::value_type *pair = 0;
      le.next(pair);
      pair->item()->do_ping (poa_.in());
    }
  for (PerClientStack::ITERATOR pe (this->per_client_);
       !pe.done ();
       pe.advance ())
    {
      LiveEntry *entry = 0;
      LiveEntry **n = &entry;
      pe.next(n);
      if (entry != 0 && !entry->do_ping (poa_.in()))
        {
          this->per_client_.remove (entry);
        }

    }

  return 0;
}

void
LiveCheck::add_server (const char *server,
                       ImplementationRepository::ServerObject_ptr ref)
{
  ACE_CString s (server);
  LiveEntry *entry = 0;
  ACE_NEW (entry, LiveEntry (this, server, ref));
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
  LiveEntry *entry = 0;
  ACE_NEW_RETURN (entry, LiveEntry (this, 0, ref), false);
  if (this->per_client_.push(entry) == 0)
    {
      entry->add_listener (l);
      this->reactor()->schedule_timer (this,0,ACE_Time_Value::zero);
      return true;
    }
  return false;
}

bool
LiveCheck::add_listener (LiveListener *l)
{
  LiveEntry *entry = 0;
  ACE_CString key (l->server());
  int result = entry_map_.find (key, entry);
  if (result == -1 || entry == 0)
    {
      return false;
    }

  entry->add_listener (l);
  ACE_Time_Value now (ACE_OS::time());
  ACE_Time_Value next = entry->next_check ();

  if (next <= now)
    {
      // ACE_DEBUG ((LM_DEBUG,
      //             ACE_TEXT ("(%P|%t) LiveCheck::add_listener %x, ")
      //             ACE_TEXT ("immediate callback for <%C>\n"),
      //             l, l->server()));
      this->reactor()->schedule_timer (this,0,ACE_Time_Value::zero);
    }
  else
    {
      ACE_Time_Value delay = next - now;
      // ACE_DEBUG ((LM_DEBUG,
      //             ACE_TEXT ("(%P|%t) LiveCheck::add_listener %x, ")
      //             ACE_TEXT ("callback in %d ms for <%C>\n"),
      //             l, delay.sec() * 1000 + delay.usec() / 1000, l->server()));
      this->reactor()->schedule_timer (this, 0, delay);
    }
  return true;
}

LiveStatus
LiveCheck::is_alive (const char *server)
{
  ACE_CString s(server);
  LiveEntry *entry = 0;
  int result = entry_map_.find (s, entry);
  if (result == 0 && entry != 0)
    {
      return entry->status ();
    }
  return LS_DEAD;
}
