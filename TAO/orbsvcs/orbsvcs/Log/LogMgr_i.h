// -*- C++ -*-

//=============================================================================
/**
 *  @file   LogMgr_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::LogMgr interface.
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_LOGMGR_I_H
#define TAO_TLS_LOGMGR_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/Log_i.h"

#include "log_export.h"

/**
 * @class TAO_LogMgr_i
 *
 * @brief Factory of Logs. Contains list of Logs created.
 *
 * This factory base class is used to maintain a list of logs 
 * created by it. Logs can also be removed from the list.
 */
class TAO_Log_Export TAO_LogMgr_i : public virtual POA_DsLogAdmin::LogMgr
{
public:

  // = Initialization and Termination Methods

  /// Constructor.
  TAO_LogMgr_i (void);

  /// Destructor.
  ~TAO_LogMgr_i ();

  /// Lists all logs created by the log factory.
  DsLogAdmin::LogList *
    list_logs (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// Returns a reference to the log with the supplied id.
  DsLogAdmin::Log_ptr
    find_log (DsLogAdmin::LogId id
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// Lists all log ids.
  DsLogAdmin::LogIdList *
  list_logs_by_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

 /// remove the given entry from the hash table.
 int remove (DsLogAdmin::LogId id);


protected:

  /// Define the HASHMAP.
  typedef ACE_Hash_Map_Manager <DsLogAdmin::LogId,
    DsLogAdmin::Log_var,TAO_SYNCH_MUTEX> HASHMAP;

  /// The map of Logs created.
  HASHMAP hash_map_;

  /// The Max id assigned so far.
  DsLogAdmin::LogId max_id_;

  /// A list of the current log ids.
  DsLogAdmin::LogIdList logid_list_;
};
#include /**/ "ace/post.h"
#endif /* TAO_TLS_LOGMGR_I_H */
