// file      : CCF/IDL2/SemanticGraph/Module.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Module.hpp"

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
        module_init_ ()
        {
          TypeInfo ti (typeid (Module));
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo module_ (module_init_ ());
      }

      TypeInfo const& Module::
      static_type_info () { return module_; }
    }
  }
}
