// -*- C++ -*-
//=============================================================================
/**
 *  @file   Iterator.h
 *
 *  $Id$
 *
 *  @brief  This file declares ImR's iterator.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMR_ITERATOR_H
#define IMR_ITERATOR_H

#include "Locator_Repository.h"
#include "AsyncListManager.h"
#include "tao/PortableServer/PortableServer.h"
#include "ImplRepoS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



class ImR_AsyncIterator
  : public POA_ImplementationRepository::AMH_ServerInformationIterator
{
public:
  ImR_AsyncIterator (CORBA::ULong start,
                      AsyncListManager *lister);

  virtual void next_n
  (ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_ptr _tao_rh,
   CORBA::ULong how_many);

  virtual void destroy
  (ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_ptr _tao_rh);

private:
  CORBA::ULong count_;
  AsyncListManager_ptr lister_;
};

#endif /* IMR_ITERATOR_H */
