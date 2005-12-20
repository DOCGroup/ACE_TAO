// file      : CCF/IDL2/SemanticGraph/TypeId.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/TypeId.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // TypeId
      //
      //
      namespace
      {
        TypeInfo
        type_id_init_ ()
        {
          TypeInfo ti (typeid (TypeId));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          return ti;
        }

        TypeInfo type_id_ (type_id_init_ ());
      }

      TypeInfo const& TypeId::
      static_type_info () { return type_id_; }


      // TypePrefix
      //
      //
      namespace
      {
        TypeInfo
        type_prefix_init_ ()
        {
          TypeInfo ti (typeid (TypePrefix));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          return ti;
        }

        TypeInfo type_prefix_ (type_prefix_init_ ());
      }

      TypeInfo const& TypePrefix::
      static_type_info () { return type_prefix_; }
    }
  }
}
