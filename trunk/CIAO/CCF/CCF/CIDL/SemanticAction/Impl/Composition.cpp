// file      : CCF/CIDL/SemanticAction/Impl/Composition.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SemanticAction/Impl/Composition.hpp"

#include <iostream>

using std::cerr;
using std::endl;

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Composition::
        Composition (Context& c)
            : ScopeBase<SemanticGraph::Composition> (c)
        {
        }

        void Composition::
        begin (SimpleIdentifierPtr const& id, Category::Value c)
        {
          if (ctx.trace ()) cerr << "composition " << id << endl;

          if(c == Category::entity)
          {
            now (ctx.tu ().new_node<EntityComposition> (
                   ctx.file (), id->line ()));
          }
          else if (c == Category::process)
          {
            now (ctx.tu ().new_node<ProcessComposition> (
                   ctx.file (), id->line ()));
          }
          else if (c == Category::service)
          {
            now (ctx.tu ().new_node<ServiceComposition> (
                   ctx.file (), id->line ()));
          }
          else
          {
            now (ctx.tu ().new_node<SessionComposition> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), id->lexeme ());
        }

        void Composition::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Composition::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Composition::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
