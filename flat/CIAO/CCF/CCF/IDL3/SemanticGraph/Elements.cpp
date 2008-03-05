// file      : CCF/IDL3/SemanticGraph/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      namespace
      {
        TypeInfo
        manages_init_ ()
        {
          TypeInfo ti (typeid (Manages));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo manages_ (manages_init_ ());
      }

      TypeInfo const& Manages::
      static_type_info () { return manages_; }

    }
  }
}
