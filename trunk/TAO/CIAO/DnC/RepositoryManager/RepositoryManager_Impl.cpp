/* -*- C++  -*- */
// $Id$

#include "RepositoryManager_Impl.h"
#include "Config_Handlers/TPD_Handler.h"

CIAO::RepositoryManager_Impl::RepositoryManager_Impl
   (CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
  // @@ (OO) The default size for an ACE_Hash_Map_Mapanger is quiet
  //         large.  The maximum size of an ACE_Hash_Map_Manager is
  //         also fixed, i.e. it does not grow dynamically on demand.
  //         Make sure the default size of component_map_ is
  //         appropriate for your needs.  You may also want to make
  //         the size configurable at compile-time, at least.
{
}

CIAO::RepositoryManager_Impl::RepositoryManager_Impl ()
  // @@ (OO) The default size for an ACE_Hash_Map_Mapanger is quiet
  //         large.  The maximum size of an ACE_Hash_Map_Manager is
  //         also fixed, i.e. it does not grow dynamically on demand.
  //         Make sure the default size of component_map_ is
  //         appropriate for your needs.  You may also want to make
  //         the size configurable at compile-time, at least.
{
}

CIAO::RepositoryManager_Impl::~RepositoryManager_Impl ()
{
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::RepositoryManager_Impl::
installPackage (const char* installation_name,
                const char* location
                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NameExists,
                   Deployment::PackageError))
{
  xercesc::XMLPlatformUtils::Initialize();
  XercesDOMParser *tpd_parser = new XercesDOMParser;
  XercesDOMParser *pc_parser = new XercesDOMParser;
  auto_ptr<XercesDOMParser> cleanup_parser (tpd_parser);
  auto_ptr<XercesDOMParser> cleanup_pc_parser (pc_parser);
  XercesDOMParser::ValSchemes val_schema = XercesDOMParser::Val_Auto;

  tpd_parser->setValidationScheme (val_schema);
  tpd_parser->setDoNamespaces (true);
  tpd_parser->setDoSchema (true);
  tpd_parser->setValidationSchemaFullChecking (true);
  tpd_parser->setCreateEntityReferenceNodes (false);
  tpd_parser->setIncludeIgnorableWhitespace (false);
  tpd_parser->parse (location);

  DOMDocument* tpd_doc = tpd_parser->getDocument ();

  CIAO::Config_Handler::TPD_Handler top_pc_handler (tpd_doc,
                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                   DOMNodeFilter::SHOW_TEXT);
  ACE_TString package_location = top_pc_handler.
      process_TopLevelPackageDescription ();

  pc_parser->setValidationScheme (val_schema);
  pc_parser->setDoNamespaces (true);
  pc_parser->setDoSchema (true);
  pc_parser->setValidationSchemaFullChecking (true);
  pc_parser->setCreateEntityReferenceNodes (false);
  pc_parser->setIncludeIgnorableWhitespace (false);
  pc_parser->parse (package_location.c_str());
  DOMDocument* pc_doc = pc_parser->getDocument ();

  CIAO::Config_Handler::PC_Handler pc_handler (pc_doc,
                                               DOMNodeFilter::SHOW_ELEMENT |
                                               DOMNodeFilter::SHOW_TEXT);
  pc_handler.process_PackageConfiguration (this->pc_);
  this->pc_table_.bind (installation_name, &(pc_));
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::RepositoryManager_Impl::
createPackage (const char*,
               const Deployment::PackageConfiguration&,
               const char*,
               CORBA::Boolean
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NameExists,
                   Deployment::PackageError))
{
  // @@ (OO) It appears that you should be throwing a
  //         CORBA::NO_IMPLEMENT exception here.  Please confirm.
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
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
      // @@ (OO) It appears that you should be returning a
      //         duplicated/copied version of the found
      //         PackageConfiguration.
      return pc;
    }

  // @@ (OO) It appears that you should be throwing a
  //         Deployment::NoSuchName exception here.  Please confirm.
  return 0;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
Deployment::PackageConfiguration*
CIAO::RepositoryManager_Impl::
findPackageByUUID (const char*
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
  // @@ (OO) It appears that you should be throwing a
  //         CORBA::NO_IMPLEMENT exception here.  Please confirm.
  return 0;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
findNamesByType (const char*
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) It appears that you should returning a sequence of length
  //         zero.  Please confirm.
  return 0;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllNames (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) It appears that you should returning a sequence of length
  //         zero.  Please confirm.
  return 0;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllTypes (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) It appears that you should returning a sequence of length
  //         zero.  Please confirm.
  return 0;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::RepositoryManager_Impl::
deletePackage (const char*
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::RepositoryManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ (OO) Wrong emulated exception argument.  Use
  //         ACE_ENV_ARG_PARAMETER instead.
  this->orb_->shutdown (1 ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
}
