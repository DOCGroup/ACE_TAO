// file      : CCF/CIDL/SyntaxTree/Composition.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SyntaxTree/Composition.hpp"

using namespace Introspection;

namespace CCF
{
  namespace CIDL
  {
    namespace SyntaxTree
    {
      // Composition
      //
      //
      namespace
      {
        TypeInfo
        composition_init_ ()
        {
          TypeInfo ti (typeid (Composition));
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }
        
        TypeInfo composition_ (composition_init_ ());
      }
      
      TypeInfo const& Composition::
      static_type_info () { return composition_; }
    }
  }
}
