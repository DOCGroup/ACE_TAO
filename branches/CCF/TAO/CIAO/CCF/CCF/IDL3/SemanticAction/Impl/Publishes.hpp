// file      : CCF/IDL3/SemanticAction/Impl/Publishes.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_PUBLISHES_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_PUBLISHES_HPP

#include "CCF/IDL3/SemanticAction/Publishes.hpp"

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
        class Publishes : public virtual SemanticAction::Publishes
        {
        public:
          virtual
          ~Publishes () throw () {}

          Publishes (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current),
                type_ ("")
          {
          }

          virtual void
          type (IdentifierPtr const& id)
          {
            if (trace_) cerr << "publishes " << id;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) throw ()
              {
                return d->is_a<EventTypeDef> ();
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
              cerr << "error: invalid publishes declaration" << endl;
              cerr << "no event type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid publishes declaration" << endl;
              cerr << "declaration with name \'" << name
                   << "\' visible from scope \'" << scope_->name ()
                   << "\' is not a defined event type" << endl;
              cerr << "using non-<defined event type> in publishes "
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
              new SyntaxTree::PublishesDecl (name, type_, scope_));

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

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_PUBLISHES_HPP
