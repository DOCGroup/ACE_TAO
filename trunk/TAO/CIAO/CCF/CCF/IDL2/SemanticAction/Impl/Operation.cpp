// file      : CCF/IDL2/SemanticAction/Impl/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Operation.hpp"

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

        Operation::
        ~Operation () throw ()
        {
        }

        Operation::
        Operation (Context& c)
            : Base (c)
        {
        }

        void Operation::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "operation " << id;

          type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              type_ = &resolve<Type> (from, name, complete);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid operation declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const& e)
          {
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;
            cerr << "using non-type as an operation return type is illegal"
                 << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void Operation::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          op_ = 0;

          if (type_)
          {
            SimpleName name (id->lexeme ());

            SemanticGraph::Operation& o (
              ctx.tu ().new_node<SemanticGraph::Operation> ());

            ctx.tu ().new_edge<Returns> (o, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), o, name);

            op_ = &o;
          }
        }


        void Operation::
        parameter (Direction::Value direction,
                   IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id)
        {
          if (ctx.trace ()) cerr << "parameter " << direction << " "
                                 << type_id << " " << name_id << endl;

          if (op_ == 0) return;

          Name name (type_id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, name, complete));

              Parameter* p;

              switch (direction)
              {
              case Direction::in:
                {
                  p = &ctx.tu ().new_node<InParameter> (name_id->lexeme ());
                  break;
                }
              case Direction::out:
                {
                  p = &ctx.tu ().new_node<OutParameter> (name_id->lexeme ());
                  break;
                }
              case Direction::inout:
                {
                  p = &ctx.tu ().new_node<InOutParameter> (name_id->lexeme ());
                  break;
                }
              }

              ctx.tu ().new_edge<Belongs> (*p, t);
              ctx.tu ().new_edge<Receives> (*op_, *p);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid parameter declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const& e)
          {
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;
            cerr << "using non-type as an operation parameter type is "
                 << "illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }


        void Operation::
        raises (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "raises " << id << endl;

          if (op_ == 0) return;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Exception& e (
                resolve<SemanticGraph::Exception> (from, name));

              ctx.tu ().new_edge<Raises> (*op_, e);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid raises declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << "no exception with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const& e)
          {
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not an exception declaration" << endl;
            cerr << "using non-exception type in raises declaration is "
                 << "illegal" << endl;
          }
        }
      }
    }
  }
}
