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
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"
#include "log_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class LogMgr_i;

// Bug in MSVC 5, See KB article Q167350
#if defined (_MSC_VER) && (_MSC_VER == 1100)
using DsLogAdmin::wrap;
#endif /* (_MSC_VER) && (_MSC_VER == 1100) */

class TAO_Log_Export BasicLog_i : public Log_i,
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
  destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroy the log object and all contained records.

 protected:
  LogMgr_i &logmgr_i_;
  // Used to access the hash map that holds all the Logs created.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TLS_BASICLOG_I_H */
