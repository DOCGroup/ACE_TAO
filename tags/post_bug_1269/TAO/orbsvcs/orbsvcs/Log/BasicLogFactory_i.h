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

#ifndef TLS_BASICLOGFACTORY_I_H
#define TLS_BASICLOGFACTORY_I_H
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/BasicLog_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Log_Export BasicLogFactory_i : public POA_DsLogAdmin::BasicLogFactory,
                                         public LogMgr_i
{
  // = TITLE
  //   BasicLogFactory_i
  // = DESCRIPTION
  //   Implementation of the BasicLogFactory interface.
  //
public:
  //= Initialization and termination code.
  BasicLogFactory_i (void);
  // Ctor

  ~BasicLogFactory_i ();
  // Dtor

  DsLogAdmin::BasicLogFactory_ptr
    activate (PortableServer::POA_ptr poa
              ACE_ENV_ARG_DECL);
  // Activate this servant with the POA passed in.

  DsLogAdmin::BasicLog_ptr
    create (DsLogAdmin::LogFullActionType full_action,
            CORBA::ULongLong max_size,
            DsLogAdmin::LogId_out id
            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidLogFullAction
                     ));
  // Allows clients to create new BasicLog objects. Raises
  // DsLogAdmin::NoResources and DsLogAdmin::InvalidThreshold

  DsLogAdmin::BasicLog_ptr
    create_with_id (DsLogAdmin::LogId id,
                    DsLogAdmin::LogFullActionType full_action,
                    CORBA::ULongLong max_size//,
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     DsLogAdmin::LogIdAlreadyExists,
                     DsLogAdmin::InvalidLogFullAction
                     ));
  // Same as create (), but allows clients to specify the id.

 protected:
  DsLogAdmin::LogMgr_var log_mgr_;
  // Our object ref. after <active>ation.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TLS_BASICLOGFACTORY_I_H */
