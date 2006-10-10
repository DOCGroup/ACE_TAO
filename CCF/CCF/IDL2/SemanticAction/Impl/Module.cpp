// file      : CCF/IDL2/SemanticAction/Impl/Module.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Module.hpp"

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

        Module::
        Module (Context& c)
            : ScopeBase<SemanticGraph::Module> (c)
        {
        }

        void Module::
        begin (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "module " << id << endl;

          now (ctx.tu ().new_node<SemanticGraph::Module> (
                 ctx.file (), id->line ()));

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), id->lexeme ());
        }

        void Module::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Module::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Module::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
