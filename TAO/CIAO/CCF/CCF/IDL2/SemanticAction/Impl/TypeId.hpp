// file      : CCF/IDL2/SemanticAction/Impl/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPE_ID_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPE_ID_HPP

#include "CCF/IDL2/SyntaxTree/TypeId.hpp"

// These inclusions are needed to ensure that typeprefix is applied to a
// suitable declaration.

#include "CCF/IDL2/SyntaxTree/Module.hpp"
#include "CCF/IDL2/SyntaxTree/Interface.hpp"
#include "CCF/IDL2/SyntaxTree/ValueType.hpp"


#include "CCF/IDL2/SemanticAction/TypeId.hpp"

#include <sstream>

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
        class TypeId : public virtual SemanticAction::TypeId
        {
        public:
          virtual
          ~TypeId () throw () {}

          TypeId (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current)
          {
          }

          virtual void
          begin (IdentifierPtr const& d, StringLiteralPtr const& id)
          {
            if (trace_) cerr << "typeid " << d << " " << id << endl;

            using namespace SyntaxTree;

            Name dn (d->lexeme ());

            try
            {
              ScopedName dsn = scope_->table ().resolve (
                dn,
                scope_->name (),
                scope_->peek_order ());

              Order order = scope_->create_order ();

              std::ostringstream ostr;
              ostr << order;

              SimpleName name (ostr.str ());

              TypeIdPtr ti (
                new SyntaxTree::TypeId (
                  name,
                  order,
                  scope_,
                  dsn,
                  SyntaxTree::StringLiteral (id->lexeme ())));

              scope_->insert (ti);

              if (trace_) cerr << "assigned declaration name " << ti->name ()
                               << endl;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid typeid declaration" << endl;
              cerr << "no declaration with name \'"
                   << dn << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
          }

        private:
          bool trace_;
          SyntaxTree::ScopePtr& scope_;
        };


        //
        //
        //
        class TypePrefix : public virtual SemanticAction::TypePrefix
        {
        public:
          virtual
          ~TypePrefix () throw () {}

          TypePrefix (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current)
          {
          }

          virtual void
          begin (IdentifierPtr const& d, StringLiteralPtr const& prefix)
          {
            if (trace_) cerr << "typeprefix " << d << " " << prefix << endl;

            using namespace SyntaxTree;

            Name dn (d->lexeme ());

            try
            {
              struct Predicate : public DeclarationTable::ResolvePredicate
              {
                virtual bool
                test (DeclarationPtr const& d) const throw ()
                {
                  return
                       d->is_a<SyntaxTree::Module> ()
                    || d->is_a<SyntaxTree::FileScope> ()
                    || d->is_a<SyntaxTree::InterfaceDecl> ()
                    || d->is_a<SyntaxTree::ValueTypeDecl> ();
                }
              } p;

              ScopedName dsn = scope_->table ().resolve (
                dn,
                scope_->name (),
                scope_->peek_order (),
                p);

              Order order = scope_->create_order ();

              std::ostringstream ostr;
              ostr << order;

              SimpleName name (ostr.str ());

              TypePrefixPtr tp (
                new SyntaxTree::TypePrefix (
                  name,
                  order,
                  scope_,
                  dsn,
                  SyntaxTree::StringLiteral (prefix->lexeme ())));

              scope_->insert (tp);

              if (trace_) cerr << "assigned declaration name " << tp->name ()
                               << endl;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid typeprefix declaration" << endl;
              cerr << "no declaration with name \'"
                   << dn << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid typeprefix declaration" << endl;
              cerr << "no suitable declaration with name \'"
                   << dn << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
          }

        private:
          bool trace_;
          SyntaxTree::ScopePtr& scope_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPE_ID_HPP
