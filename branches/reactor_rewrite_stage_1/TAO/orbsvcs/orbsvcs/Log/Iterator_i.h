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
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogRecordStore.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Log_Export Iterator_i : public POA_DsLogAdmin::Iterator,
                                  public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   Iterator_i
  // = DESCRIPTION
  //   Iterator to get LogRecords for the Log via a query.
  //
public:
  // = Initialization and Termination methods.
  Iterator_i (LogRecordStore::LOG_RECORD_STORE &store,
              CORBA::ULong start,
              const char *constraint,
              CORBA::ULong max_store_size,
              CORBA::ULong max_rec_list_len
              );
  //Constructor

  ~Iterator_i (void);
  //Destructor

  DsLogAdmin::RecordList* get (CORBA::ULong position,
                               CORBA::ULong how_many
                               TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidParam));
  // Gets a list of LogRecords.

  void destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This destroys the iterator.

private:
  LogRecordStore::LOG_RECORD_HASH_MAP_ITER iter_;
  // Storage.

  const char *constraint_;
  // Constraint.

  CORBA::ULong max_store_size_;
  // max entries in the storage.

  CORBA::ULong max_rec_list_len_;
  // Max rec list length.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TLS_ITERATOR_H */
