// $Id$
#include "Config_Handlers_Export.h"
#include "Deployment.hpp"

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
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    ::CIAO::Config_Handlers::Domain
    domain (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "domain")
      {
        ::CIAO::Config_Handlers::Domain r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::DeploymentPlan
    deploymentPlan (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "deploymentPlan")
      {
        ::CIAO::Config_Handlers::DeploymentPlan r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::ImplementationArtifactDescription
    implementationArtifactDescription (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "implementationArtifactDescription")
      {
        ::CIAO::Config_Handlers::ImplementationArtifactDescription r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::ComponentInterfaceDescription
    componentInterfaceDescription (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "componentInterfaceDescription")
      {
        ::CIAO::Config_Handlers::ComponentInterfaceDescription r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::ComponentImplementationDescription
    componentImplementationDescription (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "componentImplementationDescription")
      {
        ::CIAO::Config_Handlers::ComponentImplementationDescription r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::ComponentPackageDescription
    componentPackageDescription (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "componentPackageDescription")
      {
        ::CIAO::Config_Handlers::ComponentPackageDescription r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::PackageConfiguration
    packageConfiguration (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "packageConfiguration")
      {
        ::CIAO::Config_Handlers::PackageConfiguration r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }

    ::CIAO::Config_Handlers::TopLevelPackageDescription
    topLevelPackageDescription (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () == "topLevelPackageDescription")
      {
        ::CIAO::Config_Handlers::TopLevelPackageDescription r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }
  }
}

#include <XMLSchema/TypeInfo.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace
    {
      ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
    }
  }
}

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
    domain (::CIAO::Config_Handlers::Domain const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "domain")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::Domain,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::SharedResource,
      virtual ::CIAO::Config_Handlers::Writer::Node,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierProperty,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierPropertyKind,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    deploymentPlan (::CIAO::Config_Handlers::DeploymentPlan const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "deploymentPlan")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::DeploymentPlan,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentInterfaceDescription,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPortDescription,
      virtual ::CIAO::Config_Handlers::Writer::CCMComponentPortKind,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPropertyDescription,
      virtual ::CIAO::Config_Handlers::Writer::MonolithicDeploymentDescription,
      virtual ::XMLSchema::Writer::IDREF< char >, 
      virtual ::CIAO::Config_Handlers::Writer::Requirement,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::InstanceDeploymentDescription,
      virtual ::CIAO::Config_Handlers::Writer::InstanceResourceDeploymentDescription,
      virtual ::CIAO::Config_Handlers::Writer::ResourceUsageKind,
      virtual ::CIAO::Config_Handlers::Writer::PlanConnectionDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentExternalPortEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::PlanSubcomponentPortEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::ExternalReferenceEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::ConnectionResourceDeploymentDescription,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationDependency,
      virtual ::CIAO::Config_Handlers::Writer::ArtifactDeploymentDescription,
      virtual ::CIAO::Config_Handlers::Writer::ResourceDeploymentDescription,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    implementationArtifactDescription (::CIAO::Config_Handlers::ImplementationArtifactDescription const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "implementationArtifactDescription")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::ImplementationArtifactDescription,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::Requirement,
      virtual ::CIAO::Config_Handlers::Writer::NamedImplementationArtifact,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    componentInterfaceDescription (::CIAO::Config_Handlers::ComponentInterfaceDescription const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "componentInterfaceDescription")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::ComponentInterfaceDescription,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPortDescription,
      virtual ::CIAO::Config_Handlers::Writer::CCMComponentPortKind,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPropertyDescription,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    componentImplementationDescription (::CIAO::Config_Handlers::ComponentImplementationDescription const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "componentImplementationDescription")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::ComponentImplementationDescription,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentInterfaceDescription,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPortDescription,
      virtual ::CIAO::Config_Handlers::Writer::CCMComponentPortKind,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPropertyDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentAssemblyDescription,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentInstantiationDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageDescription,
      virtual ::CIAO::Config_Handlers::Writer::PackagedComponentImplementation,
      virtual ::CIAO::Config_Handlers::Writer::Requirement,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageReference,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyConnectionDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentExternalPortEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPortEndpoint,
      virtual ::XMLSchema::Writer::IDREF< char >, 
      virtual ::CIAO::Config_Handlers::Writer::ExternalReferenceEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyPropertyMapping,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPropertyReference,
      virtual ::CIAO::Config_Handlers::Writer::MonolithicImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::NamedImplementationArtifact,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationArtifactDescription,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationRequirement,
      virtual ::CIAO::Config_Handlers::Writer::ResourceUsageKind,
      virtual ::CIAO::Config_Handlers::Writer::Capability,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierProperty,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierPropertyKind,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationDependency,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    componentPackageDescription (::CIAO::Config_Handlers::ComponentPackageDescription const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "componentPackageDescription")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::ComponentPackageDescription,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentInterfaceDescription,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPortDescription,
      virtual ::CIAO::Config_Handlers::Writer::CCMComponentPortKind,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPropertyDescription,
      virtual ::CIAO::Config_Handlers::Writer::PackagedComponentImplementation,
      virtual ::CIAO::Config_Handlers::Writer::ComponentImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentAssemblyDescription,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentInstantiationDescription,
      virtual ::CIAO::Config_Handlers::Writer::Requirement,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageReference,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyConnectionDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentExternalPortEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPortEndpoint,
      virtual ::XMLSchema::Writer::IDREF< char >, 
      virtual ::CIAO::Config_Handlers::Writer::ExternalReferenceEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyPropertyMapping,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPropertyReference,
      virtual ::CIAO::Config_Handlers::Writer::MonolithicImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::NamedImplementationArtifact,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationArtifactDescription,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationRequirement,
      virtual ::CIAO::Config_Handlers::Writer::ResourceUsageKind,
      virtual ::CIAO::Config_Handlers::Writer::Capability,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierProperty,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierPropertyKind,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationDependency,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    packageConfiguration (::CIAO::Config_Handlers::PackageConfiguration const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "packageConfiguration")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::PackageConfiguration,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentInterfaceDescription,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPortDescription,
      virtual ::CIAO::Config_Handlers::Writer::CCMComponentPortKind,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPropertyDescription,
      virtual ::CIAO::Config_Handlers::Writer::PackagedComponentImplementation,
      virtual ::CIAO::Config_Handlers::Writer::ComponentImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentAssemblyDescription,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentInstantiationDescription,
      virtual ::CIAO::Config_Handlers::Writer::Requirement,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageReference,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyConnectionDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentExternalPortEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPortEndpoint,
      virtual ::XMLSchema::Writer::IDREF< char >, 
      virtual ::CIAO::Config_Handlers::Writer::ExternalReferenceEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyPropertyMapping,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPropertyReference,
      virtual ::CIAO::Config_Handlers::Writer::MonolithicImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::NamedImplementationArtifact,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationArtifactDescription,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationRequirement,
      virtual ::CIAO::Config_Handlers::Writer::ResourceUsageKind,
      virtual ::CIAO::Config_Handlers::Writer::Capability,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierProperty,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierPropertyKind,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationDependency,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }

    void
    topLevelPackageDescription (::CIAO::Config_Handlers::TopLevelPackageDescription const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
      if (e.name () != "topLevelPackageDescription")
      {
        throw 1;
      }

      struct W : virtual ::CIAO::Config_Handlers::Writer::TopLevelPackageDescription,
      virtual ::CIAO::Config_Handlers::Writer::PackageConfiguration,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentInterfaceDescription,
      virtual ::CIAO::Config_Handlers::Writer::Property,
      virtual ::CIAO::Config_Handlers::Writer::Any,
      virtual ::CIAO::Config_Handlers::Writer::DataType,
      virtual ::CIAO::Config_Handlers::Writer::TCKind,
      virtual ::CIAO::Config_Handlers::Writer::DataValue,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::short_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedShort, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedInt, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::float_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::boolean, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedByte, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::long_, char >,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::unsignedLong, char >,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPortDescription,
      virtual ::CIAO::Config_Handlers::Writer::CCMComponentPortKind,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPropertyDescription,
      virtual ::CIAO::Config_Handlers::Writer::PackagedComponentImplementation,
      virtual ::CIAO::Config_Handlers::Writer::ComponentImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentAssemblyDescription,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentInstantiationDescription,
      virtual ::CIAO::Config_Handlers::Writer::Requirement,
      virtual ::CIAO::Config_Handlers::Writer::ComponentPackageReference,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyConnectionDescription,
      virtual ::CIAO::Config_Handlers::Writer::ComponentExternalPortEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPortEndpoint,
      virtual ::XMLSchema::Writer::IDREF< char >, 
      virtual ::CIAO::Config_Handlers::Writer::ExternalReferenceEndpoint,
      virtual ::CIAO::Config_Handlers::Writer::AssemblyPropertyMapping,
      virtual ::CIAO::Config_Handlers::Writer::SubcomponentPropertyReference,
      virtual ::CIAO::Config_Handlers::Writer::MonolithicImplementationDescription,
      virtual ::CIAO::Config_Handlers::Writer::NamedImplementationArtifact,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationArtifactDescription,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationRequirement,
      virtual ::CIAO::Config_Handlers::Writer::ResourceUsageKind,
      virtual ::CIAO::Config_Handlers::Writer::Capability,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierProperty,
      virtual ::CIAO::Config_Handlers::Writer::SatisfierPropertyKind,
      virtual ::CIAO::Config_Handlers::Writer::ImplementationDependency,
      virtual ::XSCRT::Writer< char >
      {
        W (::XSCRT::XML::Element< char >& e)
        : ::XSCRT::Writer< char > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }
  }
}

