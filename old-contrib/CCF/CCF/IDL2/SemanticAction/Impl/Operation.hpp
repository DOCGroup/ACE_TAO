// file      : CCF/IDL2/SemanticAction/Impl/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_OPERATION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_OPERATION_HPP

#include "CCF/IDL2/SemanticGraph/Operation.hpp"
#include "CCF/IDL2/SemanticAction/Operation.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        struct Operation : SemanticAction::Operation, Base
        {
          Operation (Context& c);

          virtual void
          one_way ();

          virtual void
          two_way ();

          virtual void
          type (IdentifierPtr const& id);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          parameter (Direction::Value direction,
                     IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id);

          virtual void
          raises (IdentifierPtr const& id);

        private:
          bool one_way_;
          SemanticGraph::Type* type_;
          SemanticGraph::Operation* op_;
        };

      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_OPERATION_HPP
