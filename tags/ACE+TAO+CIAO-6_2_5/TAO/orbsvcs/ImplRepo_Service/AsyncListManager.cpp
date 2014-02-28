// -*- C++ -*-
// $Id$

#include "AsyncListManager.h"
#include "Iterator.h"
#include "Locator_Repository.h"
#include "ImR_Locator_i.h"

#include "orbsvcs/Log_Macros.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

AsyncListManager::AsyncListManager (const Locator_Repository *repo,
                                    PortableServer::POA_ptr poa,
                                    LiveCheck *pinger)
  :repo_ (repo),
   poa_ (PortableServer::POA::_duplicate (poa)),
   primary_ (ImplementationRepository::AMH_AdministrationResponseHandler::_nil ()),
   secondary_ (ImplementationRepository::AMH_ServerInformationIteratorResponseHandler::_nil ()),
   pinger_ (pinger),
   server_list_ (0),
   first_ (0),
   waiters_ (0),
   refcount_ (1),
   lock_ ()
{
}

AsyncListManager::~AsyncListManager (void)
{
}

PortableServer::POA_ptr
AsyncListManager::poa (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in());
}

void
AsyncListManager::final_state (void)
{
  if (ImR_Locator_i::debug() > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncListManager::final_state, ")
                      ACE_TEXT ("waiters count = %d, pinger not null? %d\n"),
                      this->waiters_, (this->pinger_ != 0)));
    }

  if (this->pinger_ != 0 && this->waiters_ > 0)
    {
      return;
    }

  bool excepted = false;
  CORBA::ULong len = this->server_list_.length ();

  if (!CORBA::is_nil (this->primary_.in()))
    {
      ImplementationRepository::ServerInformationIterator_var server_iterator;
      if (len < this->repo_->servers ().current_size())
        {
          try
            {
              ImR_AsyncIterator* imr_iter = 0;
              ACE_NEW_THROW_EX (imr_iter,
                                ImR_AsyncIterator (len, this),
                                CORBA::NO_MEMORY ());

              PortableServer::ServantBase_var tmp (imr_iter);

              PortableServer::ObjectId_var id =
                this->poa_->activate_object (imr_iter);
              CORBA::Object_var obj = this->poa_->id_to_reference (id.in ());
              server_iterator = ImplementationRepository::
                ServerInformationIterator::_unchecked_narrow (obj.in ());
            }
          catch (CORBA::Exception& ex)
            {
              ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
              this->primary_->list_excep (&h);
              excepted = true;
            }
        }
      else
        {
          server_iterator =
            ImplementationRepository::ServerInformationIterator::_nil ();
        }

      if (!excepted)
        {
          try
            {
              this->primary_->list (this->server_list_, server_iterator.in ());
            }
          catch (CORBA::Exception &ex)
            {
              ex._tao_print_exception ("ALM:final state sending list\n");
              ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
              this->primary_->list_excep (&h);
            }
        }
      this->primary_ =
        ImplementationRepository::AMH_AdministrationResponseHandler::_nil ();
    }
  else if (!CORBA::is_nil (this->secondary_.in()))
    {
      CORBA::Boolean done = this->first_ + this->server_list_.length () >=
        this->repo_->servers ().current_size ();
      this->secondary_->next_n (done, this->server_list_);
      this->secondary_ =
        ImplementationRepository::AMH_ServerInformationIteratorResponseHandler::_nil ();

    }
}

CORBA::ULong
AsyncListManager::list
(ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_ptr _tao_rh,
 CORBA::ULong start,
 CORBA::ULong count)
{
  this->secondary_ =
    ImplementationRepository::AMH_ServerInformationIteratorResponseHandler::_duplicate (_tao_rh);
  this->list_i (start, count);
  return this->first_ + this->server_list_.length ();
}

void
AsyncListManager::list
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 CORBA::ULong count)
{
  this->primary_ =
    ImplementationRepository::AMH_AdministrationResponseHandler::_duplicate (_tao_rh);
  this->list_i (0, count);
}

