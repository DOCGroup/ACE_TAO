// $Id$
#ifndef PCD_HPP
#define PCD_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class PackageConfiguration;
  }
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

#include "cpd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class PackageConfiguration : public ::XSCRT::Type
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

      // basePackage
      // 
      public:
      bool basePackage_p () const;
      ::CIAO::Config_Handlers::ComponentPackageDescription const& basePackage () const;
      ::CIAO::Config_Handlers::ComponentPackageDescription& basePackage ();
      void basePackage (::CIAO::Config_Handlers::ComponentPackageDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageDescription > basePackage_;

      // reference
      // 
      public:
      bool reference_p () const;
      ::CIAO::Config_Handlers::ComponentPackageReference const& reference () const;
      ::CIAO::Config_Handlers::ComponentPackageReference& reference ();
      void reference (::CIAO::Config_Handlers::ComponentPackageReference const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageReference > reference_;

      // selectRequirement
      // 
      public:
      bool selectRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& selectRequirement () const;
      ::CIAO::Config_Handlers::Requirement& selectRequirement ();
      void selectRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > selectRequirement_;

      // configProperty
      // 
      public:
      bool configProperty_p () const;
      ::CIAO::Config_Handlers::Property const& configProperty () const;
      ::CIAO::Config_Handlers::Property& configProperty ();
      void configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > configProperty_;

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
      PackageConfiguration ();

      PackageConfiguration (::XSCRT::XML::Element< char > const&);
      PackageConfiguration (PackageConfiguration const& s);

      PackageConfiguration&
      operator= (PackageConfiguration const& s);

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

#include <XMLSchema/Traversal.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
      struct PackageConfiguration : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::PackageConfiguration >
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
        basePackage (Type&);

        virtual void
        basePackage (Type const&);

        virtual void
        basePackage_none (Type&);

        virtual void
        basePackage_none (Type const&);

        virtual void
        reference (Type&);

        virtual void
        reference (Type const&);

        virtual void
        reference_none (Type&);

        virtual void
        reference_none (Type const&);

        virtual void
        selectRequirement (Type&);

        virtual void
        selectRequirement (Type const&);

        virtual void
        selectRequirement_none (Type&);

        virtual void
        selectRequirement_none (Type const&);

        virtual void
        configProperty (Type&);

        virtual void
        configProperty (Type const&);

        virtual void
        configProperty_none (Type&);

        virtual void
        configProperty_none (Type const&);

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
}

#include <XMLSchema/Writer.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      struct PackageConfiguration : Traversal::PackageConfiguration, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::PackageConfiguration Type;
        PackageConfiguration (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        label (Type const&);

        virtual void
        UUID (Type const&);

        virtual void
        basePackage (Type const&);

        virtual void
        reference (Type const&);

        virtual void
        selectRequirement (Type const&);

        virtual void
        configProperty (Type const&);

        virtual void
        contentLocation (Type const&);

        protected:
        PackageConfiguration ();
      };
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // PCD_HPP
