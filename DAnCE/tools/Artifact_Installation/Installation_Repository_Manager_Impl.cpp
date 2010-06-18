// $Id$

#include "Installation_Repository_Manager_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Dirent.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"
#include <fstream>

namespace DAnCE
{
  /*
   *  InstallationRepositoryManagerSvc_Impl
   */

  ACE_STATIC_SVC_DEFINE (InstallationRepositoryManagerSvc_Impl,
                         ACE_TEXT ("InstallationRepositoryManager"),
                         ACE_SVC_OBJ_T,
                         &ACE_SVC_NAME (InstallationRepositoryManagerSvc_Impl),
                         ACE_Service_Type::DELETE_THIS
                          | ACE_Service_Type::DELETE_OBJ,
                         0)
  ACE_FACTORY_DEFINE (Installation_Repository_Manager, InstallationRepositoryManagerSvc_Impl)

  InstallationRepositoryManagerSvc_Impl::InstallationRepositoryManagerSvc_Impl ()
    {
    }

  InstallationRepositoryManagerSvc_Impl::~InstallationRepositoryManagerSvc_Impl ()
    {
      TManagerSingleton::close ();
    }

  InstallationRepositoryManager* InstallationRepositoryManagerSvc_Impl::manager_instance ()
    {
      return TManagerSingleton::instance ();
    }

  int InstallationRepositoryManagerSvc_Impl::Initializer (void)
  {
    return ACE_Service_Config::process_directive (ace_svc_desc_InstallationRepositoryManagerSvc_Impl);
  }

  /*
   *  InstallationRepositoryManager_Impl
   */

  const char * InstallationRepositoryManager_Impl::default_basedir = "./.dance_artifacts";
  const char * InstallationRepositoryManager_Impl::env_basedir_var = "DANCE_INSTALLATION_REPO_BASEDIR";

  InstallationRepositoryManager_Impl::InstallationRepositoryManager_Impl ()
    : InstallationRepository_Impl ()
    {
      char * env_basedir = ACE_OS::getenv (env_basedir_var);
      if (env_basedir)
        {
          this->location_ = env_basedir;
        }
      else
        {
          this->location_ = default_basedir;
        }
      if (ACE_OS::access (this->location_.c_str (), F_OK) != 0)
        {
          ACE_OS::mkdir (this->location_.c_str ());
        }
      char* rp = ACE_OS::realpath (this->location_.c_str (), 0);
      this->location_ = rp;
      ACE_OS::free (rp);
    }

  InstallationRepositoryManager_Impl::~InstallationRepositoryManager_Impl ()
    {
    }

  InstallationRepository::auto_ptr
  InstallationRepositoryManager_Impl::open_repository (const std::string& plan)
    {
      InstallationRepository::auto_ptr r_auto_ptr (0);
      std::string path = plan;
      if (!this->has_folder (path))
        {
          if (!this->create_folder (path))
            {
              return r_auto_ptr;
            }
        }
      else
        {
          path = this->normalize (path);
        }

      InstallationRepository* rptr = 0;
      ACE_NEW_NORETURN (rptr,
                        InstallationRepository_Impl (plan, path));
      if (rptr)
        {
          r_auto_ptr = InstallationRepository::auto_ptr (rptr);
        }
      return r_auto_ptr;
    }

  int InstallationRepositoryManager_Impl::remove_repository (const std::string& plan)
    {
      return this->remove_folder (plan);
    }

  InstallationRepositoryManager::TRepositoryIds
  InstallationRepositoryManager_Impl::repositories ()
    {
      return this->folders ();
    }

  /*
   *  InstallationRepository_Impl
   */

  InstallationRepository_Impl::~InstallationRepository_Impl ()
    {
    }

  InstallationRepository_Impl::InstallationRepository_Impl (
      const std::string& plan,
      const std::string& location)
    : location_ (location),
      plan_ (plan)
    {
    }

  InstallationRepository_Impl::InstallationRepository_Impl ()
    {
    }

  std::string InstallationRepository_Impl::normalize (const std::string& location)
    {
      // location must either be an absolute path with a base matching
      // the repo location or a relative path from the repo location
      // check and return an absolute path in either case
      if (location.find (this->location_) != 0)
        {
          // should be relative path
          // this will fail later on when the path is not a correct relative
          // path
          return this->make_full_path (location);
        }
      return location;
    }

  std::string InstallationRepository_Impl::make_full_path (const std::string& location)
    {
      return this->join (this->location_, location);
    }

  std::string InstallationRepository_Impl::join (const std::string& base,
                                                 const std::string& location)
    {
      return location.empty () ?
              base :
              base + ACE_DIRECTORY_SEPARATOR_STR_A + location;
    }

  std::string InstallationRepository_Impl::get_folder (const std::string& location)
    {
      std::string::size_type n = location.rfind (ACE_DIRECTORY_SEPARATOR_STR_A);
      if (n != std::string::npos)
        {
          return location.substr (0, n);
        }
      return std::string();
    }

  const std::string& InstallationRepository_Impl::plan ()
    {
      return this->plan_;
    }

  const std::string& InstallationRepository_Impl::location ()
    {
      return this->location_;
    }

