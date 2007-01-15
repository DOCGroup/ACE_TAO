// file      : CCF/IDL3/SemanticAction/Impl/Uses.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/Component.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Uses.hpp"

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

        Uses::
        Uses (Context& c)
            : Base (c), multiple_ (false)
        {
        }

        void Uses::
        multiple ()
        {
          multiple_ = true;
        }

        void Uses::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "uses " << (multiple_ ? "multiple ": "") << id;

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
                   << "invalid uses declaration" << endl;
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
                 << "using non-interface type in uses declaration is "
                 << "illegal" << endl;
          }
        }

        void Uses::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          if (type_)
          {
            User* u;

            if (multiple_)
              u = &ctx.tu ().new_node<MultiUser> (ctx.file (), id->line ());
            else
              u = &ctx.tu ().new_node<SingleUser> (ctx.file (), id->line ());

            ctx.tu ().new_edge<Belongs> (*u, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), *u, id->lexeme ());
          }

          multiple_ = false;
        }
      }
    }
  }
}
