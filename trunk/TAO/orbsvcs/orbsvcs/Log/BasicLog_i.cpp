// $Id$

#include "orbsvcs/Log/BasicLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"

BasicLog_i::BasicLog_i (LogMgr_i &logmgr_i,
                        DsLogAdmin::LogMgr_ptr factory,
                        DsLogAdmin::LogId id,
                        DsLogAdmin::LogFullAction log_full_action,
                        CORBA::ULongLong max_size,
                        ACE_Reactor *reactor)
  : Log_i (factory, id, log_full_action, max_size, reactor),
    logmgr_i_(logmgr_i)
{
  // No-Op.
}

BasicLog_i::~BasicLog_i ()
{
  // No-Op.
}

void
BasicLog_i::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_); // check for error?

  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this,
                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}
