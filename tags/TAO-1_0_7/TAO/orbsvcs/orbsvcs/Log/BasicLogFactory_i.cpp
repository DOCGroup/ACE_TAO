// $Id$

#include "orbsvcs/Log/BasicLogFactory_i.h"
#include "ace/Auto_Ptr.h"

BasicLogFactory_i::BasicLogFactory_i (void)
{
  //No-Op
}

BasicLogFactory_i::~BasicLogFactory_i()
{
  // No-Op.
}

DsLogAdmin::BasicLogFactory_ptr
BasicLogFactory_i::activate (PortableServer::POA_ptr poa,
                           CORBA::Environment &ACE_TRY_ENV)
{
  DsLogAdmin::BasicLogFactory_var v_return;

  PortableServer::ObjectId_var oid =
    poa->activate_object (this,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (v_return._retn ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid.in (),
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (v_return._retn ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (v_return._retn ());

  v_return =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in (),
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  return v_return._retn ();
}

DsLogAdmin::BasicLog_ptr
BasicLogFactory_i::create (DsLogAdmin::LogFullAction full_action,
                           CORBA::ULongLong max_rec_size,
                           DsLogAdmin::LogId_out id,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::NoResources
                   ))
{
  // Get an id for this Log.
  this->max_id_++;

  DsLogAdmin::BasicLog_ptr basiclog =
    this->create_with_id (this->max_id_,
                          full_action,
                          max_rec_size,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // Set the id to return..
  id = this->max_id_;
  return basiclog;
}

DsLogAdmin::BasicLog_ptr
BasicLogFactory_i::create_with_id (DsLogAdmin::LogId id,
                                   DsLogAdmin::LogFullAction full_action,
                                   CORBA::ULongLong max_size,
                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   DsLogAdmin::NoResources,
                   DsLogAdmin::LogIdAlreadyExists
                   ))
{
  // Make sure the id not used up.
  if (hash_map_.find (id) == 0)
    ACE_THROW_RETURN (DsLogAdmin::LogIdAlreadyExists (),
                      DsLogAdmin::BasicLog::_nil ());

  DsLogAdmin::BasicLog_var basic_log;
  // Object to return.

  BasicLog_i* basic_log_i;

  ACE_NEW_THROW_EX (basic_log_i,
                    BasicLog_i (*this,
                                this->log_mgr_.in (),
                                id,
                                full_action,
                                max_size
                                ),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (basic_log._retn ());

  auto_ptr<BasicLog_i> basic_log_auto (basic_log_i);
  // just in case the activation fails.

  basic_log_i->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (basic_log._retn ());

  // Register with the poa
  basic_log = basic_log_i->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (basic_log._retn ());

  basic_log_i->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

  // Add to the Hash table..
  if (hash_map_.bind (id, basic_log) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      DsLogAdmin::BasicLog::_nil ());

  // All is well, release the reference.
  basic_log_auto.release ();

  return basic_log._retn ();
}
