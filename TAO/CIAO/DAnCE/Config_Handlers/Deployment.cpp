// $Id$
#include "Deployment.hpp"

namespace Deployment
{
}

namespace Deployment
{
}

namespace Deployment
{
  ::Deployment::Domain
  domain (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "domain")
    {
      ::Deployment::Domain r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::DeploymentPlan
  deploymentPlan (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "deploymentPlan")
    {
      ::Deployment::DeploymentPlan r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::ImplementationArtifactDescription
  implementationArtifactDescription (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "implementationArtifactDescription")
    {
      ::Deployment::ImplementationArtifactDescription r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::ComponentInterfaceDescription
  componentInterfaceDescription (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "componentInterfaceDescription")
    {
      ::Deployment::ComponentInterfaceDescription r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::ComponentImplementationDescription
  componentImplementationDescription (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "componentImplementationDescription")
    {
      ::Deployment::ComponentImplementationDescription r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::ComponentPackageDescription
  componentPackageDescription (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "componentPackageDescription")
    {
      ::Deployment::ComponentPackageDescription r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::PackageConfiguration
  packageConfiguration (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "packageConfiguration")
    {
      ::Deployment::PackageConfiguration r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }

  ::Deployment::TopLevelPackageDescription
  topLevelPackageDescription (xercesc::DOMDocument const* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () == "topLevelPackageDescription")
    {
      ::Deployment::TopLevelPackageDescription r (e);
      return r;
    }

    else
    {
      throw 1;
    }
  }
}

#include <XMLSchema/TypeInfo.hpp>

namespace Deployment
{
  namespace
  {
    ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());
  }
}

namespace Deployment
{
  namespace Traversal
  {
  }
}

namespace Deployment
{
  namespace Writer
  {
  }
}

namespace Deployment
{
  void
  domain (::Deployment::Domain const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "domain")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::Domain,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::SharedResource,
    virtual ::Deployment::Writer::Node,
    virtual ::Deployment::Writer::SatisfierProperty,
    virtual ::Deployment::Writer::SatisfierPropertyKind,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::Property,
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
  deploymentPlan (::Deployment::DeploymentPlan const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "deploymentPlan")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::DeploymentPlan,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::ComponentInterfaceDescription,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::ComponentPortDescription,
    virtual ::Deployment::Writer::CCMComponentPortKind,
    virtual ::Deployment::Writer::ComponentPropertyDescription,
    virtual ::Deployment::Writer::MonolithicDeploymentDescription,
    virtual ::XMLSchema::Writer::IDREF< char >, 
    virtual ::Deployment::Writer::Requirement,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
    virtual ::Deployment::Writer::InstanceDeploymentDescription,
    virtual ::Deployment::Writer::InstanceResourceDeploymentDescription,
    virtual ::Deployment::Writer::ResourceUsageKind,
    virtual ::Deployment::Writer::PlanConnectionDescription,
    virtual ::Deployment::Writer::ComponentExternalPortEndpoint,
    virtual ::Deployment::Writer::PlanSubcomponentPortEndpoint,
    virtual ::Deployment::Writer::ExternalReferenceEndpoint,
    virtual ::Deployment::Writer::ConnectionResourceDeploymentDescription,
    virtual ::Deployment::Writer::ImplementationDependency,
    virtual ::Deployment::Writer::ArtifactDeploymentDescription,
    virtual ::Deployment::Writer::ResourceDeploymentDescription,
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
  implementationArtifactDescription (::Deployment::ImplementationArtifactDescription const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "implementationArtifactDescription")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::ImplementationArtifactDescription,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::Requirement,
    virtual ::Deployment::Writer::NamedImplementationArtifact,
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
  componentInterfaceDescription (::Deployment::ComponentInterfaceDescription const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "componentInterfaceDescription")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::ComponentInterfaceDescription,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::ComponentPortDescription,
    virtual ::Deployment::Writer::CCMComponentPortKind,
    virtual ::Deployment::Writer::ComponentPropertyDescription,
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
  componentImplementationDescription (::Deployment::ComponentImplementationDescription const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "componentImplementationDescription")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::ComponentImplementationDescription,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::ComponentInterfaceDescription,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::ComponentPortDescription,
    virtual ::Deployment::Writer::CCMComponentPortKind,
    virtual ::Deployment::Writer::ComponentPropertyDescription,
    virtual ::Deployment::Writer::ComponentAssemblyDescription,
    virtual ::Deployment::Writer::SubcomponentInstantiationDescription,
    virtual ::Deployment::Writer::ComponentPackageDescription,
    virtual ::Deployment::Writer::PackagedComponentImplementation,
    virtual ::Deployment::Writer::Requirement,
    virtual ::Deployment::Writer::ComponentPackageReference,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
    virtual ::Deployment::Writer::AssemblyConnectionDescription,
    virtual ::Deployment::Writer::ComponentExternalPortEndpoint,
    virtual ::Deployment::Writer::SubcomponentPortEndpoint,
    virtual ::XMLSchema::Writer::IDREF< char >, 
    virtual ::Deployment::Writer::ExternalReferenceEndpoint,
    virtual ::Deployment::Writer::AssemblyPropertyMapping,
    virtual ::Deployment::Writer::SubcomponentPropertyReference,
    virtual ::Deployment::Writer::MonolithicImplementationDescription,
    virtual ::Deployment::Writer::NamedImplementationArtifact,
    virtual ::Deployment::Writer::ImplementationArtifactDescription,
    virtual ::Deployment::Writer::ImplementationRequirement,
    virtual ::Deployment::Writer::ResourceUsageKind,
    virtual ::Deployment::Writer::Capability,
    virtual ::Deployment::Writer::SatisfierProperty,
    virtual ::Deployment::Writer::SatisfierPropertyKind,
    virtual ::Deployment::Writer::ImplementationDependency,
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
  componentPackageDescription (::Deployment::ComponentPackageDescription const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "componentPackageDescription")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::ComponentPackageDescription,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::ComponentInterfaceDescription,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::ComponentPortDescription,
    virtual ::Deployment::Writer::CCMComponentPortKind,
    virtual ::Deployment::Writer::ComponentPropertyDescription,
    virtual ::Deployment::Writer::PackagedComponentImplementation,
    virtual ::Deployment::Writer::ComponentImplementationDescription,
    virtual ::Deployment::Writer::ComponentAssemblyDescription,
    virtual ::Deployment::Writer::SubcomponentInstantiationDescription,
    virtual ::Deployment::Writer::Requirement,
    virtual ::Deployment::Writer::ComponentPackageReference,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
    virtual ::Deployment::Writer::AssemblyConnectionDescription,
    virtual ::Deployment::Writer::ComponentExternalPortEndpoint,
    virtual ::Deployment::Writer::SubcomponentPortEndpoint,
    virtual ::XMLSchema::Writer::IDREF< char >, 
    virtual ::Deployment::Writer::ExternalReferenceEndpoint,
    virtual ::Deployment::Writer::AssemblyPropertyMapping,
    virtual ::Deployment::Writer::SubcomponentPropertyReference,
    virtual ::Deployment::Writer::MonolithicImplementationDescription,
    virtual ::Deployment::Writer::NamedImplementationArtifact,
    virtual ::Deployment::Writer::ImplementationArtifactDescription,
    virtual ::Deployment::Writer::ImplementationRequirement,
    virtual ::Deployment::Writer::ResourceUsageKind,
    virtual ::Deployment::Writer::Capability,
    virtual ::Deployment::Writer::SatisfierProperty,
    virtual ::Deployment::Writer::SatisfierPropertyKind,
    virtual ::Deployment::Writer::ImplementationDependency,
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
  packageConfiguration (::Deployment::PackageConfiguration const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "packageConfiguration")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::PackageConfiguration,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::ComponentPackageDescription,
    virtual ::Deployment::Writer::ComponentInterfaceDescription,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::ComponentPortDescription,
    virtual ::Deployment::Writer::CCMComponentPortKind,
    virtual ::Deployment::Writer::ComponentPropertyDescription,
    virtual ::Deployment::Writer::PackagedComponentImplementation,
    virtual ::Deployment::Writer::ComponentImplementationDescription,
    virtual ::Deployment::Writer::ComponentAssemblyDescription,
    virtual ::Deployment::Writer::SubcomponentInstantiationDescription,
    virtual ::Deployment::Writer::Requirement,
    virtual ::Deployment::Writer::ComponentPackageReference,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
    virtual ::Deployment::Writer::AssemblyConnectionDescription,
    virtual ::Deployment::Writer::ComponentExternalPortEndpoint,
    virtual ::Deployment::Writer::SubcomponentPortEndpoint,
    virtual ::XMLSchema::Writer::IDREF< char >, 
    virtual ::Deployment::Writer::ExternalReferenceEndpoint,
    virtual ::Deployment::Writer::AssemblyPropertyMapping,
    virtual ::Deployment::Writer::SubcomponentPropertyReference,
    virtual ::Deployment::Writer::MonolithicImplementationDescription,
    virtual ::Deployment::Writer::NamedImplementationArtifact,
    virtual ::Deployment::Writer::ImplementationArtifactDescription,
    virtual ::Deployment::Writer::ImplementationRequirement,
    virtual ::Deployment::Writer::ResourceUsageKind,
    virtual ::Deployment::Writer::Capability,
    virtual ::Deployment::Writer::SatisfierProperty,
    virtual ::Deployment::Writer::SatisfierPropertyKind,
    virtual ::Deployment::Writer::ImplementationDependency,
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
  topLevelPackageDescription (::Deployment::TopLevelPackageDescription const& s, xercesc::DOMDocument* d)
  {
    ::XSCRT::XML::Element< char > e (d->getDocumentElement ());
    if (e.name () != "topLevelPackageDescription")
    {
      throw 1;
    }

    struct W : virtual ::Deployment::Writer::TopLevelPackageDescription,
    virtual ::Deployment::Writer::PackageConfiguration,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< char >, char >,
    virtual ::Deployment::Writer::ComponentPackageDescription,
    virtual ::Deployment::Writer::ComponentInterfaceDescription,
    virtual ::Deployment::Writer::Property,
    virtual ::Deployment::Writer::Any,
    virtual ::Deployment::Writer::DataType,
    virtual ::Deployment::Writer::TCKind,
    virtual ::Deployment::Writer::DataValue,
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
    virtual ::Deployment::Writer::ComponentPortDescription,
    virtual ::Deployment::Writer::CCMComponentPortKind,
    virtual ::Deployment::Writer::ComponentPropertyDescription,
    virtual ::Deployment::Writer::PackagedComponentImplementation,
    virtual ::Deployment::Writer::ComponentImplementationDescription,
    virtual ::Deployment::Writer::ComponentAssemblyDescription,
    virtual ::Deployment::Writer::SubcomponentInstantiationDescription,
    virtual ::Deployment::Writer::Requirement,
    virtual ::Deployment::Writer::ComponentPackageReference,
    virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::ID< char >, char >,
    virtual ::Deployment::Writer::AssemblyConnectionDescription,
    virtual ::Deployment::Writer::ComponentExternalPortEndpoint,
    virtual ::Deployment::Writer::SubcomponentPortEndpoint,
    virtual ::XMLSchema::Writer::IDREF< char >, 
    virtual ::Deployment::Writer::ExternalReferenceEndpoint,
    virtual ::Deployment::Writer::AssemblyPropertyMapping,
    virtual ::Deployment::Writer::SubcomponentPropertyReference,
    virtual ::Deployment::Writer::MonolithicImplementationDescription,
    virtual ::Deployment::Writer::NamedImplementationArtifact,
    virtual ::Deployment::Writer::ImplementationArtifactDescription,
    virtual ::Deployment::Writer::ImplementationRequirement,
    virtual ::Deployment::Writer::ResourceUsageKind,
    virtual ::Deployment::Writer::Capability,
    virtual ::Deployment::Writer::SatisfierProperty,
    virtual ::Deployment::Writer::SatisfierPropertyKind,
    virtual ::Deployment::Writer::ImplementationDependency,
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

