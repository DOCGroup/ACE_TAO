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

