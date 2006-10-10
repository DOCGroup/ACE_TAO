// file      : CCF/IDL2/SemanticAction/Impl/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Fundamental.hpp"
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
        Operation (Context& c)
            : Base (c)
        {
        }

        void Operation::
        one_way ()
        {
          if (ctx.trace ()) cerr << "oneway ";
          one_way_ = true;
        }

        void Operation::
        two_way ()
        {
          if (ctx.trace ()) cerr << "twoway ";
          one_way_ = false;
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
              type_ = &resolve<Type> (from, name, Flags::complete);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid operation declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-type as an operation return type is illegal"
                 << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void Operation::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          if (one_way_)
          {
            if (dynamic_cast<Void*> (type_) == 0)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid operation declaration" << endl;

              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "oneway operation should have void as a return type"
                   << endl;
            }

            op_ = &ctx.tu ().new_node<OneWayOperation> (
              ctx.file (), id->line ());
          }
          else
            op_ = &ctx.tu ().new_node<TwoWayOperation> (
              ctx.file (), id->line ());

          SimpleName name (id->lexeme ());
          ctx.tu ().new_edge<Defines> (ctx.scope (), *op_, name);

          if (type_)
          {
            ctx.tu ().new_edge<Returns> (*op_, *type_);
          }
        }


        void Operation::
        parameter (Direction::Value direction,
                   IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id)
        {
          if (ctx.trace ()) cerr << "parameter " << direction << " "
                                 << type_id << " " << name_id << endl;

          Name name (type_id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          struct NotIn : Resolve {};

          try
          {
            try
            {
              if (one_way_ && direction != Direction::in)
                throw NotIn ();

              Type& t (resolve<Type> (from, name, Flags::complete));

              Parameter* p (0);

              switch (direction)
              {
              case Direction::in:
                {
                  p = &ctx.tu ().new_node<InParameter> (
                    ctx.file (), name_id->line (), name_id->lexeme ());
                  break;
                }
              case Direction::out:
                {
                  p = &ctx.tu ().new_node<OutParameter> (
                    ctx.file (), name_id->line (), name_id->lexeme ());
                  break;
                }
              case Direction::inout:
                {
                  p = &ctx.tu ().new_node<InOutParameter> (
                    ctx.file (), name_id->line (), name_id->lexeme ());
                  break;
                }
              }

              ctx.tu ().new_edge<Belongs> (*p, t);
              ctx.tu ().new_edge<Receives> (*op_, *p);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << type_id->line () << ": error: "
                   << "invalid parameter declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "no type with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;

            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "using non-type as an operation parameter type is "
                 << "illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
          catch (NotIn const&)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "parameter of oneway operation should have \'in\' "
                 << "direction" << endl;
          }
        }


        void Operation::
        raises (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "raises " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          struct OneWay : Resolve {};

          try
          {
            try
            {
              if (one_way_)
                throw OneWay ();

              SemanticGraph::Exception& e (
                resolve<SemanticGraph::Exception> (from, name));

              ctx.tu ().new_edge<Raises> (*op_, e);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid raises declaration" << endl;
              throw;
            }
          }
          catch (NotFound const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "no exception with name \'" << name
                 << "\' visible from scope \'" << from << "\'" << endl;
          }
          catch (WrongType const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not an exception declaration" << endl;

            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "using non-exception type in raises declaration is "
                 << "illegal" << endl;
          }
          catch (OneWay const&)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "oneway operation may not raise exceptions" << endl;
          }
        }
      }
    }
  }
}
