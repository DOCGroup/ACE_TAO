// file      : CCF/IDL2/SemanticGraph/Exception.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Exception.hpp"

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
        exception_init_ ()
        {
          TypeInfo ti (typeid (Exception));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo exception_ (exception_init_ ());
      }

      TypeInfo const& Exception::
      static_type_info () { return exception_; }
    }
  }
}
