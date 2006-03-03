// file      : CCF/IDL2/Traversal/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_TYPE_ID_HPP
#define CCF_IDL2_TRAVERSAL_TYPE_ID_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/TypeId.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Typeid and typeprefix are two idiotic constructs of IDL.
      // They normally should not result in any directly generated
      // code so only minimal (and most generic) support is provided.
      //

      struct TypeId : Node<SemanticGraph::TypeId>
      {
      };

      struct TypePrefix : Node<SemanticGraph::TypePrefix>
      {
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_TYPE_ID_HPP
