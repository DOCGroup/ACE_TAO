// file      : CCF/IDL2/SemanticAction/Impl/Struct.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_STRUCT_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_STRUCT_HPP

#include "CCF/IDL2/SyntaxTree/Struct.hpp"
#include "CCF/IDL2/SemanticAction/Struct.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

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
        class Struct : public virtual SemanticAction::Struct,
                       public virtual ScopeBase<SyntaxTree::StructDeclPtr>
        {
        public:
          virtual
          ~Struct () throw () {}


          Struct (bool trace,
                  SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::StructDeclPtr> (scope),
                trace_ (trace),
                name_ ("::") //@@ dirty
          {
          }

          virtual void
          begin (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "struct " << id << endl;
            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          open_scope ()
          {
            if (trace_) cerr << "scope open" << endl;

            using namespace SyntaxTree;

            StructDefPtr s (new StructDef (name_, scope_));

            scope_->insert (s);
            push (s);
            scope_ = s;

            name_ = SimpleName ("::"); //indicate that we are done
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
            if (trace_) cerr << "end" << endl;

            using namespace SyntaxTree;

            if (name_ != SimpleName ("::"))
            {
              scope_->insert (
                StructForwardDeclPtr(
                  new SyntaxTree::StructForwardDecl (name_, scope_)));

              name_ = SimpleName ("::"); //indicate that we are done
            }
            else
            {
              pop ();
            }
          }

        private:
          bool trace_;
          SyntaxTree::SimpleName name_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_STRUCT_HPP
