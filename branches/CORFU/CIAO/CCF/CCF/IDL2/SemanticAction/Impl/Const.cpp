// file      : CCF/IDL2/SemanticAction/Impl/Const.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Const.hpp"

#include "CCF/IDL2/SemanticGraph/Fundamental.hpp"
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

        Const::
        Const (Context& c)
            : Base (c)
        {
        }

        void Const::
        begin (IdentifierPtr const& type_id,
               SimpleIdentifierPtr const& name_id)
        {
          if (ctx.trace ())
            cerr << "const " << type_id << " " << name_id << endl;

          const_ = 0;

          SimpleName name (name_id->lexeme ());
          Name type_name (type_id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, type_name, Flags::complete));

              if (dynamic_cast<Octet*> (&t) ||
                  dynamic_cast<Short*> (&t) ||
                  dynamic_cast<UnsignedShort*> (&t) ||
                  dynamic_cast<Long*> (&t) ||
                  dynamic_cast<UnsignedLong*> (&t) ||
                  dynamic_cast<LongLong*> (&t) ||
                  dynamic_cast<UnsignedLongLong*> (&t))
              {
                // Integer constant.
                //
                const_ = &ctx.tu ().new_node<SemanticGraph::IntConst> (
                  ctx.file (), name_id->line ());

                ctx.tu ().new_edge<Belongs> (*const_, t);
                ctx.tu ().new_edge<Defines> (ctx.scope (), *const_, name);
              }

            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << type_id->line () << ": error: "
                   << "invalid const declaration" << endl;
              throw;
            }

            //@@ I am not handling NotUnique here. For example if
            //   I provide module name as type then the compiler
            //   will ICE. Think about other places it may happen
            //   (attribute, value memebr, typeded, others?).
            //
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "no type with name \'" << type_name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "declaration with name \'" << type_name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;

            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "using non-type as a const type is illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void Const::
        expr ()
        {
          //@@ Need to check if int_exp_stack is empty.
          //
          if (const_ && ctx.int_exp_size () > 0)
          {
            IntExpression& expr (ctx.int_exp_pop ());

            ctx.tu ().new_edge<Initializes> (expr, *const_);
          }
        }
      }
    }
  }
}
