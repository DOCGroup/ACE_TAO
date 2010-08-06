// file      : CCF/IDL2/SemanticAction/Impl/Interface.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Interface.hpp"

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

        Interface::
        Interface (Context& c)
            : ScopeBase<SemanticGraph::Interface> (c)
        {
        }

        void Interface::
        begin_abstract_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "abstract interface def " << id << endl;

          qualifier_ = Qualifier::abstract;

          SimpleName name (id->lexeme ());

          //@@ Not handling NotUnique exception.
          //
          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<AbstractInterface&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<AbstractInterface> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Interface::
        begin_abstract_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "abstract interface fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<AbstractInterface&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<AbstractInterface> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Interface::
        begin_local_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "local interface def " << id << endl;

          qualifier_ = Qualifier::local;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<LocalInterface&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<LocalInterface> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Interface::
        begin_local_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "local interface fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<LocalInterface&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<LocalInterface> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Interface::
        begin_unconstrained_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "unconstrained interface def " << id
                                 << endl;

          qualifier_ = Qualifier::unconstrained;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<UnconstrainedInterface&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<UnconstrainedInterface> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Interface::
        begin_unconstrained_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "unconstrained interface fwd " << id
                                 << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<UnconstrainedInterface&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<UnconstrainedInterface> (
                   ctx.file (), id->line ()));
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Interface::
        inherits (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " inherits " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Interface& i (
                resolve<SemanticGraph::Interface> (from, name, Flags::defined));

              switch (qualifier_)
              {
              case Qualifier::abstract:
                {
                  if (dynamic_cast<AbstractInterface*> (&i)) break;

                  throw WrongType (i.scoped_name ());
                }
              case Qualifier::unconstrained:
                {
                  if (dynamic_cast<UnconstrainedInterface*> (&i) ||
                      dynamic_cast<AbstractInterface*> (&i)) break;

                  throw WrongType (i.scoped_name ());
                }
              default: break;
              }

              check_inheritance (now ().inherits_begin (),
                                 now ().inherits_end (),
                                 i);

              ctx.tu ().new_edge<Inherits> (now (), i);
              ctx.tu ().new_edge<Extends> (now (), i);
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
                 << "no interface with name \'" << name
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
                 << "attempt to inherit from forward-declared interface "
                 << e.name () << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "inheritance from forward-declared interface is illegal"
                 << endl;
          }
          catch (AlreadyInherited const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "directly inheriting from interface \'" << e.name ()
                 << "\' more than once is illegal" << endl;
          }
        }

        void Interface::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Interface::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Interface::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
