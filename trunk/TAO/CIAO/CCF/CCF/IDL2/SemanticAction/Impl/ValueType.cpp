// file      : CCF/IDL2/SemanticAction/Impl/ValueType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/ValueType.hpp"

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

        ValueType::
        ~ValueType () throw () {}

        ValueType::
        ValueType (Context& c)
            : ScopeBase<SemanticGraph::ValueType> (c)
        {
        }

        void ValueType::
        begin_abstract_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "abstract valuetype def " << id << endl;

          qualifier_ = Qualifier::abstract;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<AbstractValueType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<AbstractValueType> ());
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void ValueType::
        begin_abstract_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "abstract valuetype fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<AbstractValueType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<AbstractValueType> ());
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void ValueType::
        begin_concrete_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "concrete valuetype def " << id << endl;

          qualifier_ = Qualifier::concrete;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<ConcreteValueType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<ConcreteValueType> ());
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void ValueType::
        begin_concrete_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "concrete valuetype fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<ConcreteValueType&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<ConcreteValueType> ());
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void ValueType::
        inherits (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "inherits " << id << endl;
        }

        void ValueType::
        supports (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "supports " << id << endl;
        }

        void ValueType::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void ValueType::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void ValueType::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
