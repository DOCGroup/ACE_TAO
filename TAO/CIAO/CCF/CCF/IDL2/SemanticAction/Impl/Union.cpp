// file      : CCF/IDL2/SemanticAction/Impl/Union.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Union.hpp"

#include "CCF/IDL2/SemanticGraph/Enum.hpp"
#include "CCF/IDL2/SemanticGraph/Fundamental.hpp"

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

        Union::
        Union (Context& c)
            : ScopeBase<SemanticGraph::Union> (c)
        {
        }

        void Union::
        begin_def (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "union def " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Union&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Union> ());
          }

          ctx.tu ().new_edge<Defines> (ctx.scope (), now (), name);
        }

        void Union::
        begin_fwd (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "union fwd " << id << endl;

          SimpleName name (id->lexeme ());

          if (Nameable* s = lookup (ctx.tu (), ctx.scope (), name))
          {
            now (dynamic_cast<SemanticGraph::Union&>(*s));
          }
          else
          {
            now (ctx.tu ().new_node<SemanticGraph::Union> ());
          }

          ctx.tu ().new_edge<Mentions> (ctx.scope (), now (), name);
        }

        void Union::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "type " << id << endl;

          type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              type_ = &resolve<Type> (from, name);

              if (!(dynamic_cast<Enum*> (type_) ||
                    dynamic_cast<Boolean*> (type_) ||
                    dynamic_cast<Char*> (type_) ||
                    dynamic_cast<Wchar*> (type_) ||
                    dynamic_cast<Short*> (type_) ||
                    dynamic_cast<UnsignedShort*> (type_) ||
                    dynamic_cast<Long*> (type_) ||
                    dynamic_cast<UnsignedLong*> (type_) ||
                    dynamic_cast<LongLong*> (type_) ||
                    dynamic_cast<UnsignedLongLong*> (type_)))
              {
                throw WrongType (type_->scoped_name ());
              }

              ctx.tu ().new_edge<Specialized> (now (), *type_);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid union declaration" << endl;
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
                 << "\' is not a valid discriminant type" << endl;
          }
        }

        void Union::
        open_scope ()
        {
          if (ctx.trace ()) cerr << "scope open" << endl;

          push (ctx.scope ());
          ctx.scope (now ());
        }

        void Union::
        member_type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "type " << id << endl;
        }

        void Union::
        member_name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "name " << id << endl;
        }

        void Union::
        close_scope ()
        {
          ctx.scope (top ());
          pop ();

          if (ctx.trace ()) cerr << "scope close" << endl;
        }

        void Union::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
