// file      : CCF/CIDL/SemanticGraph/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      namespace
      {
        TypeInfo
        implements_init_ ()
        {
          TypeInfo ti (typeid (Implements));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo implements_ (implements_init_ ());
      }

      TypeInfo const& Implements::
      static_type_info () { return implements_; }
    }
  }
}
