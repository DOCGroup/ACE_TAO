#include "orbsvcs/Log/BasicLogFactory_i.h"
#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_BasicLogFactory_i::TAO_BasicLogFactory_i () = default;

TAO_BasicLogFactory_i::~TAO_BasicLogFactory_i () = default;

DsLogAdmin::BasicLogFactory_ptr
TAO_BasicLogFactory_i::activate (CORBA::ORB_ptr orb,
                                 PortableServer::POA_ptr poa)
{
  TAO_LogMgr_i::init (orb, poa);

  PortableServer::ObjectId_var const oid =
    this->factory_poa_->activate_object (this);

  CORBA::Object_var const obj =
    this->factory_poa_->id_to_reference (oid.in ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ());

  DsLogAdmin::BasicLogFactory_var v_return =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in ());

  return v_return._retn ();
}

DsLogAdmin::BasicLog_ptr
TAO_BasicLogFactory_i::create (DsLogAdmin::LogFullActionType full_action,
                               CORBA::ULongLong max_size,
                               DsLogAdmin::LogId_out id_out)
{
  this->create_i (full_action,
                  max_size,
                  nullptr,
                  id_out);
  DsLogAdmin::LogId const id = id_out;

#if (TAO_HAS_MINIMUM_POA == 0)
  DsLogAdmin::Log_var const log =
    this->create_log_reference (id);
#else
  DsLogAdmin::Log_var const log =
    this->create_log_object (id);
#endif

  // narrow to BasicLog
  DsLogAdmin::BasicLog_var basic_log =
    DsLogAdmin::BasicLog::_narrow (log.in ());

  return basic_log._retn ();
}

DsLogAdmin::BasicLog_ptr
TAO_BasicLogFactory_i::create_with_id (DsLogAdmin::LogId id,
                                       DsLogAdmin::LogFullActionType full_action,
                                       CORBA::ULongLong max_size)
{
  this->create_with_id_i (id,
                          full_action,
                          max_size,
                          nullptr);

#if (TAO_HAS_MINIMUM_POA == 0)
  DsLogAdmin::Log_var log =
    this->create_log_reference (id);
#else
  DsLogAdmin::Log_var log =
    this->create_log_object (id);
#endif

  // narrow to BasicLog
  DsLogAdmin::BasicLog_var basic_log =
    DsLogAdmin::BasicLog::_narrow (log.in ());

  return basic_log._retn ();
}

CORBA::RepositoryId
TAO_BasicLogFactory_i::create_repositoryid ()
{
  return CORBA::string_dup (DsLogAdmin::_tc_BasicLog->id ());
}

PortableServer::ServantBase *
TAO_BasicLogFactory_i::create_log_servant (DsLogAdmin::LogId id)
{
  TAO_BasicLog_i *basic_log_i {};

  ACE_NEW_THROW_EX (basic_log_i,
                    TAO_BasicLog_i (this->orb_.in (),
                                    this->log_poa_.in (),
                                    *this,
                                    this->log_mgr_.in (),
                                    id),
                    CORBA::NO_MEMORY ());

  basic_log_i->init ();

  return basic_log_i;
}

TAO_END_VERSIONED_NAMESPACE_DECL
