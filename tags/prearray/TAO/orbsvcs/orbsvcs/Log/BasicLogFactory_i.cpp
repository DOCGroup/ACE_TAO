#include "orbsvcs/Log/BasicLogFactory_i.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (Log,
           BasicLogFactory_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_BasicLogFactory_i::TAO_BasicLogFactory_i (void)
{
}

TAO_BasicLogFactory_i::~TAO_BasicLogFactory_i (void)
{
}

DsLogAdmin::BasicLogFactory_ptr
TAO_BasicLogFactory_i::activate (CORBA::ORB_ptr orb,
                                 PortableServer::POA_ptr poa
                                 ACE_ENV_ARG_DECL)
{
  TAO_LogMgr_i::init (orb, poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());


  PortableServer::ObjectId_var oid =
    this->factory_poa_->activate_object (this
					 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  CORBA::Object_var obj =
    this->factory_poa_->id_to_reference (oid.in ()
					 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  DsLogAdmin::BasicLogFactory_var v_return =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in ()
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLogFactory::_nil ());

  return v_return._retn ();
}

DsLogAdmin::BasicLog_ptr
TAO_BasicLogFactory_i::create (DsLogAdmin::LogFullActionType full_action,
                               CORBA::ULongLong max_size,
                               DsLogAdmin::LogId_out id_out
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidLogFullAction
                   ))
{
  this->create_i (full_action,
		  max_size,
		  0,
		  id_out
		  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());
  DsLogAdmin::LogId id = id_out;

#if (TAO_HAS_MINIMUM_POA == 0)
  DsLogAdmin::Log_var log =
    this->create_log_reference (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());
#else
  DsLogAdmin::Log_var log =
    this->create_log_object (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());
#endif

  // narrow to BasicLog
  DsLogAdmin::BasicLog_var basic_log =
    DsLogAdmin::BasicLog::_narrow (log.in ());

  return basic_log._retn ();
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
  this->create_with_id_i (id,
			  full_action,
			  max_size,
			  0
			  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());

#if (TAO_HAS_MINIMUM_POA == 0)
  DsLogAdmin::Log_var log =
    this->create_log_reference (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());
#else
  DsLogAdmin::Log_var log =
    this->create_log_object (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (DsLogAdmin::BasicLog::_nil ());
#endif

  // narrow to BasicLog
  DsLogAdmin::BasicLog_var basic_log =
    DsLogAdmin::BasicLog::_narrow (log.in ());

  return basic_log._retn ();
}

CORBA::RepositoryId
TAO_BasicLogFactory_i::create_repositoryid ()
{
  return CORBA::string_dup ("IDL:omg.org/DsLogAdmin:BasicLog:1.0");
}

PortableServer::ServantBase*
TAO_BasicLogFactory_i::create_log_servant (DsLogAdmin::LogId id
					   ACE_ENV_ARG_DECL)
{
  TAO_BasicLog_i* basic_log_i;

  ACE_NEW_THROW_EX (basic_log_i,
                    TAO_BasicLog_i (this->orb_.in (),
				    this->log_poa_.in (),
                                    *this,
                                    this->log_mgr_.in (),
                                    id),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  basic_log_i->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return basic_log_i;
}

TAO_END_VERSIONED_NAMESPACE_DECL
