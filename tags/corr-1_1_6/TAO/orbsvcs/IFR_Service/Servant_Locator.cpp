// $Id$

#include "Servant_Locator.h"
#include "Servant_Factory.h"

ACE_RCSID(IFR_Service, Servant_Locator, "$Id$")

IFR_ServantLocator::IFR_ServantLocator (TAO_Repository_i *repo)
  : repo_ (repo)
{
}

PortableServer::Servant
IFR_ServantLocator::preinvoke (
    const PortableServer::ObjectId &oid,
    PortableServer::POA_ptr poa,
    const char * /* operation */,
    PortableServer::ServantLocator::Cookie &cookie
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var s =
    PortableServer::ObjectId_to_string (oid);

  ACE_Configuration_Section_Key servant_key;
  int status =
    this->repo_->config ()->expand_path (this->repo_->root_key (),
                                         s.in (),
                                         servant_key,
                                         0);

  if (status != 0)
    {
      // If we're here, destroy() has been called.
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
    }

  PortableServer::Servant servant =
    this->repo_->servant_factory ()->create_tie (servant_key,
                                                 poa,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  cookie = servant;

  return servant;
}

void
IFR_ServantLocator::postinvoke (
    const PortableServer::ObjectId & /* oid */,
    PortableServer::POA_ptr /* poa */,
    const char * /* operation */,
    PortableServer::ServantLocator::Cookie /* cookie */,
    PortableServer::Servant servant
    TAO_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  delete servant;
}
