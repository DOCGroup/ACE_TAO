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
        Attribute (Context& c)
            : Base (c)
        {
        }

        void Attribute::
        begin_ro ()
        {
          if (ctx.trace ()) cerr << "readonly attribute" << endl;

          readonly_ = true;


        }

        void Attribute::
        begin_rw ()
        {
          if (ctx.trace ()) cerr << "readwrite attribute" << endl;

          readonly_ = false;
        }

        void Attribute::
        type (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          type_ = 0;

          try
          {
            try
            {
              type_ = &resolve<Type> (from, name, Flags::complete);
            }
            catch (Resolve const&)
            {
              cerr << ctx.file () << ":" << id->line () << ": error: "
                   << "invalid attribute declaration" << endl;
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
            cerr << "using non-type as an attribute type is illegal" << endl;
          }
          catch (NotComplete const& e)
          {
            cerr << ctx.file () << ":" << id->line () << ": error: "
                 << "type \'" << e.name () << "\' is not complete" << endl;
          }
        }

        void Attribute::
        name (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << id << endl;

          if (type_ != 0)
          {
            if (readonly_)
              a_ = &ctx.tu ().new_node<ReadAttribute> (
                ctx.file (), id->line ());
            else
              a_ = &ctx.tu ().new_node<ReadWriteAttribute> (
                ctx.file (), id->line ());

            ctx.tu ().new_edge<Belongs> (*a_, *type_);
            ctx.tu ().new_edge<Defines> (ctx.scope (), *a_, id->lexeme ());
          }
        }

        void Attribute::
        get_raises (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "get-raise " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Exception& e (
                resolve<SemanticGraph::Exception> (from, name));

              ctx.tu ().new_edge<GetRaises> (
                dynamic_cast<ReadAttribute&> (*a_), e);
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

        void Attribute::
        set_raises (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "set-raise " << id << endl;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              SemanticGraph::Exception& e (
                resolve<SemanticGraph::Exception> (from, name));

              ctx.tu ().new_edge<SetRaises> (
                dynamic_cast<WriteAttribute&> (*a_), e);
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

        void Attribute::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
