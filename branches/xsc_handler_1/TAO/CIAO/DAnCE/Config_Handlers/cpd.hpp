// $Id$
#ifndef CPD_HPP
#define CPD_HPP

// Forward declarations.
//
namespace Deployment
{
  class PackagedComponentImplementation;
  class ComponentPackageDescription;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "ccd.hpp"

#include "cid.hpp"

namespace Deployment
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
    ::Deployment::ComponentImplementationDescription const& referencedImplementation () const;
    ::Deployment::ComponentImplementationDescription& referencedImplementation ();
    void referencedImplementation (::Deployment::ComponentImplementationDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentImplementationDescription > referencedImplementation_;

    public:
    PackagedComponentImplementation (::XMLSchema::string< char > const& name__,
    ::Deployment::ComponentImplementationDescription const& referencedImplementation__);

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
    ::Deployment::ComponentInterfaceDescription const& realizes () const;
    ::Deployment::ComponentInterfaceDescription& realizes ();
    void realizes (::Deployment::ComponentInterfaceDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentInterfaceDescription > realizes_;

    // configProperty
    // 
    public:
    bool configProperty_p () const;
    ::Deployment::Property const& configProperty () const;
    ::Deployment::Property& configProperty ();
    void configProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > configProperty_;

    // implementation
    // 
    public:
    bool implementation_p () const;
    ::Deployment::PackagedComponentImplementation const& implementation () const;
    ::Deployment::PackagedComponentImplementation& implementation ();
    void implementation (::Deployment::PackagedComponentImplementation const& );

    protected:
    ::std::auto_ptr< ::Deployment::PackagedComponentImplementation > implementation_;

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
    ComponentPackageDescription ();

    ComponentPackageDescription (::XSCRT::XML::Element< char > const&);
    ComponentPackageDescription (ComponentPackageDescription const& s);

    ComponentPackageDescription&
    operator= (ComponentPackageDescription const& s);

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
    struct PackagedComponentImplementation : ::XMLSchema::Traversal::Traverser< ::Deployment::PackagedComponentImplementation >
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
      referencedImplementation (Type&);

      virtual void
      referencedImplementation (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct ComponentPackageDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::ComponentPackageDescription >
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
      realizes (Type&);

      virtual void
      realizes (Type const&);

      virtual void
      realizes_none (Type&);

      virtual void
      realizes_none (Type const&);

      virtual void
      configProperty (Type&);

      virtual void
      configProperty (Type const&);

      virtual void
      configProperty_none (Type&);

      virtual void
      configProperty_none (Type const&);

      virtual void
      implementation (Type&);

      virtual void
      implementation (Type const&);

      virtual void
      implementation_none (Type&);

      virtual void
      implementation_none (Type const&);

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
    struct PackagedComponentImplementation : Traversal::PackagedComponentImplementation, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::PackagedComponentImplementation Type;
      PackagedComponentImplementation (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      name (Type const&);

      virtual void
      referencedImplementation (Type const&);

      protected:
      PackagedComponentImplementation ();
    };

    struct ComponentPackageDescription : Traversal::ComponentPackageDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::ComponentPackageDescription Type;
      ComponentPackageDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      label (Type const&);

      virtual void
      UUID (Type const&);

      virtual void
      realizes (Type const&);

      virtual void
      configProperty (Type const&);

      virtual void
      implementation (Type const&);

      virtual void
      infoProperty (Type const&);

      virtual void
      contentLocation (Type const&);

      protected:
      ComponentPackageDescription ();
    };
  }
}

namespace Deployment
{
}

#endif // CPD_HPP
