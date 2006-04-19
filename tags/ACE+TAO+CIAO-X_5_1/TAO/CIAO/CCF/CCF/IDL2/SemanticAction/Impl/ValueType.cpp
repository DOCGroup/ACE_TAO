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
                // Abstract valuetype may only inherit from abstract ones.
                //
                if (!abstract)
                {
                  cerr << "abstract valuetype `" << now ().name ()
                       << "\' may not inherit from concrete valuetype `"
                       << v.scoped_name () << "\'" << endl;
                  return;
                }
              }
              else
              {
                // Concrete valuetype may inherit from single concrete
                // valutype in which case it should be the first in the
                // inheritance list.
                //
                if (now ().inherits_begin () != now ().inherits_end () &&
                    !abstract)
                {
                  cerr << "concrete valuetype `" << v.scoped_name ()
                       << "\' is not the first in the inheritance list of "
                       << "valuetype `" << now ().name () << "\'" << endl;
                  return;
                }
              }

              ctx.tu ().new_edge<Inherits> (now (), v);
              ctx.tu ().new_edge<Extends> (now (), v);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no valuetype with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << "incompatible type in inheritance specification" << endl;
          }
          catch (NotDefined const& e)
          {
            cerr << "attempt to inherit from the forward-declared valuetype "
                 << e.name () << endl;
            cerr << "inheritance from a forward-declared valuetype is illegal"
                 << endl;
          }
          catch (AlreadyInherited const& e)
          {
            cerr << "directly inheriting from valuetype \'" << e.name ()
                 << "\' more than once is illegal" << endl;
          }
        }

        void ValueType::
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
