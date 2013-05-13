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
#include "tao/PortableServer/PortableServer.h"
#include "ImplRepoS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
  ImR_Iterator (CORBA::ULong n, Locator_Repository& repo, PortableServer::POA_ptr poa);

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
