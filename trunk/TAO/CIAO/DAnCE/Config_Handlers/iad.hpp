// $Id$
#ifndef IAD_HPP
#define IAD_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class NamedImplementationArtifact;
    class ImplementationArtifactDescription;
  }
}

#include <memory>
#include <vector>
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

#include "Basic_Deployment_Data.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class NamedImplementationArtifact : public ::XSCRT::Type
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

      // referencedArtifact
      //
      public:
      ::CIAO::Config_Handlers::ImplementationArtifactDescription const& referencedArtifact () const;
      ::CIAO::Config_Handlers::ImplementationArtifactDescription& referencedArtifact ();
      void referencedArtifact (::CIAO::Config_Handlers::ImplementationArtifactDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ImplementationArtifactDescription > referencedArtifact_;

      public:
      NamedImplementationArtifact ();
      NamedImplementationArtifact (::XSCRT::XML::Element< char > const&);
      NamedImplementationArtifact (NamedImplementationArtifact const& s);

      NamedImplementationArtifact&
      operator= (NamedImplementationArtifact const& s);

      private:
      char regulator__;
    };

    class ImplementationArtifactDescription : public ::XSCRT::Type
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

      // location
      //
      public:
      typedef ::std::vector< ::XMLSchema::string< char > >::iterator location_iterator;
      typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator location_const_iterator;
      location_iterator begin_location ();
      location_iterator end_location ();
      location_const_iterator begin_location () const;
      location_const_iterator end_location () const;
      void add_location (::XMLSchema::string< char > const& );

      protected:
      ::std::vector< ::XMLSchema::string< char > > location_;

      // execParameter
      //
      public:
      bool execParameter_p () const;
      ::CIAO::Config_Handlers::Property const& execParameter () const;
      ::CIAO::Config_Handlers::Property& execParameter ();
      void execParameter (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > execParameter_;

      // deployRequirement
      //
      public:
      bool deployRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& deployRequirement () const;
      ::CIAO::Config_Handlers::Requirement& deployRequirement ();
      void deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

      // dependsOn
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact >::iterator dependsOn_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact >::const_iterator dependsOn_const_iterator;
      dependsOn_iterator begin_dependsOn ();
      dependsOn_iterator end_dependsOn ();
      dependsOn_const_iterator begin_dependsOn () const;
      dependsOn_const_iterator end_dependsOn () const;
      void add_dependsOn (::CIAO::Config_Handlers::NamedImplementationArtifact const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact > dependsOn_;

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
      ImplementationArtifactDescription ();
      ImplementationArtifactDescription (::XSCRT::XML::Element< char > const&);
      ImplementationArtifactDescription (ImplementationArtifactDescription const& s);

      ImplementationArtifactDescription&
      operator= (ImplementationArtifactDescription const& s);

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

#endif // IAD_HPP
