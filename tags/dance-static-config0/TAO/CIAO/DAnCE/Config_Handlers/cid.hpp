// $Id$
#ifndef CID_HPP
#define CID_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class SubcomponentInstantiationDescription;
    class SubcomponentPropertyReference;
    class AssemblyPropertyMapping;
    class ComponentAssemblyDescription;
    class MonolithicImplementationDescription;
    class ComponentImplementationDescription;
  }
}

#include <memory>
#include <vector>
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

#include "Basic_Deployment_Data.hpp"

#include "cpd.hpp"

#include "iad.hpp"

namespace CIAO
{
  namespace Config_Handlers
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
      ::CIAO::Config_Handlers::ComponentPackageDescription const& package () const;
      ::CIAO::Config_Handlers::ComponentPackageDescription& package ();
      void package (::CIAO::Config_Handlers::ComponentPackageDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageDescription > package_;

      // configProperty
      //
      public:
      bool configProperty_p () const;
      ::CIAO::Config_Handlers::Property const& configProperty () const;
      ::CIAO::Config_Handlers::Property& configProperty ();
      void configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > configProperty_;

      // selectRequirement
      //
      public:
      bool selectRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& selectRequirement () const;
      ::CIAO::Config_Handlers::Requirement& selectRequirement ();
      void selectRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > selectRequirement_;

      // reference
      //
      public:
      bool reference_p () const;
      ::CIAO::Config_Handlers::ComponentPackageReference const& reference () const;
      ::CIAO::Config_Handlers::ComponentPackageReference& reference ();
      void reference (::CIAO::Config_Handlers::ComponentPackageReference const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageReference > reference_;

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
      SubcomponentInstantiationDescription ();
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
      ::CIAO::Config_Handlers::SubcomponentInstantiationDescription const& instance () const;
      ::CIAO::Config_Handlers::SubcomponentInstantiationDescription& instance ();
      void instance (::CIAO::Config_Handlers::SubcomponentInstantiationDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SubcomponentInstantiationDescription > instance_;

      public:
      SubcomponentPropertyReference ();
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
      ::CIAO::Config_Handlers::SubcomponentPropertyReference const& delegatesTo () const;
      ::CIAO::Config_Handlers::SubcomponentPropertyReference& delegatesTo ();
      void delegatesTo (::CIAO::Config_Handlers::SubcomponentPropertyReference const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SubcomponentPropertyReference > delegatesTo_;

      public:
      AssemblyPropertyMapping ();
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
      typedef ::std::vector< ::CIAO::Config_Handlers::SubcomponentInstantiationDescription >::iterator instance_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::SubcomponentInstantiationDescription >::const_iterator instance_const_iterator;
      instance_iterator begin_instance ();
      instance_iterator end_instance ();
      instance_const_iterator begin_instance () const;
      instance_const_iterator end_instance () const;
      void add_instance (::CIAO::Config_Handlers::SubcomponentInstantiationDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::SubcomponentInstantiationDescription > instance_;

      // connection
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::AssemblyConnectionDescription >::iterator connection_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::AssemblyConnectionDescription >::const_iterator connection_const_iterator;
      connection_iterator begin_connection ();
      connection_iterator end_connection ();
      connection_const_iterator begin_connection () const;
      connection_const_iterator end_connection () const;
      void add_connection (::CIAO::Config_Handlers::AssemblyConnectionDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::AssemblyConnectionDescription > connection_;

      // externalProperty
      //
      public:
      bool externalProperty_p () const;
      ::CIAO::Config_Handlers::AssemblyPropertyMapping const& externalProperty () const;
      ::CIAO::Config_Handlers::AssemblyPropertyMapping& externalProperty ();
      void externalProperty (::CIAO::Config_Handlers::AssemblyPropertyMapping const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::AssemblyPropertyMapping > externalProperty_;

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
      ::CIAO::Config_Handlers::Property const& execParameter () const;
      ::CIAO::Config_Handlers::Property& execParameter ();
      void execParameter (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > execParameter_;

      // primaryArtifact
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact >::iterator primaryArtifact_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact >::const_iterator primaryArtifact_const_iterator;
      primaryArtifact_iterator begin_primaryArtifact ();
      primaryArtifact_iterator end_primaryArtifact ();
      primaryArtifact_const_iterator begin_primaryArtifact () const;
      primaryArtifact_const_iterator end_primaryArtifact () const;
      void add_primaryArtifact (::CIAO::Config_Handlers::NamedImplementationArtifact const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact > primaryArtifact_;

      // deployRequirement
      //
      public:
      bool deployRequirement_p () const;
      ::CIAO::Config_Handlers::ImplementationRequirement const& deployRequirement () const;
      ::CIAO::Config_Handlers::ImplementationRequirement& deployRequirement ();
      void deployRequirement (::CIAO::Config_Handlers::ImplementationRequirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ImplementationRequirement > deployRequirement_;

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
      ::CIAO::Config_Handlers::ComponentInterfaceDescription const& implements () const;
      ::CIAO::Config_Handlers::ComponentInterfaceDescription& implements ();
      void implements (::CIAO::Config_Handlers::ComponentInterfaceDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentInterfaceDescription > implements_;

      // assemblyImpl
      //
      public:
      bool assemblyImpl_p () const;
      ::CIAO::Config_Handlers::ComponentAssemblyDescription const& assemblyImpl () const;
      ::CIAO::Config_Handlers::ComponentAssemblyDescription& assemblyImpl ();
      void assemblyImpl (::CIAO::Config_Handlers::ComponentAssemblyDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentAssemblyDescription > assemblyImpl_;

      // monolithicImpl
      //
      public:
      bool monolithicImpl_p () const;
      ::CIAO::Config_Handlers::MonolithicImplementationDescription const& monolithicImpl () const;
      ::CIAO::Config_Handlers::MonolithicImplementationDescription& monolithicImpl ();
      void monolithicImpl (::CIAO::Config_Handlers::MonolithicImplementationDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::MonolithicImplementationDescription > monolithicImpl_;

      // configProperty
      //
      public:
      bool configProperty_p () const;
      ::CIAO::Config_Handlers::Property const& configProperty () const;
      ::CIAO::Config_Handlers::Property& configProperty ();
      void configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > configProperty_;

      // capability
      //
      public:
      bool capability_p () const;
      ::CIAO::Config_Handlers::Capability const& capability () const;
      ::CIAO::Config_Handlers::Capability& capability ();
      void capability (::CIAO::Config_Handlers::Capability const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Capability > capability_;

      // dependsOn
      //
      public:
      bool dependsOn_p () const;
      ::CIAO::Config_Handlers::ImplementationDependency const& dependsOn () const;
      ::CIAO::Config_Handlers::ImplementationDependency& dependsOn ();
      void dependsOn (::CIAO::Config_Handlers::ImplementationDependency const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ImplementationDependency > dependsOn_;

      // infoProperty
      //
      public:
      bool infoProperty_p () const;
      ::CIAO::Config_Handlers::Property const& infoProperty () const;
      ::CIAO::Config_Handlers::Property& infoProperty ();
      void infoProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > infoProperty_;

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
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // CID_HPP
