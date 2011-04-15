// $Id$

/**
 * @file Artifact_Installation_Handler_Svc.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_ARTIFACT_INSTALLATION_HANDLER_SVC_H
#define DANCE_ARTIFACT_INSTALLATION_HANDLER_SVC_H

#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"
#include "DAnCE_Artifact_Installation_Handler.h"

#include "DAnCE_Artifact_Installation_export.h"

namespace DAnCE
  {
    class DAnCE_Artifact_Installation_Export ArtifactInstallationHandlerSvc
      : public ACE_Service_Object
      {
        public:
          ArtifactInstallationHandlerSvc ();
          virtual ~ArtifactInstallationHandlerSvc ();

          /// Initialization hook.
          virtual int init (int argc, ACE_TCHAR *argv[]);
          /// Deinitialization hook.
          virtual int fini ();

          virtual ArtifactInstallationHandler* handler_instance () = 0;
      };
  }

#include /**/ "ace/post.h"
#endif /* DANCE_ARTIFACT_INSTALLATION_HANDLER_SVC_H */
