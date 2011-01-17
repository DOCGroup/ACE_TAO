// -*- C++ -*-

//=============================================================================
/**
 *  @file   Hash_Persistence_Strategy.h
 *
 *  $Id$
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_HASH_PERSISTENCE_STRATEGY_H
#define TAO_TLS_HASH_PERSISTENCE_STRATEGY_H

#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/Log_Persistence_Strategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Hash_Persistence_Strategy
 *
 * @brief Concrete Strategy for Log / Log Record Storage
 *
 * Stores log parameters and log records in hash maps
 */
class TAO_Log_Serv_Export TAO_Hash_Persistence_Strategy
  : public TAO_Log_Persistence_Strategy
{
public:
  // = Initialization and Termination Methods

  /// Constructor.
  TAO_Hash_Persistence_Strategy ();

  /// Destructor.
  virtual ~TAO_Hash_Persistence_Strategy ();

  /// @brief Log Store Factory
  virtual TAO_LogStore*
    create_log_store (TAO_LogMgr_i* mgr);

private:
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TLS_HASH_PERSISTENCE_STRATEGY_H */
