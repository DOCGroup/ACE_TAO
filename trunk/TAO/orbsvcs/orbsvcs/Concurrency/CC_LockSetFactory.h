/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//    CC_LockSetFactory.h
//
// = DESCRIPTION
//      This class implements the lock set factory interface from the
//      concurrency service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#ifndef _CC_LOCKSETFACTORY_H
#include "ace/pre.h"
#define _CC_LOCKSETFACTORY_H

// #include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlS.h"
#include "concurrency_export.h"

class TAO_Concurrency_Export CC_LockSetFactory : public POA_CosConcurrencyControl::LockSetFactory
{
  // = TITLE
  //     CC_LockSetFactory
  //
  // = DESCRIPTION
  //     This class implements the LockSetFactory interface that is
  //     part of the CosConcurrency service. Please consult the idl
  //     file for detailed descriptions apart from the comments in
  //     this file.
public:

  // = Initialization and termination methods.
  CC_LockSetFactory (void);
  // Default constructor.

  ~CC_LockSetFactory (void);
  // Destructor.

  virtual CosConcurrencyControl::LockSet_ptr create (
      CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CosConcurrencyControl::LockSet_ptr create_related (
      CosConcurrencyControl::LockSet_ptr which,
      CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_SYNCH_MUTEX lock_;
  // Lock to serialize the access to the factory.
};

#include "ace/post.h"
#endif /* _CC_LOCKSETFACTORY_H */
