/* -*- C++  -*- */
// $Id$

#include "RepositoryManager_Impl.h"
#include "Repository_Manager_conf.h"
#include "Config_Handlers/TPD_Handler.h"
#include "Config_Handlers/DnC_Dump.h"

CIAO::RepositoryManager_Impl::RepositoryManager_Impl
   (CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    pc_table_ (MAX_PACKAGES)
{
}

CIAO::RepositoryManager_Impl::RepositoryManager_Impl ()
  : pc_table_ (MAX_PACKAGES)
{
}

CIAO::RepositoryManager_Impl::~RepositoryManager_Impl ()
{
}

void
CIAO::RepositoryManager_Impl::
installPackage (const char* installation_name,
                const char* location
                ACE_ENV_ARG_DECL)
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
          throw Deployment::PackageError ();
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
          throw Deployment::PackageError ();
        }

      CIAO::Config_Handler::PC_Handler pc_handler (pc_doc,
                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                   DOMNodeFilter::SHOW_TEXT);
      Deployment::PackageConfiguration pc;
      pc_handler.process_PackageConfiguration (pc);
      //Deployment::DnC_Dump::dump (pc);
      this->pc_table_.bind (installation_name, (pc));
      //Deployment::DnC_Dump::dump (*(&pc));
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

void
CIAO::RepositoryManager_Impl::
createPackage (const char*,
               const Deployment::PackageConfiguration&,
               const char*,
               CORBA::Boolean
               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NameExists,
                   Deployment::PackageError))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

Deployment::PackageConfiguration*
CIAO::RepositoryManager_Impl::findPackageByName (const char* name
                                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
  Deployment::PackageConfiguration pc;
  if (pc_table_.find (name, pc) == 0)
    {
      //Deployment::PackageConfiguration* dup_pc = pc;
      //Deployment::DnC_Dump::dump (*pc);
      //return dup_pc;
      Deployment::PackageConfiguration_var pc_var = 0;
      ACE_NEW_THROW_EX (pc_var,
                        Deployment::PackageConfiguration (pc),
                        CORBA::NO_MEMORY ());
      //Deployment::DnC_Dump::dump (*pc_var);
      return pc_var._retn ();
    }
  else
    {
      ACE_THROW_RETURN (Deployment::NoSuchName (), 0);
    }
}

Deployment::PackageConfiguration*
CIAO::RepositoryManager_Impl::
findPackageByUUID (const char*
                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
findNamesByType (const char*
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllNames (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::StringSeq*
CIAO::RepositoryManager_Impl::
getAllTypes (ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
CIAO::RepositoryManager_Impl::
deletePackage (const char*
               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::NoSuchName))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
CIAO::RepositoryManager_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (1 ACE_ENV_SINGLE_ARG_PARAMETER);
}
