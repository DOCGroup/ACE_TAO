
// $Id$

/**
 * @file repository_manager_admin.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>?
 *
 * Class which implements most repository admin functions.
 */

#ifndef REPOSITORY_MANAGER_ADMIN_H_
#define REPOSITORY_MANAGER_ADMIN_H_

#include /**/ "ace/pre.h"
#include "dance/Deployment/Deployment_RepositoryManagerC.h"

namespace DAnCE
{
  namespace RepositoryManager
  {
    /**
     * @class Admin
     * @brief Implements administrative functions for the Repository Manager.
     */
    class Admin
    {
    public:
      Admin (Deployment::RepositoryManager_ptr rm);

      /// Install a package at a provided filesystem path.
      bool install_package (const ACE_TCHAR *uri,
                            const ACE_TCHAR *name,
                            bool replace);

      /// Create new package.
      bool create_package (const ACE_TCHAR *pc_path,
                           const ACE_TCHAR *name,
                           const ACE_TCHAR *baselocation,
                           bool replace);

      /// Uninstall a package with a provided UUID.
      /// Fails if the NoSuchName exception was raised.
      bool uninstall_package (const ACE_TCHAR *uuid);

      /// List all installed packages
      ::CORBA::StringSeq * list_packages (void);

      /// List all installed package types
      ::CORBA::StringSeq * list_types (void);

      /// Find package names by type
      ::CORBA::StringSeq * find_by_type (const ACE_TCHAR *type);

      /// Attempt to shutdown the server.
      bool shutdown (void);

    private:
      Deployment::RepositoryManager_var rm_;
    };
  }
}

#include /**/ "ace/post.h"

#endif /* REPOSITORY_MANAGER_ADMIN_H_ */
