// $Id$
#include "Config_Handlers_Export.h"
#ifndef TOPLEVEL_HPP
#define TOPLEVEL_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class TopLevelPackageDescription;
  }
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "pcd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class Config_Handlers_Export TopLevelPackageDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // package
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::PackageConfiguration >::iterator package_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::PackageConfiguration >::const_iterator package_const_iterator;
      package_iterator begin_package ();
      package_iterator end_package ();
      package_const_iterator begin_package () const;
      package_const_iterator end_package () const;
      void add_package (::CIAO::Config_Handlers::PackageConfiguration const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::PackageConfiguration > package_;

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
      struct Config_Handlers_Export TopLevelPackageDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::TopLevelPackageDescription >
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
}

#include <XMLSchema/Writer.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      struct Config_Handlers_Export TopLevelPackageDescription : Traversal::TopLevelPackageDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::TopLevelPackageDescription Type;
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
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // TOPLEVEL_HPP
