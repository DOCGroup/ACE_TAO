// file      : CCF/CIDL/SemanticAction/Impl/HomeExecutor.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SemanticAction/Impl/HomeExecutor.hpp"

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

        HomeExecutor::
        HomeExecutor (Context& c)
            : Base (c)
        {
        }

        void HomeExecutor::
        begin (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "home executor " << id << endl;

          id_ = id;
          he_ = &ctx.tu ().new_node<SemanticGraph::HomeExecutor> (
            ctx.file (), id->line ());
        }

        void HomeExecutor::
        implements (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "implements " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          c_ = 0;

          try
          {
            try
            {
              SemanticGraph::Home& h (
                resolve<SemanticGraph::Home> (from, name, Flags::defined));

              c_ = &dynamic_cast<SemanticGraph::Component&> (
                h.manages ().managee ());

              ctx.tu ().new_edge<Implements> (*he_, h);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid implements specification" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no home with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "incompatible type in implements specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "attempt to implement forward-declared home "
                 << e.name () << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "implementation of forward-declared home is illegal"
                 << endl;
          }
        }

        void HomeExecutor::
        manages (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "manages " << id << endl;

          if (c_ != 0)
          {
            SemanticGraph::ComponentExecutor& ce (
              ctx.tu ().new_node<SemanticGraph::ComponentExecutor> (
                ctx.file (), id->line ()));

            ctx.tu ().new_edge<Implements> (ce, *c_);

            ctx.tu ().new_edge<Defines> (ctx.scope (), ce, id->lexeme ());
            ctx.tu ().new_edge<Defines> (ctx.scope (), *he_, id_->lexeme ());

            ctx.tu ().new_edge<Manages> (*he_, ce);
          }
        }

        void HomeExecutor::
        end ()
        {
          id_ = 0;
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
