// $Id$
#ifndef PCD_HPP
#define PCD_HPP

// Forward declarations.
//
namespace Deployment
{
  class PackageConfiguration;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

#include "cpd.hpp"

namespace Deployment
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
    ::Deployment::ComponentPackageDescription const& basePackage () const;
    ::Deployment::ComponentPackageDescription& basePackage ();
    void basePackage (::Deployment::ComponentPackageDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentPackageDescription > basePackage_;

    // reference
    // 
    public:
    bool reference_p () const;
    ::Deployment::ComponentPackageReference const& reference () const;
    ::Deployment::ComponentPackageReference& reference ();
    void reference (::Deployment::ComponentPackageReference const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentPackageReference > reference_;

    // selectRequirement
    // 
    public:
    bool selectRequirement_p () const;
    ::Deployment::Requirement const& selectRequirement () const;
    ::Deployment::Requirement& selectRequirement ();
    void selectRequirement (::Deployment::Requirement const& );

    protected:
    ::std::auto_ptr< ::Deployment::Requirement > selectRequirement_;

    // configProperty
    // 
    public:
    bool configProperty_p () const;
    ::Deployment::Property const& configProperty () const;
    ::Deployment::Property& configProperty ();
    void configProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > configProperty_;

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

namespace Deployment
{
}

#include <XMLSchema/Traversal.hpp>

namespace Deployment
{
  namespace Traversal
  {
    struct PackageConfiguration : ::XMLSchema::Traversal::Traverser< ::Deployment::PackageConfiguration >
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

#include <XMLSchema/Writer.hpp>

namespace Deployment
{
  namespace Writer
  {
    struct PackageConfiguration : Traversal::PackageConfiguration, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::PackageConfiguration Type;
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

namespace Deployment
{
}

#endif // PCD_HPP
