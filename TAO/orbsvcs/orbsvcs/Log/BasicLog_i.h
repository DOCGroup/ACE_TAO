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

#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/Log_i.h"
#include "log_export.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_LogMgr_i;

/**
 * @class TAO_BasicLog_i
 *
 * @brief It allows clients to write, query and delete records from the log.
 *
 * The class supports the @c destroy> method to destroy the Log.
 */
class TAO_Log_Export TAO_BasicLog_i :
  public TAO_Log_i,
  public POA_DsLogAdmin::BasicLog,
  public virtual PortableServer::RefCountServantBase
{
public:

  /// Constructor.
  TAO_BasicLog_i (TAO_LogMgr_i &logmgr_i,
                  DsLogAdmin::LogMgr_ptr factory,
                  DsLogAdmin::LogId id,
                  DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
                  CORBA::ULongLong max_size = 0,
                  ACE_Reactor *reactor = ACE_Reactor::instance ());

  /// Duplicate the log.
  virtual DsLogAdmin::Log_ptr copy (DsLogAdmin::LogId &id
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Duplicate the log specifying an id.
  virtual DsLogAdmin::Log_ptr copy_with_id (DsLogAdmin::LogId id
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Destroy the log object and all contained records.
  void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Destructor.
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~TAO_BasicLog_i (void);

protected:
	
  /// Used to access the hash map that holds all the Logs created.
  TAO_LogMgr_i &logmgr_i_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_TLS_BASICLOG_I_H */
