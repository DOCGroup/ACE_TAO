/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Iterator_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::Iterator interface.
 *
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  
 */
//=============================================================================

#ifndef TAO_TLS_ITERATOR_H
#define TAO_TLS_ITERATOR_H
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/LogRecordStore.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Iterator_i
 *
 * @brief Iterator to get LogRecords for the log via a query.
 */
class TAO_Log_Export TAO_Iterator_i : 
  public POA_DsLogAdmin::Iterator,
  public virtual PortableServer::RefCountServantBase
{
public:

  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Iterator_i (TAO_LogRecordStore::LOG_RECORD_STORE &store,
                  CORBA::ULong start,
                  const char *constraint,
                  CORBA::ULong max_store_size,
                  CORBA::ULong max_rec_list_len
                  );

  /// Destructor.
  ~TAO_Iterator_i (void);

  /// Gets a list of LogRecords.
  DsLogAdmin::RecordList* get (CORBA::ULong position,
                               CORBA::ULong how_many
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidParam));

  /// This destroys the iterator.
  void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Storage.
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP_ITER iter_;

  /// Constraint.
  const char *constraint_;

  /// Max entries in the storage.
  CORBA::ULong max_store_size_;

  /// Max rec list length.
  CORBA::ULong max_rec_list_len_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_TLS_ITERATOR_H */
