// $Id$

/**
 * @file Artifact_Installation_Handler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_ARTIFACT_INSTALLATION_HANDLER_H
#define DANCE_ARTIFACT_INSTALLATION_HANDLER_H

#include /**/ "ace/pre.h"

#include "DAnCE_Artifact_Installation_HandlerC.h"
#include "DAnCE_Artifact_Installation_export.h"
#include <map>
#include <string>

namespace DAnCE
  {
    class DAnCE_Artifact_Installation_Export ArtifactInstallationHandler
      {
        public:
          typedef std::map<std::string,
                           std::string> TPropertyMap;

          virtual const std::string& protocol_prefix (void) = 0;

          virtual void initialize () = 0;

          virtual void clear () = 0;

          virtual void install (const std::string& plan_uuid,
                                std::string& location,
                                const TPropertyMap& properties) = 0;

          virtual void remove (const std::string& plan_uuid,
                               const std::string& location) = 0;

        protected:
          std::string basename (const std::string& location, char dirsep = ACE_DIRECTORY_SEPARATOR_CHAR_A);
      };

    class DAnCE_Artifact_Installation_Export ArtifactInstallationProperties
      {
        public:
          typedef ArtifactInstallationHandler::TPropertyMap TPropertyMap;

          ArtifactInstallationProperties (const TPropertyMap& propmap);
          ArtifactInstallationProperties (const TPropertyMap& propmap, const std::string& protocol);

          static const std::string PROPERTY_BASE; // edu.vanderbilt.dre.DAnCE.artifact.
          static const std::string PROP_KEEP_INTERMEDIATES;
          static const std::string PROP_NAME;
          static const std::string PROP_PATH;
          static const std::string PROP_EXCLUSIVE;

          bool has_property (const std::string& propname);
          const std::string& get_property (const std::string& propname);

        private:
          static const std::string empty_;

          const TPropertyMap& propmap_;
          std::string protocol_;
      };
  };

#if defined (__ACE_INLINE__)
#include "DAnCE_Artifact_Installation_Handler.inl"
#endif

#include /**/ "ace/post.h"
#endif /* DANCE_ARTIFACT_INSTALLATION_HANDLER_H */
