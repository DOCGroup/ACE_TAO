// file      : CCF/IDL2/SemanticGraph/Struct.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Struct.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;
      
      // Struct
      //
      //
      namespace
      {
        TypeInfo
        struct_init_ ()
        {
          TypeInfo ti (typeid (Struct));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo struct_ (struct_init_ ());
      }

      TypeInfo const& Struct::
      static_type_info () { return struct_; }

    }
  }
}
