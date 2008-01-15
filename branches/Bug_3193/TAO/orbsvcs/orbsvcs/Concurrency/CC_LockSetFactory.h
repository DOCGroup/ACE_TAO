// -*- C++ -*-

//=============================================================================
/**
 *  @file    CC_LockSetFactory.h
 *
 *  $Id$
 *
 *    This class implements the lock set factory interface from the
 *    concurrency service.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#ifndef _CC_LOCKSETFACTORY_H
#define _CC_LOCKSETFACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosConcurrencyControlS.h"
#include "orbsvcs/Concurrency/concurrency_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class CC_LockSetFactory
 *
 * @brief CC_LockSetFactory
 *
 * This class implements the LockSetFactory interface that is
 * part of the CosConcurrency service. Please consult the idl
 * file for detailed descriptions apart from the comments in
 * this file.
 */
class TAO_Concurrency_Serv_Export CC_LockSetFactory
  : public POA_CosConcurrencyControl::LockSetFactory
{
public:

  // = Initialization and termination methods.
  /// Default constructor.
  CC_LockSetFactory (void);

  /// Destructor.
  ~CC_LockSetFactory (void);

  virtual CosConcurrencyControl::LockSet_ptr create ();

  virtual CosConcurrencyControl::LockSet_ptr create_related (
      CosConcurrencyControl::LockSet_ptr which);

private:
  /// Lock to serialize the access to the factory.
  TAO_SYNCH_MUTEX lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* _CC_LOCKSETFACTORY_H */
