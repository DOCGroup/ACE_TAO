// file      : CCF/IDL2/SemanticAction/Impl/Typedef.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Typedef.hpp"

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Sequence.hpp"

#include <iostream>

using std::cerr;
using std::endl;

//@@ names used in smantic actions are inconsistent
//   e.g. begin () vs name (), etc
//

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Typedef::
        ~Typedef () throw ()
        {
        }

        Typedef::
        Typedef (Context& c)
            : Base (c)
        {
        }

        void Typedef::
        begin (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "typedef " << id << endl;

          type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              type_ = &resolve<Type> (from, name);
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid typedef declaration" << endl;
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
            cerr << "using non-type in typedef is illegal" << endl;
          }
        }

        void Typedef::
        begin_seq (IdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << "typedef sequence<" << id << ">" << endl;

          type_ = 0;

          Name name (id->lexeme ());
          ScopedName from (ctx.scope ().scoped_name ());

          try
          {
            try
            {
              Type& t (resolve<Type> (from, name));

              UnboundedSequence& s (ctx.tu ().new_node<UnboundedSequence> ());
              ctx.tu ().new_edge<Specialized> (s, t);

              type_ = &s;
            }
            catch (Resolve const&)
            {
              cerr << "error: invalid sequence declaration" << endl;
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
            cerr << "using non-type in sequence specialization is illegal"
                 << endl;
          }
        }

        void Typedef::
        declarator (SimpleIdentifierPtr const& id)
        {
          if (ctx.trace ()) cerr << " " << id << endl;

          SimpleName name (id->lexeme ());

          try
          {
            if (lookup (ctx.tu (), ctx.scope (), name) == 0)
            {
              if (type_ != 0)
              {
                ctx.tu ().new_edge<Aliases> (ctx.scope (), *type_, name);
              }

              return;
            }
          }
          catch (NotUnique const& )
          {
          }

          cerr << "error: invalid typedef declaration" << endl;
          cerr << "error: redeclaration of name " << name << endl;
        }

        void Typedef::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
