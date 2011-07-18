// file      : CCF/IDL2/SemanticAction/Impl/NumericExpression.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_NUMERIC_EXPRESSION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_NUMERIC_EXPRESSION_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticAction/NumericExpression.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct NumericExpression : SemanticAction::NumericExpression, Base
        {
          NumericExpression (Context& c);

          virtual void
          flush ();

          virtual void
          const_ (IdentifierPtr const&);

          virtual void
          integer_literal (IntegerLiteralPtr const&);

          virtual void
          pre (OperatorPtr const&);

          virtual void
          pos ();

          virtual void
          neg ();

          virtual void
          com ();

          virtual void
          mul ();

          virtual void
          div ();

          virtual void
          rem ();

          virtual void
          add ();

          virtual void
          sub ();

          virtual void
          rsh ();

          virtual void
          lsh ();

          virtual void
          and_ ();

          virtual void
          xor_ ();

          virtual void
          or_ ();


        private:
          unsigned long line_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_NUMERIC_EXPRESSION_HPP
