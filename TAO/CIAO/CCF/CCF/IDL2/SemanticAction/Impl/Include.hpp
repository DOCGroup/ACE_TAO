// file      : CCF/IDL2/SemanticAction/Impl/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP

#include <stack>

#include "CCF/IDL2/SyntaxTree/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        // This is left unimplemented for now. Later code from
        // IDL3 should be copied here.
        //
        //
        class Include : public virtual SemanticAction::Include
        {
        public:
          virtual
          ~Include () throw () {}

          Include (bool trace)
              : trace_ (trace)
          {
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            if (trace_) cerr << "include " << sl << endl;
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
          }

        public:
          bool trace_;
        };

        class SystemInclude : public virtual SemanticAction::SystemInclude
        {
        public:
          virtual
          ~SystemInclude () throw () {}

          SystemInclude (bool trace)
              : trace_ (trace)
          {
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            if (trace_) cerr << "include <" << sl->lexeme () << ">" << endl;
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
          }

        public:
          bool trace_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
