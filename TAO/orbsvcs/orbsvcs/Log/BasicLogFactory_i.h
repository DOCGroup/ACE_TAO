/* -*- C++ -*- */
// $Id$
// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   BasicLogFactory_i.h
//
// = DESCRIPTION
//   Implements the Factory for BasicLog Objects.
//
// = AUTHOR
//    Matthew Braun (mjb2@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TLS_BASICLOGFACTORY_I_H
#define TLS_BASICLOGFACTORY_I_H
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"

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
    create (DsLogAdmin::LogFullAction full_action,
            CORBA::ULongLong max_size,
            DsLogAdmin::LogId_out id
            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::NoResources
                     ));
  // Allows clients to create new BasicLog objects. Raises
  // DsLogAdmin::NoResources and DsLogAdmin::InvalidThreshold

  DsLogAdmin::BasicLog_ptr
    create_with_id (DsLogAdmin::LogId id,
                    DsLogAdmin::LogFullAction full_action,
                    CORBA::ULongLong max_size
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     DsLogAdmin::NoResources,
                     DsLogAdmin::LogIdAlreadyExists
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
