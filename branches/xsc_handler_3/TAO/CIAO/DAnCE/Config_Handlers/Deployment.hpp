// $Id$
#ifndef DEPLOYMENT_HPP
#define DEPLOYMENT_HPP

#include "Config_Handlers/Config_Handlers_Export.h"

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

#include "cpd.hpp"

#include "cdd.hpp"

#include "cdp.hpp"

#include "pcd.hpp"

#include "toplevel.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    ::CIAO::Config_Handlers::Domain Config_Handlers_Export
    domain (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::DeploymentPlan Config_Handlers_Export
    deploymentPlan (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ImplementationArtifactDescription Config_Handlers_Export
    implementationArtifactDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ComponentInterfaceDescription Config_Handlers_Export
    componentInterfaceDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ComponentImplementationDescription Config_Handlers_Export
    componentImplementationDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ComponentPackageDescription Config_Handlers_Export
    componentPackageDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::PackageConfiguration Config_Handlers_Export
    packageConfiguration (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::TopLevelPackageDescription Config_Handlers_Export
    topLevelPackageDescription (xercesc::DOMDocument const*);
  }
}

#include <XMLSchema/Traversal.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
    }
  }
}

#include <XMLSchema/Writer.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    void Config_Handlers_Export
    domain (::CIAO::Config_Handlers::Domain const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    deploymentPlan (::CIAO::Config_Handlers::DeploymentPlan const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    implementationArtifactDescription (::CIAO::Config_Handlers::ImplementationArtifactDescription const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    componentInterfaceDescription (::CIAO::Config_Handlers::ComponentInterfaceDescription const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    componentImplementationDescription (::CIAO::Config_Handlers::ComponentImplementationDescription const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    componentPackageDescription (::CIAO::Config_Handlers::ComponentPackageDescription const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    packageConfiguration (::CIAO::Config_Handlers::PackageConfiguration const&, xercesc::DOMDocument*);

    void Config_Handlers_Export
    topLevelPackageDescription (::CIAO::Config_Handlers::TopLevelPackageDescription const&, xercesc::DOMDocument*);
  }
}

#endif // DEPLOYMENT_HPP
