// file      : CCF/IDL3/SyntaxTree/Home.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SyntaxTree/Home.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL3
  {
    namespace SyntaxTree
    {

      // HomeDef
      //
      //
      namespace
      {
        TypeInfo
        home_def_init_ ()
        {
          TypeInfo ti (typeid (HomeDef));
          ti.add_base (Access::PUBLIC, true, TypeDef::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo home_def_ (home_def_init_ ());
      }

      TypeInfo const& HomeDef::
      static_type_info () { return home_def_; }
    }
  }
}
