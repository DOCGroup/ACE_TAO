// file      : CCF/IDL3/SemanticAction/Impl/EventType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticAction/Impl/EventType.hpp"

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

        EventType::
        ~EventType () throw ()
        {
        }

        EventType::
        EventType (Context& c)
            : ScopeBase<SemanticGraph::EventType> (c)
        {
        }

        void EventType::
        begin_concrete_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "concrete eventtype def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<ConcreteEventType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<ConcreteEventType> ());
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void EventType::
        begin_concrete_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "concrete eventtype fwd" << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<ConcreteEventType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<ConcreteEventType> ());
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void EventType::
        inherits (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "inherits " << id << endl;
        }

        void EventType::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void EventType::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void EventType::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }

      }
    }
  }
}
