/* -*- C++ -*- */

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

// #include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlS.h"
#include "concurrency_export.h"

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
class TAO_Concurrency_Export CC_LockSetFactory : public POA_CosConcurrencyControl::LockSetFactory
{
public:

  // = Initialization and termination methods.
  /// Default constructor.
  CC_LockSetFactory (void);

  /// Destructor.
  ~CC_LockSetFactory (void);

  virtual CosConcurrencyControl::LockSet_ptr create (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CosConcurrencyControl::LockSet_ptr create_related (
      CosConcurrencyControl::LockSet_ptr which
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Lock to serialize the access to the factory.
  TAO_SYNCH_MUTEX lock_;
};

#include /**/ "ace/post.h"
#endif /* _CC_LOCKSETFACTORY_H */
