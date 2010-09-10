// $Id$

#include "Artifact_Installation_Handler_Svc.h"
#include "Artifact_Installation_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  /*
   *  ArtifactInstallationHandlerSvc
   */

  ArtifactInstallationHandlerSvc::ArtifactInstallationHandlerSvc ()
    {
    }

  ArtifactInstallationHandlerSvc::~ArtifactInstallationHandlerSvc ()
    {
    }

  /// Initialization hook.
  int ArtifactInstallationHandlerSvc::init (int /*argc*/, ACE_TCHAR * /*argv*/[])
    {
      DANCE_TRACE ( "ArtifactInstallationHandlerSvc::init");
      return ArtifactInstallation_Impl::register_handler (this->handler_instance ());
    }

  /// Deinitialization hook.
  int ArtifactInstallationHandlerSvc::fini ()
    {
      DANCE_TRACE ( "ArtifactInstallationHandlerSvc::fini");
      return ArtifactInstallation_Impl::deregister_handler (this->handler_instance ());
    }
}
