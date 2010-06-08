// $Id$

#include "Installation_Repository_Manager.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  /*
   *  InstallationRepositoryManager
   */

  InstallationRepositoryManager* InstallationRepositoryManager::instance_ = 0;

  InstallationRepositoryManager* InstallationRepositoryManager::instance ()
    {
      return instance_;
    }

  int InstallationRepositoryManager::register_manager_instance (InstallationRepositoryManager* mgr)
    {
      if (mgr != 0)
        {
          DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("InstallationRepositoryManager::register_manager_instance - ")
                           ACE_TEXT("%@\n"),
                           mgr));
          instance_ = mgr;
          return 0;
        }
      return -1;
    }

  int InstallationRepositoryManager::deregister_manager_instance (InstallationRepositoryManager* mgr)
    {
      if (instance_ == mgr)
        {
          DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("InstallationRepositoryManager::deregister_manager_instance - ")
                           ACE_TEXT("%@\n"),
                           mgr));
          instance_ = 0;
          return 0;
        }
      return -1;
    }

  /*
   *  InstallationRepositoryManagerSvc
   */

  InstallationRepositoryManagerSvc::InstallationRepositoryManagerSvc ()
    {
    }

  InstallationRepositoryManagerSvc::~InstallationRepositoryManagerSvc ()
    {
    }

  /// Initialization hook.
  int InstallationRepositoryManagerSvc::init (int /*argc*/, ACE_TCHAR * /*argv*/[])
    {
      DANCE_TRACE ( "InstallationRepositoryManagerSvc::init");
      return InstallationRepositoryManager::register_manager_instance (this->manager_instance ());
    }

  /// Deinitialization hook.
  int InstallationRepositoryManagerSvc::fini ()
    {
      DANCE_TRACE ( "InstallationRepositoryManagerSvc::fini");
      return InstallationRepositoryManager::deregister_manager_instance (this->manager_instance ());
    }
}
