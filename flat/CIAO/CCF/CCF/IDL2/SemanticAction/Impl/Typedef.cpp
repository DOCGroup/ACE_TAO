// file      : CCF/IDL2/SemanticAction/Impl/Typedef.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Typedef.hpp"

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Array.hpp"
#include "CCF/IDL2/SemanticGraph/Sequence.hpp"
#include "CCF/IDL2/SemanticGraph/String.hpp"

#include <iostream>

using std::cerr;
using std::endl;

//@@ names used in smantic actions are inconsistent
//   e.g. begin () vs name (), etc
//

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Typedef::
        Typedef (Context& c)
            : Base (c)
        {
        }

        void Typedef::
        pre (KeywordPtr const& id)
        {
          line_ = id->line ();
        }

        void Typedef::
        begin (IdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "typedef " << id << endl;

          define_ = false;
          type_ = 0;
          array_type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              type_ = &resolve<Type> (from, name);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid typedef declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-type in typedef is illegal" << endl;
          }
        }

        void Typedef::
        begin_unbounded_seq (IdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "typedef u-sequence<" << id << ">" << endl;

          define_ = true;
          type_ = 0;
          array_type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, name));

              UnboundedSequence& s (
                ctx.tu ().new_node<UnboundedSequence> (
                  ctx.file (), line_));

              ctx.tu ().new_edge<ArgumentsWithType> (t, s);

              type_ = &s;
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid sequence declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-type in sequence specialization is illegal"
                 << endl;
          }
        }

        void Typedef::
        begin_bounded_seq (IdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "typedef b-sequence<" << id << ">" << endl;

          define_ = true;
          type_ = 0;
          array_type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, name));

              BoundedSequence& s (
                ctx.tu ().new_node<BoundedSequence> (
                  ctx.file (), line_));

              ctx.tu ().new_edge<ArgumentsWithType> (t, s);

              type_ = &s;
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid sequence declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-type in sequence specialization is illegal"
                 << endl;
          }
        }

        void Typedef::
        begin_bounded_string ()
        {
          if (ctx.trace ()) cerr << "typedef b-string" << endl;

          define_ = true;
          type_ = 0;
          array_type_ = 0;

          type_ = &ctx.tu ().new_node<BoundedString> (ctx.file (), line_);
          bound ();
        }

        void Typedef::
        begin_bounded_wstring ()
        {
          if (ctx.trace ()) cerr << "typedef b-wstring" << endl;

          define_ = true;
          type_ = 0;
          array_type_ = 0;

          type_ = &ctx.tu ().new_node<BoundedWideString> (ctx.file (), line_);
          bound ();
        }

        void Typedef::
        begin_array ()
        {
          if (ctx.trace ()) cerr << "array" << endl;

          define_ = true;
          array_type_ = 0;

          if (type_ == 0)
            return;

          if (type_->named_begin () == type_->named_end ())
          {
            cerr << ctx.file () << ":" << line_ << ": error: "
                 << "anonymous types in array declarations "
                 << "are not supported" << endl;

            cerr << ctx.file () << ":" << line_ << ": error: "
                 << "use another typedef to name this type" << endl;

            return;
          }

          Array& a (ctx.tu ().new_node<Array> (ctx.file (), line_));
          ctx.tu ().new_edge<ArgumentsWithType> (*type_, a);

          array_type_ = &a;
        }

        void Typedef::
        bound ()
        {
          if (ctx.trace ()) cerr << "bound" << endl;

          if (ctx.int_exp_size () < 1)
            return;

          IntExpression& expr (ctx.int_exp_pop ());

          if (array_type_ != 0)
          {
            Specialization& s (dynamic_cast<Specialization&> (*array_type_));
            ctx.tu ().new_edge<ArgumentsWithValue> (expr, s);
          }
          else if (type_ != 0)
          {
            Specialization& s (dynamic_cast<Specialization&> (*type_));
            ctx.tu ().new_edge<ArgumentsWithValue> (expr, s);
          }
        }

        void Typedef::
        declarator (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          SimpleName name (id->lexeme ());

          try
          {
            if (lookup (ctx.tu (), ctx.scope (), name) == 0)
            {
              if (array_type_ != 0)
              {
                assert (define_);

                ctx.tu ().new_edge<Defines> (ctx.scope (), *array_type_, name);

                define_ = false;
                array_type_ = 0;
              }
              else if (type_ != 0)
              {
                if (define_)
                {
                  ctx.tu ().new_edge<Defines> (ctx.scope (), *type_, name);
                  define_ = false;
                }
                else
                {
                  ctx.tu ().new_edge<Aliases> (ctx.scope (), *type_, name);
                }
              }
              return;
            }
          }
          catch (NotUnique const& )
          {
          }

          cerr << ctx.file () << ":" << id->line () << ": error: "
               << "invalid typedef declaration" << endl;

          cerr << ctx.file () << ":" << id->line () << ": error: "
               << "redeclaration of name " << name << endl;

          array_type_ = 0;
        }

        void Typedef::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
