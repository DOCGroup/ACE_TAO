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
    void
    domain (::CIAO::Config_Handlers::Domain const&, xercesc::DOMDocument*);

    void
    deploymentPlan (::CIAO::Config_Handlers::DeploymentPlan const&, xercesc::DOMDocument*);

    void
    implementationArtifactDescription (::CIAO::Config_Handlers::ImplementationArtifactDescription const&, xercesc::DOMDocument*);

    void
    componentInterfaceDescription (::CIAO::Config_Handlers::ComponentInterfaceDescription const&, xercesc::DOMDocument*);

    void
    componentImplementationDescription (::CIAO::Config_Handlers::ComponentImplementationDescription const&, xercesc::DOMDocument*);

    void
    componentPackageDescription (::CIAO::Config_Handlers::ComponentPackageDescription const&, xercesc::DOMDocument*);

    void
    packageConfiguration (::CIAO::Config_Handlers::PackageConfiguration const&, xercesc::DOMDocument*);

    void
    topLevelPackageDescription (::CIAO::Config_Handlers::TopLevelPackageDescription const&, xercesc::DOMDocument*);
  }
}

#endif // DEPLOYMENT_HPP
