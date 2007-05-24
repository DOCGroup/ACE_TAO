// -*- C++ -*-

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
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogAdminS.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "orbsvcs/Log/log_serv_export.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Iterator_i
 *
 * @brief Iterator to get LogRecords for the log via a query.
 */
class TAO_Log_Serv_Export TAO_Iterator_i
  : public virtual POA_DsLogAdmin::Iterator,
    public ACE_Event_Handler
{
public:

  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Iterator_i (PortableServer::POA_ptr poa, ACE_Reactor* reactor);

  /// Destructor.
  virtual ~TAO_Iterator_i (void);

  /// Gets a list of LogRecords.
  virtual DsLogAdmin::RecordList* get (CORBA::ULong position,
				       CORBA::ULong how_many)
    = 0;

  /// This destroys the iterator.
  virtual void destroy (void);

protected:
  /// POA
  PortableServer::POA_var poa_;

  /// Reactor
  ACE_Reactor* reactor_;

  /// Timeout
  static ACE_Time_Value timeout_;

  /// Timer ID
  long timer_id_;

  virtual int handle_timeout (const ACE_Time_Value&, const void *);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_ITERATOR_H */
