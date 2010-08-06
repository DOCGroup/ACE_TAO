// file      : CCF/IDL2/SemanticAction/NumericExpression.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_NUMERIC_EXPRESSION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_NUMERIC_EXPRESSION_HPP

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct NumericExpression
      {
        virtual ~NumericExpression ()
        {
        }
        
        virtual void
        flush () = 0;

        virtual void
        const_ (IdentifierPtr const&) = 0;

        virtual void
        integer_literal (IntegerLiteralPtr const&) = 0;

        virtual void
        pre (OperatorPtr const&) = 0;

        virtual void
        pos () = 0;

        virtual void
        neg () = 0;

        virtual void
        com () = 0;

        virtual void
        mul () = 0;

        virtual void
        div () = 0;

        virtual void
        rem () = 0;

        virtual void
        add () = 0;

        virtual void
        sub () = 0;

        virtual void
        rsh () = 0;

        virtual void
        lsh () = 0;

        virtual void
        and_ () = 0;

        virtual void
        xor_ () = 0;

        virtual void
        or_ () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_NUMERIC_EXPRESSION_HPP
