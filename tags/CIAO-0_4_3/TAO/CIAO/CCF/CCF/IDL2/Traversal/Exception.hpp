// file      : CCF/IDL2/Traversal/Exception.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_EXCEPTION_HPP
#define CCF_IDL2_TRAVERSAL_EXCEPTION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Exception.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct Exception : ScopeTemplate<SemanticGraph::Exception>
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

#endif  // CCF_IDL2_TRAVERSAL_EXCEPTION_HPP