  std::istream* InstallationRepository_Impl::open_artifact (const std::string& location)
    {
      ifstream* ifs = 0;
      // check that artifact actually exists
      std::string path = this->normalize (location);
      if (this->has_artifact (path))
        {
          ACE_NEW_RETURN (ifs,
                          ifstream (path.c_str (),
                                    std::ios_base::in |
                                        std::ios_base::binary),
                          0);
          if (!(*ifs))
            {
              delete ifs;
              return 0;
            }
        }
      return ifs;
    }

  std::ostream* InstallationRepository_Impl::create_artifact (std::string& location, bool exclusive)
    {
      std::string path = this->normalize (location);
      ofstream* ofs = 0;
      // check that no artifact or folder already exists with this path
      if (ACE_OS::access (path.c_str (), F_OK) != 0)
        {
          std::string folder = this->get_folder (path);

          if (!this->has_folder (folder))
            {
              if (!this->create_folder (folder))
                {
                  return ofs;
                }
            }
        }
      else
        {
          // if we want exclusiveness or its not an artifact (but a folder probably)
          // it's a nogo
          if (exclusive || !this->has_artifact (path))
            return ofs;
        }

      ACE_NEW_RETURN (ofs,
                      ofstream (path.c_str (),
                                std::ios_base::out |
                                  std::ios_base::binary),
                      0);
      if (!(*ofs))
        {
          delete ofs;
          return 0;
        }

      // everything succeeded
      location = path;
      return ofs;
    }

  bool InstallationRepository_Impl::remove_artifact (const std::string& location)
    {
      std::string path = this->normalize (location);
      return ACE_OS::unlink (path.c_str ()) == 0;
    }

  bool InstallationRepository_Impl::has_artifact (const std::string& location)
    {
      std::string path = this->normalize (location);
      ACE_stat stat;
      if (ACE_OS::stat (path.c_str (), &stat) == 0)
        {
          if ((stat.st_mode & S_IFMT) == S_IFREG)
            {
              return true;
            }
        }
      return false;
    }

  InstallationRepository::TLocations
  InstallationRepository_Impl::artifacts (const std::string& folder)
    {
      TLocations locs;

      std::string path = this->normalize (folder);
      ACE_Dirent dir (ACE_TEXT_CHAR_TO_TCHAR (path.c_str ()));
      ACE_DIRENT* dp = 0;
      while ((dp = dir.read ()) != 0)
        {
#if defined (ACE_HAS_TCHAR_DIRENT)
          std::string entry = this->join (path, ACE_TEXT_ALWAYS_CHAR (dp->d_name));
#else
          std::string entry = this->join (path, dp->d_name);
#endif
          if (this->has_artifact (entry))
            {
              locs.push_back (entry);
            }
        }

      return locs;
    }

  bool InstallationRepository_Impl::create_folder (std::string& location, bool exclusive)
    {
      std::string path = this->normalize (location);
      // check that no artifact or folder already exists with this path
      if (ACE_OS::access (path.c_str (), F_OK) != 0)
        {
          // check existence of parent folder
          std::string folder = this->get_folder (path);
          if (!this->has_folder (folder))
            {
              if (!this->create_folder (folder))
                {
                  return false;
                }
            }

          // create new folder
          if (ACE_OS::mkdir (path.c_str ()) != 0)
            {
              return false;
            }
        }
      else
        {
          // if we want exclusiveness or its not a folder (but an artifact probably)
          // it's a nogo
          if (exclusive || !this->has_folder (path))
            return false;

          // it's ok
        }

      // everything succeeded
      location = path;
      return true;
    }

  bool InstallationRepository_Impl::remove_folder (const std::string& location)
    {
      std::string path = this->normalize (location);
      if (this->has_folder (path))
        {
          TLocations locs = this->artifacts (path);
          for (TLocations::iterator itloc = locs.begin ();
              itloc != locs.end ();
              ++itloc)
            {
              if (!this->remove_artifact (*itloc))
                {
                  return false;
                }
            }

          locs = this->folders (path);
          for (TLocations::iterator itloc = locs.begin ();
              itloc != locs.end ();
              ++itloc)
            {
              if (!this->remove_folder (*itloc))
                {
                  return false;
                }
            }

          return ACE_OS::rmdir (path.c_str ()) == 0;
        }
      return false;
    }

  bool InstallationRepository_Impl::remove ()
    {
      return this->remove_folder (this->location_);
    }

  bool InstallationRepository_Impl::has_folder (const std::string& location)
    {
      std::string path = this->normalize (location);
      ACE_stat stat;
      if (ACE_OS::stat (path.c_str (), &stat) == 0)
        {
          if ((stat.st_mode & S_IFMT) == S_IFDIR)
            {
              return true;
            }
        }
      return false;
    }

  InstallationRepository::TLocations
  InstallationRepository_Impl::folders (const std::string& folder)
    {
      TLocations locs;

      std::string path = this->normalize (folder);
      ACE_Dirent dir (ACE_TEXT_CHAR_TO_TCHAR (path.c_str ()));
      ACE_DIRENT* dp = 0;
      while ((dp = dir.read ()) != 0)
        {
#if defined (ACE_HAS_TCHAR_DIRENT)
          std::string entry = this->join (path, ACE_TEXT_ALWAYS_CHAR (dp->d_name));
#else
          std::string entry = this->join (path, dp->d_name);
#endif
          if (this->has_folder (entry))
            {
              locs.push_back (entry);
            }
        }

      return locs;
    }

}
