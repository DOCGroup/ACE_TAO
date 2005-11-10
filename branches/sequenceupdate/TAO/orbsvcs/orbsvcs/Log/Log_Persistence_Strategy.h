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

#include "log_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

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
    create_log_store (CORBA::ORB_ptr orb, TAO_LogMgr_i* mgr)	= 0;

private:
};

#include /**/ "ace/post.h"

#endif /* TAO_TLS_PERSISTENCE_STRATEGY_H */
