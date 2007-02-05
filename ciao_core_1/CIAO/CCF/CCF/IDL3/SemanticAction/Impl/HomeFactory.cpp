// file      : CCF/IDL3/SemanticAction/Impl/HomeFactory.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp"

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

        HomeFactory::
        HomeFactory (Context& c)
            : Base (c)
        {
        }

        void HomeFactory::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          hf_ = 0;

          SemanticGraph::Home& h (
            dynamic_cast<SemanticGraph::Home&>(ctx.scope ()));

          SemanticGraph::Component& c (
            dynamic_cast<SemanticGraph::Component&>(h.manages ().managee ()));

          SimpleName name (id->lexeme ());

          hf_ = &ctx.tu ().new_node<SemanticGraph::HomeFactory> (
            ctx.file (), id->line ());

          ctx.tu ().new_edge<Returns> (*hf_, c);
          ctx.tu ().new_edge<Defines> (ctx.scope (), *hf_, name);
        }


        void HomeFactory::
        parameter (IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id)
        {
          if (ctx.trace ()) cerr << "parameter in " << " "
                                 << type_id << " " << name_id << endl;

          if (hf_ == 0) return;

          Name name (type_id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, name, Flags::complete));

              Parameter& p (
                ctx.tu ().new_node<InParameter> (
                  ctx.file (), name_id->line (), name_id->lexeme ()));

              ctx.tu ().new_edge<Belongs> (p, t);
              ctx.tu ().new_edge<Receives> (*hf_, p);
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
                 << "using non-type as an factory parameter type is "
                 << "illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << type_id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void HomeFactory::
        raises (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "raises " << id << endl;

          if (hf_ == 0) return;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Exception& e (
                resolve<SemanticGraph::Exception> (from, name));

              ctx.tu ().new_edge<Raises> (*hf_, e);
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
        }
      }
    }
  }
}
