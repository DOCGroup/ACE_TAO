// $Id$

/**
 * @file Installation_Repository_Manager.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_INSTALLATION_REPOSITORY_MANAGER_H
#define DANCE_INSTALLATION_REPOSITORY_MANAGER_H

#include /**/ "ace/pre.h"

#include "DAnCE_Artifact_Installation_export.h"
#include "ace/Service_Object.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Null_Mutex.h"
#include <string>
#include <istream>
#include <ostream>
#include <vector>

namespace DAnCE
  {
    class DAnCE_Artifact_Installation_Export InstallationRepository
      {
        public:
          typedef ACE_Refcounted_Auto_Ptr<InstallationRepository,
                                          ACE_Null_Mutex> auto_ptr;
          typedef std::vector<std::string> TLocations;

          virtual ~InstallationRepository ();

          virtual const std::string& plan () = 0;

          virtual const std::string& location () = 0;

          virtual std::istream* open_artifact (const std::string& location) = 0;
          virtual std::ostream* create_artifact (std::string& location, bool exclusive = false) = 0;
          virtual bool remove_artifact (const std::string& location) = 0;

          virtual bool has_artifact (const std::string& location) = 0;

          virtual TLocations artifacts (const std::string& folder = std::string()) = 0;

          virtual bool create_folder (std::string& location, bool exclusive = false) = 0;
          virtual bool remove_folder (const std::string& location) = 0;

          virtual bool has_folder (const std::string& location) = 0;

          virtual TLocations folders (const std::string& folder = std::string()) = 0;

          virtual bool remove () = 0;
      };

    class DAnCE_Artifact_Installation_Export InstallationRepositoryManager
      {
        public:
          typedef InstallationRepository::TLocations TRepositoryIds;

          virtual ~InstallationRepositoryManager ();

          virtual InstallationRepository::auto_ptr open_repository (const std::string& plan) = 0;
          virtual int remove_repository (const std::string& plan) = 0;

          virtual TRepositoryIds repositories () = 0;

        public:
          static InstallationRepositoryManager* instance ();

          static int register_manager_instance (InstallationRepositoryManager* mgr);
          static int deregister_manager_instance (InstallationRepositoryManager* mgr);

        private:
          static InstallationRepositoryManager* instance_;
      };

    class DAnCE_Artifact_Installation_Export InstallationRepositoryManagerSvc
      : public ACE_Service_Object
      {
        public:
          InstallationRepositoryManagerSvc ();
          virtual ~InstallationRepositoryManagerSvc ();

          /// Initialization hook.
          virtual int init (int argc, ACE_TCHAR *argv[]);
          /// Deinitialization hook.
          virtual int fini ();

          virtual InstallationRepositoryManager* manager_instance () = 0;
      };
  }

#include /**/ "ace/post.h"
#endif /* DANCE_INSTALLATION_REPOSITORY_MANAGER_H */
