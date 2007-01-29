// file      : CCF/IDL2/SemanticAction/Impl/Const.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_CONST_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_CONST_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticAction/Const.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Const : SemanticAction::Const, Base
        {
          Const (Context& c);

          virtual void
          begin (IdentifierPtr const& type,
                 SimpleIdentifierPtr const& name);

          virtual void
          expr ();

        private:
          SemanticGraph::Const* const_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_CONST_HPP
