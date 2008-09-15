// file      : CCF/IDL2/SemanticAction/Impl/ValueTypeMember.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/ValueTypeMember.hpp"

#include "CCF/IDL2/SemanticGraph/ValueTypeMember.hpp"

#include <iostream>

using std::cerr;
using std::endl;

// This file has no dependency on ACE, thus we turn 
// all ACE_OS checks off
// FUZZ: disable check_for_lack_ACE_OS

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        ValueTypeMember::
        ValueTypeMember (Context& c)
            : Base (c)
        {
        }

        void ValueTypeMember::
        begin_private ()
        {
          if (ctx.trace ())
            cerr << "private valuetype member" << endl;

          access_ = SemanticGraph::ValueTypeMember::Access::private_;
        }

        void ValueTypeMember::
        begin_public ()
        {
          if (ctx.trace ())
            cerr << "public valuetype member" << endl;

          access_ = SemanticGraph::ValueTypeMember::Access::public_;
        }

        void ValueTypeMember::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ())
            cerr << " type: " << id << endl;

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
                   << "invalid valuetype member declaration" << endl;
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
                 << "using non-type as a valuetype member type is illegal"
                 << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }


        void ValueTypeMember::
        name (SimpleIdentifierPtr const& id)
        {
          using namespace SemanticGraph;

          if (ctx.trace ()) cerr << " name: " << id << endl;

          if (type_)
          {
            SimpleName name (id->lexeme ());

            SemanticGraph::ValueTypeMember* m;

            switch (access_)
            {
            case SemanticGraph::ValueTypeMember::Access::private_:
              {
                m = &ctx.tu ().new_node<ValueTypePrivateMember> (
                  ctx.file (), id->line ());
                break;
              }
            case SemanticGraph::ValueTypeMember::Access::public_:
              {
                m = &ctx.tu ().new_node<ValueTypePublicMember> (
                  ctx.file (), id->line ());
                break;
              }
            default:
              {
                abort ();
              }
            }

            ctx.tu ().new_edge<Belongs> (*m, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), *m, name);
          }
        }

        void ValueTypeMember::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
