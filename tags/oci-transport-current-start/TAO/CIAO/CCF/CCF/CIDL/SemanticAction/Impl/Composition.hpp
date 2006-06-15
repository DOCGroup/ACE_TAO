// file      : CCF/CIDL/SemanticAction/Impl/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_IMPL_COMPOSITION_HPP
#define CCF_CIDL_SEMANTIC_ACTION_IMPL_COMPOSITION_HPP

#include "CCF/CIDL/SemanticGraph/Composition.hpp"
#include "CCF/CIDL/SemanticAction/Composition.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Composition : SemanticAction::Composition,
                             ScopeBase<SemanticGraph::Composition>
        {
          Composition (Context& c);

          virtual void
          begin (SimpleIdentifierPtr const& id, Category::Value c);

          virtual void
          open_scope ();

          virtual void
          close_scope ();

          virtual void
          end ();
        };
      }
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_IMPL_COMPOSITION_HPP
