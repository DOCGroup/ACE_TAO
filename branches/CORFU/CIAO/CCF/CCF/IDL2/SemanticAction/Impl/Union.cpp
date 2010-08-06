// file      : CCF/IDL2/SemanticAction/Impl/Union.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Union.hpp"

#include "CCF/IDL2/SemanticGraph/Enum.hpp"
#include "CCF/IDL2/SemanticGraph/Fundamental.hpp"

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

        Union::
        Union (Context& c)
            : ScopeBase<SemanticGraph::Union> (c)
        {
        }

        void Union::
        begin_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "union def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Union&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Union> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Union::
        begin_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "union fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Union&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Union> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Union::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "type " << id << endl;

          type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              type_ = &resolve<Type> (from, name);

              if (!(dynamic_cast<Enum*> (type_) ||
                    dynamic_cast<Boolean*> (type_) ||
                    dynamic_cast<Char*> (type_) ||
                    dynamic_cast<Wchar*> (type_) ||
                    dynamic_cast<Short*> (type_) ||
                    dynamic_cast<UnsignedShort*> (type_) ||
                    dynamic_cast<Long*> (type_) ||
                    dynamic_cast<UnsignedLong*> (type_) ||
                    dynamic_cast<LongLong*> (type_) ||
                    dynamic_cast<UnsignedLongLong*> (type_)))
              {
                throw WrongType (type_->scoped_name ());
              }

              ctx.tu ().new_edge<ArgumentsWithType> (*type_, now ());
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid union declaration" << endl;
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
                 << "\' is not a valid discriminant type" << endl;
          }
        }

        void Union::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Union::
        member_type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "type " << id << endl;

          member_type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              // With introduction of CORBA 3.1 we have a new beast:
              // union with incoplete members which itself becomes
              // incomplete.
              //
              member_type_ = &resolve<Type> (from, name/*, Flags::complete*/);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid union member declaration" << endl;
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
                 << "using non-type as an member type is illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void Union::
        member_name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "name " << id << endl;

          if (member_type_)
          {
            SimpleName name (id->lexeme ());

            SemanticGraph::UnionMember& m (
              ctx.tu ().new_node<SemanticGraph::UnionMember> (
                ctx.file (), id->line ()));

            ctx.tu ().new_edge<Belongs> (m, *member_type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), m, name);
          }
        }

        void Union::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Union::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
