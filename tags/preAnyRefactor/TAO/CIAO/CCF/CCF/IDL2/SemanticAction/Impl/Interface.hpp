// file      : CCF/IDL2/SemanticAction/Impl/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_INTERFACE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_INTERFACE_HPP

#include "CCF/IDL2/SemanticGraph/Interface.hpp"
#include "CCF/IDL2/SemanticAction/Interface.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Interface : SemanticAction::Interface,
                           ScopeBase<SemanticGraph::Interface>
        {
          Interface (Context& c);

          virtual void
          begin_abstract_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_abstract_fwd (SimpleIdentifierPtr const& id);

          virtual void
          begin_local_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_local_fwd (SimpleIdentifierPtr const& id);

          virtual void
          begin_unconstrained_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_unconstrained_fwd (SimpleIdentifierPtr const& id);

          virtual void
          inherits (IdentifierPtr const& id);

          virtual void
          open_scope ();

          virtual void
          close_scope ();

          virtual void
          end ();

        private:
          struct Qualifier
          {
            enum Value { abstract, local, unconstrained };
          };

          Qualifier::Value qualifier_;
        };

      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_INTERFACE_HPP
