/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   BasicLog_i.h
//
// = DESCRIPTION
//   Implementation of the DsLogAdmin::BasicLog interface.
//
// = AUTHOR
//    Matthew Braun (mjb2@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TLS_BASICLOG_I_H
#define TLS_BASICLOG_I_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class LogMgr_i;

class BasicLog_i : public Log_i,
                   public POA_DsLogAdmin::BasicLog,
                   public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   BasicLog_i
  // = DESCRIPTION
  //   The class supports the <destroy> method to destroy the Log.
  //
public:
  // = Initialization and Termination.
  BasicLog_i (LogMgr_i &logmgr_i,
              DsLogAdmin::LogMgr_ptr factory,
              DsLogAdmin::LogId id,
              DsLogAdmin::LogFullAction log_full_action = DsLogAdmin::wrap,
              CORBA::ULongLong max_size = 0,
              ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor

  ~BasicLog_i ();
  // Destructor.

  void
  destroy (CORBA::Environment &ACE_TRY_ENV =
	   CORBA::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroy the log object and all contained records.

 protected:
  LogMgr_i &logmgr_i_;
  // Used to access the hash map that holds all the Logs created.
};
#endif /* TLS_BASICLOG_I_H */
