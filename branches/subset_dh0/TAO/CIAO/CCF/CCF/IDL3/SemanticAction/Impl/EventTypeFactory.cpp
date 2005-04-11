// file      : CCF/IDL3/SemanticAction/Impl/EventTypeFactory.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticAction/Impl/EventTypeFactory.hpp"

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

        EventTypeFactory::
        EventTypeFactory (Context& c)
            : Base (c)
        {
        }

        void EventTypeFactory::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          f_ = 0;

          SemanticGraph::EventType& h (
            dynamic_cast<SemanticGraph::EventType&>(ctx.scope ()));

          SimpleName name (id->lexeme ());

          f_ = &ctx.tu ().new_node<SemanticGraph::EventTypeFactory> ();

          ctx.tu ().new_edge<Returns> (*f_, h);
          ctx.tu ().new_edge<Defines> (ctx.scope (), *f_, name);
        }


        void EventTypeFactory::
        parameter (IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id)
        {
          if (ctx.trace ()) cerr << "parameter in " << " "
                                 << type_id << " " << name_id << endl;

          if (f_ == 0) return;

          Name name (type_id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, name, Flags::complete));

              Parameter& p (
                ctx.tu ().new_node<InParameter> (name_id->lexeme ()));

              ctx.tu ().new_edge<Belongs> (p, t);
              ctx.tu ().new_edge<Receives> (*f_, p);
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
          catch (WrongType const&)
          {
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << from
                 << "\' is not a type declaration" << endl;
            cerr << "using non-type as an factory parameter type is "
                 << "illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void EventTypeFactory::
        raises (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "raises " << id << endl;

          if (f_ == 0) return;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Exception& e (
                resolve<SemanticGraph::Exception> (from, name));

              ctx.tu ().new_edge<Raises> (*f_, e);
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
          catch (WrongType const&)
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
