// $Id$
#include "CIDL_Traversal.hpp"
#include "CIDL_SyntaxTree.hpp"

namespace CIDL
{
  namespace Traversal
  {
    namespace
    {
      //@@ code duplication
      void
      iterate_scope (SyntaxTree::ScopePtr const& s,
                     CCF::Traversal::Visitor* v)
      {
        for (SyntaxTree::Scope::Iterator i = s->begin (); i != s->end (); i++)
        {
          (*i)->accept (v);
        }
      }
    }

    //
    // Composition
    //

    void Composition::
    visit_composition (NodePtr const& c)
    {
      visit_composition_pre   (c);
      visit_composition_scope (c);
      visit_composition_post  (c);
    }

    void Composition::
    visit_composition_pre (NodePtr const&)
    {
    }

    void Composition::
    visit_composition_scope (NodePtr const& c)
    {
      iterate_scope (c, this);
    }

    void Composition::
    visit_composition_post (NodePtr const&)
    {
    }

    //
    // HomeExecutor
    //

    void HomeExecutor::
    visit_home_executor (NodePtr const& he)
    {
      visit_home_executor_pre   (he);
      visit_home_executor_scope (he);
      visit_home_executor_post  (he);
    }

    void HomeExecutor::
    visit_home_executor_pre (NodePtr const&)
    {
    }

    void HomeExecutor::
    visit_home_executor_scope (NodePtr const& he)
    {
      iterate_scope (he, this);
    }

    void HomeExecutor::
    visit_home_executor_post (NodePtr const&)
    {
    }
  }
}
