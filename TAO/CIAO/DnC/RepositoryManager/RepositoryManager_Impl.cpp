/* -*- C++  -*- */
// $Id$

#include "RepositoryManager_Impl.h"

CIAO::RepositoryManager_Impl::RepositoryManager_Impl 
   (CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

CIAO::RepositoryManager_Impl::RepositoryManager_Impl ()
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
  DOMDocument* doc = CIAO::Config_Handler::Utils::create_document (location);
  CIAO::Config_Handler::PC_Handler pc_handler (doc,
                                               DOMNodeFilter::SHOW_ELEMENT |
                                               DOMNodeFilter::SHOW_TEXT);
  pc_handler.process_PackageConfiguration (this->pc_);
  //Deployment::DnC_Dump::dump (this->pc_);
  this->pc_table_.bind (installation_name, &(pc_));
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
  Deployment::PackageConfiguration* pc = 0;
  if (pc_table_.find (name, pc) == 0)
    {
      return pc;
    }

  return 0;
}

Deployment::PackageConfiguration*
CIAO::RepositoryManager_Impl::
findPackageByUUID (const char* name
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
  return 0;
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
findNamesByType (const char* type
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllNames (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllTypes (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
CIAO::RepositoryManager_Impl::
deletePackage (const char* name
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
}

void
CIAO::RepositoryManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (1 ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
}
