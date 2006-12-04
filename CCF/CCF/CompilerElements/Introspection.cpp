// file      : CCF/CompilerElements/Introspection.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

//
// This is a hack to avoid link dependency on Utility library.
//

#include "Utility/Introspection/Object.hpp"

namespace Utility
{
  namespace Introspection
  {
    namespace
    {
      TypeInfo object_ (typeid (Object));
    }

    TypeInfo const& Object::
    static_type_info () throw ()
    {
      return object_;
    }
  }
}
