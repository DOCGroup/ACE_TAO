// file      : CCF/IDL2/SemanticAction/Impl/Attribute.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Attribute.hpp"

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

        Attribute::
        ~Attribute () throw ()
        {
        }

        Attribute::
        Attribute (Context& c)
            : Base (c)
        {
        }

        void Attribute::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "attribute " << id;

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
              cerr << "error: invalid attribute declaration" << endl;
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
            cerr << "using non-type as an attribute type is illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void Attribute::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          if (type_)
          {
            SemanticGraph::Attribute& a (
              ctx.tu ().new_node<SemanticGraph::Attribute> ());

            ctx.tu ().new_edge<Belongs> (a, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), a, id->lexeme ());
          }
        }
      }
    }
  }
}
