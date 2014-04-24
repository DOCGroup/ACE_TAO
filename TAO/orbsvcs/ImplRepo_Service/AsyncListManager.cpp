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
   how_many_ (0),
   waiters_ (0),
   refcount_ (1),
   lock_ ()
{
}

AsyncListManager::~AsyncListManager (void)
{
  if (ImR_Locator_i::debug() > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncListManager(%@)::dtor\n"),
                      this));
    }
}

PortableServer::POA_ptr
AsyncListManager::poa (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in());
}

void
AsyncListManager::init_list (void)
{
  CORBA::ULong len =
    static_cast<CORBA::ULong> (this->repo_->servers ().current_size ());
  Locator_Repository::SIMap::ENTRY* entry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR it (this->repo_->servers ());
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
                      ACE_TEXT ("(%P|%t) AsyncListManager(%@)::init_list, %d waiters")
                      ACE_TEXT (" out of %d regsitered servers\n"),
                      this, this->waiters_, len));
    }
}

bool
AsyncListManager::make_iterator (ImplementationRepository::ServerInformationIterator_out si, CORBA::ULong start)
{
  si = ImplementationRepository::ServerInformationIterator::_nil ();
  try
    {
      ImR_AsyncIterator* imr_iter = 0;
      ACE_NEW_THROW_EX (imr_iter,
                        ImR_AsyncIterator (start, this),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var tmp (imr_iter);

      PortableServer::ObjectId_var id =
        this->poa_->activate_object (imr_iter);
      CORBA::Object_var obj = this->poa_->id_to_reference (id.in ());
      si = ImplementationRepository::
        ServerInformationIterator::_unchecked_narrow (obj.in ());
      return true;
    }
  catch (CORBA::SystemException& ex)
    {
      ex._tao_print_exception ("AsyncListManager:final state constructing iterator\n");
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
      try
        {
          this->primary_->list_excep (&h);
        }
      catch (CORBA::Exception& ex2)
        {
          ex2._tao_print_exception ("AsyncListManager:final calling list_excep\n");
        }
    }
  catch (CORBA::UserException& ex)
    {
      ex._tao_print_exception ("AsyncListManager:final state constructing iterator\n");
      ImplementationRepository::AMH_AdministrationExceptionHolder h (new CORBA::INTERNAL);
      try
        {
          this->primary_->list_excep (&h);
        }
      catch (CORBA::Exception& ex2)
        {
          ex2._tao_print_exception ("AsyncListManager:final calling list_excep\n");
        }
    }
  return false;
}

void
AsyncListManager::final_state (void)
{
  if (ImR_Locator_i::debug() > 4)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) AsyncListManager(%@)::final_state, ")
                      ACE_TEXT ("waiters count = %d, has pinger? %d\n"),
                      this, this->waiters_, (this->pinger_ != 0)));
    }

  if (this->pinger_ != 0 && this->waiters_ > 0)
    {
      return;
    }

  bool excepted = false;
  CORBA::ULong len = this->server_list_.length ();
  ImplementationRepository::ServerInformationList alt_list (this->how_many_);
  ImplementationRepository::ServerInformationList *sil = &this->server_list_;
  if (this->first_ > 0 || this->how_many_ < len)
    {
      alt_list.length (this->how_many_);
      for (CORBA::ULong i = 0; i < this->how_many_; i++)
        {
          alt_list[i] = this->server_list_[i + this->first_];
        }
      sil = &alt_list;
    }

  if (!CORBA::is_nil (this->primary_.in ()))
    {
      ImplementationRepository::ServerInformationIterator_var server_iterator;
      if (sil !=  &this->server_list_)
        {
          excepted = !this->make_iterator (server_iterator.out(), this->how_many_);
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
              this->primary_->list (*sil, server_iterator.in ());
            }
          catch (CORBA::Exception &ex)
            {
              ex._tao_print_exception ("AsyncListManager:final state sending list\n");
              ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
              this->primary_->list_excep (&h);
            }
        }
      this->primary_ =
        ImplementationRepository::AMH_AdministrationResponseHandler::_nil ();
    }
  else if (!CORBA::is_nil (this->secondary_.in()))
    {
      CORBA::Boolean done = this->first_ + sil->length() == len;
      try
        {
          this->secondary_->next_n (done, *sil);
        }
      catch (CORBA::Exception &ex)
        {
          ex._tao_print_exception ("AsyncListManager:final state sending secondary list\n");
          ImplementationRepository::AMH_ServerInformationIteratorExceptionHolder h (ex._tao_duplicate());
          this->secondary_->next_n_excep (&h);
        }
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
  return this->first_ + this->how_many_;
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
AsyncListManager::list_i (CORBA::ULong start, CORBA::ULong count)
{
  if (this->server_list_.length () == 0)
    {
      this->init_list ();
    }

  this->first_ = start;
  this->how_many_ = this->server_list_.length () - start;
  if (start > 0 || (count > 0 && count < this->how_many_))
    {
      if (count > 0 && count < this->how_many_)
        {
          this->how_many_ = count;
        }
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
                      ACE_TEXT ("(%P|%t) AsyncListManager(%@)::ping_replied, index = %d ")
                      ACE_TEXT ("status = %C, waiters = %d\n"),
                      this,index, LiveEntry::status_name (status), this->waiters_));
    }
  if (evaluate_status (index, status))
    {
      if (--this->waiters_ == 0)
        {
          this->final_state ();
        }
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
