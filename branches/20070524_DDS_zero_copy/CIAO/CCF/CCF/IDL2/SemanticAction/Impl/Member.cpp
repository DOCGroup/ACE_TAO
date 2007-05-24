// file      : CCF/IDL2/SemanticAction/Impl/Member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Member.hpp"

#include "CCF/IDL2/SemanticGraph/Member.hpp"

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
              // With introduction of CORBA 3.1 we have a new beast:
              // struct with incoplete members which itself becomes
              // incomplete.
              //
              type_ = &resolve<Type> (from, name/*, Flags::complete*/);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid member declaration" << endl;
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
                 << "using non-type as a member type is illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
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
              ctx.tu ().new_node<SemanticGraph::Member> (
                ctx.file (), id->line ()));

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
