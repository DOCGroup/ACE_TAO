// $Id$
#include "CIDL_Traversal.hpp"
#include "CIDL_SyntaxTree.hpp"

namespace CIDL
{
  namespace SyntaxTree
  {
    using CCF::Traversal::try_visit;

    namespace
    {
      //@@ repetition of what's already in IDL2 & IDL3
      bool
      try_visit_scope (CCF::Traversal::Visitor* v,
                       ScopePtr const& node)
      {
        return try_visit<Traversal::Scope> (
          &Traversal::Scope::visit_scope,
          v,
          node);
      }
    }

    //
    // Composition
    //

    void Composition::
    accept (CCF::Traversal::Visitor* v)
    {
      CompositionPtr self (ReferenceCounting::add_ref (this));

      if (!try_visit<Traversal::Composition> (
            &Traversal::Composition::visit_composition,
            v,
            self))
      {
        try_visit_scope (v, self);
      }
    }

    //
    // Home
    //

    void HomeExecutor::
    accept (CCF::Traversal::Visitor* v)
    {
      HomeExecutorPtr self (ReferenceCounting::add_ref (this));

      if (!try_visit<Traversal::HomeExecutor> (
            &Traversal::HomeExecutor::visit_home_executor,
            v,
            self))
      {
        try_visit_scope (v, self);
      }
    }
  }
}
