// $Id$
#ifndef CPD_HPP
#define CPD_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class PackagedComponentImplementation;
    class ComponentPackageDescription;
  }
}

#include <memory>
#include <vector>
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

#include "ccd.hpp"

#include "cid.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class PackagedComponentImplementation : public ::XSCRT::Type
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

      // referencedImplementation
      //
      public:
      ::CIAO::Config_Handlers::ComponentImplementationDescription const& referencedImplementation () const;
      ::CIAO::Config_Handlers::ComponentImplementationDescription& referencedImplementation ();
      void referencedImplementation (::CIAO::Config_Handlers::ComponentImplementationDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentImplementationDescription > referencedImplementation_;

      public:
      PackagedComponentImplementation ();
      PackagedComponentImplementation (::XSCRT::XML::Element< char > const&);
      PackagedComponentImplementation (PackagedComponentImplementation const& s);

      PackagedComponentImplementation&
      operator= (PackagedComponentImplementation const& s);

      private:
      char regulator__;
    };

    class ComponentPackageDescription : public ::XSCRT::Type
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

      // realizes
      //
      public:
      bool realizes_p () const;
      ::CIAO::Config_Handlers::ComponentInterfaceDescription const& realizes () const;
      ::CIAO::Config_Handlers::ComponentInterfaceDescription& realizes ();
      void realizes (::CIAO::Config_Handlers::ComponentInterfaceDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentInterfaceDescription > realizes_;

      // configProperty
      //
      public:
      bool configProperty_p () const;
      ::CIAO::Config_Handlers::Property const& configProperty () const;
      ::CIAO::Config_Handlers::Property& configProperty ();
      void configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > configProperty_;

      // implementation
      //
      public:
      bool implementation_p () const;
      ::CIAO::Config_Handlers::PackagedComponentImplementation const& implementation () const;
      ::CIAO::Config_Handlers::PackagedComponentImplementation& implementation ();
      void implementation (::CIAO::Config_Handlers::PackagedComponentImplementation const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::PackagedComponentImplementation > implementation_;

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
      ComponentPackageDescription ();
      ComponentPackageDescription (::XSCRT::XML::Element< char > const&);
      ComponentPackageDescription (ComponentPackageDescription const& s);

      ComponentPackageDescription&
      operator= (ComponentPackageDescription const& s);

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

#endif // CPD_HPP
