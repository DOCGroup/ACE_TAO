// file      : CCF/IDL2/SemanticAction/Impl/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_ELEMENTS_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_ELEMENTS_HPP

#include <stack>

#include "CCF/IDL2/SemanticAction/Elements.hpp"

//@@tmp
#include <iostream>
using std::cerr;
using std::endl;

/*
  Design notes:

  (1) I am using stack-based approach to handle cases like this:

  struct A
  {
    struct B
    {
      struct C
      {
        ...
      } c;

    } b;
  };

  Even though you can't write the same let's say for interface
  I am still using stack approach for consistency and possible
  meta-coding.

*/

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {

        //
        //
        //
        template <typename NodeTypePtr>
        class ScopeBase : public virtual SemanticAction::Scope
        {
        protected:
          virtual
          ~ScopeBase () throw () {}

          ScopeBase (SyntaxTree::ScopePtr& scope)
              : scope_ (scope)
          {
          }

        protected:
          void
          push (NodeTypePtr const& t)
          {
            stack_.push (t);
          }

          void
          pop ()
          {
            stack_.pop ();
          }

          NodeTypePtr&
          top ()
          {
            return stack_.top ();
          }

        protected:
          SyntaxTree::ScopePtr& scope_;
          std::stack<NodeTypePtr> stack_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_ELEMENTS_HPP
