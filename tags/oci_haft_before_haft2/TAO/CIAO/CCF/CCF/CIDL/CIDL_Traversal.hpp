// $Id$
#ifndef CCF_CIDL_TRAVERSAL_HPP
#define CCF_CIDL_TRAVERSAL_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_Traversal.hpp"
#include "CCF/CIDL/CIDL_SyntaxTreeFwd.hpp"

namespace CIDL
{
  namespace Traversal
  {
    // Import visitors from IDL3
    using namespace IDL3::Traversal;

    //
    // Composition
    //
    struct Composition : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::CompositionPtr
      NodePtr;

      virtual void visit_composition (NodePtr const& c);
      virtual void visit_composition_pre (NodePtr const& c);
      virtual void visit_composition_scope (NodePtr const& c);
      virtual void visit_composition_post (NodePtr const& c);
    };

    //
    // HomeExecutor
    //
    struct HomeExecutor : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::HomeExecutorPtr
      NodePtr;

      virtual void visit_home_executor (NodePtr const& he);
      virtual void visit_home_executor_pre (NodePtr const& he);
      virtual void visit_home_executor_scope (NodePtr const& he);
      virtual void visit_home_executor_post (NodePtr const& he);

    };
  }
}

#endif // CCF_CIDL_TRAVERSAL_HPP
