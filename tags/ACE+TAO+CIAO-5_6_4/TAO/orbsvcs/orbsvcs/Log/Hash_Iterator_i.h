// -*- C++ -*-

//=============================================================================
/**
 *  @file   Hash_Iterator_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::Iterator interface.
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TLS_HASH_ITERATOR_H
#define TAO_TLS_HASH_ITERATOR_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/Iterator_i.h"
#include "orbsvcs/Log/Hash_LogRecordStore.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Hash_Iterator_i
 *
 * @brief Iterator to get LogRecords for the log via a query.
 */
class TAO_Log_Serv_Export TAO_Hash_Iterator_i
  : public TAO_Iterator_i
{
public:

  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Hash_Iterator_i (PortableServer::POA_ptr poa,
		       ACE_Reactor* reactor,
		       TAO_Hash_LogRecordStore* recordstore,
                       TAO_Hash_LogRecordStore::LOG_RECORD_STORE_ITER iter,
                       TAO_Hash_LogRecordStore::LOG_RECORD_STORE_ITER iter_end,
                       CORBA::ULong start,
                       const char *constraint,
                       CORBA::ULong max_rec_list_len);

  /// Destructor.
  virtual ~TAO_Hash_Iterator_i (void);

  /// Gets a list of LogRecords.
  virtual DsLogAdmin::RecordList* get (CORBA::ULong position,
                                       CORBA::ULong how_many);

private:
  /// Pointer to record store
  TAO_Hash_LogRecordStore* recordstore_;

  /// Current Iterator.
  TAO_Hash_LogRecordStore::LOG_RECORD_STORE_ITER iter_;

  /// End Iterator.
  TAO_Hash_LogRecordStore::LOG_RECORD_STORE_ITER iter_end_;

  /// Position.
  CORBA::ULong current_position_;

  /// Constraint.
  CORBA::String_var constraint_;

  /// Max rec list length.
  CORBA::ULong max_rec_list_len_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_HASH_ITERATOR_H */
