#include "orbsvcs/Log/BasicLogFactory_i.h"


ACE_RCSID (Log,
           BasicLogFactory_i,
           "$Id$")


TAO_BasicLogFactory_i::TAO_BasicLogFactory_i (void)
{
}

TAO_BasicLogFactory_i::~TAO_BasicLogFactory_i (void)
{
}

DsLogAdmin::BasicLogFactory_ptr
TAO_BasicLogFactory_i::activate (PortableServer::POA_ptr poa
                                 ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId_var oid =
    poa->activate_object (this
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  return DsLogAdmin::BasicLogFactory::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
}

DsLogAdmin::BasicLog_ptr
TAO_BasicLogFactory_i::create (DsLogAdmin::LogFullActionType full_action,
                               CORBA::ULongLong max_rec_size,
                               DsLogAdmin::LogId_out id
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidLogFullAction
                   ))
{
  // Get an id for this Log.
  this->max_id_++;

  DsLogAdmin::BasicLog_ptr basiclog =
    this->create_with_id (this->max_id_,
                          full_action,
                          max_rec_size
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // Set the id to return..
  id = this->max_id_;

  // Store the id in the LogIdList.
  CORBA::ULong len = logid_list_.length ();
  logid_list_.length (len + 1);
  logid_list_[len] = id;

  return basiclog;
}

DsLogAdmin::BasicLog_ptr
TAO_BasicLogFactory_i::create_with_id (DsLogAdmin::LogId id,
                                       DsLogAdmin::LogFullActionType full_action,
                                       CORBA::ULongLong max_size
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   DsLogAdmin::LogIdAlreadyExists,
                   DsLogAdmin::InvalidLogFullAction
                   ))
{
  // Make sure the id not used up.
  if (hash_map_.find (id) == 0)
    ACE_THROW_RETURN (DsLogAdmin::LogIdAlreadyExists (),
                      DsLogAdmin::BasicLog::_nil ());

  DsLogAdmin::BasicLog_var basic_log;
  // Object to return.

  TAO_BasicLog_i* basic_log_i;

  ACE_NEW_THROW_EX (basic_log_i,
                    TAO_BasicLog_i (*this,
                                    this->log_mgr_.in (),
                                    id,
                                    full_action,
                                    max_size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  PortableServer::ServantBase_var safe_basic_log_i = basic_log_i;
  // Transfer ownership to the POA.

  basic_log_i->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // Register with the poa
  basic_log = basic_log_i->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // widening a BasicLog_var to a Log_var
  DsLogAdmin::Log_var log = DsLogAdmin::BasicLog::_duplicate (basic_log.in ());

  // Add to the Hash table..
  if (hash_map_.bind (id, log) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      DsLogAdmin::BasicLog::_nil ());

  return basic_log._retn ();
}
