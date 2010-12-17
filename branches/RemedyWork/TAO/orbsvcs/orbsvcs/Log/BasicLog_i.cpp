// $Id$

#include "orbsvcs/Log/BasicLog_i.h"

#include "orbsvcs/Log/LogMgr_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_BasicLog_i::TAO_BasicLog_i (CORBA::ORB_ptr orb,
                                PortableServer::POA_ptr poa,
                                TAO_LogMgr_i &logmgr_i,
                                DsLogAdmin::LogMgr_ptr factory,
                                DsLogAdmin::LogId id)
  : TAO_Log_i (orb, logmgr_i, factory, id, 0),
    poa_(PortableServer::POA::_duplicate(poa))
{
  // No-Op.
}

TAO_BasicLog_i::~TAO_BasicLog_i (void)
{
  // No-Op.
}

DsLogAdmin::Log_ptr
TAO_BasicLog_i::copy (DsLogAdmin::LogId &id)
{
  DsLogAdmin::BasicLogFactory_var basicLogFactory =
    DsLogAdmin::BasicLogFactory::_narrow (factory_.in ());

  DsLogAdmin::BasicLog_var log =
    basicLogFactory->create (DsLogAdmin::halt, 0, id);


  // Copy the attributes from the log.
  this->copy_attributes (log.in ());

  return log._retn ();

}

DsLogAdmin::Log_ptr
TAO_BasicLog_i::copy_with_id (DsLogAdmin::LogId id)
{
  DsLogAdmin::BasicLogFactory_var basicLogFactory =
    DsLogAdmin::BasicLogFactory::_narrow (factory_.in ());

  DsLogAdmin::BasicLog_var log =
    basicLogFactory->create_with_id (id, DsLogAdmin::halt, 0);

  // Copy the attributes from the log.
  this->copy_attributes (log.in ());

  return log._retn ();

}

void
TAO_BasicLog_i::destroy (void)
{
  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_);

  // Deregister with POA.
  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this);

  this->poa_->deactivate_object (id.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
