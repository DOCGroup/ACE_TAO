//=============================================================================
/**
 *  @file   Iterator.cpp
 *
 *  $Id$
 *
 *  @brief  This file declares ImR's iterator.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#include "Iterator.h"


ImR_AsyncIterator::ImR_AsyncIterator (CORBA::ULong start,
                                      AsyncListManager *lister)
  :count_ (start),
   lister_ (lister->_add_ref ())
{
}

void
ImR_AsyncIterator::next_n
  (ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_ptr _tao_rh,
   CORBA::ULong how_many)
{
  this->count_ = this->lister_->list (_tao_rh, this->count_, how_many);
}

void
ImR_AsyncIterator::destroy
  (ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_ptr _tao_rh)
{
  PortableServer::POA_var poa = this->lister_->poa ();
  PortableServer::ObjectId_var oid = poa->servant_to_id (this);
  poa->deactivate_object (oid.in());
  _tao_rh->destroy ();
}

//----------------------------------------------------------------------------
ImR_Iterator::ImR_Iterator (CORBA::ULong n,
                            Locator_Repository& repo,
                            PortableServer::POA_ptr poa)
  : repo_(repo)
  , count_(n)
  , poa_(poa)
{
}


CORBA::Boolean
ImR_Iterator::next_n (CORBA::ULong how_many,
                      ImplementationRepository::ServerInformationList_out server_list)
{
  ACE_NEW_THROW_EX (server_list,
    ImplementationRepository::ServerInformationList(0), CORBA::NO_MEMORY());

  Locator_Repository::SIMap::ENTRY* entry = 0;
  Locator_Repository::SIMap::ITERATOR it (this->repo_.servers ());

  // Number of servers that will go into the server_list.
  CORBA::ULong n = this->repo_.servers().current_size();
  if (n <= this->count_)
    {
      return 0; // We already finished.
    }
  else
    {
      n -= this->count_;
    }

  if (how_many > 0 && n > how_many)
    {
      n = how_many;
  }

  server_list->length (n);

  CORBA::ULong i = 0;
  for (; i < this->count_; ++i)
    {
      it.advance ();
    }

  for (i = 0; i < n; ++i)
    {
      it.next (entry);
      it.advance ();
      ACE_ASSERT(entry != 0);

      Server_Info_Ptr info = entry->int_id_;

      server_list[i].server = info->name.c_str ();
      server_list[i].startup.command_line = info->cmdline.c_str ();
      server_list[i].startup.environment = info->env_vars;
      server_list[i].startup.working_directory = info->dir.c_str ();
      server_list[i].startup.activation = info->activation_mode;
      server_list[i].startup.activator = info->activator.c_str ();
      server_list[i].startup.start_limit = info->start_limit;
      server_list[i].partial_ior = info->partial_ior.c_str ();
    }

  this->count_ += n;

  return 1;
}


void
ImR_Iterator::destroy (void)
{
  PortableServer::ObjectId_var oid = poa_->servant_to_id (this);
  poa_->deactivate_object (oid.in());
}
