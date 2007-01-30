// file      : CCF/IDL2/SemanticGraph/String.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/String.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // BoundedString
      //
      //
      namespace
      {
        TypeInfo
        bounded_string_init_ ()
        {
          TypeInfo ti (typeid (BoundedString));
          ti.add_base (
            Access::PUBLIC, true, Specialization::static_type_info ());
          return ti;
        }

        TypeInfo bounded_string_ (bounded_string_init_ ());
      }

      TypeInfo const& BoundedString::
      static_type_info () { return bounded_string_; }


      // BoundedWideString
      //
      //
      namespace
      {
        TypeInfo
        bounded_wide_string_init_ ()
        {
          TypeInfo ti (typeid (BoundedWideString));
          ti.add_base (
            Access::PUBLIC, true, Specialization::static_type_info ());
          return ti;
        }

        TypeInfo bounded_wide_string_ (bounded_wide_string_init_ ());
      }

      TypeInfo const& BoundedWideString::
      static_type_info () { return bounded_wide_string_; }
    }
  }
}
