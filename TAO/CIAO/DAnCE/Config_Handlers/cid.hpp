// $Id$
#ifndef CID_HPP
#define CID_HPP

// Forward declarations.
//
namespace Deployment
{
  class SubcomponentInstantiationDescription;
  class SubcomponentPropertyReference;
  class AssemblyPropertyMapping;
  class ComponentAssemblyDescription;
  class MonolithicImplementationDescription;
  class ComponentImplementationDescription;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

#include "cpd.hpp"

#include "iad.hpp"

namespace Deployment
{
  class SubcomponentInstantiationDescription : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // name
    // 
    public:
    ::XMLSchema::string< char > const& name () const;
    ::XMLSchema::string< char >& name ();
    void name (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > name_;

    // package
    // 
    public:
    ::Deployment::ComponentPackageDescription const& package () const;
    ::Deployment::ComponentPackageDescription& package ();
    void package (::Deployment::ComponentPackageDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentPackageDescription > package_;

    // configProperty
    // 
    public:
    bool configProperty_p () const;
    ::Deployment::Property const& configProperty () const;
    ::Deployment::Property& configProperty ();
    void configProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > configProperty_;

    // selectRequirement
    // 
    public:
    bool selectRequirement_p () const;
    ::Deployment::Requirement const& selectRequirement () const;
    ::Deployment::Requirement& selectRequirement ();
    void selectRequirement (::Deployment::Requirement const& );

    protected:
    ::std::auto_ptr< ::Deployment::Requirement > selectRequirement_;

    // reference
    // 
    public:
    bool reference_p () const;
    ::Deployment::ComponentPackageReference const& reference () const;
    ::Deployment::ComponentPackageReference& reference ();
    void reference (::Deployment::ComponentPackageReference const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentPackageReference > reference_;

    // id
    // 
    public:
    bool id_p () const;
    ::XMLSchema::ID< char > const& id () const;
    ::XMLSchema::ID< char >& id ();
    void id (::XMLSchema::ID< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

    public:
    SubcomponentInstantiationDescription (::XMLSchema::string< char > const& name__,
    ::Deployment::ComponentPackageDescription const& package__);

    SubcomponentInstantiationDescription (::XSCRT::XML::Element< char > const&);
    SubcomponentInstantiationDescription (SubcomponentInstantiationDescription const& s);

    SubcomponentInstantiationDescription&
    operator= (SubcomponentInstantiationDescription const& s);

    private:
    char regulator__;
  };

  class SubcomponentPropertyReference : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // propertyName
    // 
    public:
    ::XMLSchema::string< char > const& propertyName () const;
    ::XMLSchema::string< char >& propertyName ();
    void propertyName (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > propertyName_;

    // instance
    // 
    public:
    ::Deployment::SubcomponentInstantiationDescription const& instance () const;
    ::Deployment::SubcomponentInstantiationDescription& instance ();
    void instance (::Deployment::SubcomponentInstantiationDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::SubcomponentInstantiationDescription > instance_;

    public:
    SubcomponentPropertyReference (::XMLSchema::string< char > const& propertyName__,
    ::Deployment::SubcomponentInstantiationDescription const& instance__);

    SubcomponentPropertyReference (::XSCRT::XML::Element< char > const&);
    SubcomponentPropertyReference (SubcomponentPropertyReference const& s);

    SubcomponentPropertyReference&
    operator= (SubcomponentPropertyReference const& s);

    private:
    char regulator__;
  };

  class AssemblyPropertyMapping : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // name
    // 
    public:
    ::XMLSchema::string< char > const& name () const;
    ::XMLSchema::string< char >& name ();
    void name (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > name_;

    // externalName
    // 
    public:
    ::XMLSchema::string< char > const& externalName () const;
    ::XMLSchema::string< char >& externalName ();
    void externalName (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > externalName_;

    // delegatesTo
    // 
    public:
    ::Deployment::SubcomponentPropertyReference const& delegatesTo () const;
    ::Deployment::SubcomponentPropertyReference& delegatesTo ();
    void delegatesTo (::Deployment::SubcomponentPropertyReference const& );

    protected:
    ::std::auto_ptr< ::Deployment::SubcomponentPropertyReference > delegatesTo_;

    public:
    AssemblyPropertyMapping (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& externalName__,
    ::Deployment::SubcomponentPropertyReference const& delegatesTo__);

    AssemblyPropertyMapping (::XSCRT::XML::Element< char > const&);
    AssemblyPropertyMapping (AssemblyPropertyMapping const& s);

    AssemblyPropertyMapping&
    operator= (AssemblyPropertyMapping const& s);

    private:
    char regulator__;
  };

  class ComponentAssemblyDescription : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // instance
    // 
    public:
    typedef ::std::vector< ::Deployment::SubcomponentInstantiationDescription >::iterator instance_iterator;
    typedef ::std::vector< ::Deployment::SubcomponentInstantiationDescription >::const_iterator instance_const_iterator;
    instance_iterator begin_instance ();
    instance_iterator end_instance ();
    instance_const_iterator begin_instance () const;
    instance_const_iterator end_instance () const;
    void add_instance (::Deployment::SubcomponentInstantiationDescription const& );

    protected:
    ::std::vector< ::Deployment::SubcomponentInstantiationDescription > instance_;

    // connection
    // 
    public:
    typedef ::std::vector< ::Deployment::AssemblyConnectionDescription >::iterator connection_iterator;
    typedef ::std::vector< ::Deployment::AssemblyConnectionDescription >::const_iterator connection_const_iterator;
    connection_iterator begin_connection ();
    connection_iterator end_connection ();
    connection_const_iterator begin_connection () const;
    connection_const_iterator end_connection () const;
    void add_connection (::Deployment::AssemblyConnectionDescription const& );

    protected:
    ::std::vector< ::Deployment::AssemblyConnectionDescription > connection_;

    // externalProperty
    // 
    public:
    bool externalProperty_p () const;
    ::Deployment::AssemblyPropertyMapping const& externalProperty () const;
    ::Deployment::AssemblyPropertyMapping& externalProperty ();
    void externalProperty (::Deployment::AssemblyPropertyMapping const& );

    protected:
    ::std::auto_ptr< ::Deployment::AssemblyPropertyMapping > externalProperty_;

    public:
    ComponentAssemblyDescription ();

    ComponentAssemblyDescription (::XSCRT::XML::Element< char > const&);
    ComponentAssemblyDescription (ComponentAssemblyDescription const& s);

    ComponentAssemblyDescription&
    operator= (ComponentAssemblyDescription const& s);

    private:
    char regulator__;
  };

  class MonolithicImplementationDescription : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // execParameter
    // 
    public:
    bool execParameter_p () const;
    ::Deployment::Property const& execParameter () const;
    ::Deployment::Property& execParameter ();
    void execParameter (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > execParameter_;

    // primaryArtifact
    // 
    public:
    typedef ::std::vector< ::Deployment::NamedImplementationArtifact >::iterator primaryArtifact_iterator;
    typedef ::std::vector< ::Deployment::NamedImplementationArtifact >::const_iterator primaryArtifact_const_iterator;
    primaryArtifact_iterator begin_primaryArtifact ();
    primaryArtifact_iterator end_primaryArtifact ();
    primaryArtifact_const_iterator begin_primaryArtifact () const;
    primaryArtifact_const_iterator end_primaryArtifact () const;
    void add_primaryArtifact (::Deployment::NamedImplementationArtifact const& );

    protected:
    ::std::vector< ::Deployment::NamedImplementationArtifact > primaryArtifact_;

    // deployRequirement
    // 
    public:
    bool deployRequirement_p () const;
    ::Deployment::ImplementationRequirement const& deployRequirement () const;
    ::Deployment::ImplementationRequirement& deployRequirement ();
    void deployRequirement (::Deployment::ImplementationRequirement const& );

    protected:
    ::std::auto_ptr< ::Deployment::ImplementationRequirement > deployRequirement_;

    public:
    MonolithicImplementationDescription ();

    MonolithicImplementationDescription (::XSCRT::XML::Element< char > const&);
    MonolithicImplementationDescription (MonolithicImplementationDescription const& s);

    MonolithicImplementationDescription&
    operator= (MonolithicImplementationDescription const& s);

    private:
    char regulator__;
  };

  class ComponentImplementationDescription : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // label
    // 
    public:
    bool label_p () const;
    ::XMLSchema::string< char > const& label () const;
    ::XMLSchema::string< char >& label ();
    void label (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > label_;

    // UUID
    // 
    public:
    bool UUID_p () const;
    ::XMLSchema::string< char > const& UUID () const;
    ::XMLSchema::string< char >& UUID ();
    void UUID (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > UUID_;

    // implements
    // 
    public:
    bool implements_p () const;
    ::Deployment::ComponentInterfaceDescription const& implements () const;
    ::Deployment::ComponentInterfaceDescription& implements ();
    void implements (::Deployment::ComponentInterfaceDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentInterfaceDescription > implements_;

    // assemblyImpl
    // 
    public:
    bool assemblyImpl_p () const;
    ::Deployment::ComponentAssemblyDescription const& assemblyImpl () const;
    ::Deployment::ComponentAssemblyDescription& assemblyImpl ();
    void assemblyImpl (::Deployment::ComponentAssemblyDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentAssemblyDescription > assemblyImpl_;

    // monolithicImpl
    // 
    public:
    bool monolithicImpl_p () const;
    ::Deployment::MonolithicImplementationDescription const& monolithicImpl () const;
    ::Deployment::MonolithicImplementationDescription& monolithicImpl ();
    void monolithicImpl (::Deployment::MonolithicImplementationDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::MonolithicImplementationDescription > monolithicImpl_;

    // configProperty
    // 
    public:
    bool configProperty_p () const;
    ::Deployment::Property const& configProperty () const;
    ::Deployment::Property& configProperty ();
    void configProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > configProperty_;

    // capability
    // 
    public:
    bool capability_p () const;
    ::Deployment::Capability const& capability () const;
    ::Deployment::Capability& capability ();
    void capability (::Deployment::Capability const& );

    protected:
    ::std::auto_ptr< ::Deployment::Capability > capability_;

    // dependsOn
    // 
    public:
    bool dependsOn_p () const;
    ::Deployment::ImplementationDependency const& dependsOn () const;
    ::Deployment::ImplementationDependency& dependsOn ();
    void dependsOn (::Deployment::ImplementationDependency const& );

    protected:
    ::std::auto_ptr< ::Deployment::ImplementationDependency > dependsOn_;

    // infoProperty
    // 
    public:
    bool infoProperty_p () const;
    ::Deployment::Property const& infoProperty () const;
    ::Deployment::Property& infoProperty ();
    void infoProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > infoProperty_;

    // contentLocation
    // 
    public:
    bool contentLocation_p () const;
    ::XMLSchema::string< char > const& contentLocation () const;
    ::XMLSchema::string< char >& contentLocation ();
    void contentLocation (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > contentLocation_;

    public:
    ComponentImplementationDescription ();

    ComponentImplementationDescription (::XSCRT::XML::Element< char > const&);
    ComponentImplementationDescription (ComponentImplementationDescription const& s);

    ComponentImplementationDescription&
    operator= (ComponentImplementationDescription const& s);

    private:
    char regulator__;
  };
}

namespace Deployment
{
}

#include <XMLSchema/Traversal.hpp>

namespace Deployment
{
  namespace Traversal
  {
    struct SubcomponentInstantiationDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::SubcomponentInstantiationDescription >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      name (Type&);

      virtual void
      name (Type const&);

      virtual void
      package (Type&);

      virtual void
      package (Type const&);

      virtual void
      configProperty (Type&);

      virtual void
      configProperty (Type const&);

      virtual void
      configProperty_none (Type&);

      virtual void
      configProperty_none (Type const&);

      virtual void
      selectRequirement (Type&);

      virtual void
      selectRequirement (Type const&);

      virtual void
      selectRequirement_none (Type&);

      virtual void
      selectRequirement_none (Type const&);

      virtual void
      reference (Type&);

      virtual void
      reference (Type const&);

      virtual void
      reference_none (Type&);

      virtual void
      reference_none (Type const&);

      virtual void
      id (Type&);

      virtual void
      id (Type const&);

      virtual void
      id_none (Type&);

      virtual void
      id_none (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct SubcomponentPropertyReference : ::XMLSchema::Traversal::Traverser< ::Deployment::SubcomponentPropertyReference >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      propertyName (Type&);

      virtual void
      propertyName (Type const&);

      virtual void
      instance (Type&);

      virtual void
      instance (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct AssemblyPropertyMapping : ::XMLSchema::Traversal::Traverser< ::Deployment::AssemblyPropertyMapping >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      name (Type&);

      virtual void
      name (Type const&);

      virtual void
      externalName (Type&);

      virtual void
      externalName (Type const&);

      virtual void
      delegatesTo (Type&);

      virtual void
      delegatesTo (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct ComponentAssemblyDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::ComponentAssemblyDescription >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      instance (Type&);

      virtual void
      instance (Type const&);

      virtual void
      instance_pre (Type&);

      virtual void
      instance_pre (Type const&);

      virtual void
      instance_next (Type&);

      virtual void
      instance_next (Type const&);

      virtual void
      instance_post (Type&);

      virtual void
      instance_post (Type const&);

      virtual void
      connection (Type&);

      virtual void
      connection (Type const&);

      virtual void
      connection_pre (Type&);

      virtual void
      connection_pre (Type const&);

      virtual void
      connection_next (Type&);

      virtual void
      connection_next (Type const&);

      virtual void
      connection_post (Type&);

      virtual void
      connection_post (Type const&);

      virtual void
      externalProperty (Type&);

      virtual void
      externalProperty (Type const&);

      virtual void
      externalProperty_none (Type&);

      virtual void
      externalProperty_none (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct MonolithicImplementationDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::MonolithicImplementationDescription >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      execParameter (Type&);

      virtual void
      execParameter (Type const&);

      virtual void
      execParameter_none (Type&);

      virtual void
      execParameter_none (Type const&);

      virtual void
      primaryArtifact (Type&);

      virtual void
      primaryArtifact (Type const&);

      virtual void
      primaryArtifact_pre (Type&);

      virtual void
      primaryArtifact_pre (Type const&);

      virtual void
      primaryArtifact_next (Type&);

      virtual void
      primaryArtifact_next (Type const&);

      virtual void
      primaryArtifact_post (Type&);

      virtual void
      primaryArtifact_post (Type const&);

      virtual void
      deployRequirement (Type&);

      virtual void
      deployRequirement (Type const&);

      virtual void
      deployRequirement_none (Type&);

      virtual void
      deployRequirement_none (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct ComponentImplementationDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::ComponentImplementationDescription >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      label (Type&);

      virtual void
      label (Type const&);

      virtual void
      label_none (Type&);

      virtual void
      label_none (Type const&);

      virtual void
      UUID (Type&);

      virtual void
      UUID (Type const&);

      virtual void
      UUID_none (Type&);

      virtual void
      UUID_none (Type const&);

      virtual void
      implements (Type&);

      virtual void
      implements (Type const&);

      virtual void
      implements_none (Type&);

      virtual void
      implements_none (Type const&);

      virtual void
      assemblyImpl (Type&);

      virtual void
      assemblyImpl (Type const&);

      virtual void
      assemblyImpl_none (Type&);

      virtual void
      assemblyImpl_none (Type const&);

      virtual void
      monolithicImpl (Type&);

      virtual void
      monolithicImpl (Type const&);

      virtual void
      monolithicImpl_none (Type&);

      virtual void
      monolithicImpl_none (Type const&);

      virtual void
      configProperty (Type&);

      virtual void
      configProperty (Type const&);

      virtual void
      configProperty_none (Type&);

      virtual void
      configProperty_none (Type const&);

      virtual void
      capability (Type&);

      virtual void
      capability (Type const&);

      virtual void
      capability_none (Type&);

      virtual void
      capability_none (Type const&);

      virtual void
      dependsOn (Type&);

      virtual void
      dependsOn (Type const&);

      virtual void
      dependsOn_none (Type&);

      virtual void
      dependsOn_none (Type const&);

      virtual void
      infoProperty (Type&);

      virtual void
      infoProperty (Type const&);

      virtual void
      infoProperty_none (Type&);

      virtual void
      infoProperty_none (Type const&);

      virtual void
      contentLocation (Type&);

      virtual void
      contentLocation (Type const&);

      virtual void
      contentLocation_none (Type&);

      virtual void
      contentLocation_none (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };
  }
}

#include <XMLSchema/Writer.hpp>

namespace Deployment
{
  namespace Writer
  {
    struct SubcomponentInstantiationDescription : Traversal::SubcomponentInstantiationDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::SubcomponentInstantiationDescription Type;
      SubcomponentInstantiationDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      name (Type const&);

      virtual void
      package (Type const&);

      virtual void
      configProperty (Type const&);

      virtual void
      selectRequirement (Type const&);

      virtual void
      reference (Type const&);

      virtual void
      id (Type const&);

      protected:
      SubcomponentInstantiationDescription ();
    };

    struct SubcomponentPropertyReference : Traversal::SubcomponentPropertyReference, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::SubcomponentPropertyReference Type;
      SubcomponentPropertyReference (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      propertyName (Type const&);

      virtual void
      instance (Type const&);

      protected:
      SubcomponentPropertyReference ();
    };

    struct AssemblyPropertyMapping : Traversal::AssemblyPropertyMapping, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::AssemblyPropertyMapping Type;
      AssemblyPropertyMapping (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      name (Type const&);

      virtual void
      externalName (Type const&);

      virtual void
      delegatesTo (Type const&);

      protected:
      AssemblyPropertyMapping ();
    };

    struct ComponentAssemblyDescription : Traversal::ComponentAssemblyDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::ComponentAssemblyDescription Type;
      ComponentAssemblyDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      instance_pre (Type const&);

      virtual void
      instance_next (Type const&);

      virtual void
      instance_post (Type const&);

      virtual void
      connection_pre (Type const&);

      virtual void
      connection_next (Type const&);

      virtual void
      connection_post (Type const&);

      virtual void
      externalProperty (Type const&);

      protected:
      ComponentAssemblyDescription ();
    };

    struct MonolithicImplementationDescription : Traversal::MonolithicImplementationDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::MonolithicImplementationDescription Type;
      MonolithicImplementationDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      execParameter (Type const&);

      virtual void
      primaryArtifact_pre (Type const&);

      virtual void
      primaryArtifact_next (Type const&);

      virtual void
      primaryArtifact_post (Type const&);

      virtual void
      deployRequirement (Type const&);

      protected:
      MonolithicImplementationDescription ();
    };

    struct ComponentImplementationDescription : Traversal::ComponentImplementationDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::ComponentImplementationDescription Type;
      ComponentImplementationDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      label (Type const&);

      virtual void
      UUID (Type const&);

      virtual void
      implements (Type const&);

      virtual void
      assemblyImpl (Type const&);

      virtual void
      monolithicImpl (Type const&);

      virtual void
      configProperty (Type const&);

      virtual void
      capability (Type const&);

      virtual void
      dependsOn (Type const&);

      virtual void
      infoProperty (Type const&);

      virtual void
      contentLocation (Type const&);

      protected:
      ComponentImplementationDescription ();
    };
  }
}

namespace Deployment
{
}

#endif // CID_HPP
