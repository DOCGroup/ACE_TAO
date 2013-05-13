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

/**
 * @class ImR_Iterator
 *
 * @brief The Iterator for servers in the ImR.
 *
 */
class ImR_Iterator
  : public POA_ImplementationRepository::ServerInformationIterator
{
public:
  ImR_Iterator (CORBA::ULong start,
                Locator_Repository& repo,
                PortableServer::POA_ptr poa);

  /// Returns the next list of up to <how_many> servers.  If empty, will return
  /// false.
  virtual CORBA::Boolean next_n (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list
    );

  virtual void destroy (void);

private:
  Locator_Repository& repo_;
  CORBA::ULong count_;
  PortableServer::POA_ptr poa_;
};

#endif /* IMR_ITERATOR_H */
