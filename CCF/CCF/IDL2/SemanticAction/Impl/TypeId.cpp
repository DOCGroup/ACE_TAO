// file      : CCF/IDL2/SemanticAction/Impl/TypeId.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/TypeId.hpp"

#include "CCF/IDL2/SemanticGraph/TypeId.hpp"

// These inclusions are needed to ensure that typeprefix is applied to a
// suitable declaration.

#include "CCF/IDL2/SemanticGraph/Module.hpp"
#include "CCF/IDL2/SemanticGraph/Interface.hpp"
#include "CCF/IDL2/SemanticGraph/ValueType.hpp"

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

        // TypeId
        //
        //
        TypeId::
        TypeId (Context& c)
            : ctx (c)
        {
        }

        void TypeId::
        begin (IdentifierPtr const& d, StringLiteralPtr const& id)
        {
          if (ctx.trace ())
            cerr << "typeid " << d << " " << id << endl;

          Name name (d->lexeme ());
          SemanticGraph::StringLiteral tid (id->value ());

          try
          {
            ScopedName from (ctx.scope ().scoped_name ());
            Nameables nodes (resolve (ctx.tu (), from, name));

            ScopedName full ((**(nodes.begin ())).scoped_name ());

            SemanticGraph::TypeId& ti (
              ctx.tu ().new_node<SemanticGraph::TypeId> (
                ctx.file (), d->line (), full, tid));

            ctx.tu ().new_edge<Defines> (ctx.scope (), ti, "typeid");
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << d->line () << ": error: "
                 << "invalid typeid declaration" << endl;

            cerr << ctx.file () << ":" << d->line () << ": error: "
                 << "no declaration with name \'"
                 << name << "\' visible from scope \'"
                 << ctx.scope ().scoped_name () << "\'" << endl;
          }
        }

        void TypeId::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }


        // TypePrefix
        //
        //
        TypePrefix::
        TypePrefix (Context& c)
            : ctx (c)
        {
        }

        void TypePrefix::
        begin (IdentifierPtr const& d, StringLiteralPtr const& prefix)
        {
          if (ctx.trace ()) cerr << "typeprefix " << d << " " << prefix
                                 << endl;

          Name name (d->lexeme ());
          SemanticGraph::StringLiteral tprefix (prefix->value ());

          try
          {
            ScopedName from (ctx.scope ().scoped_name ());
            Nameables nodes (resolve (ctx.tu (), from, name));

            Nameable& node (**(nodes.begin ()));

            dynamic_cast<SemanticGraph::Scope&> (node);

            ScopedName full (node.scoped_name ());

            SemanticGraph::TypePrefix& tp (
              ctx.tu ().new_node<SemanticGraph::TypePrefix> (
                ctx.file (), d->line (), full, tprefix));

            ctx.tu ().new_edge<Defines> (ctx.scope (), tp, "typeprefix");
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << d->line () << ": error: "
                 << "invalid typeprefix declaration" << endl;

            cerr << ctx.file () << ":" << d->line () << ": error: "
                 << "no declaration with name \'"
                 << name << "\' visible from scope \'"
                 << ctx.scope ().scoped_name () << "\'" << endl;
          }
          catch (std::bad_cast const&)
          {
            cerr << ctx.file () << ":" << d->line () << ": error: "
                 << "invalid typeprefix declaration" << endl;

            cerr << ctx.file () << ":" << d->line () << ": error: "
                 << "no suitable declaration with name \'"
                 << name << "\' visible from scope \'"
                 << ctx.scope ().scoped_name () << "\'" << endl;
          }
        }

        void TypePrefix::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
