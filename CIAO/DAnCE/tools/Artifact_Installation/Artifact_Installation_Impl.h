// $Id$

/**
 * @file Artifact_Installation.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_ARTIFACT_INSTALLATION_IMPL_H
#define DANCE_ARTIFACT_INSTALLATION_IMPL_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#include "ace/Service_Object.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "DAnCE_Artifact_InstallationS.h"
#include "DAnCE_Artifact_Installation_export.h"
#include "DAnCE_Artifact_Installation_Handler.h"
#include <map>
#include <vector>
#include <stack>

namespace DAnCE
  {
    class ArtifactRegistry
      {
        public:
          struct Version
          {
            std::string protocol_;
            std::string location_;
            Version ();
            Version (const std::string& protocol,
                     const std::string& location);
            Version (const Version& version);
            Version& operator =(const Version& version);
          };
          typedef std::vector<Version> TVersions;

          ArtifactRegistry ();
          ArtifactRegistry (const ArtifactRegistry& ar);
          ~ArtifactRegistry ();

          ArtifactRegistry& operator =(const ArtifactRegistry& ar);

          const std::string& location () const;

          TVersions& versions ();
          const TVersions& versions () const;

        private:
          TVersions  versions_;
      };

    class DAnCE_Artifact_Installation_Export ArtifactInstallation_Impl
      : public POA_DAnCE::ArtifactInstallation
      {
        public:
          typedef ACE_Recursive_Thread_Mutex TLOCK;
          typedef ArtifactInstallationHandler::TPropertyMap TPropertyMap;

          ArtifactInstallation_Impl ();
          virtual ~ArtifactInstallation_Impl ();

          virtual void initialize ();

          virtual void clear ();

          virtual void install (const char * plan_uuid,
                const ::Deployment::ArtifactDeploymentDescription & artifact);

          virtual void remove (const char * plan_uuid,
                               const char * artifact_name);

          virtual void remove_all (const char * plan_uuid);

          virtual char * get_artifact_location (const char * uuid,
                                                const char * artifact_name);

        private:
          typedef std::stack<std::string> TProtocolStack;

          // key is artifact name
          typedef std::map<std::string,
                           ArtifactRegistry> TArtifactsMap;
          // key is plan uuid
          typedef std::map<std::string,
                           TArtifactsMap> TArtifactsRegistry;

          void parse_uri (const char* plan_uuid,
                          std::string& location,
                          TProtocolStack& protstack);

          void install_i (const std::string& plan_uuid,
                          const std::string& name,
                          TProtocolStack& protstack,
                          std::string& location,
                          const TPropertyMap& properties);

          void register_version (const std::string& plan_uuid,
                                 const std::string& name,
                                 const std::string& protocol,
                                 const std::string& location);

          void remove_i (const char * plan_uuid,
                         const char * artifact_name,
                         ArtifactRegistry& artifact_reg);

          void remove_intermediates (const std::string& plan_uuid,
                                     const std::string& artifact_name);

          void remove_artifact_registry (const std::string& plan_uuid,
                                         const std::string& name,
                                         ArtifactRegistry& artifact_reg);

          ArtifactRegistry& get_artifact_registry (const std::string& plan_uuid,
                                                   const std::string& name);

          void remove_artifacts_map (const std::string& plan_uuid,
                                     TArtifactsMap& artifacts_map);

          TArtifactsRegistry artifacts_;
          TLOCK artifact_lock_;

        public:

          static int register_handler (ArtifactInstallationHandler* aih);

          static int deregister_handler (ArtifactInstallationHandler* aih);

          typedef std::map<std::string,
                           ArtifactInstallationHandler*> THandlerMap;

        private:
          static THandlerMap handlers_;
          static TLOCK handler_lock_;
      };
  };

#if defined (__ACE_INLINE__)
#include "Artifact_Installation_Impl.inl"
#endif

#include /**/ "ace/post.h"
#endif /* DANCE_ARTIFACT_INSTALLATION_IMPL_H */