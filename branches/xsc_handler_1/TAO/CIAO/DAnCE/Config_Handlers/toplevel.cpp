// $Id$
#include "toplevel.hpp"

namespace Deployment
{
  // TopLevelPackageDescription
  // 

  TopLevelPackageDescription::
  TopLevelPackageDescription ()
  :
  regulator__ ()
  {
  }

  TopLevelPackageDescription::
  TopLevelPackageDescription (::Deployment::TopLevelPackageDescription const& s)
  :
  regulator__ ()
  {
    package_.reserve (s.package_.size ());
    {
      for (package_const_iterator i (s.package_.begin ());
      i != s.package_.end ();
      ++i) add_package (*i);
    }
  }

  ::Deployment::TopLevelPackageDescription& TopLevelPackageDescription::
  operator= (::Deployment::TopLevelPackageDescription const& s)
  {
    package_.clear ();
    package_.reserve (s.package_.size ());
    {
      for (package_const_iterator i (s.package_.begin ());
      i != s.package_.end ();
      ++i) add_package (*i);
    }

    return *this;
  }


  // TopLevelPackageDescription
  // 
  TopLevelPackageDescription::package_iterator TopLevelPackageDescription::
  begin_package ()
  {
    return package_.begin ();
  }

  TopLevelPackageDescription::package_iterator TopLevelPackageDescription::
  end_package ()
  {
    return package_.end ();
  }

  TopLevelPackageDescription::package_const_iterator TopLevelPackageDescription::
  begin_package () const
  {
    return package_.begin ();
  }

  TopLevelPackageDescription::package_const_iterator TopLevelPackageDescription::
  end_package () const
  {
    return package_.end ();
  }

  void TopLevelPackageDescription::
  add_package (::Deployment::PackageConfiguration const& e)
  {
    if (package_.capacity () < package_.size () + 1)
    {
      ::std::vector< ::Deployment::PackageConfiguration > v;
      v.reserve (package_.size () + 1);

      while (package_.size ())
      {
        //@@ VC6
        ::Deployment::PackageConfiguration& t = package_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        package_.pop_back ();
      }

      package_.swap (v);
    }

    package_.push_back (e);
    package_.back ().container (this);
  }
}

namespace Deployment
{
  // TopLevelPackageDescription
  //

  TopLevelPackageDescription::
  TopLevelPackageDescription (::XSCRT::XML::Element< char > const& e)
  :Base__ (e), regulator__ ()
  {

    ::XSCRT::Parser< char > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< char > e (p.next_element ());
      ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == "package")
      {
        ::Deployment::PackageConfiguration t (e);
        add_package (t);
      }

      else 
      {
      }
    }
  }
}

namespace Deployment
{
}

#include <XMLSchema/TypeInfo.hpp>

namespace Deployment
{
  namespace
  {
    ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

    struct TopLevelPackageDescriptionTypeInfoInitializer
    {
      TopLevelPackageDescriptionTypeInfoInitializer ()
      {
        ::XSCRT::TypeId id (typeid (TopLevelPackageDescription));
        ::XSCRT::ExtendedTypeInfo nf (id);

        nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
        ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
      }
    };

    TopLevelPackageDescriptionTypeInfoInitializer TopLevelPackageDescriptionTypeInfoInitializer_;
  }
}

namespace Deployment
{
  namespace Traversal
  {
    // TopLevelPackageDescription
    //
    //

    void TopLevelPackageDescription::
    traverse (Type& o)
    {
      pre (o);
      package (o);
      post (o);
    }

    void TopLevelPackageDescription::
    traverse (Type const& o)
    {
      pre (o);
      package (o);
      post (o);
    }

    void TopLevelPackageDescription::
    pre (Type&)
    {
    }

    void TopLevelPackageDescription::
    pre (Type const&)
    {
    }

    void TopLevelPackageDescription::
    package (Type& o)
    {
      // VC6 anathema strikes again
      //
      TopLevelPackageDescription::Type::package_iterator b (o.begin_package()), e (o.end_package());

      if (b != e)
      {
        package_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) package_next (o);
        }

        package_post (o);
      }
    }

    void TopLevelPackageDescription::
    package (Type const& o)
    {
      // VC6 anathema strikes again
      //
      TopLevelPackageDescription::Type::package_const_iterator b (o.begin_package()), e (o.end_package());

      if (b != e)
      {
        package_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) package_next (o);
        }

        package_post (o);
      }
    }

    void TopLevelPackageDescription::
    package_pre (Type&)
    {
    }

    void TopLevelPackageDescription::
    package_pre (Type const&)
    {
    }

    void TopLevelPackageDescription::
    package_next (Type&)
    {
    }

    void TopLevelPackageDescription::
    package_next (Type const&)
    {
    }

    void TopLevelPackageDescription::
    package_post (Type&)
    {
    }

    void TopLevelPackageDescription::
    package_post (Type const&)
    {
    }

    void TopLevelPackageDescription::
    post (Type&)
    {
    }

    void TopLevelPackageDescription::
    post (Type const&)
    {
    }
  }
}

namespace Deployment
{
  namespace Writer
  {
    // TopLevelPackageDescription
    //
    //

    TopLevelPackageDescription::
    TopLevelPackageDescription (::XSCRT::XML::Element< char >& e)
    : ::XSCRT::Writer< char > (e)
    {
    }

    TopLevelPackageDescription::
    TopLevelPackageDescription ()
    {
    }

    void TopLevelPackageDescription::
    traverse (Type const& o)
    {
      Traversal::TopLevelPackageDescription::traverse (o);
    }

    void TopLevelPackageDescription::
    package_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("package", top_ ()));
    }

    void TopLevelPackageDescription::
    package_next (Type const& o)
    {
      package_post (o);
      package_pre (o);
    }

    void TopLevelPackageDescription::
    package_post (Type const&)
    {
      pop_ ();
    }
  }
}

namespace Deployment
{
}

