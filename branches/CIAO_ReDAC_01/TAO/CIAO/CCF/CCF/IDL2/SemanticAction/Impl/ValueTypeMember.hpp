// file      : CCF/IDL2/SemanticAction/Impl/ValueTypeMember.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_MEMBER_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_MEMBER_HPP

#include "CCF/IDL2/SemanticGraph/ValueTypeMember.hpp"
#include "CCF/IDL2/SemanticAction/ValueTypeMember.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct ValueTypeMember : SemanticAction::ValueTypeMember, Base
        {
          ValueTypeMember (Context& c);

          virtual void
          begin_private ();

          virtual void
          begin_public ();

          virtual void
          type (IdentifierPtr const& id);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          end ();

        private:
          SemanticGraph::ValueTypeMember::Access::Value access_;
          SemanticGraph::Type* type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_VALUE_TYPE_MEMBER_HPP
