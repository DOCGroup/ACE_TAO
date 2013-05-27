// $Id$

/**
 * @file Installation_Repository_Manager_Impl.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_INSTALLATION_REPOSITORY_MANAGER_IMPL_H
#define DANCE_INSTALLATION_REPOSITORY_MANAGER_IMPL_H

#include /**/ "ace/pre.h"

#include "Installation_Repository_Manager.h"
#include "Installation_Repository_Manager_export.h"
#include "ace/Singleton.h"

namespace DAnCE
  {
    class Installation_Repository_Manager_Export InstallationRepository_Impl
      : public InstallationRepository
      {
        public:
          virtual ~InstallationRepository_Impl ();

          virtual const std::string& plan ();

          virtual const std::string& location ();

          virtual std::istream* open_artifact (const std::string& location);
          virtual std::ostream* create_artifact (std::string& location, bool exclusive = false);
          virtual bool remove_artifact (const std::string& location);

          virtual bool has_artifact (const std::string& location);

          virtual TLocations artifacts (const std::string& folder = std::string());

          virtual bool create_folder (std::string& location, bool exclusive = false);
          virtual bool remove_folder (const std::string& location);

          virtual bool has_folder (const std::string& location);

          virtual TLocations folders (const std::string& folder = std::string());

          virtual bool remove ();

        protected:
          std::string normalize (const std::string& location);
          std::string make_full_path (const std::string& location);
          std::string get_folder (const std::string& location);
          std::string join (const std::string& base, const std::string& location);

          InstallationRepository_Impl ();

          InstallationRepository_Impl (const std::string& plan,
                                       const std::string& location);

          friend class InstallationRepositoryManager_Impl;

          std::string location_;
          std::string plan_;
      };

    class Installation_Repository_Manager_Export InstallationRepositoryManager_Impl
      : public InstallationRepositoryManager,
        private InstallationRepository_Impl
      {
        public:
          InstallationRepositoryManager_Impl ();
          virtual ~InstallationRepositoryManager_Impl ();

          virtual InstallationRepository::auto_ptr open_repository (const std::string& plan);
          virtual int remove_repository (const std::string& plan);

          virtual TRepositoryIds repositories ();

        private:
          static const char * default_basedir;
          static const char * env_basedir_var;
      };

    class Installation_Repository_Manager_Export InstallationRepositoryManagerSvc_Impl
      : public InstallationRepositoryManagerSvc
      {
        public:
          InstallationRepositoryManagerSvc_Impl ();
          virtual ~InstallationRepositoryManagerSvc_Impl ();

          virtual InstallationRepositoryManager* manager_instance ();

          static int Initializer (void);
        private:
          typedef ACE_Unmanaged_Singleton<InstallationRepositoryManager_Impl,
                                          ACE_Null_Mutex> TManagerSingleton;
      };

    static int DAnCE_Requires_InstallationRepositoryManager_Service_Initializer =
      InstallationRepositoryManagerSvc_Impl::Initializer ();

    ACE_STATIC_SVC_DECLARE (InstallationRepositoryManagerSvc_Impl)
    ACE_FACTORY_DECLARE (Installation_Repository_Manager, InstallationRepositoryManagerSvc_Impl)
  }

#include /**/ "ace/post.h"
#endif /* DANCE_INSTALLATION_REPOSITORY_MANAGER_IMPL_H */
