// file      : CCF/IDL2/SemanticAction/Impl/Typedef.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticAction/Typedef.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Typedef : SemanticAction::Typedef, Base
        {
          Typedef (Context& c);

          virtual void
          begin (IdentifierPtr const& id);

          virtual void
          begin_seq (IdentifierPtr const& id);

          virtual void
          declarator (SimpleIdentifierPtr const& id);

          virtual void
          end ();

        private:
          bool define_;
          SemanticGraph::Type* type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP
