// file      : CCF/IDL3/SemanticAction/Impl/Home.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/Component.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Home.hpp"

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

        Home::
        Home (Context& c)
            : ScopeBase<SemanticGraph::Home> (c)
        {
        }

        void Home::
        begin (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "home " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Home&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Home> ());
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }


        void Home::
        inherits (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "inherits " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Home& h (
                resolve<SemanticGraph::Home> (from, name, Flags::defined));

              ctx.tu ().new_edge<Inherits> (now (), h);
              ctx.tu ().new_edge<Extends> (now (), h);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no home with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << "incompatible type in inheritance specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << "attempt to inherit from forward-declared home "
                 << e.name () << endl;
            cerr << "inheritance from forward-declared home is illegal"
                 << endl;
          }
        }


        void Home::
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
            cerr << "attempt to support from forward-declared component "
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


        void Home::
        manages (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "manages " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              ctx.tu ().new_edge<Manages> (
                now (),
                resolve<SemanticGraph::Component> (from, name, Flags::defined));
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid manages specification" << endl;
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
            cerr << "incompatible type in manages specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << "attempt to manage forward-declared component "
                 << e.name () << endl;
            cerr << "management of forward-declared component is illegal"
                 << endl;
          }
        }

        void Home::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Home::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Home::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
