// file      : CCF/IDL2/Traversal/Module.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_TRAVERSAL_MODULE_HPP
#define CCF_IDL2_TRAVERSAL_MODULE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Module.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      //
      struct Module : ScopeTraverser
      {
        typedef
        SyntaxTree::ModulePtr
        NodePtr;

        Module ()
        {
          map (typeid (SyntaxTree::Module), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Module> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_MODULE_HPP
