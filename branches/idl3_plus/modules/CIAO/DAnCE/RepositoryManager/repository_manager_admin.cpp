// $Id$
#include "repository_manager_admin.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "RepositoryManagerDaemonC.h"

namespace DAnCE
{
  namespace RepositoryManager
  {
    Admin::Admin (Deployment::RepositoryManager_ptr rm)
      : rm_ (Deployment::RepositoryManager::_duplicate (rm))
    {
      DANCE_TRACE ("Admin::Admin");
    }
    

    /// Install a package at a provided filesystem path.
    bool
    Admin::install_package (const ACE_TCHAR *uri,
                            const ACE_TCHAR *name,
                            bool replace)
    {
      DANCE_TRACE ("Admin::install_package");
      
      try
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "Admin::install_package - "
                        "Installing package with URI: %C, name: %C\n",
                        uri, name));
          this->rm_->installPackage (name,
                                     uri,
                                     replace);
          DANCE_DEBUG ((LM_TRACE, DLINFO "Admin::install_package - "
                        "Package installed successfully\n"));
        }
      catch (Deployment::NameExists &)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::install_package - "
                        "Package with name %C already installed.\n",
                        name));
          return false;
        }
      catch (Deployment::PackageError &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::install_package - "
                        "Internal error while installing package with name %C: %C - %C\n",
                        name, ex.source.in (), ex.reason.in ()));
          return false;
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::install_package - "
                        "Unexpected CORBA Exception while installing package with name: %C.  Reason: %C\n",
                        name,
                        ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::install_package - "
                        "Unexpected C++ exception while installing package with name: %C\n",
                        name));
          return false;
        }
      
      return true;
    }
      
    /// Create new package.
    bool 
    Admin::create_package (const ACE_TCHAR * /* pc_path */,
                           const ACE_TCHAR * /* name */,
                           const ACE_TCHAR * /* baselocation */,
                           bool /* replace */)
    {
      DANCE_TRACE ("Admin::create_package");
      
      return false;
    }
      
    /// Uninstall a package with a provided UUID.
    /// Fails if the NoSuchName exception was raised.
    bool 
    Admin::uninstall_package (const ACE_TCHAR *uuid)
    {
      DANCE_TRACE ("Admin::uninstall_package");
      
      try
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "Admin::uninstall_package - "
                        "Attempting to uninstall package %s\n",
                        uuid));
          this->rm_->deletePackage (uuid);
          DANCE_DEBUG ((LM_INFO, DLINFO "Admin::uninstall_package - "
                        "Successfully uninstalled package %s\n",
                        uuid));
        }
      catch (Deployment::NoSuchName &)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::uninstall_package - "
                        "No package with the given UUID found: %C\n",
                        uuid));
          return false;
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::uninstall_package - "
                        "Unexpected CORBA Exception while uninstalling package with uuid: %C.  Reason: %C\n",
                        uuid,
                        ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::uninstall_package - "
                        "Unexpected C++ exception while installing package with uuid: %C\n",
                        uuid));
          return false;
        }
      
      return true;
    }
      
    /// List all installed packages
    ::CORBA::StringSeq * 
    Admin::list_packages (void)
    {
      DANCE_TRACE ("Admin::list_packages");
      
      try
        {
          CORBA::StringSeq_var packages = this->rm_->getAllNames ();
          return packages._retn ();
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::list_packages - "
                        "Unexpected CORBA Exception while listing packages: %C\n",
                        ex._info ().c_str ()));
          return 0;
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::list_package - "
                        "Unexpected C++ exception while listing packages\n"));
          return 0;
        }

      return 0;
    }
      
    /// List all installed package types
    ::CORBA::StringSeq * 
    Admin::list_types (void)
    {
      DANCE_TRACE ("Admin::list_types");
      
      try
        {
          CORBA::StringSeq_var packages = this->rm_->getAllTypes ();
          return packages._retn ();
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::list_types - "
                        "Unexpected CORBA Exception while listing package types: %C\n",
                        ex._info ().c_str ()));
          return 0;
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::list_types - "
                        "Unexpected C++ exception while listing package types\n"));
          return 0;
        }

      return 0;
    }
      
    /// Find package names by type
    ::CORBA::StringSeq *
    Admin::find_by_type (const ACE_TCHAR *type)
    {
      DANCE_TRACE ("Admin::find_by_type");
      
      if (type == 0)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::find_by_type - "
                        "Nill type passed to find_by_type\n"));
          return 0;
        }
      
      try
        {
          ::CORBA::StringSeq_var types = this->rm_->findNamesByType (type);
          types._retn ();
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::find_by_type - "
                        "Unexpected CORBA Exception while listing packages of type %C: %C\n",
                        type,
                        ex._info ().c_str ()));
          return 0;
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::find_by_type - "
                        "Unexpected C++ exception while listing packages by type %C\n",
                        type));
          return 0;
        }      
      
      return 0;
    }
      
    /// Attempt to shutdown the server.
    bool
    Admin::shutdown (void)
    {
      DANCE_TRACE ("Admin::shutdown");
      
      DANCE_DEBUG ((LM_TRACE, DLINFO "Admin::shutdown - "
                    "Attempting to shut down Repository Manager\n"));
      CIAO::RepositoryManagerDaemon_var rmd = 
        CIAO::RepositoryManagerDaemon::_narrow (this->rm_.in ());
      DANCE_DEBUG ((LM_INFO, DLINFO "Admin::shutdown - "
                    "Repository Manager shut down.\n"));      
      if (CORBA::is_nil (rmd.in ()))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "Admin::shutdown - "
                        "Unable to narrow provided RM reference to a CIAO::RepositoryManagerDaemon\n"));
          return false;
        }
      
      rmd->shutdown ();
      
      return true;
    }
  }
}
