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

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/LogMgr_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export BasicLogFactory_i :
public POA_DsLogAdmin::BasicLogFactory,
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
    activate (PortableServer::POA_ptr poa,
              CORBA::Environment &ACE_TRY_ENV);
  // Activate this servant with the POA passed in.

  DsLogAdmin::BasicLog_ptr
    create (DsLogAdmin::LogFullAction full_action,
            CORBA::ULongLong max_size,
            DsLogAdmin::LogId_out id,
            CORBA_Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::NoResources,
                     ));
  // Allows clients to create new BasicLog objects. Raises
  // DsLogAdmin::NoResources and DsLogAdmin::InvalidThreshold

  DsLogAdmin::BasicLog_ptr
    create_with_id (DsLogAdmin::LogId id,
                    DsLogAdmin::LogFullAction full_action,
                    CORBA::ULongLong max_size,
                    CORBA_Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     DsLogAdmin::NoResources,
                     DsLogAdmin::LogIdAlreadyExists,
                     ));
  // Same as create (), but allows clients to specify the id.

 protected:
  DsLogAdmin::LogMgr_var log_mgr_;
  // Our object ref. after <active>ation.
};
#endif /* TLS_BASICLOGFACTORY_I_H */
