// file      : CCF/CIDL/Traversal/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_CIDL_TRAVERSAL_COMPOSITION_HPP
#define CCF_CIDL_TRAVERSAL_COMPOSITION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/CIDL/SyntaxTree/Composition.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      //
      //
      //
      struct Composition : IDL2::Traversal::ScopeTraverser
      {
        typedef
        SyntaxTree::CompositionPtr
        NodePtr;

        Composition ()
        {
          map (typeid (SyntaxTree::Composition), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Composition> ());
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

#endif  // CCF_CIDL_TRAVERSAL_COMPOSITION_HPP
