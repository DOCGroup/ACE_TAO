// $Id$
#ifndef DEPLOYMENT_HPP
#define DEPLOYMENT_HPP

// Forward declarations.
//
namespace Deployment
{
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

namespace Deployment
{
}

namespace Deployment
{
  ::Deployment::Domain
  domain (xercesc::DOMDocument const*);

  ::Deployment::DeploymentPlan
  deploymentPlan (xercesc::DOMDocument const*);

  ::Deployment::ImplementationArtifactDescription
  implementationArtifactDescription (xercesc::DOMDocument const*);

  ::Deployment::ComponentInterfaceDescription
  componentInterfaceDescription (xercesc::DOMDocument const*);

  ::Deployment::ComponentImplementationDescription
  componentImplementationDescription (xercesc::DOMDocument const*);

  ::Deployment::ComponentPackageDescription
  componentPackageDescription (xercesc::DOMDocument const*);

  ::Deployment::PackageConfiguration
  packageConfiguration (xercesc::DOMDocument const*);

  ::Deployment::TopLevelPackageDescription
  topLevelPackageDescription (xercesc::DOMDocument const*);
}

#include <XMLSchema/Traversal.hpp>

namespace Deployment
{
  namespace Traversal
  {
  }
}

#include <XMLSchema/Writer.hpp>

namespace Deployment
{
  namespace Writer
  {
  }
}

namespace Deployment
{
  void
  domain (::Deployment::Domain const&, xercesc::DOMDocument*);

  void
  deploymentPlan (::Deployment::DeploymentPlan const&, xercesc::DOMDocument*);

  void
  implementationArtifactDescription (::Deployment::ImplementationArtifactDescription const&, xercesc::DOMDocument*);

  void
  componentInterfaceDescription (::Deployment::ComponentInterfaceDescription const&, xercesc::DOMDocument*);

  void
  componentImplementationDescription (::Deployment::ComponentImplementationDescription const&, xercesc::DOMDocument*);

  void
  componentPackageDescription (::Deployment::ComponentPackageDescription const&, xercesc::DOMDocument*);

  void
  packageConfiguration (::Deployment::PackageConfiguration const&, xercesc::DOMDocument*);

  void
  topLevelPackageDescription (::Deployment::TopLevelPackageDescription const&, xercesc::DOMDocument*);
}

#endif // DEPLOYMENT_HPP
