// file      : CCF/IDL3/SemanticAction/Impl/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_COMPONENT_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_COMPONENT_HPP

#include "CCF/IDL3/SemanticGraph/Component.hpp"
#include "CCF/IDL3/SemanticAction/Component.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Component : SemanticAction::Component,
                           ScopeBase<SemanticGraph::Component>
        {
          Component (Context& c);

          virtual void
          begin_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_fwd (SimpleIdentifierPtr const& id);

          virtual void
          inherits (IdentifierPtr const& id);

          virtual void
          supports (IdentifierPtr const& id);

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

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_COMPONENT_HPP
