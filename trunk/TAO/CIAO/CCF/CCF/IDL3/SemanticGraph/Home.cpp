// file      : CCF/IDL3/SemanticGraph/Home.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/Home.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Home
      //
      //
      namespace
      {
        TypeInfo
        home_init_ ()
        {
          TypeInfo ti (typeid (Home));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo home_ (home_init_ ());
      }

      TypeInfo const& Home::
      static_type_info () { return home_; }


      // HomeFactory
      //
      //
      namespace
      {
        TypeInfo
        home_factory_init_ ()
        {
          TypeInfo ti (typeid (HomeFactory));
          ti.add_base (
            Access::PUBLIC, true, TwoWayOperation::static_type_info ());
          return ti;
        }

        TypeInfo home_factory_ (home_factory_init_ ());
      }

      TypeInfo const& HomeFactory::
      static_type_info () { return home_factory_; }


      // HomeFinder
      //
      //
      namespace
      {
        TypeInfo
        home_finder_init_ ()
        {
          TypeInfo ti (typeid (HomeFinder));
          ti.add_base (
            Access::PUBLIC, true, TwoWayOperation::static_type_info ());
          return ti;
        }

        TypeInfo home_finder_ (home_finder_init_ ());
      }

      TypeInfo const& HomeFinder::
      static_type_info () { return home_finder_; }
    }
  }
}
