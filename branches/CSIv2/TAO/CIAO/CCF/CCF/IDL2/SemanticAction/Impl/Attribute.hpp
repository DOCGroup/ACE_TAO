// file      : CCF/IDL2/SemanticAction/Impl/Attribute.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP

#include "CCF/IDL2/SemanticAction/Attribute.hpp"

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
        class Attribute : public virtual SemanticAction::Attribute
        {
        public:
          virtual
          ~Attribute () throw () {}

          Attribute (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current),
                type_ ("")
          {
          }

        public:
          virtual void
          type (IdentifierPtr const& id)
          {
            if (trace_) cerr << "attribute " << id;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) throw ()
              {
                return d->is_a<TypeDecl> ();
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
              cerr << "error: invalid attribute declaration" << endl;
              cerr << "no type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid attribute declaration" << endl;
              cerr << "declaration with name \'" << name
                   << "\' visible from scope \'" << scope_->name ()
                   << "\' is not a type declaration" << endl;
              cerr << "using non-type as an attribute type is illegal"
                   << endl;
            }

          }

          virtual void
          name (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << " " << id << endl;

            using namespace SyntaxTree;
            SimpleName name (id->lexeme ());

            AttributeDeclPtr a (
              new SyntaxTree::AttributeDecl (name, type_, scope_));

            scope_->insert (a);

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

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_ATTRIBUTE_HPP
