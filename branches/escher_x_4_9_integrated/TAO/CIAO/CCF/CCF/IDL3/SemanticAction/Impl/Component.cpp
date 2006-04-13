// file      : CCF/IDL3/SemanticAction/Impl/Component.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticAction/Impl/Component.hpp"

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

        Component::
        Component (Context& c)
            : ScopeBase<SemanticGraph::Component> (c)
        {
        }

        void Component::
        begin_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "component def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Component&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Component> ());
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Component::
        begin_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "component fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Component&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Component> ());
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Component::
        inherits (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " inherits " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Component& c (
                resolve<SemanticGraph::Component> (from, name, Flags::defined));

              ctx.tu ().new_edge<Inherits> (now (), c);
              ctx.tu ().new_edge<Extends> (now (), c);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no component with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << "incompatible type in inheritance specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << "attempt to inherit from forward-declared component "
                 << e.name () << endl;
            cerr << "inheritance from forward-declared component is illegal"
                 << endl;
          }
        }

        void Component::
        supports (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " supports " << id << endl;

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
              cerr << "error: invalid supports specification" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no interface with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << "incompatible type in supports specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << "attempt to support forward-declared interface "
                 << e.name () << endl;
            cerr << "support of forward-declared interface is illegal"
                 << endl;
          }
          catch (AlreadySupported const& e)
          {
            cerr << "directly supporting interface \'" << e.name ()
                 << "\' more than once is illegal" << endl;
          }
        }

        //@@ Implementation of *_scope is the same for all cases.
        //
        void Component::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Component::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Component::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
