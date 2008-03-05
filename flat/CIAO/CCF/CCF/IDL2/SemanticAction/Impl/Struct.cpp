// file      : CCF/IDL2/SemanticAction/Impl/Struct.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Struct.hpp"

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

        Struct::
        Struct (Context& c)
            : ScopeBase<SemanticGraph::Struct> (c)
        {
        }

        // @@ I need a mechanism for propagating exceptions from
        //    semantic actions to the parser in order to stop
        //    semantic action invocation.
        //

        void Struct::
        begin_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "struct def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Struct&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Struct> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Struct::
        begin_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "struct fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Struct&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Struct> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Struct::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Struct::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Struct::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
