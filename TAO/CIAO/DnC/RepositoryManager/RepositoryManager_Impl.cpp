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
  try
    {
      xercesc::XMLPlatformUtils::Initialize();
    }
  catch (const XMLException& xml_e)
    {
      char* message = XMLString::transcode (xml_e.getMessage());
      ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
      ACE_DEBUG ((LM_DEBUG, "Error during initialization : %s\n", message));
      return;
    }
  try
    {
      CIAO::Config_Handler::Config_Error_Handler tpd_err_handler;
      CIAO::Config_Handler::Config_Error_Handler pc_err_handler;
      std::auto_ptr<DOMBuilder> tpd_parser (CIAO::Config_Handler::Utils::
                                            create_parser ());
      tpd_parser->setErrorHandler(&tpd_err_handler);
      DOMDocument* tpd_doc = tpd_parser->parseURI (location);

      if (tpd_err_handler.getErrors())
        {
          throw DOMException ();
        }

      CIAO::Config_Handler::TPD_Handler top_pc_handler 
        (tpd_doc,
         DOMNodeFilter::SHOW_ELEMENT |
         DOMNodeFilter::SHOW_TEXT);
      ACE_TString package_location = top_pc_handler.
          process_TopLevelPackageDescription ();

      std::auto_ptr<DOMBuilder> pc_parser (CIAO::Config_Handler::Utils::
                                           create_parser ());
      pc_parser->setErrorHandler(&pc_err_handler);
      DOMDocument* pc_doc = pc_parser->parseURI (package_location.c_str());

      if (pc_err_handler.getErrors())
        {
          throw DOMException ();
        }

      CIAO::Config_Handler::PC_Handler pc_handler (pc_doc,
                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                   DOMNodeFilter::SHOW_TEXT);
      pc_handler.process_PackageConfiguration (this->pc_);
      this->pc_table_.bind (installation_name, &(pc_));
    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
      return;
    }
  catch (const DOMException& e)
    {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      ACE_ERROR ((LM_ERROR, "\nException occured while parsing %s: \
                  \n",location));
      ACE_ERROR ((LM_ERROR, "DOMException code: %d\n ", e.code));
      if (DOMImplementation::loadDOMExceptionMsg (e.code, errText, maxChars))
        {
          char* message = XMLString::transcode (errText);
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
          ACE_ERROR ((LM_ERROR, "Message is: %s\n", message));
        }
      ACE_ERROR ((LM_ERROR, "Caught DOM exception\n"));
      return;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
      return;
    }
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
