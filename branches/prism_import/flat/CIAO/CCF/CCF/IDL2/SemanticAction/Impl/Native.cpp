// file      : CCF/IDL2/SemanticAction/Impl/Native.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Native.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Native.hpp"

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

        Native::
        Native (Context& c)
            : Base (c)
        {
        }

        void Native::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "native " << id << endl;

          SimpleName name (id->lexeme ());

          SemanticGraph::Native& n (
            ctx.tu ().new_node<SemanticGraph::Native> (
              ctx.file (), id->line ()));

          ctx.tu ().new_edge<Defines> (ctx.scope (), n, name);
        }

        void Native::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
