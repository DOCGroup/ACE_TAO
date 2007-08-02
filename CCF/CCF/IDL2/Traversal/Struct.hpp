// file      : CCF/IDL2/Traversal/Struct.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_STRUCT_HPP
#define CCF_IDL2_TRAVERSAL_STRUCT_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Struct.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct Struct : ScopeTemplate<SemanticGraph::Struct>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_STRUCT_HPP
