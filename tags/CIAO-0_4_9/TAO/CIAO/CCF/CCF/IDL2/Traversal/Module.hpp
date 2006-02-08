// file      : CCF/IDL2/Traversal/Module.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_MODULE_HPP
#define CCF_IDL2_TRAVERSAL_MODULE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Module.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct Module : ScopeTemplate<SemanticGraph::Module>
      {
        virtual void
        traverse (Type& m)
        {
          pre (m);
          name (m);
          names (m);
          post (m);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        name (Type&)
        {
        }

        virtual void
        post (Type&)
        {
        }
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_MODULE_HPP
