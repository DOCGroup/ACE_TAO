// $Id$
#ifndef TOPLEVEL_HPP
#define TOPLEVEL_HPP

// Forward declarations.
//
namespace Deployment
{
  class TopLevelPackageDescription;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "pcd.hpp"

namespace Deployment
{
  class TopLevelPackageDescription : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // package
    // 
    public:
    typedef ::std::vector< ::Deployment::PackageConfiguration >::iterator package_iterator;
    typedef ::std::vector< ::Deployment::PackageConfiguration >::const_iterator package_const_iterator;
    package_iterator begin_package ();
    package_iterator end_package ();
    package_const_iterator begin_package () const;
    package_const_iterator end_package () const;
    void add_package (::Deployment::PackageConfiguration const& );

    protected:
    ::std::vector< ::Deployment::PackageConfiguration > package_;

    public:
    TopLevelPackageDescription ();

    TopLevelPackageDescription (::XSCRT::XML::Element< char > const&);
    TopLevelPackageDescription (TopLevelPackageDescription const& s);

    TopLevelPackageDescription&
    operator= (TopLevelPackageDescription const& s);

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
    struct TopLevelPackageDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::TopLevelPackageDescription >
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
      package (Type&);

      virtual void
      package (Type const&);

      virtual void
      package_pre (Type&);

      virtual void
      package_pre (Type const&);

      virtual void
      package_next (Type&);

      virtual void
      package_next (Type const&);

      virtual void
      package_post (Type&);

      virtual void
      package_post (Type const&);

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
    struct TopLevelPackageDescription : Traversal::TopLevelPackageDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::TopLevelPackageDescription Type;
      TopLevelPackageDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      package_pre (Type const&);

      virtual void
      package_next (Type const&);

      virtual void
      package_post (Type const&);

      protected:
      TopLevelPackageDescription ();
    };
  }
}

namespace Deployment
{
}

#endif // TOPLEVEL_HPP
