// file      : CCF/IDL2/SemanticAction/Impl/Union.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_UNION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_UNION_HPP

#include "CCF/IDL2/SemanticGraph/Union.hpp"
#include "CCF/IDL2/SemanticAction/Union.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Union : SemanticAction::Union, ScopeBase<SemanticGraph::Union>
        {
          Union (Context& c);

          virtual void
          begin_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_fwd (SimpleIdentifierPtr const& id);

          virtual void
          type (IdentifierPtr const& id);

          virtual void
          open_scope ();

          virtual void
          member_type (IdentifierPtr const& id);

          virtual void
          member_name (SimpleIdentifierPtr const& id);

          virtual void
          close_scope ();

          virtual void
          end ();

        private:
          SemanticGraph::Type* type_;
          SemanticGraph::Type* member_type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_UNION_HPP
