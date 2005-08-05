// $Id$
#ifndef DEPLOYMENT_HPP
#define DEPLOYMENT_HPP

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
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

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
    ::CIAO::Config_Handlers::Domain
    domain (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::DeploymentPlan
    deploymentPlan (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ImplementationArtifactDescription
    implementationArtifactDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ComponentInterfaceDescription
    componentInterfaceDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ComponentImplementationDescription
    componentImplementationDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::ComponentPackageDescription
    componentPackageDescription (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::PackageConfiguration
    packageConfiguration (xercesc::DOMDocument const*);

    ::CIAO::Config_Handlers::TopLevelPackageDescription
    topLevelPackageDescription (xercesc::DOMDocument const*);
  }
}

#endif // DEPLOYMENT_HPP
