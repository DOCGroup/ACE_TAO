// file      : CCF/IDL2/SemanticAction/Impl/Member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Member.hpp"

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

        Member::
        ~Member () throw ()
        {
        }

        Member::
        Member (Context& c)
            : Base (c)
        {
        }

        void Member::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "member " << id << endl;

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
              cerr << "error: invalid member declaration" << endl;
              throw;
            }

            //@@ I am not handling NotUnique here. For example if
            //   I provide module name as type then the compiler
            //   will ICE. Think about other places it may happen
            //   (attribute, value memebr, typeded, others?).
            //
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
            cerr << "using non-type as an member type is illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }


        void Member::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          if (type_)
          {
            SimpleName name (id->lexeme ());

            SemanticGraph::Member& m (
              ctx.tu ().new_node<SemanticGraph::Member> ());

            ctx.tu ().new_edge<Belongs> (m, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), m, name);
          }
        }

        void Member::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
