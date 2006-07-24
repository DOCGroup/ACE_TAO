#include "orbsvcs/Log/BasicLog_i.h"

#include "orbsvcs/Log/LogMgr_i.h"

ACE_RCSID (Log,
           BasicLog_i,
           "$Id$")

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
TAO_BasicLog_i::copy (DsLogAdmin::LogId &id
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::BasicLogFactory_var basicLogFactory =
    DsLogAdmin::BasicLogFactory::_narrow (factory_.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  DsLogAdmin::BasicLog_var log =
    basicLogFactory->create (DsLogAdmin::halt, 0, id
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());


  // Copy the attributes from the log.
  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();

}

DsLogAdmin::Log_ptr
TAO_BasicLog_i::copy_with_id (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::BasicLogFactory_var basicLogFactory =
    DsLogAdmin::BasicLogFactory::_narrow (factory_.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  DsLogAdmin::BasicLog_var log =
    basicLogFactory->create_with_id (id, DsLogAdmin::halt, 0
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  // Copy the attributes from the log.
  this->copy_attributes (log.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::Log::_nil ());

  return log._retn ();

}

void
TAO_BasicLog_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Remove ourselves from the list of logs.
  this->logmgr_i_.remove (this->logid_
			  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Deregister with POA.
  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this
			       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (id.in ()
				 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_END_VERSIONED_NAMESPACE_DECL
