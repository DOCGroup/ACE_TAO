// file      : CCF/IDL2/SemanticAction/Impl/Exception.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Exception.hpp"

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

        Exception::
        Exception (Context& c)
            : ScopeBase<SemanticGraph::Exception> (c)
        {
        }

        void Exception::
        begin (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "exception " << id << endl;

          SimpleName name (id->lexeme ());

          now (ctx.tu ().new_node<SemanticGraph::Exception> (
                 ctx.file (), id->line ()));

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Exception::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Exception::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Exception::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
