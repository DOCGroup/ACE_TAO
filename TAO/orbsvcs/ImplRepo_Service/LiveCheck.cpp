// -*- C++ -*-
//

#include "LiveCheck.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

LiveListener::LiveListener (const char *server)
 : server_(server)
{
}

const ACE_CString &
LiveListener::server (void) const
{
  return this->server_;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

LiveEntry::LiveEntry (LiveCheck *owner,
                      ImplementationRepository::ServerObject_ptr ref)
  : owner_ (owner),
    ref_ (ImplementationRepository::ServerObject::_duplicate (ref)),
    liveliness_ (LS_UNKNOWN),
    next_check_ (ACE_OS::time()),
    retry_count_ (0),
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
  for (ACE_Vector_Iterator<LiveListener *> i (this->listeners_);
       !i.done();
       i.advance())
    {
      LiveListener *item = 0;
      LiveListener **ll  = &item;
      i.next(ll);
      if (item != 0)
        {
          item->status_changed (this->liveliness_);
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
  if (this->next_check_ > now || this->liveliness_ == LS_DEAD || this->ping_away_)
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
      this->next_check_ = now + ACE_Time_Value (0,5000); // retry delay
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
      delete pair;
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
  for (LiveEntryMap::iterator i (this->entry_map_);
       !i.done();
       i.advance ())
    {
      LiveEntryMap::value_type *pair = 0;
      i.next(pair);
      pair->item()->do_ping(poa_.in());
    }
  return 0;
}

void
LiveCheck::add_server (const char *server,
                       ImplementationRepository::ServerObject_ptr ref)
{
  ACE_CString s(server);
  LiveEntry *entry = 0;
  ACE_NEW (entry, LiveEntry(this,ref));
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
LiveCheck::add_listener (LiveListener *l)
{
  LiveEntry *entry = 0;
  int result = entry_map_.find (l->server(), entry);
  if (result == 0 && entry != 0)
    {
      entry->add_listener (l);
      ACE_Time_Value now (ACE_OS::time());
      ACE_Time_Value next = entry->next_check ();
      if (next <= now)
        {
          this->reactor()->schedule_timer (this,0,ACE_Time_Value::zero);
        }
      else
        {
          ACE_Time_Value delay = next - now;
          this->reactor()->schedule_timer (this, 0, delay);
        }

    }
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
