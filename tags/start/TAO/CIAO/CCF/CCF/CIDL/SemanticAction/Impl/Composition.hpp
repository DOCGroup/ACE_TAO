// file      : CCF/CIDL/SemanticAction/Impl/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_IMPL_COMPOSITION_HPP
#define CCF_CIDL_SEMANTIC_ACTION_IMPL_COMPOSITION_HPP

#include "CCF/CIDL/SyntaxTree/Composition.hpp"
#include "CCF/CIDL/SemanticAction/Composition.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class Composition :
          public virtual SemanticAction::Composition,
          public virtual ScopeBase<SyntaxTree::CompositionPtr>
        {
        public:
          virtual
          ~Composition () throw () {}

          Composition (bool trace, SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::CompositionPtr> (scope),
                trace_ (trace)
          {
          }

          virtual void
          begin (SimpleIdentifierPtr const& id, Category::Value c)
          {
            if (trace_) cerr << "composition " << c << " " << id << endl;

            SyntaxTree::SimpleName name (id->lexeme ());

            SyntaxTree::Composition::Category::Value category =
              SyntaxTree::Composition::Category::SESSION;

            if (c == Category::ENTITY)
            {
              category = SyntaxTree::Composition::Category::ENTITY;
            }
            else if (c == Category::PROCESS)
            {
              category = SyntaxTree::Composition::Category::PROCESS;
            }
            else if (c == Category::SERVICE)
            {
              category = SyntaxTree::Composition::Category::SERVICE;
            }

            SyntaxTree::CompositionPtr cp (
              new SyntaxTree::Composition (name, category, scope_));

            scope_->insert (cp);
            push (cp);
          }

          virtual void
          open_scope ()
          {
            scope_ = top ();
          }

          virtual void
          close_scope ()
          {
            scope_ = scope_->scope ();
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

#endif  // CCF_CIDL_SEMANTIC_ACTION_IMPL_COMPOSITION_HPP
