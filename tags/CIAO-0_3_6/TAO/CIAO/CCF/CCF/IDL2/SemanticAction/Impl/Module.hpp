// file      : CCF/IDL2/SemanticAction/Impl/Module.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_MODULE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_MODULE_HPP

#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

#include "CCF/IDL2/SemanticAction/Module.hpp"

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
        class Module : public virtual SemanticAction::Module,
                       public virtual ScopeBase<SyntaxTree::ModulePtr>
        {
        public:
          virtual
          ~Module () throw () {}


          Module (bool trace,
                  SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::ModulePtr> (scope),
                trace_ (trace)
          {
          }

          virtual void
          begin (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "module " << id << endl;

            using namespace SyntaxTree;

            SimpleName name (id->lexeme ());

            ModulePtr m (new SyntaxTree::Module (name, scope_));

            scope_->insert (m);
            push (m);
          }

          virtual void
          open_scope ()
          {
            if (trace_) cerr << "scope open" << endl;
            scope_ = top ();
          }

          virtual void
          close_scope ()
          {
            scope_ = scope_->scope ();
            if (trace_) cerr << "scope close" << endl;
          }

          virtual void
          end ()
          {
            pop ();
            if (trace_) cerr << "end" << endl;
          }

        private:
          bool trace_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_MODULE_HPP
