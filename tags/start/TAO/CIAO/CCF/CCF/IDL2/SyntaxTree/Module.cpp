// file      : CCF/IDL2/SyntaxTree/Module.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL2/SyntaxTree/Module.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      //  Module
      //
      //
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