void
AsyncListManager::list_i (CORBA::ULong start, CORBA::ULong how_many)
{
  this->first_ = start;
  CORBA::ULong len =
    static_cast<CORBA::ULong> (this->repo_->servers ().current_size ()) - start;
  if (how_many > 0 && len > how_many)
    {
      len = how_many;
    }
  Locator_Repository::SIMap::ENTRY* entry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR it (this->repo_->servers ());
  for (;start > 0; start--)
    {
      it.advance ();
    }
  this->server_list_.length (len);
  this->waiters_ = 0;
  for (CORBA::ULong i = 0; i < len; i++)
    {
      it.next (entry);
      it.advance ();

      Server_Info_Ptr info = entry->int_id_;
      info->setImRInfo (&this->server_list_[i]);
      if (this->pinger_ != 0)
        {
          ListLiveListener *l = 0;
          ACE_NEW (l, ListLiveListener (info->ping_id (),
                                        i,
                                        this,
                                        *this->pinger_));

          LiveListener_ptr llp (l);
          if (!l->start ())
            {
              this->server_list_[i].activeStatus =
                ImplementationRepository::ACTIVE_NO;
              l->cancel ();
            }
          else
            {
              if (!evaluate_status (i,l->status()))
                {
                  this->waiters_++;
                }
              else
                {
                  l->cancel ();
                }
            }
        }
    }

  if (ImR_Locator_i::debug() > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncListManager::list_i, %d waiters")
                      ACE_TEXT (" out of %d regsitered servers\n"),
                      this->waiters_, len));
    }

  if (this->waiters_ == 0)
    {
      this->final_state ();
    }
}

bool
AsyncListManager::evaluate_status (CORBA::ULong index, LiveStatus status)
{
  bool is_final = true;
  switch (status)
    {
    case LS_ALIVE:
    case LS_LAST_TRANSIENT:
      this->server_list_[index].activeStatus =
        ImplementationRepository::ACTIVE_YES;
      break;
    case LS_TIMEDOUT:
      this->server_list_[index].activeStatus =
        ImplementationRepository::ACTIVE_MAYBE;
      break;
    case LS_DEAD:
      this->server_list_[index].activeStatus =
        ImplementationRepository::ACTIVE_NO;
      break;
    default:
      is_final = false;
    }
  return is_final;
}

void
AsyncListManager::ping_replied (CORBA::ULong index, LiveStatus status)
{
  if (ImR_Locator_i::debug() > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncListManager::ping_replied, index = %d ")
                      ACE_TEXT ("status = %C\n"),
                      index, LiveEntry::status_name (status)));
    }
  if (evaluate_status (index, status))
    {
      this->waiters_--;
      this->final_state();
      return;
    }
}

AsyncListManager *
AsyncListManager::_add_ref (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);
  ++this->refcount_;

  return this;
}

void
AsyncListManager::_remove_ref (void)
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

ListLiveListener::ListLiveListener (const char *server,
                                    CORBA::ULong index,
                                    AsyncListManager *owner,
                                    LiveCheck &pinger)
  :LiveListener (server),
   owner_ (owner->_add_ref ()),
   pinger_ (pinger),
   status_ (LS_UNKNOWN),
   index_ (index),
   started_ (false)
{

}

ListLiveListener::~ListLiveListener (void)
{
}

bool
ListLiveListener::start (void)
{
  bool rtn = this->pinger_.add_poll_listener (this);
  this->started_ = true;
  return rtn;
}

LiveStatus
ListLiveListener::status (void)
{
  return this->status_;
}

void
ListLiveListener::cancel (void)
{
  this->pinger_.remove_listener (this);
}

bool
ListLiveListener::status_changed (LiveStatus status)
{
  this->status_ = status;
  if (status == LS_TRANSIENT)
    {
      return false;
    }
  else
    {
      if (this->started_)
        this->owner_->ping_replied (this->index_, status);
    }
  return true;
}
