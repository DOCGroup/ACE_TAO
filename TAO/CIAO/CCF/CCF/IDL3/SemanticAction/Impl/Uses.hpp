// file      : CCF/IDL3/SemanticAction/Impl/Uses.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_USES_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_USES_HPP

#include "CCF/IDL3/SemanticAction/Uses.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class Uses : public virtual SemanticAction::Uses
        {
        public:
          virtual
          ~Uses () throw () {}

          Uses (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current),
                type_ ("")
          {
          }

          virtual void
          type (IdentifierPtr const& id)
          {
            if (trace_) cerr << "uses " << id;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) throw ()
              {
                return d->is_a<InterfaceDecl> ();
              }
            } p;

            try
            {
              ScopedName sn = scope_->table ().resolve (
                name,
                scope_->name (),
                scope_->peek_order (),
                p);

              type_ = sn;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid uses declaration" << endl;
              cerr << "no interface type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid uses declaration" << endl;
              cerr << "declaration with name \'" << name
                   << "\' visible from scope \'" << scope_->name ()
                   << "\' is not an interface type declaration" << endl;
              cerr << "using non-<interface type> in uses "
                   << "declaration is illegal" << endl;
            }
          }

          virtual void
          name (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << " " << id << endl;

            using namespace SyntaxTree;

            SimpleName name (id->lexeme ());

            DeclarationPtr d (
              new SyntaxTree::UsesDecl (name, type_, scope_));

            scope_->insert (d);

            type_ = ScopedName ("");
          }

        private:
          bool trace_;
          SyntaxTree::ScopePtr& scope_;
          SyntaxTree::ScopedName type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_USES_HPP
