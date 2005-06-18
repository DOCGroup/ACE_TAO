// file      : CCF/IDL2/Traversal/Enum.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_ENUM_HPP
#define CCF_IDL2_TRAVERSAL_ENUM_HPP

#include "CCF/IDL2/SemanticGraph/Enum.hpp"
#include "CCF/IDL2/Traversal/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Minimal support for now. Talk to me if you need more.
      //
      struct Enum : Node<SemanticGraph::Enum>
      {
      };

      struct Enumerator : Node<SemanticGraph::Enumerator>
      {
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_ENUM_HPP
