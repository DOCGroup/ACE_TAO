/* -*- C++ -*- */

// $Id$
// ============================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Iterator_i.h
//
// = DESCRIPTION
//   Implementation of the DsLogAdmin::Iterator interface.
//
// = AUTHOR
//    Matthew Braun (mjb2@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TLS_ITERATOR_H
#define TLS_ITERATOR_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogRecordStore.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Iterator_i : public virtual POA_DsLogAdmin::Iterator
{
  // = TITLE
  //   Iterator_i
  // = DESCRIPTION
  //   Iterator to get LogRecords for the Log via a query.
  //
public:
  // = Initialization and Termination methods.
  Iterator_i (void);
  //Constructor

  ~Iterator_i (void);
  //Destructor

  DsLogAdmin::RecordList * get (CORBA::ULong position,
                                CORBA::ULong how_many,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidParam));
  // Gets a list of LogRecords.

  void destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This destroys the iterator.
};

#endif /* TLS_ITERATOR_H */
