/* -*- C++ -*- $Id$ */

#include "orbsvcs/Log/BasicLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"

BasicLog_i::BasicLog_i (LogMgr_i &logmgr_i,
                        DsLogAdmin::LogMgr_ptr factory,
                        DsLogAdmin::LogId id,
                        DsLogAdmin::LogFullActionType log_full_action,
                        CORBA::ULongLong max_size,
                        ACE_Reactor *reactor)
  : Log_i (factory, id, 0, log_full_action, max_size, reactor),
    logmgr_i_(logmgr_i)
{
  // No-Op.
}

BasicLog_i::~BasicLog_i ()
{
  // No-Op.
}

DsLogAdmin::Log_ptr 
BasicLog_i::copy (DsLogAdmin::LogId &id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::BasicLogFactory_var basicLogFactory =
    DsLogAdmin::BasicLogFactory::_narrow (factory_.in () 
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  DsLogAdmin::BasicLog_var log = 
    basicLogFactory->create (DsLogAdmin::halt, 0, id
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // Copy the attributes from the log.
  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);

  return log._retn ();

}

DsLogAdmin::Log_ptr 
BasicLog_i::copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::BasicLogFactory_var basicLogFactory =
    DsLogAdmin::BasicLogFactory::_narrow (factory_.in () 
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  DsLogAdmin::BasicLog_var log = 
    basicLogFactory->create_with_id (id, DsLogAdmin::halt, 0 
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // Copy the attributes from the log.
  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);

  return log._retn ();

}

void
BasicLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_); // check for error?

  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
