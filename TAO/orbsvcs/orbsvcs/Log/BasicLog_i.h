/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   BasicLog_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::BasicLog interface.
 *
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================


#ifndef TAO_TLS_BASICLOG_I_H
#define TAO_TLS_BASICLOG_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/Log/log_serv_export.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;

/**
 * @class TAO_BasicLog_i
 *
 * @brief It allows clients to write, query and delete records from the log.
 *
 * The class supports the @c destroy method to destroy the Log.
 */
class TAO_Log_Serv_Export TAO_BasicLog_i :
  public TAO_Log_i,
  public POA_DsLogAdmin::BasicLog
{
public:

  /// Constructor.
  TAO_BasicLog_i (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa,
                  TAO_LogMgr_i &logmgr_i,
                  DsLogAdmin::LogMgr_ptr factory,
                  DsLogAdmin::LogId id);

  /// Duplicate the log.
  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id);

  /// Duplicate the log specifying an id.
  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id);

  /// Destroy the log object and all contained records.
  void destroy (void);

protected:

  /// Destructor.
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~TAO_BasicLog_i (void);

private:

  /// POA.
  PortableServer::POA_var	poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_TLS_BASICLOG_I_H */
