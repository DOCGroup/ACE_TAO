// file      : CCF/CIDL/Traversal/HomeExecutor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_TRAVERSAL_HOME_EXECUTOR_HPP
#define CCF_CIDL_TRAVERSAL_HOME_EXECUTOR_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/CIDL/SyntaxTree/HomeExecutor.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      //
      //
      //
      struct HomeExecutor : IDL2::Traversal::ScopeTraverser
      {
        typedef
        SyntaxTree::HomeExecutorPtr
        NodePtr;

        HomeExecutor ()
        {
          map (typeid (SyntaxTree::HomeExecutor), this);
        }


        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::HomeExecutor> ());
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

#endif  // CCF_CIDL_TRAVERSAL_HOME_EXECUTOR_HPP
