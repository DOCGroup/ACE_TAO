// file      : CCF/CIDL/SemanticAction/Impl/HomeExecutor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_IMPL_HOME_EXECUTOR_HPP
#define CCF_CIDL_SEMANTIC_ACTION_IMPL_HOME_EXECUTOR_HPP

#include "CCF/CIDL/SemanticGraph/Executor.hpp"
#include "CCF/CIDL/SemanticAction/HomeExecutor.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct HomeExecutor : SemanticAction::HomeExecutor, Base
        {
          HomeExecutor (Context& c);

          virtual void
          begin (SimpleIdentifierPtr const& id);

          virtual void
          implements (IdentifierPtr const& id);

          virtual void
          manages (SimpleIdentifierPtr const& id);

          virtual void
          end ();

        private:
          SimpleIdentifierPtr id_;
          SemanticGraph::HomeExecutor* he_;
          SemanticGraph::Component* c_;
        };
      }
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_IMPL_HOME_EXECUTOR_HPP
