// $Id$

#include "File_Installation_Handler.h"
#include "Installation_Repository_Manager.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "ace/ACE.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_unistd.h"
#include <fstream>

namespace DAnCE
  {
    /*
     *  FileInstallationHandlerSvc
     */

    ACE_STATIC_SVC_DEFINE (FileInstallationHandlerSvc,
                           ACE_TEXT ("FileInstallationHandler"),
                           ACE_SVC_OBJ_T,
                           &ACE_SVC_NAME (FileInstallationHandlerSvc),
                           ACE_Service_Type::DELETE_THIS
                           | ACE_Service_Type::DELETE_OBJ,
                           0)
    ACE_FACTORY_DEFINE (Artifact_Installation_Handler, FileInstallationHandlerSvc)

    FileInstallationHandlerSvc::FileInstallationHandlerSvc ()
    {
    }

    FileInstallationHandlerSvc::~FileInstallationHandlerSvc ()
    {
      THandlerSingleton::close ();
    }

    ArtifactInstallationHandler* FileInstallationHandlerSvc::handler_instance ()
    {
      return THandlerSingleton::instance ();
    }

    int FileInstallationHandlerSvc::Initializer (void)
    {
      return ACE_Service_Config::process_directive (ace_svc_desc_FileInstallationHandlerSvc);
    }

    /*
     *  FileInstallationHandler
     */

    const std::string FileInstallationHandler::protocol ("file");

    FileInstallationHandler::FileInstallationHandler ()
    {
      DANCE_TRACE ( "FileInstallationHandler::FileInstallationHandler");
    }

    FileInstallationHandler::~FileInstallationHandler ()
    {
      DANCE_TRACE ( "FileInstallationHandler::~FileInstallationHandler");
    }

    const std::string& FileInstallationHandler::protocol_prefix (void)
    {
      return FileInstallationHandler::protocol;
    }

    void FileInstallationHandler::initialize ()
    {
    }

    void FileInstallationHandler::clear ()
    {
    }

    void FileInstallationHandler::install (const std::string& plan_uuid,
                                           std::string& location,
                                           const TPropertyMap& properties)
    {
      InstallationRepository::auto_ptr repo (
        InstallationRepositoryManager::instance ()->open_repository (plan_uuid));

      if (repo)
        {
          ArtifactInstallationProperties propreader (properties, this->protocol_prefix());

          ifstream* ifs = 0;
          ACE_NEW_NORETURN (ifs,
                            ifstream (location.c_str (),
                                      std::ios_base::in |
                                      std::ios_base::binary));
          ACE_Auto_Ptr<ifstream> pifs (ifs);
          if (pifs.get ()!=0 && (*pifs))
            {
              std::string path = propreader.get_property (
                  ArtifactInstallationProperties::PROP_NAME);
              if (path.empty ())
                path = this->basename (location);
              bool exclusive = false;
              const std::string& propval = propreader.get_property (
                  ArtifactInstallationProperties::PROP_EXCLUSIVE);
              exclusive = (!propval.empty () && (propval == "yes" || propval == "true"));
              ACE_Auto_Ptr<ostream> pofs (repo->create_artifact(path, exclusive));
              if (pofs.get ()!=0 && (*pofs))
                {
                  // copy artifact content
                  (*pofs) << pifs->rdbuf ();
                  // check state
                  if (!(*pofs))
                    {
                      std::string err = "failed to copy artifact from ";
                      err += location;
                      err += " to ";
                      err += path;
                      throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
                    }
                  location = path;
                }
              else
                {
                  std::string err = "failed to create repository artifact version for ";
                  err += path;
                  throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
                }
            }
          else
            {
              std::string err = "failed to open artifact location : ";
              err += location;
              throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
            }

        }
      else
        {
          throw Deployment::PlanError (plan_uuid.c_str (), "failed to open installation repository");
        }
    }

    void FileInstallationHandler::remove (const std::string& plan_uuid,
                                          const std::string& location)
    {
      InstallationRepository::auto_ptr repo (
        InstallationRepositoryManager::instance ()->open_repository (plan_uuid));

      if (repo)
        {
          if (repo->has_artifact (location))
            {
              if (!repo->remove_artifact (location))
                {
                  std::string err = "failed to remove artifact : ";
                  err += location;
                  throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
                }
            }
          else
            {
              std::string err = "cannot remove non-existing artifact : ";
              err += location;
              throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
            }
        }
      else
        {
          throw Deployment::PlanError (plan_uuid.c_str (), "failed to open installation repository");
        }
    }

  }
