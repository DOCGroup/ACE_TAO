// file      : CCF/IDL2/SemanticGraph/Array.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Array.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Array
      //
      //
      namespace
      {
        TypeInfo
        array_init_ ()
        {
          TypeInfo ti (typeid (Array));
          ti.add_base (
            Access::PUBLIC, true, Specialization::static_type_info ());
          return ti;
        }

        TypeInfo array_ (array_init_ ());
      }

      TypeInfo const& Array::
      static_type_info () { return array_; }
    }
  }
}
