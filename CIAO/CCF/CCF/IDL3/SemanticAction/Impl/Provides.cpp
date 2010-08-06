// file      : CCF/IDL3/SemanticAction/Impl/Provides.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/Component.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Provides.hpp"

#include <iostream>

using std::cerr;
using std::endl;

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Provides::
        Provides (Context& c)
            : Base (c)
        {
        }

        void Provides::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "provides " << id;

          type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              type_ = &resolve<SemanticGraph::Interface> (from, name);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid provides declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no interface with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not an interface declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-interface type in provides declaration is "
                 << "illegal" << endl;
          }
        }

        void Provides::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          if (type_)
          {
            Provider& p (
              ctx.tu ().new_node<Provider> (
                ctx.file (), id->line ()));

            ctx.tu ().new_edge<Belongs> (p, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), p, id->lexeme ());
          }
        }
      }
    }
  }
}
