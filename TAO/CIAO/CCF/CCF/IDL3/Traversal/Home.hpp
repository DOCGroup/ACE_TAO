// file      : CCF/IDL3/Traversal/Home.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_TRAVERSAL_HOME_HPP
#define CCF_IDL3_TRAVERSAL_HOME_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL3/SyntaxTree/Home.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      //
      //
      //
      struct HomeDef : IDL2::Traversal::ScopeTraverser
      {
        typedef
        SyntaxTree::HomeDefPtr
        NodePtr;

        HomeDef ()
        {
          map (typeid (SyntaxTree::HomeDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::HomeDef> ());
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

#endif  // CCF_IDL3_TRAVERSAL_HOME_HPP
