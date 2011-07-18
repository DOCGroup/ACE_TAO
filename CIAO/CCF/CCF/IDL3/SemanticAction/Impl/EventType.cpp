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
        EventType (Context& c)
            : ScopeBase<SemanticGraph::EventType> (c)
        {
        }

        void EventType::
        begin_abstract_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "abstract eventtype def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<AbstractEventType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<AbstractEventType> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void EventType::
        begin_abstract_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "abstract eventtype fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<AbstractEventType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<AbstractEventType> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void EventType::
        begin_concrete_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << "concrete eventtype def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<ConcreteEventType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<ConcreteEventType> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void EventType::
        begin_concrete_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "concrete eventtype fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<ConcreteEventType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<ConcreteEventType> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void EventType::
        inherits (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "inherits " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::ValueType& v (
                resolve<SemanticGraph::ValueType> (from, name, Flags::defined));

              check_inheritance (now ().inherits_begin (),
                                 now ().inherits_end (),
                                 v);


              bool abstract (dynamic_cast<AbstractValueType*> (&v) != 0);

              if (dynamic_cast<AbstractValueType*> (&now ()) != 0)
              {
                // Abstract eventtype may only inherit from abstract ones.
                //
                if (!abstract)
                {
                  cerr << ctx.file () << ":" << id->line () << ": error: "
                       << "invalid inheritance specification" << endl;

                  cerr << ctx.file () << ":" << id->line () << ": error: "
                       << "abstract eventtype `" << now ().name ()
                       << "\' may not inherit from concrete valuetype `"
                       << v.scoped_name () << "\'" << endl;
                  return;
                }
              }
              else
              {
                // Concrete eventtype may inherit from single concrete
                // valutype in which case it should be the first in the
                // inheritance list.
                //
                if (now ().inherits_begin () != now ().inherits_end () &&
                    !abstract)
                {
                  cerr << ctx.file () << ":" << id->line () << ": error: "
                       << "invalid inheritance specification" << endl;

                  cerr << ctx.file () << ":" << id->line () << ": error: "
                       << "concrete valuetype `" << v.scoped_name ()
                       << "\' is not the first in the inheritance list of "
                       << "eventtype `" << now ().name () << "\'" << endl;
                  return;
                }
              }

              ctx.tu ().new_edge<Inherits> (now (), v);
              ctx.tu ().new_edge<Extends> (now (), v);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid inheritance specification" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no valuetype with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "incompatible type in inheritance specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "attempt to inherit from the forward-declared valuetype "
                 << e.name () << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "inheritance from a forward-declared valuetype is illegal"
                 << endl;
          }
          catch (AlreadyInherited const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "directly inheriting from valuetype \'" << e.name ()
                 << "\' more than once is illegal" << endl;
          }
        }

        void EventType::
        supports (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "supports " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Interface& i (
                resolve<SemanticGraph::Interface> (from, name, Flags::defined));

              check_support (now ().supports_begin (),
                             now ().supports_end (),
                             i);

              ctx.tu ().new_edge<Supports> (now (), i);
              ctx.tu ().new_edge<Extends> (now (), i);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid supports specification" << endl;
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
                 << "incompatible type in supports specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "attempt to support forward-declared interface "
                 << e.name () << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "support of forward-declared interface is illegal"
                 << endl;
          }
          catch (AlreadySupported const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "directly supporting interface \'" << e.name ()
                 << "\' more than once is illegal" << endl;
          }
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
