/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   LogMgr_i.h
//
// = DESCRIPTION
//   Implementation of the LogManager i,e DsLogAdmin::LogMgr interface.
//
// = AUTHOR
//    Matthew Braun (mjb2@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TLS_LOGMGR_I_H
#define TLS_LOGMGR_I_H
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/Log/BasicLog_i.h"
#include "log_export.h"

class TAO_Log_Export LogMgr_i : public virtual POA_DsLogAdmin::LogMgr
{
  // = TITLE
  //   LogMgr_i
  // = DESCRIPTION
  //   This class has methods to query for Log objects.
  //
public:
  // Initialization and Termination Methods
  LogMgr_i (void);
  // Constructor.

  ~LogMgr_i ();
  // Destructor.

  DsLogAdmin::LogList *
    list_logs (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  // Lists all logs created by the log factory

  DsLogAdmin::Log_ptr
    find_log (DsLogAdmin::LogId id
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  // Returns a reference to the log with the supplied id

  DsLogAdmin::LogIdList *
  list_logs_by_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  // Lists all log ids

 int remove (DsLogAdmin::LogId id);
 // remove the given entry from the hash table.

protected:
  typedef ACE_Hash_Map_Manager <DsLogAdmin::LogId,
    DsLogAdmin::BasicLog_var,TAO_SYNCH_MUTEX> HASHMAP;
  // Define the HASHMAP.

  HASHMAP hash_map_;
  // The map of Logs created.

  DsLogAdmin::LogId max_id_;
  // The Max id assigned so far.
};
#include "ace/post.h"
#endif /* TLS_LOGMGR_I_H */
