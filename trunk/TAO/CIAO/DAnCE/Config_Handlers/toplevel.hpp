// $Id$
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
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

#include "pcd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class TopLevelPackageDescription : public ::XSCRT::Type
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

#endif // TOPLEVEL_HPP
