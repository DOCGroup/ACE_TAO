/* -*- C++  -*- */
// $Id$

#include "RepositoryManager_Impl.h"

CIAO::RepositoryManager_Impl::RepositoryManager_Impl 
   (CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa)
{
}

CIAO::RepositoryManager_Impl::~RepositoryManager_Impl ()
{
}

void
CIAO::RepositoryManager_Impl::
installPackage (const char* installation_name,
                const char* location
                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NameExists,
                   Deployment::PackageError))
{
}

void
CIAO::RepositoryManager_Impl::
createPackage (const char* installation_name,
               const Deployment::PackageConfiguration& pc,
               const char* base_location,
               CORBA::Boolean replace
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NameExists,
                   Deployment::PackageError))
{
}

Deployment::PackageConfiguration*
CIAO::RepositoryManager_Impl::
findPackageByName (const char* name
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
}

Deployment::PackageConfiguration*
CIAO::RepositoryManager_Impl::
findPackageByUUID (const char* name
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
findNamesByType (const char* type
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllNames (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllTypes (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
CIAO::RepositoryManager_Impl::
deletePackage (const char* name
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
}
