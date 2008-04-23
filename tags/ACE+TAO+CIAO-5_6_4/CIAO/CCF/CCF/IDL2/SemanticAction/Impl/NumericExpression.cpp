// file      : CCF/IDL2/SemanticAction/Impl/NumericExpression.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/NumericExpression.hpp"

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/IntExpression.hpp"

#include <iostream>

using std::cerr;
using std::endl;

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        NumericExpression::
        NumericExpression (Context& c)
            : Base (c)
        {
        }

        void NumericExpression::
        flush ()
        {
          if (ctx.trace ())
            cerr << "expression" << endl;

          ctx.int_exp_flush ();
        }

        void NumericExpression::
        const_ (IdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "const " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Const& c (resolve<Const> (from, name));

              if (IntConst* ic = dynamic_cast<IntConst*> (&c))
              {
                ctx.int_exp_push (*ic);
              }
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid reference to const" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no const with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a const declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-const as a reference to const is illegal"
                 << endl;
          }
        }

        void NumericExpression::
        integer_literal (IntegerLiteralPtr const& il)
        {
          if (ctx.trace ())
            cerr << "integer literal " << il->value () << endl;

          ctx.int_exp_push (
            ctx.tu ().new_node<IntLiteral> (
              ctx.file (), il->line (), il->value ()));
        }

        void NumericExpression::
        pre (OperatorPtr const& op)
        {
          line_ = op->line ();
        }

        void NumericExpression::
        pos ()
        {
          if (ctx.trace ())
            cerr << "unary +" << endl;

          // Nothing to do.
        }

        void NumericExpression::
        neg ()
        {
          if (ctx.trace ())
            cerr << "unary -" << endl;

          if (ctx.int_exp_size () < 1)
            return;

          IntExpression& expr (ctx.int_exp_pop ());

          IntNeg& neg (ctx.tu ().new_node<IntNeg> (ctx.file (), line_));

          ctx.tu ().new_edge<IntNegates> (neg, expr);

          ctx.int_exp_push (neg);
        }

        void NumericExpression::
        com ()
        {
          if (ctx.trace ())
            cerr << "unary ~" << endl;

          if (ctx.int_exp_size () < 1)
            return;

          IntExpression& expr (ctx.int_exp_pop ());

          IntCom& com (ctx.tu ().new_node<IntCom> (ctx.file (), line_));

          ctx.tu ().new_edge<IntComplements> (com, expr);

          ctx.int_exp_push (com);
        }

        void NumericExpression::
        mul ()
        {
          if (ctx.trace ())
            cerr << "*" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& multiplier (ctx.int_exp_pop ());
          IntExpression& multiplicand (ctx.int_exp_pop ());

          IntMul& mul (ctx.tu ().new_node<IntMul> (ctx.file (), line_));

          ctx.tu ().new_edge<IntMultiplies> (mul, multiplicand);
          ctx.tu ().new_edge<IntMultiplies> (mul, multiplier);

          ctx.int_exp_push (mul);
        }

        void NumericExpression::
        div ()
        {
          if (ctx.trace ())
            cerr << "/" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& divisor (ctx.int_exp_pop ());
          IntExpression& divident (ctx.int_exp_pop ());

          IntDiv& div (ctx.tu ().new_node<IntDiv> (ctx.file (), line_));

          ctx.tu ().new_edge<IntDivides> (div, divident);
          ctx.tu ().new_edge<IntDivides> (div, divisor);

          ctx.int_exp_push (div);
        }

        void NumericExpression::
        rem ()
        {
          if (ctx.trace ())
            cerr << "%" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& divisor (ctx.int_exp_pop ());
          IntExpression& divident (ctx.int_exp_pop ());

          IntRem& rem (ctx.tu ().new_node<IntRem> (ctx.file (), line_));

          ctx.tu ().new_edge<IntDivides> (rem, divident);
          ctx.tu ().new_edge<IntDivides> (rem, divisor);

          ctx.int_exp_push (rem);
        }


        void NumericExpression::
        add ()
        {
          if (ctx.trace ())
            cerr << "+" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& second_item (ctx.int_exp_pop ());
          IntExpression& first_item (ctx.int_exp_pop ());

          IntAdd& add (ctx.tu ().new_node<IntAdd> (ctx.file (), line_));

          ctx.tu ().new_edge<IntAdds> (add, first_item);
          ctx.tu ().new_edge<IntAdds> (add, second_item);

          ctx.int_exp_push (add);
        }

        void NumericExpression::
        sub ()
        {
          if (ctx.trace ())
            cerr << "-" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& subtrahend (ctx.int_exp_pop ());
          IntExpression& minuend (ctx.int_exp_pop ());

          IntSub& sub (ctx.tu ().new_node<IntSub> (ctx.file (), line_));

          ctx.tu ().new_edge<IntSubtracts> (sub, minuend);
          ctx.tu ().new_edge<IntSubtracts> (sub, subtrahend);

          ctx.int_exp_push (sub);
        }

        void NumericExpression::
        rsh ()
        {
          if (ctx.trace ())
            cerr << ">>" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& factor (ctx.int_exp_pop ());
          IntExpression& pattern (ctx.int_exp_pop ());

          IntRsh& rsh (ctx.tu ().new_node<IntRsh> (ctx.file (), line_));

          ctx.tu ().new_edge<IntShifts> (rsh, pattern);
          ctx.tu ().new_edge<IntShifts> (rsh, factor);

          ctx.int_exp_push (rsh);
        }

        void NumericExpression::
        lsh ()
        {
          if (ctx.trace ())
            cerr << "<<" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& factor (ctx.int_exp_pop ());
          IntExpression& pattern (ctx.int_exp_pop ());

          IntLsh& lsh (ctx.tu ().new_node<IntLsh> (ctx.file (), line_));

          ctx.tu ().new_edge<IntShifts> (lsh, pattern);
          ctx.tu ().new_edge<IntShifts> (lsh, factor);

          ctx.int_exp_push (lsh);
        }

        void NumericExpression::
        and_ ()
        {
          if (ctx.trace ())
            cerr << "&" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& second_pattern (ctx.int_exp_pop ());
          IntExpression& first_pattern (ctx.int_exp_pop ());

          IntAnd& and_ (ctx.tu ().new_node<IntAnd> (ctx.file (), line_));

          ctx.tu ().new_edge<IntConjuncts> (and_, first_pattern);
          ctx.tu ().new_edge<IntConjuncts> (and_, second_pattern);

          ctx.int_exp_push (and_);
        }

        void NumericExpression::
        xor_ ()
        {
          if (ctx.trace ())
            cerr << "^" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& second_pattern (ctx.int_exp_pop ());
          IntExpression& first_pattern (ctx.int_exp_pop ());

          IntXor& xor_ (ctx.tu ().new_node<IntXor> (ctx.file (), line_));

          ctx.tu ().new_edge<IntExclusivelyDisjuncts> (xor_, first_pattern);
          ctx.tu ().new_edge<IntExclusivelyDisjuncts> (xor_, second_pattern);

          ctx.int_exp_push (xor_);
        }

        void NumericExpression::
        or_ ()
        {
          if (ctx.trace ())
            cerr << "|" << endl;

          if (ctx.int_exp_size () < 2)
            return;

          IntExpression& second_pattern (ctx.int_exp_pop ());
          IntExpression& first_pattern (ctx.int_exp_pop ());

          IntOr& or_ (ctx.tu ().new_node<IntOr> (ctx.file (), line_));

          ctx.tu ().new_edge<IntInclusivelyDisjuncts> (or_, first_pattern);
          ctx.tu ().new_edge<IntInclusivelyDisjuncts> (or_, second_pattern);

          ctx.int_exp_push (or_);
        }
      }
    }
  }
}
