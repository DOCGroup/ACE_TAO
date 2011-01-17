// -*- C++ -*-

//=============================================================================
/**
 *  @file   Log_Persistence_Strategy.h
 *
 *  $Id$
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_PERSISTENCE_STRATEGY_H
#define TAO_TLS_PERSISTENCE_STRATEGY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Log/log_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogStore;
class TAO_LogMgr_i;

namespace CORBA
{
  class ORB;
  typedef ORB* ORB_ptr;
}

/**
 * @class TAO_Log_Persistence_Strategy
 *
 * @brief Base Strategy for Log / Log Record Storage
 *
 */
class TAO_Log_Serv_Export TAO_Log_Persistence_Strategy
  : public ACE_Service_Object
{
public:
  /// @brief Log Store Factory
  virtual TAO_LogStore*
    create_log_store (TAO_LogMgr_i* logmgr_i)     = 0;

private:
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TLS_PERSISTENCE_STRATEGY_H */
