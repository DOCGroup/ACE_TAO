// file      : CCF/IDL2/SemanticGraph/Member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Member.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      namespace
      {
        TypeInfo
        member_init_ ()
        {
          TypeInfo ti (typeid (Member));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo member_ (member_init_ ());
      }

      TypeInfo const& Member::
      static_type_info () { return member_; }
    }
  }
}
