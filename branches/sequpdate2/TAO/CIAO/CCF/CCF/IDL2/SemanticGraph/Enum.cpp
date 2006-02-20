// file      : CCF/IDL2/SemanticGraph/Enum.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Enum.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Enum
      //
      //
      namespace
      {
        TypeInfo
        enum_init_ ()
        {
          TypeInfo ti (typeid (Enum));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo enum_ (enum_init_ ());
      }

      TypeInfo const& Enum::
      static_type_info () { return enum_; }


      // Enumerator
      //
      //
      namespace
      {
        TypeInfo
        enumerator_init_ ()
        {
          TypeInfo ti (typeid (Enumerator));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo enumerator_ (enumerator_init_ ());
      }

      TypeInfo const& Enumerator::
      static_type_info () { return enumerator_; }
    }
  }
}
