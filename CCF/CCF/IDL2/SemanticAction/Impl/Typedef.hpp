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
          pre (KeywordPtr const& id);

          virtual void
          begin (IdentifierPtr const& id);

          virtual void
          begin_unbounded_seq (IdentifierPtr const& id);

          virtual void
          begin_bounded_seq (IdentifierPtr const& id);

          virtual void
          begin_bounded_string ();

          virtual void
          begin_bounded_wstring ();

          virtual void
          begin_array ();

          virtual void
          bound ();

          virtual void
          declarator (SimpleIdentifierPtr const& id);

          virtual void
          end ();

        private:
          bool define_;
          unsigned long line_;
          SemanticGraph::Type* type_;
          SemanticGraph::Type* array_type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP
