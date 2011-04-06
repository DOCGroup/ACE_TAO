// $Id$

#include "Installation_Repository_Manager.h"
#include "dance/Logger/Log_Macros.h"

namespace DAnCE
{
  InstallationRepository::~InstallationRepository ()
  {
    DANCE_TRACE ("InstallationRepository::constructor");
  }

  /*
   *  InstallationRepositoryManager
   */

  InstallationRepositoryManager* InstallationRepositoryManager::instance_ = 0;

  InstallationRepositoryManager::~InstallationRepositoryManager ()
  {
    DANCE_TRACE ("InstallationRepository::destructor");
  }

  InstallationRepositoryManager* InstallationRepositoryManager::instance ()
  {
    DANCE_TRACE ("InstallationRepository::instance");
    return instance_;
  }

  int InstallationRepositoryManager::register_manager_instance (InstallationRepositoryManager* mgr)
  {
    DANCE_TRACE ("InstallationRepository::register_manager_instance");

    if (mgr != 0)
      {
        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
          DLINFO ACE_TEXT("InstallationRepositoryManager::register_manager_instance -")
          ACE_TEXT(" %@\n"),
          mgr));

        instance_ = mgr;
        return 0;
      }

    return -1;
  }

  int InstallationRepositoryManager::deregister_manager_instance (InstallationRepositoryManager* mgr)
  {
    DANCE_TRACE ("InstallationRepository::deregister_manager_instance");

    if (instance_ == mgr)
      {
        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
          DLINFO ACE_TEXT("InstallationRepositoryManager::deregister_manager_instance -")
          ACE_TEXT(" %@\n"),
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
    DANCE_TRACE ("InstallationRepositoryManagerSvc::constructor");
  }

  InstallationRepositoryManagerSvc::~InstallationRepositoryManagerSvc ()
  {
    DANCE_TRACE ("InstallationRepositoryManagerSvc::destructor");
  }

  /// Initialization hook.
  int InstallationRepositoryManagerSvc::init (int /*argc*/, ACE_TCHAR * /*argv*/[])
  {
    DANCE_TRACE ("InstallationRepositoryManagerSvc::init");
    return InstallationRepositoryManager::register_manager_instance (
      this->manager_instance ());
  }

  /// Deinitialization hook.
  int InstallationRepositoryManagerSvc::fini ()
  {
    DANCE_TRACE ("InstallationRepositoryManagerSvc::fini");
    return InstallationRepositoryManager::deregister_manager_instance (
      this->manager_instance ());
  }
}
