// file      : CCF/IDL2/SemanticAction/Impl/Enum.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Enum.hpp"

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

        Enum::
        Enum (Context& c)
            : Base (c)
        {
        }

        void Enum::
        begin (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "enum " << id << endl;

          type_ = 0;

          SimpleName name (id->lexeme ());

          type_ = &ctx.tu ().new_node<SemanticGraph::Enum> (
            ctx.file (), id->line ());

          ctx.tu ().new_edge<Defines> (ctx.scope (), *type_, name);
        }

        void Enum::
        enumerator (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "enumerator " << id << endl;

          //@@ Need to check for redeclaration of the name.
          //

          SimpleName name (id->lexeme ());

          if (type_ != 0)
          {
            Enumerator& e (
              ctx.tu ().new_node<Enumerator> (
                ctx.file (), id->line ()));

            ctx.tu ().new_edge<Belongs> (e, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), e, name);
          }
        }

        void Enum::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
