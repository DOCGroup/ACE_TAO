// file      : CCF/IDL2/SemanticGraph/Attribute.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Attribute.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Attribute
      //
      //
      namespace
      {
        TypeInfo
        attribute_init_ ()
        {
          TypeInfo ti (typeid (Attribute));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo attribute_ (attribute_init_ ());
      }

      TypeInfo const& Attribute::
      static_type_info () { return attribute_; }
    }
  }
}
