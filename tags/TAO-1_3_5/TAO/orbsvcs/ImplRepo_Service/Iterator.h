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

#include "Server_Repository.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/PortableServer/ImplRepoS.h"

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
  /// Constructor
  /// Ownership of iterator is transfered to this class (we'll delete it)
  ImR_Iterator (Server_Repository::HASH_IMR_ITER *iterator,
                PortableServer::POA_ptr poa);

  /// Destructor
  ~ImR_Iterator ();

  /// Returns the next list of up to <how_many> servers.  If empty, will return
  /// false.
  virtual CORBA::Boolean next_n (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Destroys the iterator.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Our very own iterator for transversing the server repository.
  Server_Repository::HASH_IMR_ITER *iterator_;

  /// Our lovely POA.
  PortableServer::POA_var poa_;
};

#endif /* IMR_ITERATOR_H */
