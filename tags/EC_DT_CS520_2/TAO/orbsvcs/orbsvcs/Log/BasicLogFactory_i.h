/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   BasicLogFactory_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::BasicLogFactory interface.
 *
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_BASICLOGFACTORY_I_H
#define TAO_TLS_BASICLOGFACTORY_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/BasicLog_i.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_BasicLogFactory_i
 *
 * @brief The BasicLogFactory is a factory that is used to create event-unaware Logs.
 */
class TAO_Log_Export TAO_BasicLogFactory_i : 
  public POA_DsLogAdmin::BasicLogFactory,
  public TAO_LogMgr_i
{
public:

  //= Initialization and termination code.

  /// Constructor.
  TAO_BasicLogFactory_i (void);

  /// Destructor.
  ~TAO_BasicLogFactory_i ();

  /// Activate this servant with the POA passed in.
  DsLogAdmin::BasicLogFactory_ptr
    activate (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);

  /// Allows clients to create new BasicLog objects. Raises
  /// DsLogAdmin::NoResources and DsLogAdmin::InvalidThreshold.
  DsLogAdmin::BasicLog_ptr
    create (DsLogAdmin::LogFullActionType full_action,
            CORBA::ULongLong max_size,
            DsLogAdmin::LogId_out id
            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidLogFullAction
                     ));

  /// Same as create (), but allows clients to specify the id.
  DsLogAdmin::BasicLog_ptr
    create_with_id (DsLogAdmin::LogId id,
                    DsLogAdmin::LogFullActionType full_action,
                    CORBA::ULongLong max_size
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     DsLogAdmin::LogIdAlreadyExists,
                     DsLogAdmin::InvalidLogFullAction
                     ));

protected:

  /// Our object ref. after <active>ation.
  DsLogAdmin::LogMgr_var log_mgr_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_BASICLOGFACTORY_I_H */
