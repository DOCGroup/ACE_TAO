// file      : CCF/IDL2/SemanticAction/Impl/ValueType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_HPP

#include "CCF/IDL2/SemanticGraph/ValueType.hpp"
#include "CCF/IDL2/SemanticAction/ValueType.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct ValueType : SemanticAction::ValueType,
                           ScopeBase<SemanticGraph::ValueType>
        {
          ValueType (Context& c);

          virtual void
          begin_abstract_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_abstract_fwd (SimpleIdentifierPtr const& id);

          virtual void
          begin_concrete_def (SimpleIdentifierPtr const& id);

          virtual void
          begin_concrete_fwd (SimpleIdentifierPtr const& id);

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

        private:
          struct Qualifier
          {
            enum Value { abstract, concrete };
          };

          Qualifier::Value qualifier_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_HPP
